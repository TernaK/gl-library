//
//  Light.cpp
//  gl_library
//
//  Created by Terna Kpamber on 2/25/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "Light.hpp"

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
             glm::vec3 position)
{
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->position = position;
}

void Light::setInShader(const Shader& shader, std::string uniformName,
                           bool useShader)
{
  shader.setVector3f(uniformName+".ambient", this->ambient, useShader);
  shader.setVector3f(uniformName+".diffuse", this->diffuse, useShader);
  shader.setVector3f(uniformName+".specular", this->specular, useShader);
  shader.setVector3f(uniformName+".position", this->position, useShader);
  shader.setFloat(uniformName+".Kc", this->Kc, useShader);
  shader.setFloat(uniformName+".Kl", this->Kl, useShader);
  shader.setFloat(uniformName+".Kq", this->Kq, useShader);
}
