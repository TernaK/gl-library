#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out v_data
{
  vec3 f_Normal;
  vec3 f_Position;
  vec3 f_PositionInLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 normalTransform;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightProjectionViewProduct;

void main() {
  vs_out.f_Position = (model * vec4(position, 1.0)).xyz;
  gl_Position = projection * view * vec4(vs_out.f_Position, 1.0);
  
  vec4 posInLight = lightProjectionViewProduct * vec4(vs_out.f_Position, 1.0);
  vs_out.f_PositionInLightSpace = posInLight.xyz / posInLight.w;
  
  vs_out.f_Normal = normalize(mat3(normalTransform) * normal);
}
