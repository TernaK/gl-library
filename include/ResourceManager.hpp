//
//  ResourceManager.hpp
//  playground-modern
//
//  Created by Terna Kpamber on 3/5/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include "Shader.hpp"
#include "Texture.hpp"
#include <iostream>
#include <map>

class ResourceManager
{
public:
  /* methods */
  Shader getShader(std::string shaderName);
  Texture getTexture(std::string textureName);
  
  void addShader(const Shader& shader, std::string shaderName);
  void addTexture(const Texture& shader, std::string textureName);
  
  /* members */
  std::map<std::string, Texture> textures;
  std::map<std::string, Shader> shaders;
};

#endif /* ResourceManager_hpp */
