pcb_hight = 1.6;

module capp(){
	color("grey") cylinder($fn=100, r = 12.5, h = 50);
	color("gold") translate([5, 0, 0]) cube([1, 1, 6], true);
	color("gold") translate([-5, 0, 0]) cube([1, 1, 6], true);
}

module cap(){
	color("red") cube([8, 18, 15]);
	color("gold") translate([4.5, 18 / 2 - 15 / 2, 0]) cube([1, 1, 6], true);
	color("gold") translate([4.5, 18 / 2 + 15 / 2, 0]) cube([1, 1, 6], true);
}

module rj45(){
	difference(){
		color("silver") cube([16, 15.6, 13.2], false);
		color("grey") translate([8, 3.9, 13.33/2]) cube([11.5, 8, 7.5], true);
		color("grey") translate([8, 3.9, 13.33-2]) cube([6, 8, 2], true);
		color("grey") translate([16-3, 0.45, 13.33-2]) cube([4, 1, 2], true);
		color("grey") translate([3, 0.45, 13.33-2]) cube([4, 1, 2], true);
	}
	color("green") translate([16-3, 0.5, 13.33-2]) cube([4, 1, 2], true);
	color("yellow") translate([3, 0.5, 13.33-2]) cube([4, 1, 2], true);
  
  color("grey") translate([(16 - 12.7) / 2, 5.4, -3]) cylinder($fn = 100, r = 1.6, h = 3);
  color("grey") translate([(16 - 12.7) / 2 + 12.7, 5.4, -3]) cylinder($fn = 100, r = 1.6, h = 3);
  color("silver") translate([0, 5.4 + 3.54, -3]) cylinder($fn = 100, r = 0.6, h = 3);
  color("silver") translate([16, 5.4 + 3.54, -3]) cylinder($fn = 100, r = 0.6, h = 3);
  color("silver") translate([(16 - 13.72) / 2, 5.4 + 9.12, -3]) cylinder($fn = 100, r = 0.6, h = 3);
  color("silver") translate([(16 - 9.14) / 2, 5.4 + 9.12, -3]) cylinder($fn = 100, r = 0.6, h = 3);
  color("silver") translate([(16 - 13.72) / 2 + 13.72, 5.4 + 9.12, -3]) cylinder($fn = 100, r = 0.6, h = 3);
  color("silver") translate([(16 - 9.14) / 2 + 9.14, 5.4 + 9.12, -3]) cylinder($fn = 100, r = 0.6, h = 3);
  
  for(a = [0:3]){
    color("silver") translate([(16 - 12.7) / 2 + 2.79 + a * 2.03, 5.4 + 2.54, -3]) cylinder($fn = 100, r = 0.6, h = 3);
    color("silver") translate([(16 - 12.7) / 2 + 2.79 + a * 2.03 + 1.02, 5.4 + 4.32, -3]) cylinder($fn = 100, r = 0.6, h = 3);
  }
}

module soic8(){
  translate([1.1, 0, 0]) color("grey") cube([4, 5, 1.75]);
  for(a = [0:3]){
    color("silver") translate([1.1 / 2, 0.33 + a * 1.27 + 0.51 / 2]) cube([1.1, 0.51, 0.19], true);
    color("silver") translate([1.1 / 2 + 4 + 1.1, 0.33 + a * 1.27 + 0.51 / 2]) cube([1.1, 0.51, 0.19], true);
  }
}

module r2512(){
  color("silver") cube([1.5, 3.2, 1.6]);
  translate([1.5, 0, 0]) color("grey") cube([3.35, 3.2, 1.6]);
  translate([1.5 + 3.35, 0, 0]) color("silver") cube([1.5, 3.2, 1.6]);
}

module akl230(l){
	difference(){
		color("grey") cube([l * 5.08 + 1.2, 12, 7.8]);
		for(a = [0:l - 1]){
			color("grey") translate([0.6 + a * 5.08, 4, 1.4]) cube([5.09, 9, 5]);
		}
	}
	for(a = [0:l - 1]){
			color("gold") translate([0.6 + a * 5.08 + 5.08/2, 7, 3.8]) cube([1, 10, 1], true);
	}
	for(a = [0:l - 1]){
			color("gold") translate([0.6 + a * 5.08 + 5.08/2, 2, 0.175]) cube([1, 1, 3.45 + 3.8], true);
	} 
}

module akl182(l){
	difference(){
		color("grey") cube([l * 3.5 + 1.6, 11.2, 8.7]);
		for(a = [0:l - 1]){
			color("grey") translate([0.8 + a * 3.5, 4, 1.4]) cube([3.51, 9, 5]);
		}
	}
	for(a = [0:l - 1]){
			color("gold") translate([0.8 + a * 3.5 + 3.5/2, 11.2 - 8.9 / 2, 3.1]) cube([1, 8.9, 1], true);
	}
	for(a = [0:l - 1]){
			color("gold") translate([0.8 + a * 3.5 + 3.5/2, 11.2 - 8.9, -0.2]) cube([1, 1, 3.5 + 3.1], true);
	} 
}

module pin2(l){
  for(a = [0:l - 1]){
		color("gold") translate([2.54 / 2 + a * 2.54, 1.5 + 2.54, -(3 + 1.27) / 2 + 1.27]) cube([0.64, 0.64, 3 + 1.27], true);
    color("gold") translate([2.54 / 2 + a * 2.54, 2.54 + 1.5 + 2.54, -(3 + 1.27 + 2.54) / 2 + 1.27 + 2.54]) cube([0.64, 0.64, 3 + 1.27 + 2.54], true);
	}
  for(a = [0:l - 1]){
		color("gold") translate([2.54 / 2 + a * 2.54, -(6 + 2.54 + 1.5) / 2 + 1.5 + 2.54, 1.27]) cube([0.64, 6 + 2.54 + 1.5, 0.64], true);
    color("gold") translate([2.54 / 2 + a * 2.54, -(6 + 2.54 + 1.5 + 2.54) / 2 + 1.5 + 2.54 + 2.54, 1.27 + 2.54]) cube([0.64, 6 + 2.54 + 1.5 + 2.54, 0.64], true);
	}
  color("grey") translate([0, 0, 0]) cube([l * 2.54, 2.54, 5.08]);
}

module soc2(l){
  for(a = [0:l - 1]){
		color("gold") translate([2.54 / 2 + a * 2.54, 2.54 / 2, -3 / 2]) cube([0.64, 0.64, 3], true);
    color("gold") translate([2.54 / 2 + a * 2.54, 2.54 + 2.54 / 2, -3 / 2]) cube([0.64, 0.64, 3], true);
	}
  difference(){
    color("grey") translate([0, 0, 0]) cube([l * 2.54, 5.08, 2.54 + 6]);
    for(a = [0:l - 1]){
		color("gold") translate([2.54 / 2 + a * 2.54, 2.54 / 2, 5]) cube([0.64, 0.64, 10], true);
    color("gold") translate([2.54 / 2 + a * 2.54, 2.54 + 2.54 / 2, 5]) cube([0.64, 0.64, 10], true);
	}
  }
}

module usb(){
  difference(){
    color("silver") cube([7.8, 5.06, 2.5]);
    color("silver") translate([7.8 / 2, 2, 2.5 / 2]) cube([7, 5, 1.8], true);
  }
}

module heatsink(l){
  translate([46, 0, 0]) rotate([90, 0, 180]) color("grey") linear_extrude(height = 94, convexity = 10, twist = 0) polygon( points=[[0, 1], [0, 9.5], [7, 9.5], 
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
	//difference(){
	//	color("grey") cube([46, 94, 33]);
	//	color("grey") translate([8, -1, -1]) cube([5, 97, 30]);
	//	color("grey") translate([16, -1, -1]) cube([5, 97, 30]);
	//	color("grey") translate([24, -1, -1]) cube([5, 97, 30]);
	//	color("grey") translate([32, -1, -1]) cube([5, 97, 30]);
	//}
}

module fan(){
	difference(){
		color("grey") cube([45, 45, 10], false);
		color("grey") translate([22.5, 22.5, -1]) cylinder($fn=100, r = 20, h = 12);
    color("grey") translate([4, 4, -1]) cylinder($fn=100, r = 1.5, h = 12);
    color("grey") translate([4 + 37, 4, -1]) cylinder($fn=100, r = 1.5, h = 12);
    color("grey") translate([4, 4 + 37, -1]) cylinder($fn=100, r = 1.5, h = 12);
    color("grey") translate([4 + 37, 4 + 37, -1]) cylinder($fn=100, r = 1.5, h = 12);
	}
	color("grey") translate([22.5, 22.5, 0]) cylinder($fn=100, r = 10, h = 10);
}

module lqfp100(){
  color("grey") translate([1, 1, 0]) cube([14, 14, 1.6]);
  for(a = [0:24]){
    color("silver") translate([a * 0.5 + 2, 0.5, 0.05]) cube([0.22, 1, 0.1], true);
    color("silver") translate([a * 0.5 + 2, 0.5 + 15, 0.05]) cube([0.22, 1, 0.1], true);
    color("silver") translate([0.5, a * 0.5 + 2, 0.05]) cube([1, 0.22, 0.1], true);
    color("silver") translate([0.5 + 15, a * 0.5 + 2, 0.05]) cube([1, 0.22, 0.1], true);
  }
}

module lqfp48(){
  color("grey") translate([1, 1, 0]) cube([7, 7, 1.6]);
  for(a = [0:11]){
    color("silver") translate([a * 0.5 + 1.75, 0.5, 0.05]) cube([0.22, 1, 0.1], true);
    color("silver") translate([a * 0.5 + 1.75, 0.5 + 8, 0.05]) cube([0.22, 1, 0.1], true);
    color("silver") translate([0.5, a * 0.5 + 1.75, 0.05]) cube([1, 0.22, 0.1], true);
    color("silver") translate([0.5 + 8, a * 0.5 + 1.75, 0.05]) cube([1, 0.22, 0.1], true);
  }
}

module iram256(){
  difference(){
    color("grey") cube([22.3, 62, 5.7]);
    translate([11.4, 3, -1]) color("grey") cylinder($fn=100, r = 1.7, h = 7);
    translate([11.4, 3 + 56, -1]) color("grey") cylinder($fn=100, r = 1.7, h = 7);
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

module f4_pcb(){
	difference(){
    translate([0, 1, 0]) color("green") cube([50, 100, pcb_hight], false);
    color("green") translate([44, 93.5, -1]) cube([10, 16, 4], false);
  }
}

module f1_pcb(){
	difference(){
    color("green") translate([0, 5, 0]) cube([46, 90, pcb_hight], false);
    color("green") translate([32.5, 30, -1]) cylinder($fn = 100, r = 3, h = 3);
    color("green") translate([32.5, 30 + 56, -1]) cylinder($fn = 100, r = 3, h = 3);
    color("green") translate([5, 10, -1]) cylinder($fn = 100, r = 1.5, h = 3);
    color("green") translate([5, 28, -1]) cylinder($fn = 100, r = 1.5, h = 3);
    color("green") translate([5 + 36, 10, -1]) cylinder($fn = 100, r = 1.5, h = 3);
  }
}

module cover(){
  difference(){
    translate([-1, -9, 54]) color("silver") cube([48, 104, 0.95]);
    translate([5, 61, 53]) color("silver") cube([9.5, 23.5, 3]);
    translate([11, 43, 53]) color("silver") cube([3.5, 11, 3]);
    translate([8.5, 52, 53]) color("silver") cube([6, 10, 3]);
    translate([8.5, 15.5, 53]) color("silver") cube([6, 26, 3]);
    translate([4, -5, 53]) color("silver") cylinder($fn = 100, r = 1.25, h = 3);
    translate([42, -5, 53]) color("silver") cylinder($fn = 100, r = 1.25, h = 3);
  }
  translate([40, 0, 55]) color("black") rotate([0, 0, 90]) text("STMBL 3.4", font = "Liberation Sans", size = 13);
  //translate([20, 4, 55]) color("black") rotate([0, 0, 90]) text("IO", font = "Liberation Sans", size = 3);
  //translate([15, 10, 55]) color("black") rotate([0, 0, 90]) text("1", font = "Liberation Sans", size = 2);
  //translate([15, 32, 55]) color("black") rotate([0, 0, 90]) text("10", font = "Liberation Sans", size = 2);
  //translate([24, 10, 55]) color("black") rotate([0, 0, 90]) text("11", font = "Liberation Sans", size = 2);
  //translate([24, 32, 55]) color("black") rotate([0, 0, 90]) text("20", font = "Liberation Sans", size = 2);
  //translate([6, 38, 55]) color("black") rotate([0, 0, 90]) text("FB0", font = "Liberation Sans", size = 2);
  //translate([8, 38, 55]) color("black") rotate([0, 0, 90]) text("5V 12V", font = "Liberation Sans", size = 2);
  //translate([19, 38, 55]) color("black") rotate([0, 0, 90]) text("FB1", font = "Liberation Sans", size = 2);
  //translate([17, 38, 55]) color("black") rotate([0, 0, 90]) text("5V 12V", font = "Liberation Sans", size = 2);
  //translate([10, 48, 55]) color("black") rotate([0, 0, 90]) text("USB", font = "Liberation Sans", size = 2);
  translate([-1, -10, -12]) color("silver") cube([1, 105, 66]);
  difference(){
    translate([-1, 95, -12]) color("silver") cube([23.8, 0.95, 66]);
    translate([-2, 94, 1]) color("silver") cube([27, 3, 9]);
    translate([15, 94, 22.5]) color("silver") cube([9.5, 3, 333]);
    translate([5, 97, -10]) color("silver") rotate([90, 0, 0]) cylinder($fn = 100, r = 1.5, h = 3);
  }
  
  difference(){
    translate([0, -9.95, -12]) color("silver") cube([15.5, 1, 66]);
    //translate([15.5, -11, 4.5]) color("silver") cube([8, 3, 50]);
    translate([4.5, -11, 0]) color("silver") rotate([-90, 0, 0]) cylinder($fn=100, r = 1.5, h = 3);
    translate([23, -11, -18.5]) color("silver") rotate([-90, 0, 0]) cylinder($fn=100, r = 20, h = 3);
  }
  translate([-1 + 46 + 1, -10, -12]) color("silver") cube([1, 105, 66]);
  difference(){
    translate([23, 95, -12]) color("silver") cube([24, 0.95, 66]);
    translate([37, 94, 1]) color("silver") cube([11, 3, 9.5]);
    translate([23, 94, 1]) color("silver") cube([5, 3, 9]);
    translate([15, 94, 22.5]) color("silver") cube([9.5, 3, 33]);
    translate([5 + 36, 97, -10]) color("silver") rotate([90, 0, 0]) cylinder($fn = 100, r = 1.5, h = 3);
  }
  difference(){
    translate([29, -9.95, -12]) color("silver") cube([17, 1, 66]);
    //translate([21, -11, 0]) color("silver") cube([8, 3, 64]);
    translate([4.5 + 37, -11, 0]) color("silver") rotate([-90, 0, 0]) cylinder($fn=100, r = 1.5, h = 3);
    translate([23, -11, -18.5]) color("silver") rotate([-90, 0, 0]) cylinder($fn=100, r = 20, h = 3);
  }
 
}

module f4(){
	f4_pcb();
	translate([1, 0, pcb_hight]) rj45();
	translate([1 + 16, 0, pcb_hight]) rj45();
	translate([1 + 16 + 16, 0, pcb_hight]) rj45();
  translate([0, 105 - 11.2 + 1, pcb_hight]) rotate([0, 0, 0]) akl182(6);
  translate([6 * 3.5 + 1.6, 105 - 11.2 + 1, pcb_hight]) rotate([0, 0, 0]) akl182(2);
  translate([11.2 - 1, 94 , 0]) rotate([0, 180, 90]) akl182(6);
  translate([-1, 54, 0]) rotate([0, 180, -90]) usb();
  translate([6 - 1, 26, 0]) rotate([0, 180, -90]) pin2(10);
  translate([6 - 1, 63, 0]) rotate([0, 180, -90]) pin2(3);
  translate([1 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([1 + 0.25 + 5 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([1 + 0.25 + 5 + 0.25 + 5 + 0.25 + 2.5, 28, 0]) rotate([180, 0, 90]) soic8();
  translate([1 + 0.25 + 5 + 0.25 + 5 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([17 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([17 + 0.25 + 5 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([17 + 0.25 + 5 + 0.25, 28, 0]) rotate([180, 0, 90]) soic8();
  translate([17 + 0.25 + 5 + 0.25+ 5 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([33 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([33 + 0.25 + 5 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([33 + 0.25 + 5 + 0.25, 28, 0]) rotate([180, 0, 90]) soic8();
  translate([33 + 0.25 + 5 + 0.25+ 5 + 0.25, 18, 0]) rotate([180, 0, 90]) soic8();
  translate([50, 41, 0]) rotate([0, 180, 90]) pin2(2);
  translate([20, 60, 0]) rotate([180, 0, 0]) lqfp100();
}

module f1(){
	f1_pcb();
	translate([32, 45, pcb_hight]) rotate([0, 0, -45]) capp();
	translate([32, 45 + 26, pcb_hight]) rotate([0, 0, 45]) capp();
	translate([2, 45, pcb_hight]) cap();
	translate([0, 96 - 12, pcb_hight]) akl230(3);
	translate([3 * 5.08 + 1.2, 96 - 12, pcb_hight]) akl230(2);
  translate([46 - 2 * 3.5 - 1.6, 96 - 11.2, pcb_hight]) akl182(2);
  translate([20, 45, 0]) rotate([180, 0, 0]) lqfp48();
  translate([15, 31, 0]) rotate([180, 0, 0]) soic8();
  translate([5, 40, 0]) rotate([180, 0, 0]) soic8();
  translate([21, 27, -8]) iram256();
  translate([9.5,49,0]) rotate([0, 180, 0]) r2512();
  translate([9.5,52.5,0]) rotate([0, 180, 0]) r2512();
  translate([9.5,56,0]) rotate([0, 180, 0]) r2512();
}

module spacer(l){
  difference(){
    color("grey") cylinder($fn = 100, r = 3, h = l);
    translate([0, 0, -1]) color("grey") cylinder($fn = 100, r = 1.5, h = l + 2);
  }
}

module m3(l){
  translate([0, 0, -l]) color("silver") cylinder($fn = 100, r = 1.5, h = l);
  color("silver") cylinder($fn = 100, r = 2.5, h = 2);
}

module ethercat(){
  difference(){
    color("green") cube([46, 51, pcb_hight]);
    translate([4, 4, -1]) color("green") cylinder($fn = 100, r = 1.5, h = 3);
    translate([4 + 38, 4, -1]) color("green") cylinder($fn = 100, r = 1.5, h = 3);
  }
  translate([46 / 2 - 16, -1, pcb_hight]) rj45();
  translate([46 / 2, -1, pcb_hight]) rj45();
  translate([9, 25, 0]) rotate([180, 0, 90]) soc2(10);
  translate([4, 4, -3]) spacer(3);
  translate([4, 4, pcb_hight]) m3(8);
  translate([4 + 38, 4, -3]) spacer(3);
  translate([4 + 38, 4, pcb_hight]) m3(8);
}

module stmbl(){
	rotate([0, 90, 0]) translate([-50 - 2 - 2 - 33 - 8, 0, 15 ]) f4();
	translate([1, 10, 33 + 8]) f1();
	rotate([90, 0, 0]) translate([1.5, -41 + 33 + 8, -11]) fan();
	translate([1, 11, 0]) heatsink();
  translate([5.5, 1, 4]) rotate([90, 0, 0]) m3(15);
  translate([5.5 + 37, 1, 4]) rotate([90, 0, 0]) m3(15);
  translate([5.5, 0, 4 + 37]) rotate([90, 0, 0]) m3(10);
  translate([5.5 + 37, 0, 4 + 37]) rotate([90, 0, 0]) m3(10);
  translate([6, 20, 33]) spacer(8);
  translate([6 + 36, 20, 33]) spacer(8);
  translate([6, 20, 33 + 8 + pcb_hight]) m3(15);
  translate([6 + 36, 20, 33 + 8 + pcb_hight]) m3(15);
  translate([22 + 11.4, 27 + 3 + 10, 33 + 5.7]) m3(10);
  translate([22 + 11.4, 27 + 3 + 10 + 56, 33 + 5.7]) m3(10);
  translate([1, 1, 96 + 3]) ethercat();
  //translate([1, 10, 33 + 8]) cover();
  translate([6, 106, 31]) rotate([-90, 0, 0]) m3(5);
  translate([6 + 36, 106, 31]) rotate([-90, 0, 0]) m3(5);
}



stmbl();