#!/usr/bin/env python
#
# Copyright 2013 StalkR <github-misc@stalkr.net>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
#
# 2015-01-28, Thomas Kindler <mail@t-kindler.de>: 
#
#    * Taken from github, added copyright header. See:
#        https://github.com/StalkR/misc/blob/master/crypto/crc32.py
#        http://blog.stalkr.net/2011/03/crc-32-forging.html
#
#    * Changed forge() semantics to /change/ the bytes at pos instead of
#      inserting there. Also return the forged CRC instead of changed data.
#
#


#https://code.google.com/p/tweakcrc/
#http://blog.stalkr.net/2011/03/crc-32-forging.html


"""Calculate and manipulate CRC32.
http://en.wikipedia.org/wiki/Cyclic_redundancy_check
-- StalkR
"""
import struct
import sys

# Polynoms in reversed notation
POLYNOMS = {
  'CRC-32-IEEE': 0x04C11DB7, # 802.3
}

class Error(Exception):
  pass

class CRC32(object):
  """A class to calculate and manipulate CRC32.

  Use one instance per type of polynom you want to use.
  Use calc() to calculate a crc32.
  Use forge() to forge crc32 by adding 4 bytes anywhere.
  """
  def __init__(self, type="CRC-32-IEEE"):
    if type not in POLYNOMS:
      raise Error("Unknown polynom. %s" % type)
    self.polynom = POLYNOMS[type]


  def calc_next(self, crc, data):
    crc = crc ^ data
    for i in range(32):
      if (crc & 0x80000000):
        crc = (crc << 1) ^ self.polynom
      else:
        crc = (crc << 1)
    return crc & 0xffffffff

  def calc_rev(self, crc, data):
    for i in range(32):
      if (crc & 1):
        crc = ((crc ^ self.polynom) >> 1) | 0x80000000
      else:
        crc = crc >> 1
    crc = crc ^ data
    return crc

  def calc(self, s):
    """Calculate crc32 of a string."""

    crc = 0xffffffff
    for i in range(0, len(s), 4):
      c = struct.unpack('<L', s[i:i + 4])[0]
      crc = self.calc_next(crc, c)

    return crc

  def rcalc(self, s, wanna_crc):
    crc = wanna_crc
    for i in range(len(s) - 4, -4, -4): # go from last byte to 0
      c = struct.unpack('<L', s[i:i + 4])[0]
      crc = self.calc_rev(crc, c)
    return crc

  def forge(self, wanted_crc, s, pos):
    """Forge crc32 of a string by changing 4 bytes at position pos."""

    # forward calculation of CRC up to pos, sets current forward CRC state
    fwd_crc = self.calc(s[:pos])

    # backward calculation of CRC up to pos, sets wanted backward CRC state
    bkd_crc = wanted_crc
    for i in range(len(s) - 4, pos, -4): # go from last byte to pos + 4
      c = struct.unpack('<L', s[i:i + 4])[0]
      bkd_crc = self.calc_rev(bkd_crc, c)

    # deduce the 4 bytes we need to insert
    bkd_crc = self.calc_rev(bkd_crc, 0) # xor 0 to ignore data
    bkd_crc = bkd_crc ^ fwd_crc


    # test result
    assert(self.calc(s[:pos] + struct.pack('<L', bkd_crc) + s[pos+4:]) == wanted_crc)

    return bkd_crc


if __name__=='__main__':
  if len(sys.argv) > 1:
    arg = sys.argv[1]
  else:
    arg = "\x12\x34\x56\x78"
    check = "\xdf\x8a\x8a\x2b"

  # CRC32 with default polynom
  crc = CRC32().calc(arg[::-1])
  print ("CRC32(%s) = 0x%08x" % (repr(arg), crc))
  assert(struct.pack('<L', crc) == check[::-1])

  rcrc = CRC32().rcalc(arg[::-1], crc)
  print ("RCRC32(%s, 0x%08x) = 0x%08x" % (repr(arg), crc, rcrc))
  assert(rcrc == 0xffffffff)


