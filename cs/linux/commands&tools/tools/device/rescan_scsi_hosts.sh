#!/bin/bash

for host in /sys/class/scsi_host/*; do 
	echo "- - -" | sudo tee $host/scan; 
	echo -n "rescan scsi host $host, cur sd*: "
	ls /dev/sd* ; 
done