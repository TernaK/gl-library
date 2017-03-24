//
//  SineWaveCubes.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/7/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

/* object to render */
vector<GLfloat> vertices = {
  -0.5f, -0.5f, 0.5f,
  0.5f, -0.5, 0.5f,
  -0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,
};

vector<GLuint> indices = {
  0,1,2, 3,2,1,//front
  5,4,7, 6,7,4,//back
  4,0,6, 2,6,0,//left
  1,5,3, 7,3,5,//rigt
  2,3,6, 7,6,3,//top
  4,5,0, 1,0,5//bottom
};

vector<GLfloat> normals = {
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  0.0f,  0.0f, 1.0f,
  
  0.0f,  0.0f, -1.0f,
  0.0f,  0.0f, -1.0f,
  0.0f,  0.0f, -1.0f,
  0.0f,  0.0f, -1.0f,
  0.0f,  0.0f, -1.0f,
  0.0f,  0.0f, -1.0f,
  
  -1.0f,  0.0f,  0.0f,
  -1.0f,  0.0f,  0.0f,
  -1.0f,  0.0f,  0.0f,
  -1.0f,  0.0f,  0.0f,
  -1.0f,  0.0f,  0.0f,
  -1.0f,  0.0f,  0.0f,
  
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f,
  0.0f, -1.0f,  0.0f
};

int numX = 21;//make odd
int numZ = 5;//make odd

void updatePositions(vector<glm::vec3>& cubePositions, float t)
{
  float TX = 8;//x period
  float dOffset = 0.0f;//z offset
  float speedUp = 5;
  
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
      
      cubePositions.push_back(glm::vec3(positionX, positionY, positionZ));
    }
  }
}


int main(int argc, char * argv[])
{
  
  GLFWwindow *window = glGetWindow();
  
  //callback
  glfwSetKeyCallback(window, keyCallback);
  
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  GLfloat aspectRatio = GLfloat(width)/height;
  
  /* objects to render */
  GLNode cube = GLNode(vertices, normals, indices);
  cube.scale = glm::vec3(0.7);
  
  /* shader */
  Shader cubeShader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  
  /* render loop */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    /* select and shader */
    cubeShader.use();
    
    /* setup model/view/projection */
    glm::vec3 eye = glm::vec3(-10,10,18);
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    cubeShader.setMatrix4("view", view);
    cubeShader.setMatrix4("projection", projection);
    
    /* setup light */
    glm::vec3 lightColor = glm::vec3(1.0);
    glm::vec3 ligthPosition = glm::vec3(0,2,2);
    cubeShader.setVector3f("eyePosition", eye);
    cubeShader.setVector3f("lightColor", lightColor);
    cubeShader.setVector3f("lightPosition", ligthPosition);
    
    /* render */
    vector<glm::vec3> cubePositions;
    updatePositions(cubePositions, glfwGetTime());
    for(auto position: cubePositions)
    {
      cube.position = position;
      cube.draw(cubeShader);
    }
    
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
