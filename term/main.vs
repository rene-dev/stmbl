attribute vec2 coord2d;
varying vec4 f_color;
uniform float offset_x;
uniform float scale_x;

void main(void) {
  gl_Position = vec4((coord2d.x + offset_x) * scale_x, coord2d.y, 0, 1);
  f_color = vec4(coord2d.xy / 2.0 + 0.5, 1, 1);
}