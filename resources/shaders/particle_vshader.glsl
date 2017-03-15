#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

uniform mat4 view;
uniform mat4 projection;

out vs_data
{
  vec3 f_Position;
  vec4 f_Color;
} vs_out;

void main()
{
  vs_out.f_Color = color;
  vs_out.f_Position = position;
  gl_Position = projection * view * vec4(position, 1.0);
}