#!/bin/bash
CRC="python ../Tools/pycrc-0.8.3/pycrc.py"
rm -rf crc8.h crc8.c crc16.h crc16.c crc32.h crc32.c
#1wire crc, for smartserial and hv packets
$CRC --model dallas-1-wire --algorithm table-driven --table-idx-width 8 --symbol-prefix crc8_  --generate c -o crc8.c
$CRC --model dallas-1-wire --algorithm table-driven --table-idx-width 8 --symbol-prefix crc8_  --generate h -o crc8.h
#16bit crc, for config in flash
$CRC --model crc-16	       --algorithm table-driven --table-idx-width 8 --symbol-prefix crc16_ --generate c -o crc16.c
$CRC --model crc-16        --algorithm table-driven --table-idx-width 8 --symbol-prefix crc16_ --generate h -o crc16.h
#32bit crc for firmware
$CRC --model crc-32	       --algorithm table-driven --table-idx-width 8 --symbol-prefix crc32_ --generate c -o crc32.c
$CRC --model crc-32        --algorithm table-driven --table-idx-width 8 --symbol-prefix crc32_ --generate h -o crc32.h
