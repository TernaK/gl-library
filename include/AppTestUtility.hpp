//
//  AppTestUtility.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/5/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>
using namespace std;

// glfw init
void InitGLFW(bool visible) {
  glfwInit();
  if(!visible) glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_SAMPLES, 4);
}

// glew init
void InitGLEW() {
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK){
    puts("failed to create init glew");
    glfwTerminate();
    exit(-1);
  }
}

void CheckErrors() {
  GLenum err;
  while((err = glGetError() && err != GL_NO_ERROR)) {
    cerr << err << endl;
  }
}

// setup window
GLFWwindow* glGetWindow(GLfloat width = 640, GLfloat height = 480, std::string windowTitle = "gl render", bool visible = true)
{
  InitGLFW(visible);
  
  GLFWwindow *window = glfwCreateWindow(width, height, windowTitle.c_str(), nullptr, nullptr);
  if(window == nullptr){
    puts("failed to create glfw window");
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);
  
  InitGLEW();
  
  return window;
}

/* define your own callback */
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
