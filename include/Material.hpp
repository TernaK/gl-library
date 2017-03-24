//
//  Material.hpp
//  gl_library
//
//  Created by Terna Kpamber on 2/24/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include "Shader.hpp"
#include <glm/glm.hpp>

class Material
{
public:
  /* methods */
  Material() {};
  Material(glm::vec3 ambient,
           glm::vec3 diffuse,
           glm::vec3 specular, GLfloat shininess);
  void setInShader(const Shader& shader, std::string uniformName = "material",
                           bool useShader = false);
  
  /* members */
  glm::vec3 ambient = glm::vec3(0.0215,0.1745,0.0215);
  glm::vec3 diffuse = glm::vec3(0.07568,0.61424,0.07568);
  glm::vec3 specular = glm::vec3(0.633,0.727811,0.633);
  GLfloat shininess = 32;
};

/******** MATERIALS ********
Material emerald = Material(glm::vec3(0.0215,0.1745,0.0215),
                            glm::vec3(0.07568,0.61424,0.07568),
                            glm::vec3(0.633,0.727811,0.633),
														0.6);
 **************************/

#endif /* Material_hpp */
