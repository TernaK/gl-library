//
//  SineSurface.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/9/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.hpp"

int numX = 21;//make odd
int numZ = 13;//make odd

vector<vector<glm::vec3>> createGrid(float t);

int main(int argc, char * argv[])
{
  
  GLFWwindow *window = glGetWindow();
  
  //callback
  glfwSetKeyCallback(window, keyCallback);
  
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  GLfloat aspectRatio = GLfloat(width)/height;
  
  /* shader */
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl", "resources/shaders/material_gshader.glsl");
  
  /* light */
  Light light;
  
  /* render loop */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    // object to render
    vector<GLfloat> vertices;
    vector<GLfloat> normals;
    
    // objects to render
    GLNode surface = GLShapes::createSurface(createGrid(glfwGetTime()));
    
    // select and shader
    shader.use();
    
    // setup model/view/projection
    glm::vec3 eye = glm::vec3(-1.5,2,4);
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    // setup light
    light.position = glm::vec3(0,0.7,0.7);
    shader.setVector3f("eyePosition", eye);
    light.setInShader(shader);
    
    // render
    surface.draw(shader);
    
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

vector<vector<glm::vec3>> createGrid(float t)
{
  float TX = 19;//x period
  float dOffset = 0.5f;//z offset
  float speedUp = 7;
  float scaleFactor = 0.2f;
  
  // form grid of locations
  vector<vector<glm::vec3>> grid = vector<vector<glm::vec3>>(numZ, vector<glm::vec3>(numX, glm::vec3(0.0)));
  
  int x0 = -(numX-1)/2;
  int z0 = -(numZ-1)/2;
  
  float offset = 0;
  for(int z = 0; z < numZ; z++)
  {
    offset += dOffset;
    float positionZ = z0 + z;
    for(int x = 0; x < numX; x++)
    {
      float positionX = x0 + x;
      float positionY = sin(2*M_PI*( (t*speedUp) + positionX + offset) / TX);
      grid[z][x] = glm::vec3(positionX, positionY, positionZ) * scaleFactor;
    }
  }
  
  return grid;
}
