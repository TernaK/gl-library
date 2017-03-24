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
  
public:
  
  GLTextNode();
  
  GLTextNode(std::string text, GLfloat height = 0.05f);
  
  void draw(Shader shader);
  
  float scale = 1.0f;
  glm::vec2 translation = glm::vec2(0);
};

#endif /* GLTextureNode_hpp */
