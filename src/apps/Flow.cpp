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
  int width;// = 640;
  int height;// = 480;
  
  // create window
  GLFWwindow* window = glGetWindow(320, 240, "none", false);
  glfwGetFramebufferSize(window, &width, &height);
//  glEnable(GL_MULTISAMPLE);
  
  // framebuffer
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  
  // create renderbuffers
  GLuint colorRenderBuffer;
  glGenRenderbuffers(1, &colorRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  
  GLuint depthRenderBuffer;
  glGenRenderbuffers(1, &depthRenderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  
  // bind renderbuffer to framebuffer
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);
  
  // check status
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cerr << "ERROR: framebuffer is not complete" << endl;
    glfwTerminate();
    exit(-1);
  }
  
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  glReadBuffer(GL_COLOR_ATTACHMENT0);
  
  // set viewport
  glViewport(0, 0, width, height);
  
  // shader
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  shader.use();
  
  glm::vec3 eye = glm::vec3(0,10,1);
  glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
  glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(width)/height, 0.1f, 50.0f);
  shader.setMatrix4("view", view);
  shader.setMatrix4("projection", projection);
  shader.setVector3f("eyePosition", eye);
  
  // setup light
  Light light;
  light.position = glm::vec3(2,10,2);
  light.Kq = 0.005;
  light.Kl = 0.005;
  light.setInShader(shader);
  
  GLNode object = GLShapes::createCube(1.0);
  object.scale.y = 0.3;
  
  cv::namedWindow("frambuffer render");
  
  GLbyte *pixelData = new GLbyte[width*height*3];
  do {
    glfwPollEvents();
    
    // render
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float t = glfwGetTime();
    float radius = 2.0f;
    glm::vec3 position(radius * cos(t), 1.0f, radius * sin(t));
    object.position = position;
    
    object.draw(shader);
    
    // read pixels
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    
    // create cv Mat
    cv::Mat image = cv::Mat(height, width, CV_8UC3, pixelData);
    cv::imshow("frambuffer render", image);
    shouldClose = (cv::waitKey(30) == 27);
    
    CheckErrors();
    glfwSwapBuffers(window);
  } while(!shouldClose);
  
  delete [] pixelData;
  glDeleteFramebuffers(1, &framebuffer);
  glDeleteRenderbuffers(1, &colorRenderBuffer);
  glDeleteRenderbuffers(1, &depthRenderBuffer);
  glfwTerminate();
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
