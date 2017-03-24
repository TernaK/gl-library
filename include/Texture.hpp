//
//  Texture.hpp
//  gl_library
//
//  Created by Terna Kpamber on 2/25/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <iostream>
#include <assert.h>
#include "Shader.hpp"

class Texture
{
public:
  /* methods */
  Texture(){};
  Texture(GLubyte *data, int width, int height, int channels);
  Texture(std::string imageFile);
  
  void activate(Shader shader, GLenum textureUnit = 0);
  
  /* members */
  GLuint textureId = 0;
  int width = 0, height = 0, channels = 0;
  
private:
  void init(GLubyte *data);
};

#endif /* Texture_hpp */
