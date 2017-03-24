//
//  Shapes.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/15/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.hpp"

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
  GLfloat aspectRatio = GLfloat(width)/height;
  
  // shader
//  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl", "resources/shaders/material_gshader.glsl");
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  
  // light
  Light light;
  
  // shapes
  GLNode cube = GLShapes::createCube(0.5);
  GLNode cylinder = GLShapes::createCylinder();
  GLNode sphere = GLShapes::createSphere(0.2, 11, 11);
  GLNode torrus = GLShapes::createTorrus(0.7, 1.0, 15, 11);
  cube.position = glm::vec3(0,-0.8,0);
  sphere.position = glm::vec3(0,0.75,0);
  
  GLNode nodes;
  nodes.addChild(&cube);
  nodes.addChild(&sphere);
  nodes.addChild(&torrus);
  nodes.addChild(&cylinder);
  
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    // select and shader
    shader.use();
    
    // setup model/view/projection
    glm::vec3 eye = glm::vec3(10,10,10) * zoom;
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 50.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    // setup light
    light.position = glm::vec3(5,5,5);
    light.Kq = 0.01;
    light.Kl = 0.005;
    shader.setVector3f("eyePosition", eye);
    light.setInShader(shader);

    // render
    nodes.draw(shader);
    nodes.rotation += glm::vec3(0.01,0.01,0.01) * 0.2f;
    cube.rotation += glm::vec3(0,0.01,0.01) * 0.8f;
    sphere.scale = glm::vec3(1.1) + glm::vec3(1.0f) * float(sin(glfwGetTime())/2.0f);
    cylinder.scale = sphere.scale * 0.5f;
    torrus.rotation += glm::vec3(0,1,0) * 0.02f;
    
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
