module leg(length, width, height, thickness){
   color("silver") translate([length / 2 + length / 4, 0, thickness / 2]) cube([length / 2, width, thickness], true);
   color("silver") translate([length / 2, 0, height / 2 + thickness / 4]) cube([thickness, width, height + thickness / 2], true);
   color("silver") translate([length / 4, 0, height]) cube([length / 2, width, thickness], true);
}

module SMD_INDUCTOR_32x25(){
   color([0.2, 0.2, 0.2]) translate([0, 0, 1]) cube([3.2, 2.5, 2], true);
}
module Symbol_Danger_Top_Small(){}
module R_0805(){
   translate([0.0, 0.0, 0.5]) color([0.5, 0.5, 0.5]) cube([2, 1.25, 1.0], true);
}
module SOT_223(){
   color([0.2, 0.2, 0.2]) translate([0, 0, 0.8]) cube([6.5, 3.5, 1.6], true);
   rotate([0, 0, 90]) translate([3.5 / 2.0, 0, 0]) leg(1.75, 3, 0.8, 0.25);
   rotate([0, 0, 270]) translate([3.5 / 2.0, 0 * 2.3 - 2.0 * 2.3 / 2.0, 0]) leg(1.75, 0.7, 0.8, 0.25);
   rotate([0, 0, 270]) translate([3.5 / 2.0, 1 * 2.3 - 2.0 * 2.3 / 2.0, 0]) leg(1.75, 0.7, 0.8, 0.25);
   rotate([0, 0, 270]) translate([3.5 / 2.0, 2 * 2.3 - 2.0 * 2.3 / 2.0, 0]) leg(1.75, 0.7, 0.8, 0.25);

}
module R_2512(){
   translate([0.0, 0.0, 0.75]) color([0.5, 0.5, 0.5]) cube([6.35, 3.2, 1.5], true);
}
module C_1808(){
   translate([0.0, 0.0, 0.5]) color([0.8, 0.6, 0.0]) cube([4.6, 2.0, 1.0], true);
}
module SMA_Standard(){
   translate([0.0, 0.0, 1.15]) color([0.4, 0.4, 0.4]) cube([4.6, 2.9, 2.3], true);
}
module Measurement_Point_Round_SMD_Pad_Small(){}
module SOIC_8_N(){
   difference(){
      translate([0, 0, 1.0]) color([0.2, 0.2, 0.2]) cube([5, 4, 1.5], true);
      translate([0, -2, 2.0]) color([0.2, 0.2, 0.2]) rotate([-45, 0, 0]) cube([6, 1, 1], true);
   }
   for(a = [0:3]){
      rotate([0, 0, 90]) translate([4 / 2.0, a * 0.9 - 3.0 * 0.9 / 2.0, 0]) leg(1.1, 0.51, 0.7, 0.19);
      rotate([0, 0, 270]) translate([4 / 2.0, a * 0.9 - 3.0 * 0.9 / 2.0, 0]) leg(1.1, 0.51, 0.7, 0.19);
   }
}
module SOLDER_JUMPER_3_NC(){}
module C_0805(){
   translate([0.0, 0.0, 0.5]) color([0.8, 0.6, 0.0]) cube([2, 1.25, 1.0], true);
}
module Symbol_OSHW_Logo_SilkScreen(){}
module R_0603(){
   translate([0.0, 0.0, 0.3]) color([0.5, 0.5, 0.5]) cube([1.6, 0.8, 0.6], true);
}
module R_1206(){
   translate([0.0, 0.0, 0.5]) color([0.5, 0.5, 0.5]) cube([3.2, 1.6, 1.0], true);
}
module Crystal_SMD_0603_4Pads(){
   color([0.8, 0.8, 0.8]) translate([0, 0, 0.5]) cube([5, 3.5, 1], true);
}
module AC(){}
module C_0603(){
   translate([0.0, 0.0, 0.3]) color([0.8, 0.6, 0.0]) cube([1.6, 0.8, 0.6], true);
}
module LQFP_48_7x7mm_Pitch0_5mm(){
   color([0.2, 0.2, 0.2]) translate([0, 0, 0.8]) cube([7, 7, 1.6], true);
   for(a = [0:11]){
     rotate([0, 0, 0]) translate([7.0 / 2.0, a * 0.5 - 11.0 * 0.5 / 2.0, 0]) leg(1.0, 0.22, 0.8, 0.1);
     rotate([0, 0, 90]) translate([7.0 / 2.0, a * 0.5 - 11.0 * 0.5 / 2.0, 0]) leg(1.0, 0.22, 0.8, 0.1);
     rotate([0, 0, 180]) translate([7.0 / 2.0, a * 0.5 - 11.0 * 0.5 / 2.0, 0]) leg(1.0, 0.22, 0.8, 0.1);
     rotate([0, 0, 270]) translate([7.0 / 2.0, a * 0.5 - 11.0 * 0.5 / 2.0, 0]) leg(1.0, 0.22, 0.8, 0.1);
  }
}
module Symbol_HighVoltage_Type2_Top_VerySmall(){}
module SOLDER_JUMPER(){}
module ESD(){}
module SOIC_7(){
   SOIC_8_N();
}
module LQFP_100_14x14mm_Pitch0_5mm(){
   color([0.2, 0.2, 0.2]) translate([0, 0, 0.8]) cube([14, 14, 1.6], true);
   for(a = [0:24]){
     rotate([0, 0, 0]) translate([14.0 / 2.0, a * 0.5 - 24.0 * 0.5 / 2.0, 0]) leg(1.0, 0.22, 0.8, 0.1);
     rotate([0, 0, 90]) translate([14.0 / 2.0, a * 0.5 - 24.0 * 0.5 / 2.0, 0]) leg(1.0, 0.22, 0.8, 0.1);
     rotate([0, 0, 180]) translate([14.0 / 2.0, a * 0.5 - 24.0 * 0.5 / 2.0, 0]) leg(1.0, 0.22, 0.8, 0.1);
     rotate([0, 0, 270]) translate([14.0 / 2.0, a * 0.5 - 24.0 * 0.5 / 2.0, 0]) leg(1.0, 0.22, 0.8, 0.1);
  }
}
module LED_0805(){
   translate([0.0, 0.0, 0.5]) color([0.4, 0.4, 0.4]) cube([2, 1, 1.0], true);
   translate([0.0, 0.2, 0.05]) color([1.0, 0.0, 0.0]) cylinder(r = 0.5, h = 0.9, $fn = 20);
   
}
module USB_Micro_B_10103594_0001LF(){
   rotate([90, 0, 0]) translate([-4, 0, -2.5]) difference(){
    color("silver") linear_extrude(height = 5.06, convexity = 10, twist = 0) polygon( points = [[1.1, 0], [0, 1.1], [0, 2.5], [7.8, 2.5], [7.8, 1.1], [6.7, 0]]);
     //color("silver") cube([7.8, 2.5, 5.06]);
    //color("silver") translate([7.8 / 2, 2, 2.5 / 2]) cube([7, 5, 1.8], true);
    color("silver") translate([0, -0.4, 0.1]) linear_extrude(height = 5, convexity = 10, twist = 0) polygon( points = [[1.2, 0.7], [0.4, 1.5], [0.4, 0.7 + 1.8], [0.4 + 7, 0.7 + 1.8], [0.4 + 7, 1.5], [6.6, 0.7]]);
  }
}
module SOT_23(){
   color([0.2, 0.2, 0.2]) translate([0, 0, 0.5]) cube([2.9, 1.3, 1.0], true);
   rotate([0, 0, 90]) translate([1.3 / 2.0, 1 * 0.9 - 2.0 * 0.9 / 2.0, 0]) leg(0.55, 0.4, 0.6, 0.11);
   rotate([0, 0, 270]) translate([1.3 / 2.0, 0 * 0.9 - 2.0 * 0.9 / 2.0, 0]) leg(0.55, 0.4, 0.6, 0.11);
   rotate([0, 0, 270]) translate([1.3 / 2.0, 2 * 0.9 - 2.0 * 0.9 / 2.0, 0]) leg(0.55, 0.4, 0.6, 0.11);
}
module SOT_23_6(){
   color([0.2, 0.2, 0.2]) translate([0, 0, 0.5]) cube([1.3, 2.9, 1.0], true);
   for(a = [0:2]){
      rotate([0, 0, 0]) translate([1.3 / 2.0, a * 0.9 - 2.0 * 0.9 / 2.0, 0]) leg(0.55, 0.4, 0.6, 0.11);
      rotate([0, 0, 180]) translate([1.3 / 2.0, a * 0.9 - 2.0 * 0.9 / 2.0, 0]) leg(0.55, 0.4, 0.6, 0.11);
   }
}
module heatsink(l){
   translate([46, 0, 0]) rotate([90, 0, 180]) color([0.3, 0.3, 0.3]) linear_extrude(height = 94, convexity = 10, twist = 0) polygon( points=[[0, 1], [0, 9.5], [7, 9.5], 
   [7, 12.5], [0, 12.5], [0, 32], [1, 33], 
   [3.5, 33], [3.5, 25], [6.5, 25], [6.5, 33], 
   [39.5, 33], [39.5, 25], [42.5, 25], [42.5, 33], 
   [45, 33], [46, 32], [46, 17.5], [39, 17.5], 
   [39, 14.5], [46, 14.5], [46, 1], [45, 0], [43, 0], 
   [43, 7], [40, 7], [40, 0], [38, 0], [37, 1], 
   [37, 27], [35, 29], [33.5, 29], [31.5, 27], 
   [31.5, 3], [30.5, 2], [29.5, 3], [29.5, 27], 
   [27.5, 29], [26, 29], [24, 27], [24, 3], [23, 2], 
   [22, 3], [22, 27], [20, 29], [18.5, 29], [16.5, 27], 
   [16.5, 3], [15.5, 2], [14.5, 3], [14.5, 27], 
   [12.5, 29], [11, 29], [9, 27], [9, 1], [8, 0], 
   [6, 0], [6, 7], [3, 7], [3, 0], [1, 0]]);
}
module Socket_Strip_Straight_1x04(){}
module Measurement_Point_Round_SMD_Pad_Small(){}
module Pin_Header_Straight_1x04(){}
module C_Radial_D26_L45_P10(){
   color([0.2, 0.2, 0.2]) cylinder(r = 13, h = 50, $fn = 50);
   color("silver") translate([5, 0, -3.0]) cylinder(r = 0.75, h = 3, $fn = 50);
   color("silver") translate([-5, 0, -3.0]) cylinder(r = 0.75, h = 3, $fn = 50);
}
module NPI31W(){}
module Fiducial_1mm_Outer_CopperTop(){}
module akl182_6(){
   difference(){
    translate([-3.5/2 - 0.7, -8, 0]) color("green") cube([6 * 3.5 + 1.4, 9.2, 7.0]);
    for(a = [0:5]){
         color("green") translate([a * 3.5 -3.5/2, -9, 1]) cube([3.2, 9, 5]);
      }
   }
   for(a = [0:5]){
         color("gold") translate([a * 3.5, -4 + 0.8/4, 3]) cube([0.8, 8 + 0.8 / 2, 0.8], true);
   }
   for(a = [0:5]){
         color("gold") translate([a * 3.5, 0, -0.5 / 2]) cube([0.8, 0.8, 3.5 + 3], true);
   }
}
module Pin_Header_Angled_1x02(){}
module akl182_2(){
   difference(){
    translate([-3.5/2 - 0.7, -8, 0]) color("green") cube([2 * 3.5 + 1.4, 9.2, 7.0]);
    for(a = [0:1]){
         color("green") translate([a * 3.5 -3.5/2, -9, 1]) cube([3.2, 9, 5]);
      }
   }
   for(a = [0:1]){
         color("gold") translate([a * 3.5, -4 + 0.8/4, 3]) cube([0.8, 8 + 0.8 / 2, 0.8], true);
   }
   for(a = [0:1]){
         color("gold") translate([a * 3.5, 0, -0.5 / 2]) cube([0.8, 0.8, 3.5 + 3], true);
   }
}
module Pin_Header_Angled_1x04(){
 //   for(a = [0:3]){
 //      color("gold") translate([0, -a * 2.54, -(3 + 1.27) / 2 + 1.27]) cube([0.64, 0.64, 3 + 1.27], true);
 //      color("gold") translate([(6 + 2.54 + 1.5) / 2 - 0.64 / 4, -a * 2.54, 1.27]) cube([6 + 2.54 + 1.5 + 0.64 / 2, 0.64, 0.64], true);
 // }   
 //   color([0.2, 0.2, 0.2]) translate([2.54 / 2, - 4 * 2.54 + 2.54 / 2, 0]) cube([2.54, 4 * 2.54, 2.54]);
}
module IRAM256(){
   translate([-62/2, -22.3 / 2, 8]) rotate([180, 0, 90]){
      difference(){
       color([0.2, 0.2, 0.2]) cube([22.3, 62, 5.7]);
       translate([11.4, 3, -1]) color([0.2, 0.2, 0.2]) cylinder(r = 1.7, h = 7);
       translate([11.4, 3 + 56, -1]) color([0.2, 0.2, 0.2]) cylinder(r = 1.7, h = 7);
       translate([11.4 - 3.4/2, -0.1, -0.1]) color([0.2, 0.2, 0.2]) cube([3.4, 3.1, 6]);
       translate([11.4 - 3.4/2, 56 + 3, -0.1]) color([0.2, 0.2, 0.2]) cube([3.4, 3.1, 6]);
       translate([0, 62, 2.9]) rotate([0, 0, 45]) color([0.2, 0.2, 0.2]) cube([5, 5, 6], true);
       translate([0, 0, 2.9]) rotate([0, 0, 45]) color([0.2, 0.2, 0.2]) cube([5, 5, 6], true);
       translate([22.3, 62, 2.9]) rotate([0, 0, 45]) color([0.2, 0.2, 0.2]) cube([5, 5, 6], true);
       translate([22.3, 0, 2.9]) rotate([0, 0, 45]) color([0.2, 0.2, 0.2]) cube([5, 5, 6], true);
     }
     for(a = [0:6]){
       translate([-(16.75 - 11.4) / 2, 62/2 - a * 2.54 - 2.54, 3.2]) color("silver") cube([16.75 - 11.4, 0.6, 0.5], true);
       translate([-(16.75 - 11.4), 62/2 - a * 2.54 - 2.54, (4.7 + 5.7 - 3.2)/2 + 3.2]) color("silver") cube([0.6, 0.5, 4.7 + 5.7 - 3.2], true);
       translate([-(16.75 - 11.4 + 5) / 2, 62/2 - a * 2.54 - 1.27, 3.2]) color("silver") cube([16.75 - 11.4 + 5, 0.6, 0.5], true);
       translate([-(16.75 - 11.4 + 5), 62/2 - a * 2.54 - 1.27, (4.7 + 5.7 - 3.2)/2 + 3.2]) color("silver") cube([0.6, 0.5, 4.7 + 5.7 - 3.2], true);
     }
     
     for(a = [0:3]){
       translate([-(16.75 - 11.4) / 2, 62/2 + a * 5.08 + 2.54, 3.2]) color("silver") cube([16.75 - 11.4, 0.6, 0.5], true);
       translate([-(16.75 - 11.4), 62/2 + a * 5.08 + 2.54, (4.7 + 5.7 - 3.2)/2 + 3.2]) color("silver") cube([0.6, 0.5, 4.7 + 5.7 - 3.2], true);
   }
   for(a = [1:3]){
       translate([-(16.75 - 11.4 + 5) / 2, 62/2 + a * 5.08 + 1.27, 3.2]) color("silver") cube([16.75 - 11.4 + 5, 0.6, 0.5], true);
       translate([-(16.75 - 11.4 + 5), 62/2 + a * 5.08 + 1.27, (4.7 + 5.7 - 3.2)/2 + 3.2]) color("silver") cube([0.6, 0.5, 4.7 + 5.7 - 3.2], true);
     }   
  }
}
module RJ45_LED(){
   rotate([0, 0, 180]) translate([-16.5 / 2, 0, 0]){
      difference(){
         color("silver") cube([16.5, 15.6, 13.2], false);
         translate([0.25, 0,0]){
            color([0.2, 0.2, 0.2]) translate([8, 3.9, 13.33/2]) cube([11.5, 8, 7.5], true);
            color([0.2, 0.2, 0.2]) translate([8, 3.9, 13.33-2]) cube([6, 8, 2], true);
            color("grey") translate([16-3, 0.45, 13.33-2]) cube([4, 1, 2], true);
            color("grey") translate([3, 0.45, 13.33-2]) cube([4, 1, 2], true);
         }
      }
      translate([0.25, 0,0]){
         color("green") translate([16-3, 0.5, 13.33-2]) cube([4, 1, 2], true);
         color("yellow") translate([3, 0.5, 13.33-2]) cube([4, 1, 2], true);
     
         color([0.2, 0.2, 0.2]) translate([(16 - 12.7) / 2, 5.4, -3]) cylinder(r = 1.6, h = 3, $fn = 20);
         color([0.2, 0.2, 0.2]) translate([(16 - 12.7) / 2 + 12.7, 5.4, -3]) cylinder(r = 1.6, h = 3, $fn = 20);
         color("silver") translate([-0.13, 5.4 + 3.54, -3]) cylinder(r = 0.6, h = 3, $fn = 20);
         color("silver") translate([16.26, 5.4 + 3.54, -3]) cylinder(r = 0.6, h = 3, $fn = 20);
         color("silver") translate([(16 - 13.72) / 2, 5.4 + 9.12, -3]) cylinder(r = 0.6, h = 3, $fn = 20);
         color("silver") translate([(16 - 9.14) / 2, 5.4 + 9.12, -3]) cylinder(r = 0.6, h = 3, $fn = 20);
         color("silver") translate([(16 - 13.72) / 2 + 13.72, 5.4 + 9.12, -3]) cylinder(r = 0.6, h = 3, $fn = 20);
         color("silver") translate([(16 - 9.14) / 2 + 9.14, 5.4 + 9.12, -3]) cylinder(r = 0.6, h = 3, $fn = 20);
     
         for(a = [0:3]){
            color("silver") translate([(16 - 12.7) / 2 + 2.79 + a * 2.03 + 1.02, 5.4 + 2.54, -3]) cylinder(r = 0.6, h = 3, $fn = 20);
            color("silver") translate([(16 - 12.7) / 2 + 2.79 + a * 2.03, 5.4 + 4.32, -3]) cylinder(r = 0.6, h = 3, $fn = 20);
         }
      }
   }
}
module RM5_08_1x2(){
   difference(){
    translate([-5.08/2 - 0.6, -2, 0]) color("green") cube([2 * 5.08 + 1.2, 12, 7.8]);
    for(a = [0:1]){
         color("green") translate([a * 5.08 -5.09/2, 2, 1]) cube([5.09, 9, 5]);
      }
   }
   for(a = [0:1]){
         color("gold") translate([a * 5.08, 5 - 1/4, 3.8]) cube([1, 10 + 1 / 2, 1], true);
   }
   for(a = [0:1]){
         color("gold") translate([a * 5.08, 0, 0.175]) cube([1, 1, 3.45 + 3.8], true);
   }
}

module RM5_08_1x3(){
   difference(){
    translate([-5.08/2 - 0.6, -2, 0]) color("green") cube([3 * 5.08 + 1.2, 12, 7.8]);
    for(a = [0:2]){
         color("green") translate([a * 5.08 -5.09/2, 2, 1]) cube([5.09, 9, 5]);
      }
   }
   for(a = [0:2]){
         color("gold") translate([a * 5.08, 5 - 1/4, 3.8]) cube([1, 10 + 1 / 2, 1], true);
   }
   for(a = [0:2]){
         color("gold") translate([a * 5.08, 0, 0.175]) cube([1, 1, 3.45 + 3.8], true);
   }
}
module Socket_Strip_Angled_2x06(){
   for(a = [0:5]){
      color("gold") translate([a * 2.54, 0, -(3 + 1.27 + 2.54) / 2 + 1.27 + 2.54]) cube([0.64, 0.64, 3 + 1.27 + 2.54], true);
      color("gold") translate([a * 2.54, -2.54, -(3 + 1.27) / 2 + 1.27]) cube([0.64, 0.64, 3 + 1.27], true);
   }
   for(a = [0:5]){
      color("gold") translate([a * 2.54, -(6 + 2.54 * 2 + 1.5 - 8) / 2 + 0.64 / 4, 1.27 + 2.54]) cube([0.64, 6 + 2.54 + 1.5 + 2.54 - 8 + 0.64 / 2, 0.64], true);
      color("gold") translate([a * 2.54, -(6 + 2.54 + 1.5 - 8) / 2 - 2.54 + 0.64 / 4, 1.27]) cube([0.64, 6 + 2.54 + 1.5 - 8 + 0.64 / 2, 0.64], true);
   }
   difference(){
      color([0.2, 0.2, 0.2]) translate([-1.27, -2.54 - 6 - 2.54 - 1.27, 0]) cube([6 * 2.54, 2.54 + 6, 5.08]);
      for(a = [0:5]){
         color("gold") translate([a * 2.54, -2.54 - 1.27 - 6 - 6 / 2, 1.27]) cube([0.64, 6, 0.64], true);
         color("gold") translate([a * 2.54, -2.54 - 1.27 - 6 - 6 / 2, 1.27 + 2.54]) cube([0.64, 6, 0.64], true);
      }
   }
}

module fan(){
	difference(){
		color([0.2, 0.2, 0.2]) cube([45, 45, 10.5], false);
		color([0.2, 0.2, 0.2]) translate([22.5, 22.5, -1]) cylinder(r = 20, h = 12);
    color([0.2, 0.2, 0.2]) translate([4, 4, -1]) cylinder(r = 1.5, h = 12);
    color([0.2, 0.2, 0.2]) translate([4 + 37, 4, -1]) cylinder(r = 1.5, h = 12);
    color([0.2, 0.2, 0.2]) translate([4, 4 + 37, -1]) cylinder(r = 1.5, h = 12);
    color([0.2, 0.2, 0.2]) translate([4 + 37, 4 + 37, -1]) cylinder(r = 1.5, h = 12);
	}
	color([0.2, 0.2, 0.2]) translate([22.5, 22.5, 0]) cylinder(r = 10, h = 10);
  for(a = [0:6]){
    color([0.2, 0.2, 0.2]) translate([22.5, 22.5, 0]) rotate([0, 0, 360 / 7 * a]) translate([12, 0, 7.5]) rotate([30, 0, 0]) cube([14, 7, 1], true); 
  }
}
