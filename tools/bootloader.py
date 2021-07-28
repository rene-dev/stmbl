#!/usr/bin/env python
import time
import sys
try:
  import serial.tools.list_ports
except ImportError:
  print("missing pyserial, run sudo apt install python3-serial")
  sys.exit(-1)

for i in range(0, 20):
  done = False
  time.sleep(1)
  for port in serial.tools.list_ports.comports():
    if "483:5740" in port[2]:
        print ("Reseting stmbl at " + port[2])
        try:
          stmbl = serial.Serial(port[0])
          stmbl.write('bootloader\n'.encode())
          stmbl.close()
          done = True
        except OSError:
          print ("Cannot open port, check permissions, or disconnect Servoterm")
          done = True

  if not done:
    print ("No boards found, or board is already in bootloader")
    break
