//
//  Text.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/23/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.hpp"
#include "GLTextNode.hpp"

float zoom = 0.5;

float randomFloat()
{
  return float(arc4random() % 1001) / 1000.0f;
}

vector<vector<glm::vec3>> createRandomGrid(int numX, int numZ, int resolution);
void createSurface(vector<GLfloat>& vertices, vector<GLfloat>& normals, int numX, int numZ, int resolution);

int main(int argc, char * argv[])
{
  GLFWwindow *window = glGetWindow();
  
  // callback
  glfwSetKeyCallback(window, keyCallback);
  
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  
  // shader
  Shader shader = Shader("resources/shaders/text_vshader.glsl", "resources/shaders/text_fshader.glsl");
  shader.use();
  
  // text node
  GLTextNode tn("text");
  tn.scale = 0.5;
  tn.position = glm::vec2(-0.5,-0.5);
  
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    tn.draw(shader);
    
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    zoom += 0.1;
  if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    zoom -= 0.1;
}
