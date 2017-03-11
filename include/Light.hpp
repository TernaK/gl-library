//
//  Light.hpp
//  playground-modern
//
//  Created by Terna Kpamber on 2/25/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef Light_hpp
#define Light_hpp

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

class Light
{
public:
  /* methods */
  Light(glm::vec3 ambient = glm::vec3(1.0),
        glm::vec3 diffuse = glm::vec3(1.0),
        glm::vec3 specular = glm::vec3(1.0),
        glm::vec3 position = glm::vec3(0,0,5));
  void setInShader(const Shader& shader, std::string uniformName = "light",
                   bool useShader = false);
  
  /* members */
  glm::vec3 position = glm::vec3(0,0,5);
  
  glm::vec3 ambient = glm::vec3(0.1);
  glm::vec3 diffuse = glm::vec3(0.1);
  glm::vec3 specular = glm::vec3(0.05);
  
  GLfloat Kc = 1;			//constant
  GLfloat Kl = 0.09;	//linear
  GLfloat Kq = 0.032; //quadratic
};

#endif /* Light_hpp */
