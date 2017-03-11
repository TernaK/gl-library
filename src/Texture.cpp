//
//  Texture.cpp
//  playground-modern
//
//  Created by Terna Kpamber on 2/25/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "Texture.hpp"
using namespace std;

Texture::Texture(GLubyte *data, int width, int height, int channels)
{
  assert(width * height * channels > 0);
  this->width = width;
  this->height = height;
  this->channels = channels;
  init(data);
}

Texture::Texture(std::string imageFile)
{
  GLubyte *image = SOIL_load_image(imageFile.c_str(),
                                   &this->width, &this->height, &this->channels,
                                   SOIL_LOAD_RGB);
  if(!image)
    cout << "ERROR::CALSS::TEXTURE:: could not open " << imageFile << endl;
  
  init(image);
  SOIL_free_image_data(image);
}


void Texture::init(GLubyte *data) {
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::activate(Shader shader, GLenum textureUnit)
{
  glActiveTexture(GL_TEXTURE0+textureUnit);
  glBindTexture(GL_TEXTURE_2D, textureId);
}
