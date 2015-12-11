#!/usr/bin/python
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

"""Calculate and manipulate CRC32.
http://en.wikipedia.org/wiki/Cyclic_redundancy_check
-- StalkR
"""
import struct
import sys

# Polynoms in reversed notation
POLYNOMS = {
  'CRC-32-IEEE': 0xedb88320, # 802.3
  'CRC-32C': 0x82F63B78, # Castagnoli
  'CRC-32K': 0xEB31D82E, # Koopman
  'CRC-32Q': 0xD5828281,
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
    self.table, self.reverse = [0]*256, [0]*256
    self._build_tables()

  def _build_tables(self):
    for i in range(256):
      fwd = i
      rev = i << 24
      for j in range(8, 0, -1):
        # build normal table
        if (fwd & 1) == 1:
          fwd = (fwd >> 1) ^ self.polynom
        else:
          fwd >>= 1
        self.table[i] = fwd & 0xffffffff
        # build reverse table =)
        if rev & 0x80000000 == 0x80000000:
          rev = ((rev ^ self.polynom) << 1) | 1
        else:
          rev <<= 1
        rev &= 0xffffffff
        self.reverse[i] = rev

  def calc(self, s):
    """Calculate crc32 of a string.
    Same crc32 as in (binascii.crc32)&0xffffffff.
    """
    crc = 0xffffffff
    for c in s:
      crc = (crc >> 8) ^ self.table[(crc ^ ord(c)) & 0xff]
    return crc^0xffffffff

  def forge(self, wanted_crc, s, pos):
    """Forge crc32 of a string by changing 4 bytes at position pos."""
    
    # forward calculation of CRC up to pos, sets current forward CRC state
    fwd_crc = 0xffffffff
    for c in s[:pos]:
      fwd_crc = (fwd_crc >> 8) ^ self.table[(fwd_crc ^ ord(c)) & 0xff]
    
    # backward calculation of CRC up to pos, sets wanted backward CRC state
    bkd_crc = wanted_crc^0xffffffff
    for c in s[pos+4:][::-1]:
      bkd_crc = ((bkd_crc << 8)&0xffffffff) ^ self.reverse[bkd_crc >> 24] ^ ord(c)
    
    # deduce the 4 bytes we need to insert
    for c in struct.pack('<L',fwd_crc)[::-1]:
      bkd_crc = ((bkd_crc << 8)&0xffffffff) ^ self.reverse[bkd_crc >> 24] ^ ord(c)
        
    # test result 
    assert(self.calc(s[:pos] + struct.pack('<L', bkd_crc) + s[pos+4:]) == wanted_crc)
       
    return bkd_crc


if __name__=='__main__':
  if len(sys.argv) > 1:
    arg = sys.argv[1]
  else:
    arg = "test"
  
  # CRC32 with default polynom
  crc = CRC32().calc(arg)
  print "CRC32(%s) = 0x%08x" % (arg, crc)
  
  # check with library
  from binascii import crc32
  assert(crc == crc32(arg)&0xffffffff)
