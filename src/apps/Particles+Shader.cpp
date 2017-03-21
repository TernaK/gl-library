//
//  Particles+Shader.cpp
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

#define LIFE_MAX 5.0f

vector<glm::vec3> gravityFunction(const vector<Particle>& particles, float time)
{
  vector<glm::vec3> forces;
  for(int i = 0; i < particles.size(); i++)
    forces.push_back(particles[i].mass * glm::vec3(0,-10,0));
  
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

void initParticleFunction(Particle& particle, int index)
{
  glm::vec3 position = glm::vec3(0,-0.5,0);
  position.x += ((float(arc4random() % 100) / 100.0f) - 0.5) * 0.1;
  position.y += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.05;
  position.z += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.1;
  
  glm::vec3 velocity = glm::vec3(0,5,0);
  velocity.x += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 3;
  velocity.y += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 1;
  velocity.z += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.1;
  
  particle.position = position;
  particle.velocity = velocity;
  particle.life = LIFE_MAX - 1.0 + (float(arc4random() % 100) / 100.0f);
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
  Shader shader = Shader("resources/shaders/particle_vshader.glsl", "resources/shaders/particle_fshader.glsl");
  
  // light
  Light light;
  
  // particle system
  ParticleSystem ps(500, gravityFunction, initParticleFunction);
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
    glm::vec3 eye = glm::vec3(-1.5,2,2);
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 10.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    // setup light
    light.position = glm::vec3(3,3,3);
    shader.setVector3f("eyePosition", eye);
    light.setInShader(shader);
    
    // render
    ps.update(dt);
    vector<GLfloat> positionsAndColors;
    ps.getPositons(positionsAndColors);
    long colorsOffset = positionsAndColors.size() * sizeof(GLfloat);
    vector<GLfloat> colors = getColors(ps.particles, ps.getLifeMax());
    positionsAndColors.insert(positionsAndColors.end(), colors.begin(), colors.end());
    
    //setup buffers
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * positionsAndColors.size(), positionsAndColors.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)colorsOffset);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, (GLuint)ps.particles.size());
    glBindVertexArray(0);
    
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
