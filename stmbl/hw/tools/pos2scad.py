# python pos2scad.py stmbl_4.0-all-pos.csv stmbl_4.0.emn > pcb.scad
import csv
import sys
from sets import Set

pcb = 1.6

pos_file = open(sys.argv[1], 'rt')
idf_file = open(sys.argv[2], 'rt')

print "include <lib.scad>"
print "pcb_height = " + str(pcb) + ";"


try:
    idf = idf_file.readlines()
    state = "start"
    cutout = "-1"
    for line in idf:
        if state == "outline":
            l = line.split(" ")
            if ".END_BOARD_OUTLINE" in l[0]:
                print "      ]);"
                print "   }"
                print "}"
                state = "end"
                break
            if cutout == "-1":
                print "module pcb(){"
                print "   difference(){"
                print "      color(\"green\") linear_extrude(height = " + pcb + ", convexity = 10, twist = 0) polygon( points=["
                cutout = "0"
            if l[0] != cutout:
                cutout = l[0]
                print "      ]);"
                print "      color(\"green\") translate([0, 0, -0.1]) linear_extrude(height = " + pcb + " + 0.2, convexity = 10, twist = 0) polygon( points=["
            if l[0] == cutout:
                #if float(l[3]) != 0.0:
                #    for i in range(0, 20):
                print "         [" + l[1] + ", " + l[2] + "], "
        
        if state == "outline start":
            pcb = line.replace("\n", "")
            print "pcb_height = " + str(pcb) + ";"
            state = "outline"
            
        if state == "start":
            if ".BOARD_OUTLINE" in line:
                state = "outline start"
        
    reader = list(csv.reader(pos_file))
    reader.pop(0)
    
    # packages
    for row in reader:
        row[2] = row[2].replace("-", "_")
        row[2] = row[2].replace(".", "_")
        
    packages = Set([])
    for row in reader:
        packages.add(row[2])
    #for p in packages:
        #print "module " + p + "(){}"
    
    # place
    print "module place(){"
    for row in reader:
        if row[6] == "top":
            print "   translate([" + row[3] + ", " + row[4] + ", " + str(pcb) + "]) rotate([0, 0, " + row[5] + "]) " + row[2] + "(); // " + row[0] + " " + row[1]
        else:
            print "   translate([" + row[3] + ", " + row[4] + ", 0]) rotate([180, 0, 0]) rotate([0, 0, 360 - " + row[5] + "]) " + row[2] + "(); // " + row[0] + " " + row[1]
    print "}"

finally:
    pos_file.close()
