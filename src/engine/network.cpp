#include "network.h"

network::network(config *config) {
    p_server_port = atoi( config->get( "server_port", "network", "64640").c_str());

    p_started = false;

    p_socket_descriptor = NULL;

    p_networkIdManager = new RakNet::NetworkIDManager();
}

network::~network()
{
    if( p_networkIdManager)
        delete p_networkIdManager;
    if( p_socket_descriptor)
        delete p_socket_descriptor;
    RakNet::RakPeerInterface::DestroyInstance(p_peerInterface);
}

void network::init() {
    // get peer interface
    p_peerInterface = RakNet::RakPeerInterface::GetInstance();
}

void network::startServer() {
    p_is_server = true;
    p_started = true;

    p_socket_descriptor = new RakNet::SocketDescriptor( p_server_port, 0);
    p_peerInterface->Startup(MAX_CLIENTS, p_socket_descriptor, 1);

    // We need to let the server accept incoming connections from the clients
    p_peerInterface->SetMaximumIncomingConnections(MAX_CLIENTS);

    printf( "network::startServer() start as server\n");
}

void network::startClient( std::string ip_address) {
    p_is_server = false;
    p_started = true;

    p_socket_descriptor = new RakNet::SocketDescriptor();
    p_peerInterface->Startup( 1, p_socket_descriptor, 1);

    p_peerInterface->Connect( ip_address.c_str(), p_server_port, 0,0);

    printf( "network::startClient() start as server\n");
}

void network::process( level *level) {
    RakNet::Packet *l_packet;

    if( level->getWorld()->getFileName() != p_world_file) {
        p_world_file = level->getWorld()->getFileName();
        RakNet::MessageID typeId;
        typeId=ID_CHANGE_LEVEL;
        RakNet::BitStream myBitStream;
        myBitStream.Write((RakNet::MessageID)typeId);
        writeString( p_world_file, &myBitStream);
        p_peerInterface->Send(&myBitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
    }

    for (l_packet=p_peerInterface->Receive(); l_packet; p_peerInterface->DeallocatePacket(l_packet), l_packet=p_peerInterface->Receive())
    {
        switch (l_packet->data[0])
            {
            case ID_REMOTE_DISCONNECTION_NOTIFICATION:
                printf("Another client has disconnected.\n");
                break;
            case ID_REMOTE_CONNECTION_LOST:
                printf("Another client has lost the connection.\n");
                break;
            case ID_REMOTE_NEW_INCOMING_CONNECTION: {
                printf("Another client has connected.\n");
                break;
            }
            case ID_CONNECTION_REQUEST_ACCEPTED:
                printf("Our connection request has been accepted.\n");
                break;
            case ID_NEW_INCOMING_CONNECTION: {
                printf("A connection is incoming.\n");
                RakNet::MessageID typeId;
                typeId=ID_CHANGE_LEVEL;
                RakNet::BitStream myBitStream;
                myBitStream.Write((RakNet::MessageID)typeId);
                writeString( p_world_file, &myBitStream);
                p_peerInterface->Send(&myBitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, l_packet->guid, false);
                break;
            }
            case ID_NO_FREE_INCOMING_CONNECTIONS:
                printf("The server is full.\n");
                break;
            case ID_DISCONNECTION_NOTIFICATION:
                if (p_is_server){
                    printf("A client has disconnected.\n");
                } else {
                    printf("We have been disconnected.\n");
                }
                break;
            case ID_CONNECTION_LOST:
                if (p_is_server){
                    printf("A client lost the connection.\n");
                } else {
                    printf("Connection lost.\n");
                }
                break;
            case ID_ENTITY:
                processPacket( l_packet, level->getEntityList());
            break;
            case ID_CHANGE_LEVEL: {
                if( p_is_server)
                    break;
                RakNet::MessageID typeId;
                RakNet::RakString l_file;

                RakNet::BitStream myBitStream(l_packet->data, l_packet->length, false);
                myBitStream.Read(typeId);
                readString( &l_file, &myBitStream );
                level->getWorld()->setLoadWorld( l_file.C_String());
            }
            break;
            default:
                printf("Message with identifier %i has arrived.\n", l_packet->data[0]);
                break;
            }
    }

    if( p_is_server)
    for( auto l_entity:level->getEntityList()->getEntitys()) {

        bool l_createClientSide = false;

        if( l_entity->hasNetworkInit() == false) {
            l_entity->SetNetworkIDManager( p_networkIdManager);
            l_createClientSide = true;
            l_entity->setNetworkInitFlag();
        }

        RakNet::MessageID useTimeStamp; // Assign this to ID_TIMESTAMP
        RakNet::Time timeStamp; // Put the system time in here returned by RakNet::GetTime()
        RakNet::MessageID typeId; // This will be assigned to a type I've added after ID_USER_PACKET_ENUM, lets say ID_SET_TIMED_MINE
        useTimeStamp = ID_TIMESTAMP;
        timeStamp = RakNet::GetTime();
        typeId=ID_ENTITY;
        RakNet::BitStream myBitStream;
        myBitStream.Write((RakNet::MessageID)typeId);
        myBitStream.Write(useTimeStamp);
        myBitStream.Write(timeStamp);
        // Assume we have a Mine* mine object
        myBitStream.Write( l_entity->getId());
        writeString( l_entity->getType()->getName(), &myBitStream);
        writeString( l_entity->getAction(), &myBitStream);
        myBitStream.Write( l_entity->getFrame());
        myBitStream.Write( l_entity->getPosition().x);
        myBitStream.Write( l_entity->getPosition().y);
        myBitStream.Write( l_entity->getVelocity().x);
        myBitStream.Write( l_entity->getVelocity().y);
        myBitStream.Write( (int)l_entity->getDirection());
        myBitStream.Write( (int)l_createClientSide);
        myBitStream.Write( l_entity->GetNetworkID()); // In the struct this is NetworkID networkId

        p_peerInterface->Send(&myBitStream, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
    }
}

unsigned char network::getPacketIdentifier( RakNet::Packet *p)
{
	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	else
		return (unsigned char) p->data[0];
}

void network::writeString( std::string cstring, RakNet::BitStream *output)
{
    p_stringCompressor.EncodeString( cstring.c_str(), 256, output);
}

void network::readString(RakNet::RakString *myString, RakNet::BitStream *input)
{
    p_stringCompressor.DecodeString( myString, 256, input);
}

void network::processPacket( RakNet::Packet *packet, entitylist *entitylist)
{
    int l_id = -1;
    fvec2 l_pos;
    fvec2 l_vel;
    RakNet::RakString l_typeName;
    RakNet::RakString l_action;
    RakNet::MessageID useTimeStamp;
    RakNet::Time timeStamp;
	RakNet::BitStream myBitStream(packet->data, packet->length, false); // The false is for efficiency so we don't make a copy of the passed data
	RakNet::MessageID typeId;
	int l_action_frame;
	int l_createObject;
	int l_dir;

	/// read
	myBitStream.Read(typeId);
	myBitStream.Read(useTimeStamp);
	myBitStream.Read(timeStamp);
	myBitStream.Read(l_id);
	readString( &l_typeName, &myBitStream );
	readString( &l_action, &myBitStream );
	myBitStream.Read( l_action_frame);
    myBitStream.Read( l_pos.x);
    myBitStream.Read( l_pos.y);
    myBitStream.Read( l_vel.x);
    myBitStream.Read( l_vel.y);
    myBitStream.Read( l_dir);
    myBitStream.Read( l_createObject);

    entity *p_obj = entitylist->getEntity( l_id);

	// if dont exist create one
	if( p_obj == NULL) {
        entitytype *l_type = entitylist->getType( l_typeName.C_String());
        if( l_type == NULL) {
            printf( "network::processPacket type %s didnt found\n", l_typeName.C_String());
            return;
        }
        // create one if not there and you have permission
        if( l_createObject)
            entitylist->create( l_type, l_pos, l_id, true);
        return;
	}

	// set pos
    p_obj->setPos( l_pos );
    p_obj->setVelocity( l_vel);

    p_obj->setDirection( l_dir);

    // set action
    p_obj->setAction( l_action.C_String(), false);
    p_obj->setFrame( l_action_frame);

}
