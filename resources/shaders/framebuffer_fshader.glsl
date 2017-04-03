#version 330 core

in v_data {
  vec2 f_Texcoord;
} fs_in;

uniform sampler2D textureObject;

out vec4 color;

void main() {
  float depth = texture(textureObject, fs_in.f_Texcoord).x;
  color = vec4(vec3(depth), 1.0);
}
