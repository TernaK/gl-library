#version 330 core

in vec2 f_texCoord;

uniform sampler2D textureImage;

out vec4 color;

void main()
{
  color = texture(textureImage, f_texCoord);
}
