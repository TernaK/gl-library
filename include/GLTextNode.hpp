//
//  GLTextureNode.hpp
//  gl-library
//
//  Created by Terna Kpamber on 2/25/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef GLTextureNode_hpp
#define GLTextureNode_hpp

#include "GLNode.hpp"
#include "Texture.hpp"
#include <opencv2/opencv.hpp>

class GLTextNode {
private:
  Texture texture;
  GLuint VAO, VBO;
  glm::vec2 size;
  
public:
  
  GLTextNode();
  
  GLTextNode(std::string text, GLfloat midHeight = 0.05f);
  
  void draw(Shader shader);
  
  float scale = 1.0f;
  glm::vec2 position = glm::vec2(0);
  glm::vec2 getSize(){ return this->size * this->scale; };
};

#endif /* GLTextureNode_hpp */
