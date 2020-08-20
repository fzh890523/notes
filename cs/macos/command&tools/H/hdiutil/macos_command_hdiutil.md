
```sh
hdiutil help                               1
Usage: hdiutil <verb> <options>
<verb> is one of the following:
help            	flatten
attach          	imageinfo
detach          	internet-enable
eject           	isencrypted
verify          	makehybrid
create          	mount
compact         	mountvol
convert         	unmount
burn            	plugins
info            	resize
checksum        	segment
chpass          	pmap
erasekeys       	udifderez
unflatten       	udifrez
help			   display more detailed help

Usage:	hdiutil attach [options] <image>
	hdiutil attach -help

Usage:	hdiutil detach [options] <devname>
	hdiutil detach -help

	eject is a synonym for detach...
Usage:	hdiutil detach [options] <devname>
	hdiutil detach -help

Usage:	hdiutil verify [options] <image>
	hdiutil verify -help

Usage:	hdiutil create <sizespec> [options] <imagepath>
	hdiutil create -help

Usage:	hdiutil compact [options] <image>
	hdiutil compact -help

Usage:	hdiutil convert -format <format> -o <outfile> [options] <image>
	hdiutil convert -help

Usage:	hdiutil burn [options] <image>
	hdiutil burn -help

Usage:	hdiutil info [options]
	hdiutil info -help

Usage:	hdiutil checksum -type <checksumType> [options] <image>
	hdiutil checksum -help

Usage:	hdiutil chpass [options] <image>
	hdiutil chpass -help

Usage:	hdiutil erasekeys <image>
	hdiutil erasekeys -help

Usage:	hdiutil unflatten [options] <image>
	hdiutil unflatten -help

Usage:	hdiutil flatten [options] <image>
	hdiutil flatten -help

Usage:	hdiutil imageinfo [options] <image>
	hdiutil imageinfo -help

Usage:	hdiutil internet-enable [options] <image>
	hdiutil internet-enable -help

Usage:	hdiutil isencrypted <image>
	hdiutil isencrypted -help

Usage:	hdiutil makehybrid -o <outfile> [options] <source>
	hdiutil makehybrid -help

Usage:	hdiutil attach [options] <image>
	hdiutil attach -help

Usage:	hdiutil mountvol [options] <devname>
	hdiutil mountvol -help

Usage:	hdiutil unmount [options] <mountpoint>
	hdiutil unmount -help

Usage:	hdiutil plugins [options]
	hdiutil plugins -help

Usage:	hdiutil resize <sizespec> [options] <image>
	hdiutil resize -help

Usage:	hdiutil segment -o <outfile> -segmentCount <num> [options] <image>
	hdiutil segment -o <outfile> -segmentSize <size> [options] <image>
	hdiutil segment -help

Usage: hdiutil pmap [options] <image|device>
	hdiutil pmap -help

Usage:	hdiutil udifderez [options] <image>
	hdiutil udifderez -help

Usage:	hdiutil udifrez [options] <image>
	hdiutil udifrez -help
```
