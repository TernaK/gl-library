#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoords;

out v_data
{
  vec3 f_Position;
  vec2 f_Textcoords;
} vs_out;

uniform float scale;
uniform vec2 translation;

void main() {
  vs_out.f_Position = (position * scale) + vec3(translation, 0);
  vs_out.f_Textcoords = texcoords;
  gl_Position = vec4(vs_out.f_Position, 1.0);
}
