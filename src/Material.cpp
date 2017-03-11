//
//  Material.cpp
//  playground-modern
//
//  Created by Terna Kpamber on 2/24/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include <glm/glm.hpp>
#include "Material.hpp"
using namespace std;

Material::Material(glm::vec3 ambient,
                   glm::vec3 diffuse,
                   glm::vec3 specular, GLfloat shininess)
{
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
  this->shininess = shininess;
};

void Material::setInShader(const Shader& shader, std::string uniformName,
                         bool useShader)
{
  shader.setVector3f(uniformName+".ambient", this->ambient, useShader);
  shader.setVector3f(uniformName+".diffuse", this->diffuse, useShader);
  shader.setVector3f(uniformName+".specular", this->specular, useShader);
  shader.setFloat(uniformName+".shininess", this->shininess, useShader);
}
#include "Shader.hpp"
