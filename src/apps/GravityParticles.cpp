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

int numX = 21;//make odd
int numZ = 13;//make odd


typedef struct Particle
{
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 force;
  float mass;
  float life;
  Particle(glm::vec3 pos = glm::vec3(0), glm::vec3 vel = glm::vec3(0), float life = 5, float m = 1)
  {
    this->position = pos;
    this->velocity = vel;
    this->mass = m;
    this->life = life;
  }
} Particle;

class ParticleSystem
{
public:
  std::vector<Particle> particles;
  
  ParticleSystem(int numParticles)
  {
    for(int i = 0; i < numParticles; i++)
    {
      Particle p;
      initParticle(p);
      particles.push_back(p);
    }
  }
  
  void initParticle(Particle& particle)
  {
    glm::vec3 position = glm::vec3(-1,0.5,0);
    position.x += ((float(arc4random() % 100) / 100.0f) - 0.5) * 0.2;
    position.y += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.05;
    position.z += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.5;
    
    glm::vec3 velocity = glm::vec3(3,2,0);
    velocity.x += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 1;
    velocity.y += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 1;
    velocity.z += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.1;
    
    particle.position = position;
    particle.velocity = velocity;
    particle.life = 4 + ((float(arc4random() % 100) / 100.0f) - 0.5) * 2;
  }
  
  void update(float dt)
  {
    dt = dt/2;
    for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
    {
      it->force = it->mass * glm::vec3(0,-10,0);//gravity
      it->velocity += dt * (it->force / it->mass);
      it->position += dt * it->velocity;
      it->life -= dt;
      if(it->life < 0)
        initParticle((*it));
    }
  }
  
  void getPositons(vector<GLfloat>& positions)
  {
    positions.clear();
    for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
    {
      positions.insert(positions.end(), glm::value_ptr(it->position), glm::value_ptr(it->position)+3);
    }
  }
  
};

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
  
  /* light */
  Light light;
  
  ParticleSystem ps(100);
  
  float clock = 0;
  
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
    glm::vec3 eye = glm::vec3(-1.5,2,2);
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
      GLNode cube = GLNode(vertices, normals, indices);
      cube.scale = glm::vec3(0.1);
      cube.position.x = positions[i*3];
      cube.position.y = positions[i*3 + 1];
      cube.position.z = positions[i*3 + 2];
      
      cube.draw(shader);
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
