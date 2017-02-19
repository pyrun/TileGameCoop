#! /bin/bash

HEIGHT=13
WIDTH=40
CHOICE_HEIGHT=4
BACKTITLE="TileCoopGame"
TITLE="Creating option"
MENU="Choose one of the following options:"

OPTIONS=(1 "Creating game folder"
         2 "Build makefile"
         3 "Build makefile64"
		 4 "Make"
         5 "Quit")

CHOICE=$(dialog --clear \
                --backtitle "$BACKTITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)


DATE=$(date +"%m-%d-%y")

clear
case $CHOICE in
        1)
			mkdir -p "linux/$DATE"
			cp -a "../bin/linux/." "linux/$DATE"
			cp -a "../res/." "linux/$DATE"
			cd "linux/$DATE"
            ;;
        2)
			./cbp2make.linux-x86 -in "../src/TileGameCoop.cbp" -out "../src/makefile"
            ;;
        3)
			./cbp2make.linux-x86_64 -in "../src/TileGameCoop.cbp" -out "../src/makefile"
            ;;
        4)
			cd "../src/"
			make linux
            ;;

        5)
            echo "Quit"
            ;;
esac
