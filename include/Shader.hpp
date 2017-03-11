//
//  Shader.hpp
//  playground-modern
//
//  Created by Terna Kpamber on 2/22/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
  /* methods */
  Shader(){};
  
  // create shader using file paths
  Shader(std::string vertexShaderPath,
         std::string fragmentShaderPath,
         std::string geometryShaderPath = "");
  
  // generate shader using sources
  static Shader ShaderFromSources(std::string vertexShaderSource,
                                  std::string fragmentShaderSource,
                                  std::string geometryShaderSource = "");
  
  // use shader
  const Shader& use() const;
  
  /* for setting uniforms */
  void setFloat(std::string name, GLfloat value, GLboolean useShader = false) const;
  void setInteger(std::string name, GLint value, GLboolean useShader = false) const;
  void setVector2f(std::string name, const glm::vec2& value, GLboolean useShader = false) const;
  void setVector3f(std::string name, const glm::vec3 &value, GLboolean useShader = false) const;
  void setVector4f(std::string name, const glm::vec4 &value, GLboolean useShader = false) const;
  void setMatrix4(std::string name, const glm::mat4 &matrix, GLboolean useShader = false) const;
  
  /* members */
  GLuint program = 0;
  
private:
  // helper method for creating shader
  static GLuint createShaderProgram(std::string vertexShaderSource, std::string fragmentShaderSource, std::string geometryShaderSource);
};

#endif /* Shader_hpp */
