include <pcb.scad>
include <v4_bracket.scad>

module pin(){
   for(a = [0:3]){
      color("gold") translate([0, -a * 2.54, -(3 + 1.27) / 2 + 1.27]) cube([0.64, 0.64, 3 + 1.27], true);
      color("gold") translate([(6 + 2.54 + 1.5) / 2 - 0.64 / 4, -a * 2.54, 1.27]) cube([6 + 2.54 + 1.5 + 0.64 / 2, 0.64, 0.64], true);
  }   
   color([0.2, 0.2, 0.2]) translate([2.54 / 2, - 4 * 2.54 + 2.54 / 2, 0]) cube([2.54, 4 * 2.54, 2.54]);
}

module f4(){
   difference(){
      union(){
         translate([-198, 103.75, 0]) pcb();
         translate([-198, 103.75, 0]) place();
      }
      color("green") translate([10, 0, -70]) rotate([0, 0, 180]) cube([120, 100, 80]);
   }
   translate([-198, 103.75, 0]) translate([147.000000, -99.750000, 1.60000]) rotate([0, 0, 270.000000]) pin();
}

module f3(){
   rotate([180, 0, 0]) translate([0, 0, -pcb_height - 8]) difference(){
      union(){
         translate([-198, 103.75, 0]) pcb();
         translate([-198, 103.75, 0]) place();
      }
      color("green") translate([-110, 0, -70]) cube([120, 100, 80]);
   }
}

module spacer(l){
  difference(){
    color([0.3, 0.3, 0.3]) cylinder($fn = 20, r = 3, h = l);
    translate([0, 0, -1]) color("grey") cylinder($fn = 20, r = 1.5, h = l + 2);
  }
}

module m3(l){
  translate([0, 0, -l]) color("darkgrey") cylinder($fn = 20, r = 1.5, h = l);
  difference(){
    color("darkgrey") cylinder($fn = 20, r = 2.5, h = 2);
    color("darkgrey") translate([0, 0, 0.5]) cylinder($fn = 20, r = 1.3, h = 1.6);
  }
}



f3();
translate([14, 4.75 + 1.27 + pcb_height, 45 - 33 - pcb_height]) rotate([90, 0, 0]) f4();
translate([6, 0, -33]) rotate([0, 0, 90]) heatsink();
translate([6, 0, 45 - 33]) rotate([0, 90, 0]) fan();


translate([16, 4.5, 4 - 33]) rotate([0, 90, 0]) m3(15);
translate([16, 4.5 + 37, 4 - 33]) rotate([0, 90, 0]) m3(15);
translate([17, 4.5, 4 + 37 - 33]) rotate([0, 90, 0]) m3(10);
translate([17, 4.5 + 37, 4 + 37 - 33]) rotate([0, 90, 0]) m3(10);
// translate([6, 20, 33]) spacer(8);
// translate([6 + 36, 20, 33]) spacer(8);
// translate([6, 20, 33 + 8 + pcb_hight]) m3(15);
// translate([6 + 36, 20, 33 + 8 + pcb_hight]) m3(15);
translate([-81, 33, 8 + pcb_height]) m3(15);
translate([-21, 33, 8 + pcb_height]) m3(15);
translate([-52.5, 5, 8 + pcb_height]) m3(15);
translate([-81, 33, 5.7]) spacer(2.3);
translate([-21, 33, 5.7]) spacer(2.3);
translate([-52.5, 5, 0]) spacer(8);//translate([1 + 8, 1, 96 + 4]) io();
//translate([1, 10, 33 + 8]) cover();
translate([-89, 5, -2]) rotate([0, -90, 0]) m3(5);
translate([-89, 5 + 36, -2]) rotate([0, -90, 0]) m3(5);
translate([-70.5 + 27 + 6, 32, 42]) rotate([0, 0, 180]) bracket2();
translate([-52.5, 7.5 - 1.6, 42]) rotate([90, 0, 0]) m3(6);
