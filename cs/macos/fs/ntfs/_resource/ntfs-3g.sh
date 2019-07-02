

remount(){
	# uuid=$(diskutil info "$FILENAME" | grep UUID | cut -d ':' -f2 | tr -d ' ')
	# volumeName=$(diskutil info "$FILENAME" | grep "Volume Name" | cut -d ':' -f2 | tr -d ' ')
    deviceNode=$(diskutil info "$FILENAME" | grep "Device Node:" | awk '{print $3}')
	if [ -z "$deviceNode" ]; then
        echo "Error. Can not get device node for $FILENAME"
        return 1
    fi

    diskutil unmount "$FILENAME"
    sudo /usr/local/bin/ntfs-3g "${deviceNode}" "$FILENAME" -olocal -oallow_other;
}

checkDisk(){
	filetype=$(diskutil info "$FILENAME" | grep "Type (Bundle):" | cut -d ':' -f2 | tr -d ' ')
	#echo $filetype
	if [ "$filetype" = "ntfs" ]; then
        remount
	fi

	if [ "$filetype" = "" ]; then
		echo "Error. Please, select a NTFS device"
	fi
}

#Check sudo
if [[ $(/usr/bin/id -u) -ne 0 ]]; then
    echo "This script should be run as ROOT. Try sudo"
    exit
fi

if [ "$1" = "ALL" ]; then
    for FILENAME in "/Volumes"/*; do
        checkDisk;
    done
else
    select FILENAME in "/Volumes"/*
    do
        case "$FILENAME" in
            "$QUIT")
            echo "Exiting."
            break
            ;;
            *)
            echo "You picked "$FILENAME" "
            checkDisk;
            ;;
    esac
    done
fi
