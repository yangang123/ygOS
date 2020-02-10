#!/bin/bash


if [ $# != 1 ] ; then 
echo "USAGE: $0 </dev/name>" 
exit 1; 
fi 

./tools/mkimage -n tools/imximage.cfg.cfgtmp -T imximage -e 0x87800000 -d ygos.bin ygos.imx
sudo dd if=ygos.imx of=$1 bs=1K seek=1 && sync
