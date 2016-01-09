#version 120

uniform mat4 mvp;

void main(void)
{
  gl_Position = mvp * gl_Vertex;
}