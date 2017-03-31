//
//  Framebuffer.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/31/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.hpp"
#include "GLTextNode.hpp"

float zoom = 0.5;

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
  
  //shader
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  Shader postShader = Shader("resources/shaders/vshader.glsl", "resources/shaders/fshader.glsl");
  
  //texture object
  vector<GLfloat> vertices = {
    -1,-1,0, 1,-1,0, -1,1,0,
    1,1,0,   -1,1,0, 1,-1,0
  };
  vector<GLfloat> texcoords = {
    0,1, 1,1, 0,0,
    1,0, 0,0, 1,1
  };
  
  GLuint VAO, verticesVBO, texcoordsVBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &verticesVBO);
  glGenBuffers(1, &texcoordsVBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (GLvoid*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  glBindBuffer(GL_ARRAY_BUFFER, texcoordsVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, (GLvoid*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  
  glBindVertexArray(0);
  
  Texture t = Texture("resources/textures/awesomeface.png");
  
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    postShader.use();
    
    //texture
    t.activate(postShader);
    GLint loc = glGetUniformLocation(postShader.program, "textureObject");
    glUniform1i(loc, 0);
    
    //draw
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
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
