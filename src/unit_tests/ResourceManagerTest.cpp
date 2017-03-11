//
//  testResourceManager.cpp
//  playground-modern
//
//  Created by Terna Kpamber on 3/5/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "AppTestUtility.hpp"
using namespace std;

GLFWwindow *window;

TEST(ShaderTest, TestInitialization)
{
  Shader s("resources/shaders/rm_vshader.glsl", "resources/shaders/rm_fshader.glsl");
  ASSERT_GT(s.program, 0);
}

TEST(TextureTest, TestInitialization)
{
  Texture t("resources/textures/awesomeface.png");
  ASSERT_GT(t.textureId, 0);
}

TEST(ResourceManagerTest, TestInitialization)
{
  Shader s("resources/shaders/rm_vshader.glsl", "resources/shaders/rm_fshader.glsl");
  Texture t("resources/textures/awesomeface.png");
  
  ResourceManager rm;
  
  // catch non-existent shaders
  rm.addShader(s, "shader");
  rm.addTexture(t, "texture");
  ASSERT_GT(rm.getShader("shader").program, 0);
  ASSERT_EQ(rm.getShader("texture").program, 0);
  ASSERT_GT(rm.getTexture("texture").textureId, 0);
  ASSERT_EQ(rm.getTexture("shader").textureId, 0);
  
  // catch duplicate shaders
  rm.addShader(s, "shader");
  rm.addTexture(t, "texture");
  ASSERT_EQ(rm.shaders.size(), 1);
  ASSERT_EQ(rm.textures.size(), 1);
}

int main(int argc, char * argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  
  window = glGetWindow();
  
  return RUN_ALL_TESTS();
}


