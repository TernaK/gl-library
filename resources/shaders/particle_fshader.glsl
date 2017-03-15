#version 330 core

in vs_data
{
  vec3 f_Position;
  vec4 f_Color;
} vs_out;

out vec4 color;

void main()
{
  color = vs_out.f_Color;
}