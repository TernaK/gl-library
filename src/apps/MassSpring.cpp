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
#include "MassSpringSystem.hpp"

#define WIDTH 9
#define HEIGHT 9
#define REST 0.3f
#define SPRING_Ks 0.01f
#define SPRING_Kd 0.005f

float zoom = 1.0f;

vector<glm::vec3> forceFunction(const vector<Particle>& particles, float time)
{
  vector<glm::vec3> forces;
  for(int i = 0; i < particles.size(); i++)
    forces.push_back(particles[i].mass * glm::vec3(0,-1,0));
  
  return forces;
}

void initParticleFunction(Particle& particle, int index)
{
  glm::vec3 position;
  position.x += (index % WIDTH) - (WIDTH / 2);
  position.y += 1.0f;
  position.z += (index / WIDTH) - (HEIGHT / 2);
  
  glm::vec3 velocity = glm::vec3(0,0,0);
  
  particle.position = position * REST;
  particle.velocity = velocity;
  
  particle.life = 10.0;
}

void updateFunction(std::vector<Particle>& particles, float dt)
{
  // get forces
  vector<glm::vec3> forces = forceFunction(particles, glfwGetTime());
  float floor = -1.0f;
  
  for(int i = 0; i < particles.size(); i++)
  {
    Particle& p = particles[i];
    p.velocity += dt * (forces[i] / p.mass);
    p.position += dt * p.velocity;
    
    // constrain floor
    p.position.y = p.position.y > floor ? p.position.y : floor;
	}
  
  
  //  p.position.y = sin( (p.position.x / 2.0f) + glfwGetTime() * 2.0f);
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
  Shader shader = Shader("resources/shaders/particle_vshader.glsl", "resources/shaders/particle_fshader.glsl");
  
  // light
  Light light;
  
  // particle system
  MassSpringSystem ps(WIDTH*HEIGHT, forceFunction, initParticleFunction, updateFunction);
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
    glm::vec3 eye = glm::vec3(-3, 3, 3) / zoom;
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 50.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    // setup light
    light.position = glm::vec3(3,3,3);
    shader.setVector3f("eyePosition", eye);
    light.setInShader(shader);
    
    // render
    ps.update(dt);
    for_each(ps.particles.begin(), ps.particles.end(), [](Particle& p){ p.life = 10.0f; });//update the life
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
  if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    zoom += 0.1;
  if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    zoom -= 0.1;
}
