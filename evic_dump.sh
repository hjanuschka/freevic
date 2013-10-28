#!/bin/bash
SG=$1

sg_raw -v -v -r 0 /dev/$SG cc 80 02 00 80 00 00 04 00 00
sg_raw -v -r 0 /dev/$SG 1e 00 00 00 01 00 00
sg_raw -v -r 512 -o evic_core.dmp /dev/$SG c9 00 03 00 80 00 00 04 00 00  
xxd evic_core.dmp
sg_raw -v -v -r 0 /dev/$SG cc 80 02 00 80 00 00 04 00 00
sg_raw -v -r 0 /dev/$SG 1e 00 00 00 01 00 00
sg_raw -v -r 512 -o evic_settings.dmp /dev/$SG c9 00 02 00 80 00 00 04 00 00  
xxd evic_settings.dmp

F=$$;
mkdir /media/shared/$F;
cp *.dmp /media/shared/$F/
