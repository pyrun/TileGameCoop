#ifndef NETWORK_H
#define NETWORK_H

#include "StringTable.h"
#include "RakPeerInterface.h"

#include "Kbhit.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "NetworkIDManager.h"
#include "RakNetTypes.h"
#include "GetTime.h"
#include "StringCompressor.h"
#include "level.h"

#include "config.h"

#define MAX_CLIENTS 10
#define NETWORK_IDLE_TIME 1000 // sec
#define NETWORK_IDLE_TIME_DESTROY_OBJ NETWORK_IDLE_TIME*2 //

enum {
    ID_SET_TIMED_MINE = ID_USER_PACKET_ENUM,
    ID_ENTITY,
    ID_ENTITY_DELETE,
    ID_ENTITY_UPDATE,
    ID_CHANGE_LEVEL
};

class network
{
    public:
        network( config *config);
        virtual ~network();

        void init();
        void startServer();
        void startClient( std::string ip_address = "127.0.0.1");

        void process( level *level);

        void sendDeleteObject( int id);

        unsigned char getPacketIdentifier( RakNet::Packet *p);
        void writeString( std::string cstring, RakNet::BitStream *output);
        void readString(RakNet::RakString *myString, RakNet::BitStream *input);
        void processPacket( RakNet::Packet *packet, entitylist *entitylist);

        bool isClient() { return !p_is_server;}
        bool isStarted() { return p_started;}
    protected:

    private:
        int p_server_port;
        RakNet::RakPeerInterface *p_peerInterface;
        RakNet::SocketDescriptor *p_socket_descriptor;
        RakNet::NetworkIDManager *p_networkIdManager;
        RakNet::StringCompressor p_stringCompressor;

        bool p_is_server;
        bool p_started;

        bool b_reset_create;

        std::string p_world_file;
};

#endif // NETWORK_H
