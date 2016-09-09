#! /bin/sh
"true" '''\'
if command -v python2 > /dev/null; then
  exec python2 "$0" "$@"
else
  exec python "$0" "$@"
fi
exit $?
'''
import serial.tools.list_ports
done = False

for port in serial.tools.list_ports.comports():
   if "483:5740" in port[2]:
      print "Reseting stmbl at " + port[2]
      stmbl = serial.Serial(port[0])
      stmbl.write('bootloader\n')
      stmbl.close()
      done = True

if not done:
   print "No boards found"
