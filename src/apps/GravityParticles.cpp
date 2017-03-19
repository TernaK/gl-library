//
//  GravityParticles.cpp
//  playground-modern
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
#include "ParticleSystem.hpp"

int numX = 21;//make odd
int numZ = 13;//make odd


vector<glm::vec3> gravityFunction(const vector<Particle>& particles, float time)
{
  vector<glm::vec3> forces;
  for(int i = 0; i < particles.size(); i++)
    forces.push_back(particles[i].mass * glm::vec3(0,-10,0));
  
  return forces;
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
  
  /* shader */
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl", "resources/shaders/material_gshader.glsl");
  
  /* light */
  Light light;
  
  ParticleSystem ps(50, gravityFunction);
  
  float clock = 0;
  
  GLNode cube = GLShapes::createCube(0.1);
  GLNode sphere = GLShapes::createSphere(0.05, 11,11);
  
  /* render loop */
  while(!glfwWindowShouldClose(window))
  {
    //timing
    float now = glfwGetTime();
    float dt = now - clock;
    clock = now;
    
    
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    /* select and shader */
    shader.use();
    
    /* setup model/view/projection */
    glm::vec3 eye = glm::vec3(2,2,2);
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    /* setup light */
    light.position = glm::vec3(3,3,3);
    shader.setVector3f("eyePosition", eye);
    light.setInShader(shader);
    
    /* render */
    ps.update(dt);
    vector<GLfloat> positions;
    ps.getPositons(positions);
    
    for(int i = 0; i < positions.size()/3; i++)
    {
      GLNode* node = i % 2 == 0 ? &cube : & sphere;
      node->position = glm::make_vec3((positions.data() + i*3));
      node->draw(shader);
    }
    
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
//  if(key == GLFW_KEY_UP && action == GLFW_PRESS)
//    scale += 0.1;
//  if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
//    scale -= 0.1;
}

