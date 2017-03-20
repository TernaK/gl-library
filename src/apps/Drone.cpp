//
//  Drone
//  playground-modern
//
//  Created by Terna Kpamber on 3/19/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.hpp"

float zoom = 1.0;

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
  
  
  Shader meshShader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl", "resources/shaders/material_gshader.glsl");
  Shader solidShader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  
  ResourceManager rm;
  rm.addShader(meshShader, "mesh");
  rm.addShader(solidShader, "solid");
  
  // light
  Light light;
  
  // drone
  GLNode motor1 = GLShapes::createCylinder(0.55,0.35);
  GLNode motor2 = GLShapes::createCylinder(0.55,0.35);
  GLNode motor3 = GLShapes::createCylinder(0.55,0.35);
  GLNode motor4 = GLShapes::createCylinder(0.55,0.35);
  
  GLNode rotor1 = GLShapes::createTorrus(0.8,1.1,17);
  GLNode rotor2 = GLShapes::createTorrus(0.8,1.1,17);
  GLNode rotor3 = GLShapes::createTorrus(0.8,1.1,17);
  GLNode rotor4 = GLShapes::createTorrus(0.8,1.1,17);
  
  motor1.addChild(&rotor1);
  motor2.addChild(&rotor2);
  motor3.addChild(&rotor3);
  motor4.addChild(&rotor4);
  
  motor1.position = glm::vec3(-2.2,0,3);
  motor2.position = glm::vec3(-2.2,0,-3);
  motor3.position = glm::vec3(2.2,0,3);
  motor4.position = glm::vec3(2.2,0,-3);
  
  GLNode rod1 = GLShapes::createCylinder(0.08, 2.5);
  GLNode rod2 = GLShapes::createCylinder(0.08, 2.5);
  GLNode rod3 = GLShapes::createCylinder(0.08, 2.5);
  GLNode rod4 = GLShapes::createCylinder(0.08, 2.5);
  
  rod1.rotation = glm::vec3(M_PI/2.5,0,M_PI/3.5);
  rod2.rotation = glm::vec3(-M_PI/2.5,0,M_PI/3.5);
  rod3.rotation = glm::vec3(M_PI/2.5,0,-M_PI/3.5);
  rod4.rotation = glm::vec3(-M_PI/2.5,0,-M_PI/3.5);
  
  rod1.position = glm::vec3(-1.6,-0.25,2.1);
  rod2.position = glm::vec3(-1.6,-0.25,-2.1);
  rod3.position = glm::vec3(1.6,-0.25,2.1);
  rod4.position = glm::vec3(1.6,-0.25,-2.1);
  
  GLNode body = GLShapes::createCube(1.0);
  body.scale = glm::vec3(2,0.6,3);
  body.position = glm::vec3(0,-0.5,0);
  
  GLNode drone;
  drone.addChild(&motor1);
  drone.addChild(&motor2);
  drone.addChild(&motor3);
  drone.addChild(&motor4);
  drone.addChild(&rod1);
  drone.addChild(&rod2);
  drone.addChild(&rod3);
  drone.addChild(&rod4);
  drone.addChild(&body);
 
  glm::vec3 motorSpeed = glm::vec3(0,2,0);
  glm::vec3 rotation = glm::vec3(0);
  
  float clock = 0;
  
  while(!glfwWindowShouldClose(window))
  {
    float now = glfwGetTime();
    float dt = now - clock;
    clock = now;
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    Shader shader = rm.getShader( sin(glfwGetTime()*2) > 0 ? "mesh" : "solid");
    
    // select and shader
    shader.use();
    
    // setup model/view/projection
    glm::vec3 eye = glm::vec3(0,10,10) * zoom;
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
    rotation = rotation += motorSpeed * dt;
    rotor1.rotation = rotation;
    rotor2.rotation = rotation;
    rotor3.rotation = -rotation;
    rotor4.rotation = -rotation;
    
    drone.draw(shader);
    drone.position = glm::vec3(0, 0.25 * sin(glfwGetTime()*3) ,0);
    
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
