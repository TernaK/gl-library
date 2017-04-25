//
//  Flow.cpp
//  gl_library
//
//  Created by Terna Kpamber on 4/24/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.hpp"
#include "GLTextNode.hpp"

int main(int argc, char * argv[])
{
  bool shouldClose = false;
  int width = 640;
  int height = 480;
  
  // render texture
  
  // framebuffer
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	
  // create renderbuffers
  
//  GLFWwindow *window = glGetWindow(320, 240);
  
  // callback
//  glfwSetKeyCallback(window, keyCallback);
//  
//  int width, height;
//  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  
  // shader
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  shader.use();
  
  glm::vec3 eye = glm::vec3(0,10,5);
  glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(width)/height, 0.1f, 50.0f);
  shader.setMatrix4("view", view);
  shader.setMatrix4("projection", projection);
  shader.setVector3f("eyePosition", eye);
  
  // setup light
  Light light;
  light.position = glm::vec3(5,5,5);
  light.Kq = 0.01;
  light.Kl = 0.005;
  light.setInShader(shader);
  
  GLNode sphere = GLShapes::createSphere(0.5, 21, 21);
  
  do {
//    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);

    float t = glfwGetTime();
    glm::vec3 position(cos(t), 1.0f, sin(t));
    sphere.position = position * 2.0f;
    
    sphere.draw(shader);

//    glfwSwapBuffers(window);
  } while(!shouldClose);
  
//  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  // if(key == GLFW_KEY_UP && action == GLFW_PRESS)
  //   zoom += 0.1;
  // if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
  //   zoom -= 0.1;
}
