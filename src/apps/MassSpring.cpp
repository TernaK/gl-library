//
//  Particles+Shader.cpp
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
#include "MassSpringSystem.hpp"

#define WIDTH 25
#define HEIGHT 11
#define REST 0.3f
#define SPRING_Ks 20.0f
#define SPRING_Kd 0.6f

float zoom = 0.4f;

vector<glm::vec3> forceFunction(MassSpringSystem& ps, float time)
{
  vector<glm::vec3> forces;
  for(int y = 0; y < HEIGHT; y++)
  {
    for(int x = 0; x < WIDTH; x++)
    {
      int i = y * WIDTH + x;
      glm::vec3 forceExt = ps.particles[i].mass * glm::vec3(0,-5,6.5*sin(3 * glfwGetTime()));
      // damping
      glm::vec3 dampingForce = -SPRING_Kd * ps.particles[i].velocity;
      // neighbour force
      forces.push_back(forceExt + dampingForce);
    }
  }
  
  for(int s = 0; s < ps.springs.size(); s++)
  {
    Spring &spring = ps.springs[s];
    glm::vec3 diffPos = ps.particles[spring.p1].position - ps.particles[spring.p2].position;
    glm::vec3 diffVel = ps.particles[spring.p1].velocity - ps.particles[spring.p2].velocity;
    glm::vec3 springForce = -spring.Ks * (glm::length(diffPos) - spring.rest) * glm::normalize(diffPos);
    glm::vec3 dampingForce = spring.Kd * diffVel * glm::normalize(diffPos);
    glm::vec3 totalSpringForce = springForce; + dampingForce;
    forces[spring.p1] += totalSpringForce;
    forces[spring.p2] += -totalSpringForce;
  }
  
  return forces;
}

void initParticleFunction(Particle& particle, int index)
{
  glm::vec3 position;
  position.x = (index % WIDTH) - (WIDTH / 2);
  position.y = HEIGHT;
  position.z = (index / WIDTH) - (HEIGHT / 2);
  
  glm::vec3 velocity = glm::vec3(0,0,0);
  
  particle.position = position * REST * 1.0f;
  particle.velocity = velocity;
  particle.life = 10.0;
}

void springInitFunction(const std::vector<Particle>& particles, std::vector<Spring>& springs) {
  
  // up down
  for(int w = 0; w < WIDTH; w++) {
    for(int s = 0; s < HEIGHT - 1; s++){
      int idx = s * WIDTH + w;
      springs.push_back(Spring(REST, SPRING_Ks, SPRING_Kd, idx, (idx+WIDTH)));
    }
  }
  
  // left right
  for(int h = 0; h < HEIGHT; h++) {
    for(int s = 0; s < WIDTH - 1; s++){
      int idx = h * WIDTH + s;
      springs.push_back(Spring(REST, SPRING_Ks, SPRING_Kd, idx, (idx+1)));
    }
  }
  
  float diagonal = sqrt(2 * REST * REST);
  for(int w = 0; w < WIDTH - 1; w++) {
    for(int s = 0; s < HEIGHT - 1; s++){
      int idx = s * WIDTH + w;
      // top-left to bottom-right
      springs.push_back(Spring(diagonal, SPRING_Ks*0.5, SPRING_Kd, idx, (idx+WIDTH+1)));
      // top-right to bottom-left
      springs.push_back(Spring(diagonal, SPRING_Ks*0.5, SPRING_Kd, idx+1, (idx+WIDTH)));
    }
  }
}

void updateFunction(MassSpringSystem& ps, float dt)
{
  // get forces
  
  vector<glm::vec3> forces = forceFunction(ps, glfwGetTime());
  float floor = -20.0f;
  dt = dt / 0.8;
  
  for(int i = 0; i < WIDTH; i++)
    forces[i] = glm::vec3(0.0f);
  
  forces[0] = glm::vec3(0.0f);
  forces[WIDTH-1] = glm::vec3(0.0f);
  for(int i = 0; i < ps.particles.size(); i++)
  {
    Particle& p = ps.particles[i];
    p.velocity += dt * (forces[i] / p.mass);
    p.position += dt * p.velocity;
    
    // constrain floor
    p.position.y = p.position.y > floor ? p.position.y : floor;
	}
  
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
  
  // light
  Light light;
  
  // particle system
  MassSpringSystem mss(WIDTH*HEIGHT, forceFunction, initParticleFunction, updateFunction, springInitFunction);
  glPointSize(5);
  
  float clock = 0;
  
  //GLNode particle = GLShapes::createSphere(0.05);
  
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
    glm::vec3 eye = glm::vec3(-4,-1,5) / zoom;
    glm::mat4 view = glm::lookAt(eye, glm::vec3(-1,0,0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    // setup light
    light.position = glm::vec3(5,3,5);
    light.Kq = 0.01;
    light.Kl = 0.01;
    shader.setVector3f("eyePosition", eye);
    light.setInShader(shader);
    
    // render
    /*
    ps.update(dt);
    for_each(ps.particles.begin(), ps.particles.end(), [&particle, &shader](Particle& p){
      p.life = 10.0f;
      particle.position = p.position;
      particle.draw(shader);
    });
     */
    mss.update(dt);
    vector<GLfloat> vertices, normals;
    MassSpringSystem::createSurface(mss, WIDTH, HEIGHT, vertices, normals);
    GLNode cloth = GLNode(vertices, normals);

    cloth.draw(shader);
    
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
