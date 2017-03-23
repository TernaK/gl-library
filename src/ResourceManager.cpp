//
//  ResourceManager.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/5/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
using namespace std;

Shader ResourceManager::getShader(std::string shaderName)
{
  map<string, Shader>::iterator it = shaders.find(shaderName);
  if(it == shaders.end())
  {
    cerr << "Could not find shader: " << shaderName << endl;
    return Shader();
  }
  return it->second;
}

Texture ResourceManager::getTexture(std::string textureName)
{
  map<string, Texture>::iterator it = textures.find(textureName);
  if(it == textures.end())
  {
    cerr << "Could not find texture: " << textureName << endl;
    return Texture();
  }
  return it->second;
}

void ResourceManager::addShader(const Shader& shader, std::string shaderName)
{
  map<string, Shader>::iterator it = shaders.find(shaderName);
  if(it == shaders.end())
    shaders[shaderName] = shader;
}

void ResourceManager::addTexture(const Texture& texture, std::string textureName)
{
  map<string, Texture>::iterator it = textures.find(textureName);
  if(it == textures.end())
    textures[textureName] = texture;
}
