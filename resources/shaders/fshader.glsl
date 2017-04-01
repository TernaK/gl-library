#version 330 core

in v_data {
  vec2 f_Texcoord;
} fs_in;

uniform sampler2D textureObject;

out vec4 color;

void main() {
  //if(fs_in.f_Texcoord.x < 0.5)
    color = texture(textureObject, fs_in.f_Texcoord);
  //else
    //color = vec4(0,0,0,1);//texture(textureObject, fs_in.f_Texcoord + vec2(0.05));
}
