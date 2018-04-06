#! /bin/sh
"true" '''\'
if command -v python2.7 > /dev/null; then
  exec python2.7 "$0" "$@"
else
  exec python "$0" "$@"
fi
exit $?
'''
import serial.tools.list_ports
done = False

for port in serial.tools.list_ports.comports():
   if "483:5740" in port[2]:
      print ("Reseting stmbl at " + port[2])
      try:
        stmbl = serial.Serial(port[0])
        stmbl.write('bootloader\n')
        stmbl.close()
        done = True
      except OSError:
        print ("Cannot open port, check permissions, or disconnect Servoterm")
        done = True

if not done:
   print ("No boards found, or board is already in bootloader")
