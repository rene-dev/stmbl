#!/usr/bin/env python
import re
import sys
import file_updater

for infile in sys.argv[2:]:
  with open(infile, 'r+') as f:
    content = f.read()
    f.seek(0, 0)
    comp_file = infile.split("/")[-1].split(".")[0]
    inc_str = "#include \"comps/" + comp_file + "_comp.h\""    
    inc = 0
    for line in f:
      linc = re.search(inc_str, line)
      if linc:
        inc = 1
    
    if inc < 1:
      f.seek(0, 0)
      f.write(inc_str + "\n")
      f.write(content)
