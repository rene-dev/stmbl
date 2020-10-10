module clip(){
  $fn = 500;
  difference(){
    linear_extrude(height = 2, center = true, convexity = 10) import(file = "clip.dxf", layer = "0");
  }
}
