#!/usr/bin/env python
import binascii
import sys

buf = open(sys.argv[1],'rb').read()
buf = (binascii.crc32(buf) & 0xFFFFFFFF)
if buf == 0:
    print "crc ok!"
    sys.exit(0)
else:
    print "This should not happen. Please report a bug."
    sys.exit(-1)
