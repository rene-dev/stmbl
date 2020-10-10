include <stmbl.scad>;
//translate([88, 0, - 52.25 - 8 - 2.5 - 1.6]) stmbl();


wall_t = 0.8;
gap_w = 1;
bridge_w = 2;
slot_l = 12;
bend = 0;
bend_a = 0.25 * (1 - bend);
corner_r = 0.5;
c = "grey";
space = 0.25;
pcb_t = 1.6;
heatsink_l = 104.5;
heatsink_w = 46;
case_h = 69;
led_r = 0.75;
$fn = 20;
back_cut = 32;

f4_h = 52.25;
fan_h = 45;
heatsink_h = 33;
heatsink_to_f3 = 8;

module bend_gap(l){
  l1 = l - gap_w - bridge_w;
  num_slots = max(floor(l1 / slot_l), 1);
  slot_l1 = l1 / num_slots;
  color(c) translate([0, 0, -1]){
    cylinder(r = gap_w / 2 * 1.5, h = wall_t + 2);
    translate([l, 0, 0]) cylinder(r = gap_w / 2 * 1.5, h = wall_t + 2);
    for(a = [0:num_slots - 1]){
      translate([a * slot_l1, 0, 0]){
        hull(){
          translate([gap_w + bridge_w, 0, 0]) cylinder(r = gap_w / 2, h = wall_t + 2);
          translate([slot_l1, 0, 0]) cylinder(r = gap_w / 2, h = wall_t + 2);
        }
      }
    }
  }
}

module bend_bridge(l){
  color(c) hull(){
    translate([bend_a, 0, 0]) cylinder(r = bend_a, h = wall_t);
    translate([l - bend_a, 0, 0]) cylinder(r = bend_a, h = wall_t);
  }
}

module back_cuts(){
  color(c) union(){
    translate([-6 - pcb_t + space, - 3.75 + space, -1]) rotate([0, 0, 180]) cube([7 + space * 3, 31.5 + space * 2, wall_t + 2]); // 24V + out
    translate([0, - 43.75 + space, -1]) rotate([0, 0, 180]) cube([28.5 + space * 2, 8 + space * 2 + 3, wall_t + 2]); // uvw
    //translate([-3.14, - 46.75 + space + 3, -1]) rotate([0, 0, 180]) cube([28.25 - 3.14 * 2, 3 + space, wall_t + 2]); // uvw
    translate([-6 + led_r, -8 + space, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
    translate([-6 + led_r, -17 + space, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
    translate([-6 + led_r, -24 + space, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
    translate([-6 + led_r, -31 + space, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
    translate([-5, -case_h + 2.6, -1]) cylinder(r = 1.5 + space / 2, h = wall_t + 2); // m3
    translate([-5 - 36, -case_h + 2.6, -1]) cylinder(r = 1.5 + space / 2, h = wall_t + 2); // m3
    translate([-23, -f4_h - fan_h + heatsink_h + 2, -1]) cylinder(r = 1 + space / 2, h = wall_t + 2); // cooling
    translate([-23 - 6, -f4_h - fan_h + heatsink_h + 2, -1]) cylinder(r = 1 + space / 2, h = wall_t + 2); // cooling
    translate([-23 + 6, -f4_h - fan_h + heatsink_h + 2, -1]) cylinder(r = 1 + space / 2, h = wall_t + 2); // cooling
    translate([-23 - 12, -f4_h - fan_h + heatsink_h + 2, -1]) cylinder(r = 1 + space / 2, h = wall_t + 2); // cooling
    translate([-23 + 12, -f4_h - fan_h + heatsink_h + 2, -1]) cylinder(r = 1 + space / 2, h = wall_t + 2); // cooling
    translate([-22.5, -f4_h - fan_h + heatsink_h + heatsink_to_f3 - led_r, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
    translate([-heatsink_w, -f4_h - fan_h + heatsink_h + heatsink_to_f3 + pcb_t - space, -1]) cube([9 + space, 7 + space * 2, wall_t + 2]); // mot temp
  }
}

module up_cuts(){
  translate([64.5, 4.75, -1]) hull(){ // usb
    translate([-2.5 - space, 0, 0]) cylinder(r = 1.5 + space, h = wall_t + 2);
    translate([2.5 + space, 0, 0]) cylinder(r = 1.5 + space, h = wall_t + 2);
  }
  translate([15.5, 6 - 0.5, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
  translate([26, 6 - 0.5, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
  translate([44, 6 - 0.5, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
  translate([49.25, 6 - 0.5, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
  translate([54.5, 6 - 0.5, -1]) cylinder(r = led_r + space, h = wall_t + 2); // led
  translate([9.5 - space, 6 + pcb_t - space, -1]) cube([31.5 + space * 2, 7 + space * 2, wall_t + 2]); // ain + cmd
  translate([41.75 - space - 1, 6 + pcb_t - space, -1]) cube([15.25 + space * 2 + 1, 5 + space * 2, wall_t + 2]); // io
}

module front_cuts(){

}

module up_text(){
  color("white") hull() {
      translate([0, 0, 0]) cylinder(r = 1, h = 0.1);
      translate([60, 0, 0]) cylinder(r = 1, h = 0.1);
      translate([0, 20, 0]) cylinder(r = 1, h = 0.1);
      translate([60, 20, 0]) cylinder(r = 1, h = 0.1);
    }
    color("black") translate([30, 12.5, 0]) linear_extrude(height = 0.2, convexity = 4) text("STMBL V4", size= 7.5, halign = "center");
    color("black") translate([30, 8, 0]) linear_extrude(height = 0.2, convexity = 4) text("https://github.com/rene-dev/stmbl", size= 2.5, halign = "center");
    
    color("black") translate([12 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("24V",size = 2, valign = "center");
    color("black") translate([15.5 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("AIN0",size = 2, valign = "center");
    color("black") translate([19 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("GND",size = 2, valign = "center");
    
    color("black") translate([22.5 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("24V",size = 2, valign = "center");
    color("black") translate([26 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("AIN1",size = 2, valign = "center");
    color("black") translate([29.5 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("GND",size = 2, valign = "center");
    
    color("black") translate([35 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("A",size = 2, valign = "center");
    color("black") translate([38.5 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("B",size = 2, valign = "center");

    color("black") translate([44 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("GO",size = 2, valign = "center");
    color("black") translate([49.25 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("RDY",size = 2, valign = "center");
    color("black") translate([54.5 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("ERR",size = 2, valign = "center");

    color("black") translate([64.5 - 10, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("USB",size = 2, valign = "center");
}

module front_text(){
  color("white") hull() {
      translate([0, 0, 0]) cylinder(r = 1, h = 0.1);
      translate([45, 0, 0]) cylinder(r = 1, h = 0.1);
      translate([0, 20, 0]) cylinder(r = 1, h = 0.1);
      translate([45, 20, 0]) cylinder(r = 1, h = 0.1);
    }
    color("black") translate([9.75 - 5, 0, 0]) linear_extrude(height = 0.2, convexity = 4) text("CMD", size= 2, halign = "center");
    color("black") translate([26.25 - 5, 0, 0]) linear_extrude(height = 0.2, convexity = 4) text("FB0", size= 2, halign = "center");
    color("black") translate([42.75 - 5, 0, 0]) linear_extrude(height = 0.2, convexity = 4) text("FB1", size= 2, halign = "center");
    
    color("black") translate([0, 20, 0]) linear_extrude(height = 0.2, convexity = 4) text("      ENC RES  UART SPI   SS  S/D", size= 1.5, valign = "center", font = "Courier New");
    color("black") translate([0, 18, 0]) linear_extrude(height = 0.2, convexity = 4) text("1 OrS A+  SIN+ RX+  MISO+ RX+ STP+", size= 1.5, valign = "center", font = "Courier New");
    color("black") translate([0, 16, 0]) linear_extrude(height = 0.2, convexity = 4) text("2 Or  A-  SIN- RX-  MISO- RX- STP-", size= 1.5, valign = "center", font = "Courier New");
    color("black") translate([0, 14, 0]) linear_extrude(height = 0.2, convexity = 4) text("3 GrS B+  COS+ CLK+ CLK+  TX+ DIR+", size= 1.5, valign = "center", font = "Courier New");
    color("black") translate([0, 12, 0]) linear_extrude(height = 0.2, convexity = 4) text("4 Bl  C-  REF- TX-  MOSI-     ERR-", size= 1.5, valign = "center", font = "Courier New");
    color("black") translate([0, 10, 0]) linear_extrude(height = 0.2, convexity = 4) text("5 BlS C+  REF+ TX+  MOSI+     ERR+", size= 1.5, valign = "center", font = "Courier New");
    color("black") translate([0,  8, 0]) linear_extrude(height = 0.2, convexity = 4) text("6 Gr  B-  COS- CLK- CLK-  TX- DIR-", size= 1.5, valign = "center", font = "Courier New");
    color("black") translate([0,  6, 0]) linear_extrude(height = 0.2, convexity = 4) text("7 BrS VCC AIN  VCC  VCC       EN+", size= 1.5, valign = "center", font = "Courier New");
    color("black") translate([0,  4, 0]) linear_extrude(height = 0.2, convexity = 4) text("8 Br  GND GND  GND  GND       EN-", size= 1.5, valign = "center", font = "Courier New");
}

module back_text(){
  color("white") hull() {
      translate([0, 0, 0]) cylinder(r = 1, h = 0.1);
      translate([30, 0, 0]) cylinder(r = 1, h = 0.1);
      translate([0, 7.5, 0]) cylinder(r = 1, h = 0.1);
      translate([30, 7.5, 0]) cylinder(r = 1, h = 0.1);
    }
    

    color("black") translate([6.25 - 4.5, 7.5, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("GND",size = 2, halign = "right", valign = "center");
    color("black") translate([6.25 + 3.5 - 4.5, 7.5, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("24V",size = 2, halign = "right", valign = "center");
    
    color("black") translate([15.25 + 0 * 3.5- 4.5, 7.5, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("OUT0",size = 2, halign = "right", valign = "center");
    color("black") translate([15.25 + 1 * 3.5 - 4.5, 7.5, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("24V",size = 2, halign = "right", valign = "center");
    color("black") translate([15.25 + 2 * 3.5 - 4.5, 7.5, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("OUT1",size = 2, halign = "right", valign = "center");
    color("black") translate([15.25 + 3 * 3.5 - 4.5, 7.5, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("24V",size = 2, halign = "right", valign = "center");
    color("black") translate([15.25 + 4 * 3.5 - 4.5, 7.5, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("OUT2",size = 2, halign = "right", valign = "center");
    color("black") translate([15.25 + 5 * 3.5 - 4.5, 7.5, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("24V",size = 2, halign = "right", valign = "center");
}

module back_text2(){
  color("white") hull() {
      translate([0, 0, 0]) cylinder(r = 1, h = 0.1);
      translate([43, 0, 0]) cylinder(r = 1, h = 0.1);
      translate([0, 6, 0]) cylinder(r = 1, h = 0.1);
      translate([43, 6, 0]) cylinder(r = 1, h = 0.1);
    }
    

    color("black") translate([3 - 1, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("TIN",size = 2, halign = "left", valign = "center");
    color("black") translate([3 + 3.5 - 1, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("GND",size = 2, halign = "left", valign = "center");
    
    //color("black") translate([15 - 1, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("max",size = 2, halign = "left", valign = "center");
    //color("black") translate([18 - 1, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("400V",size = 2, halign = "left", valign = "center");
    
    color("black") translate([21 - 1, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("320V",size = 2, halign = "left", valign = "center");
    color("black") translate([21 + 5.08 - 1, 0, 0]) rotate([0, 0, 90]) linear_extrude(height=0.2, convexity=4) text("GND",size = 2, halign = "left", valign = "center");
    
    color("black") translate([32.75 - 1, 0, 0]) rotate([0, 0, 0]) linear_extrude(height=0.2, convexity=4) text("W",size = 2, halign = "center", valign = "bottom");
    color("black") translate([32.75 + 5.08 - 1, 0, 0]) rotate([0, 0, 0]) linear_extrude(height=0.2, convexity=4) text("V",size = 2, halign = "center", valign = "bottom");
    color("black") translate([32.75 + 2 * 5.08 - 1, 0, 0]) rotate([0, 0, 0]) linear_extrude(height=0.2, convexity=4) text("U",size = 2, halign = "center", valign = "bottom");
}

module up(){
  color(c) difference(){
    hull(){
      translate([corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([heatsink_l - corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([heatsink_l - corner_r, heatsink_w - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([corner_r, heatsink_w - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
    }
    up_cuts();
  }
  translate([10, 17.5, wall_t]) up_text();
}

module front_left(){
  color(c) difference(){
      hull(){
        translate([corner_r, -corner_r, 0]) cylinder(r = corner_r, h = wall_t);
        translate([6 + pcb_t - space - corner_r, -corner_r, 0]) cylinder(r = corner_r, h = wall_t);
        translate([6 + pcb_t - space - corner_r, -52.25 - 4 - 3 + corner_r, 0]) cylinder(r = corner_r, h = wall_t);
        translate([corner_r, -52.25 - 4 - 3 + corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      }
    translate([4.4, -52.25 - 4, -1]) cylinder(r = 4 / 2 + space / 2, h = wall_t + 2); // m4

  }
}

module front_right(){
    color(c) difference(){
      union(){
        hull(){
          translate([corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
          translate([23.5 + pcb_t - space - corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
          translate([23.5 + pcb_t - space - corner_r, 52.25 - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
          translate([corner_r, 52.25 - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
        }
        hull(){
          translate([corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
          translate([6 + pcb_t - space - corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
          translate([6 + pcb_t - space - corner_r, 52.25 + 4 + 3 - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
          translate([corner_r, 52.25 + 4 + 3 - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
        }
        
      }
      translate([4.4, 52.25 + 4, -1]) cylinder(r = 4 / 2 + space / 2, h = wall_t + 2); // m4
  }
      translate([23, 5, wall_t]) rotate([0, 0, 90]) front_text();
}

module back_left(){
  color(c) difference(){
    hull(){
      translate([-corner_r, -corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([-back_cut + space / 2 + corner_r, -corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([-back_cut + space / 2 + corner_r, -case_h + corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([-corner_r, -case_h + corner_r, 0]) cylinder(r = corner_r, h = wall_t);
    }
    back_cuts();
  }
  translate([-25, -4.5, wall_t]) rotate([0, 0, -90]) back_text();
  translate([-44.5, -42.5, wall_t]) rotate([0, 0, 0]) back_text2();
}

module back_right(){
  color(c) difference(){
    translate([0, case_h, 0]) hull(){
      translate([-corner_r, -corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([-heatsink_w + back_cut + corner_r + space / 2, -corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([-heatsink_w + back_cut + corner_r + space / 2, -case_h + corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([-corner_r, -case_h + corner_r, 0]) cylinder(r = corner_r, h = wall_t);
    }
    translate([-heatsink_w, 0, 0]) rotate([0, 0, 180]) back_cuts();
  }
}


module left(){
  translate([0, -case_h, 0]) color(c) difference(){
    hull(){
      translate([corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([heatsink_l - corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([heatsink_l - corner_r, case_h - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([corner_r, case_h - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
    }
  }
}

module right(){
  translate([0, -0, 0]) color(c) difference(){
    hull(){
      translate([corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([heatsink_l - corner_r, corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([heatsink_l - corner_r, case_h - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
      translate([corner_r, case_h - corner_r, 0]) cylinder(r = corner_r, h = wall_t);
    }
  }
}


module case(){
  difference(){
    union(){
      up();
      translate([0, -bend_a / 2, 0]) bend_bridge(heatsink_l);
      translate([0, heatsink_w + bend_a / 2, 0]) bend_bridge(heatsink_l);
    }
    translate([0, - bend_a / 2, 0]) bend_gap(heatsink_l);
    translate([0, heatsink_w + bend_a / 2, 0]) bend_gap(heatsink_l);
  }

  rotate([bend * 90, 0, 0]) {
    translate([0, - bend_a, 0]) difference(){
      union(){
        left();
        translate([0, bend_a / 2, 0]) bend_bridge(heatsink_l);
        translate([-bend_a / 2, 0, 0]) rotate([0, 0, -90]) bend_bridge(43.75 - space);
        translate([-bend_a / 2, -case_h, 0]) rotate([0, 0, 90]) bend_bridge(case_h - 46.75 - 8 - space);
        translate([heatsink_l + bend_a / 2, 0, 0]) rotate([0, 0, -90]) bend_bridge(52.25 + 4 + 3);
      }
      translate([0, bend_a / 2, 0]) bend_gap(heatsink_l);
      translate([-bend_a / 2, 0, 0]) rotate([0, 0, -90]) bend_gap(43.75 - space);
      translate([-bend_a / 2, -case_h, 0]) rotate([0, 0, 90]) bend_gap(case_h - 46.75 - 8 - space);
      translate([heatsink_l + bend_a / 2, 0, 0]) rotate([0, 0, -90]) bend_gap(52.25 + 4 + 3);
    }
    translate([- bend_a, - bend_a, 0]) rotate([0, -bend * 90, 0]) difference(){
      back_left();
      translate([bend_a / 2, 0, 0]) rotate([0, 0, -90]) bend_gap(43.75 - space);
      translate([bend_a / 2,  - case_h, 0]) rotate([0, 0, 90]) bend_gap(case_h - 46.75 - 8 - space);
    }
    translate([bend_a, -bend_a, 0]) translate([heatsink_l, 0, 0]) rotate([0, bend * 90, 0]) difference(){
      front_left();
      translate([-bend_a / 2, 0, 0]) rotate([0, 0, -90]) bend_gap(52.25 + 4 + 3);
    }
  }



  translate([0, heatsink_w + bend_a, 0]) rotate([-bend * 90, 0, 0]) {
    difference(){
      union(){
        right();
        translate([0, -bend_a / 2, 0]) bend_bridge(heatsink_l);
        translate([-bend_a / 2, 0, 0]) rotate([0, 0, 90]) bend_bridge(46.75 + 8 - 7 - space);
        translate([heatsink_l + bend_a / 2, 0, 0]) rotate([0, 0, 90]) bend_bridge(52.25 + 4 + 3);
        translate([-bend_a / 2, 46.75 + 8 + space, 0]) rotate([0, 0, 90]) bend_bridge(case_h - 46.75 - 8 - space);
      }
      translate([0, - bend_a / 2, 0]) bend_gap(heatsink_l);
      translate([- bend_a / 2, 0, 0]) rotate([0, 0, 90]) bend_gap(46.75 + 8 - 7 - space);
      translate([bend_a / 2 + heatsink_l, 0, 0]) rotate([0, 0, 90]) bend_gap(52.25 + 4 + 3);
      translate([- bend_a / 2, case_h, 0]) rotate([0, 0, -90]) bend_gap(case_h - 46.75 - 8 - space);
    }
    translate([- bend_a, 0, 0]) rotate([0, -bend * 90, 0]) difference(){
      back_right();
      translate([bend_a / 2, 0, 0]) rotate([0, 0, 90]) bend_gap(46.75 + 8 - 7 - space);
      translate([bend_a / 2, 46.75 + 8 + space, 0]) rotate([0, 0, 90]) bend_gap(case_h - 46.75 - 8 - space);
    }
    translate([heatsink_l + bend_a, 0, 0]) rotate([0, bend * 90, 0]) difference(){
      front_right();
      translate([- bend_a / 2, 0, 0]) rotate([0, 0, 90]) bend_gap(52.25 + 4 + 3);
    }
  }
}

//projection(cut = false) 
case();
//projection(cut = false) 
//up_text();
