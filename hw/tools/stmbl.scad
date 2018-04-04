include <pcb_simple.scad>
include <v4_bracket.scad>
include <clip.scad>

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

module corner(r, l){
  difference(){
    color("grey") cube([r, r, l]);
    translate([r, r, -1]) color("grey") cylinder(r = r, h = l + 2);
  }
}

module corner2(r, l){
  color("grey") linear_extrude(height = l, convexity = 10, twist = 0) polygon( points = [[r, 0], [0, -r / 5], [-r / 5, 0], [0, r], [r, 0]]);
}


module cover(){
  $fn = 40;
  wall_t = 1.3;
  space = 0.5;
  fan_t = 10.5;
  fan_h = 12;
  fan_l = 45;
  fan_m3_l = 37;
  heatsink_m3_l = 36;
  heatsink_m3_h = -2.5;
  heatsink_l = 94;
  heatsink_w = 46;
  heatsink_h = 5;
  pcb_t = 1.6;
  f3_h = 8 + heatsink_h;
  f4_h = 2.5 + pcb_t + f3_h;
  f4_l = 52.25;
  f4_y = 4.75 + 2.54 / 2;
  f4_x = 2;
  f4_t = 100;
  rj45_h = 1.25;
  rj45_l = 16.5 * 3;
  rj45_t = 13.5;
  akl220_h = 11;
  akl220_l = 1.26;
  akl220_p = 5.08;
  akl182_h = 7;
  akl182_l = 1.6;
  akl182_p = 3.5;
  corner_r = 3;
  
  difference(){
    union(){
      difference(){
        translate([-wall_t, -wall_t, 0]) color("grey") cube([heatsink_l + fan_t + wall_t * 2, heatsink_w + wall_t * 2, f4_h + f4_l + wall_t]);
        color("grey") translate([0, 0, -1]) cube([heatsink_l + fan_t, heatsink_w, f4_h + f4_l + 1]);
        translate([0, 0, -1]) corner2(2, heatsink_h + 1);
        translate([0, heatsink_w, -1]) rotate([0, 0, -90]) corner2(2, heatsink_h + 1);
        translate([heatsink_l + fan_t, heatsink_w, -1]) rotate([0, 0, 180]) corner2(2, heatsink_h + fan_h + 1);
        translate([heatsink_l + fan_t, 0, -1]) rotate([0, 0, 90]) corner2(2, heatsink_h + fan_h + 1);
      }
      translate([0, 0, f4_h + 11]) rotate([0, 0, 0]) corner(corner_r, f4_l - 11);
      translate([heatsink_l + fan_t, heatsink_w, f4_h]) rotate([0, 0, 180]) corner(corner_r, f4_l);
      translate([heatsink_l + fan_t, 0, f4_h]) rotate([0, 0, 90]) corner(corner_r, f4_l);
      translate([0, heatsink_w, f4_h + 5]) rotate([0, 0, -90]) corner(corner_r, f4_l - 5);
      translate([0, 0, f4_h + f4_l]) rotate([-90, 0, 0]) corner(corner_r, heatsink_w);
      translate([heatsink_l + fan_t, heatsink_w, f4_h + f4_l]) rotate([-90, 0, 180]) corner(corner_r, heatsink_w);
      translate([0, 0, f4_h + f4_l]) rotate([0, 90, 0]) corner(corner_r, heatsink_l + fan_t);
      translate([heatsink_l + fan_t, heatsink_w, f4_h + f4_l]) rotate([180, 90, 0]) corner(corner_r, heatsink_l + fan_t);
    }
    color("grey") translate([f4_x, f4_y - space, f4_h]) cube([f4_t, pcb_t + space * 2, f4_l]);
    color("grey") translate([heatsink_l + fan_t - wall_t, heatsink_w / 2, heatsink_h + fan_h - fan_l / 2]) rotate([0, 90, 0]) cylinder(r = 22, h = wall_t * 3); // fan
    color("grey") translate([heatsink_l + fan_t - wall_t, heatsink_w / 2 - fan_m3_l / 2, heatsink_h + fan_h - fan_l / 2 + fan_m3_l / 2]) rotate([0, 90, 0]) cylinder(r = 1.75, h = wall_t * 3); // front m3
    color("grey") translate([heatsink_l + fan_t - wall_t, heatsink_w / 2 + fan_m3_l / 2, heatsink_h + fan_h - fan_l / 2 + fan_m3_l / 2]) rotate([0, 90, 0]) cylinder(r = 1.75, h = wall_t * 3); // front m3
    color("grey") translate([heatsink_l + fan_t - wall_t, f4_y + pcb_t - space, heatsink_h + fan_h + rj45_h - space]) cube([wall_t * 3, rj45_t + space * 2, rj45_l + space * 2]); // rj45
    color("grey") translate([-wall_t * 2, 0, f3_h + pcb_t - space]) cube([wall_t * 3, akl220_l + akl220_p * 3 + akl220_l + akl220_p * 2 + space, akl220_h + space * 2]); // uvw + hv
    color("grey") translate([-wall_t * 2, 37 - space, f3_h + pcb_t - space]) cube([wall_t * 3, akl182_l + akl182_p * 2 + space * 2, akl182_h + space * 2]); // mot temp
    color("grey") translate([-wall_t * 2, f4_y + pcb_t - space, f4_h + 17 - space]) cube([wall_t * 3, akl182_h + space * 2, akl182_l * 2 + akl182_p * 8 + space * 2 + 0.4]); // 24v + out
    color("grey") translate([-wall_t * 2, heatsink_w / 2 - heatsink_m3_l / 2, heatsink_h + heatsink_m3_h]) rotate([0, 90, 0]) cylinder(r = 1.75, h = wall_t * 3); // back m3
    color("grey") translate([-wall_t * 2, heatsink_w / 2 + heatsink_m3_l / 2, heatsink_h + heatsink_m3_h]) rotate([0, 90, 0]) cylinder(r = 1.75, h = wall_t * 3); // back m3
    color("grey") translate([9, f4_y + pcb_t - space, f4_h + f4_l - wall_t]) cube([akl182_l * 2 + akl182_p * 8 + space * 2 + 0.4, akl182_h + space * 2, wall_t * 3]); // cmd + in
    color("grey") translate([9 + 31 - space, f4_y + pcb_t - space, f4_h + f4_l - wall_t]) cube([2.54 * 6 + space * 6, 2.54 * 2 + space * 2, wall_t * 3]); // io
    color("grey") translate([9 + 51.5 - space, f4_y - 2.5 - space, 68]) union(){
      translate([2.5 / 2 + space, 0, 0]) cube([8 - 2.5 , 2.5 + space * 2, wall_t * 3]); 
      translate([2.5 / 2 + space, 2.5 / 2 + space, 0]) cylinder(r = 2.5 / 2 + space, h = wall_t * 3);
      translate([8 - 2.5 / 2 + space, 2.5 / 2 + space, 0]) cylinder(r = 2.5 / 2 + space, h = wall_t * 3);
    }; // usb
    
    color("grey") translate([-wall_t * 2, 5 + 1 * 6, 7]) rotate([0, 90, 0]) cylinder(r = 2, h = wall_t * 3);
    color("grey") translate([-wall_t * 2, 5 + 5 * 6, 7]) rotate([0, 90, 0]) cylinder(r = 2, h = wall_t * 3);
    color("grey") translate([-wall_t * 2, 5 + 1 * 6, -2]) cube([wall_t * 3, 24, 11]);
    color("grey") translate([-wall_t * 2, 5 + 1 * 6 - 2, -2]) cube([wall_t * 3, 28, 9]);
    //for(i = [1:6]){
    //  color("grey") translate([-1, 4, 28 + i * 6]) rotate([0, 90, 0]) cylinder(r = 2, h = 3);
    //}
    color("grey") translate([heatsink_l + fan_t - wall_t, f4_y + pcb_t - space, 0]) cube([wall_t * 3, rj45_t + space * 2, f4_h + rj45_h]); // rj45 slot
    color("grey") translate([37, heatsink_w - wall_t, 47]) rotate([-90, 0, 0]) cylinder(r = 1.75, h = wall_t * 3);
  }
  translate([heatsink_l + fan_t - 2, f4_y - wall_t + pcb_t - space, fan_h + heatsink_h + space]) color("grey") cube([2, wall_t, f4_l]);
  translate([heatsink_l + fan_t - 7, f4_y + pcb_t + rj45_t + space, fan_h + heatsink_h]) color("grey") cube([7, wall_t, f4_l]);
  translate([heatsink_l + fan_t - 15.5 - space, f4_y + pcb_t + rj45_t + space, f4_h + f4_l - 7]) color("grey") cube([10, wall_t, 7]);
  translate([heatsink_l + fan_t - 15.5 - space - wall_t, f4_y + pcb_t + space + akl182_h, f4_h + f4_l - 7]) color("grey") cube([wall_t, f4_y + pcb_t + rj45_t + space + wall_t - (f4_y + pcb_t + space + akl182_h), 7]);
  
  translate([0, f4_y + pcb_t - wall_t - space, f4_h + 9]) color("grey") cube([2 - space, wall_t, f4_l - 9]);
  translate([0, f4_y + pcb_t + space + akl182_h, f4_h + 9]) color("grey") cube([7, wall_t, f4_l - 9]);
  translate([0, f4_y + pcb_t + space + akl182_h, f4_h + f4_l - 7]) color("grey") cube([heatsink_l + fan_t - 15.5 - space, wall_t, 7]);
  color("grey") translate([37, heatsink_w - 7, 47 + 3 + space]) cube([wall_t, 7, f4_h + f4_l - 47 - 3 - space]);
  color("grey") translate([37, f4_y + pcb_t + space + akl182_h, f4_h + f4_l - 7]) cube([wall_t, heatsink_w - (f4_y + pcb_t + space + akl182_h), 7]);
  //translate([100, 48, 50]) color("grey") rotate([90, 0, 180])  linear_extrude(height = 0.6) text("STMBL 4.0", font = "Liberation Sans", size = 13);
  
  
}

module stmbl(){
  //translate([-88, 0, -5]) cover();
  translate([14, 4.75 + 1.27 + pcb_height, 45 - 33 - pcb_height]) rotate([90, 0, 0]) f4();
  //translate([-51, 46 + 1.5, 42]) rotate([-90, 0, 0]) m3(6);
  f3();translate([6, 0, -33]) rotate([0, 0, 90]) heatsink();
  translate([6, 0.5, 45 - 33]) rotate([0, 90, 0]) fan();

  translate([16.5, 4.5, 4 - 33]) rotate([0, 90, 0]) m3(15);
  translate([16.5, 4.5 + 37, 4 - 33]) rotate([0, 90, 0]) m3(15);
  translate([16.5 + 1.3, 4.5, 4 + 37 - 33]) rotate([0, 90, 0]) m3(10);
  translate([16.5 + 1.3, 4.5 + 37, 4 + 37 - 33]) rotate([0, 90, 0]) m3(10);
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
  translate([-70.5 + 27 + 6, 32, 42]) rotate([0, 0, 180]) bracket();
  translate([-52.5, 7.5 - 1.6, 42]) rotate([90, 0, 0]) m3(6);
  //translate([-25, 0.5, -34]) rotate([0, 0, 90]) clip2();
  //translate([-41, 0.5 +  4, -35]) rotate([180, 0, 0]) m3(6);
  //translate([-41, 0.5 +  4 + 37, -35]) rotate([180, 0, 0]) m3(6);
}