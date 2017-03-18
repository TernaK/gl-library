//
//  Gas.cpp
//  playground-modern
//
//  Created by Terna Kpamber on 3/17/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.hpp"
#include "ParticleSystem.hpp"

float radius = 1.414 * 0.2;

void collisionFunction(vector<Particle>& particles)
{
  vector<bool> handled = vector<bool>(particles.size(), false);
  map<int, glm::vec3> newVelocities;
  
  // evaluate positions & test collisions
  for(int p = 0; p < particles.size(); p++)
  {
    Particle* p1 = &particles[p];
    for(int q = 0; q < particles.size(); q++)
    {
      Particle* p2 = &particles[q];
      if(p == q) continue;
      
      if(!handled[p])
      {
        // get point of collision if any
        glm::vec3 diff = p1->position - p2->position;
        if(glm::length(diff) < radius)
        {
          glm::vec3 p2Normal = glm::normalize(p2->velocity);
          glm::vec3 newVelocity = glm::reflect(p1->velocity, p2Normal);
          p1->velocity = newVelocity;
//          newVelocities[p] = newVelocity;
          
          glm::vec3 contact = (p1->position + p2->position) / 2.0f;
          p1->position = contact + 1.01f * radius * p2Normal;
          
          handled[p] = true;
          break;
        }// endif
      }// end if
    }//end for

      // check bounds
    float wall = 3.0f;
    
    if(p1->position.x < -wall)
      p1->velocity = glm::reflect(p1->velocity, glm::vec3(1,0,0));
    if(p1->position.x > wall)
      p1->velocity = glm::reflect(p1->velocity, glm::vec3(-1,0,0));
    
    if(p1->position.y < -wall)
      p1->velocity = glm::reflect(p1->velocity, glm::vec3(0,1,0));
    if(p1->position.y > wall)
      p1->velocity = glm::reflect(p1->velocity, glm::vec3(0,-1,0));
    
    if(p1->position.z < -wall)
      p1->velocity = glm::reflect(p1->velocity, glm::vec3(0,0,1));
    if(p1->position.z > wall)
      p1->velocity = glm::reflect(p1->velocity, glm::vec3(0,0,-1));
    
    p1->velocity = 3.0f * glm::normalize(p1->velocity);
    p1->life = 5;
  }
  
//  for(auto nv: newVelocities)
//  {
//    particles[nv.first] = nv.second;
//  }
  
}

vector<glm::vec3> motionFunction(const vector<Particle>& particles, float time)
{
  vector<glm::vec3> forces;
  for(int i = 0; i < particles.size(); i++)
    forces.push_back(glm::vec3(0)/*glm::normalize(particles[i].velocity)*/);
  
  return forces;
}

vector<GLfloat> getColors(vector<Particle>& particles, float lifeMax)
{
  vector<GLfloat> colors;
  for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    float scale = it->life / lifeMax;
    scale = scale < 0.0f ? 0 : scale;
    scale = scale > 1.0f ? 1 : scale;
    glm::vec4 color = glm::vec4(1.0,scale,0.3,1.0) * scale;
    colors.insert(colors.end(), glm::value_ptr(color), glm::value_ptr(color)+4);
  }
  return colors;
}

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
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  
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
  
  GLNode cube = GLNode(vertices, normals, indices);
  cube.scale = glm::vec3(radius/1.2);
  
  // light
  Light light;
  
  // particle system
  ParticleSystem ps(50, motionFunction, collisionFunction);
  ps.setLifeMax(10);
  glPointSize(5);
  
  float clock = 0;
  
  // render loop
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
    
    // select and shader
    shader.use();
    
    // setup model/view/projection
    glm::vec3 eye = glm::vec3(8, 6, 8);
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    // setup light
    light.Kl = 0.035;
    light.Kq = 0;
    light.position = glm::vec3(10,10,10);
    shader.setVector3f("eyePosition", eye);
    light.setInShader(shader);
    
    // render
    ps.update(dt);
    vector<GLfloat> positions;
    ps.getPositons(positions);
    for(int i = 0; i < positions.size()/3; i++)
    {
      cube.draw(shader);
      float x = positions[i * 3];
      float y = positions[i * 3 + 1];
      float z = positions[i * 3 + 2];
      cube.position = glm::vec3(x,y,z);
//      cube.rotation += float(-i*0.02) * glm::vec3(0.5,1,0);
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
