module bracket(){
  $fn = 500;
  difference(){
    linear_extrude(height = 6, center = true, convexity = 10) import(file = "v4_bracket.dxf", layer = "ding");
    translate([27/2 + 1.75, 24.6, 0]) rotate([90, 0, 0]) cylinder(r = 1.35, h = 5);
    translate([27/2 - 1.75, 24.6, 0]) rotate([90, 0, 0]) cylinder(r = 1.35, h = 5);
    //translate([27 / 2 + 3/2, 8, 3]) rotate([0, 0, 90]) text("STMBL",3.5);
  }
}

module bracket2(){
  $fn = 500;
  difference(){
    linear_extrude(height = 6, center = true, convexity = 10) import(file = "v4_bracket.dxf", layer = "ding2");
    translate([27/2 + 1.75, 24.6, 0]) rotate([90, 0, 0]) cylinder(r = 1.35, h = 5);
    translate([27/2 - 1.75, 24.6, 0]) rotate([90, 0, 0]) cylinder(r = 1.35, h = 5);
    translate([27/2, -14.1, 0]) rotate([90, 0, 180]) cylinder(r = 1.35, h = 5.1);
    //translate([27 / 2 + 3/2, 8, 3]) rotate([0, 0, 90]) text("STMBL",3.5);
  }
}

//bracket2();
