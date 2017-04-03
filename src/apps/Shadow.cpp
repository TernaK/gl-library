//
//  Shadow.cpp
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
  float aspectRatio = float(width)/height;
  
  //shader
  Shader preShader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  Shader postShader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  //Shader postShader = Shader("resources/shaders/shadow_vshader.glsl", "resources/shaders/shadow_fshader.glsl");
  
  Light light;
  
  GLNode body = GLShapes::createSphere(0.5, 21, 21);
  body.position.z = -1.0f;
  
  //texture object
  vector<GLfloat> vertices = {
    -1,-1,0, 1,-1,0, -1,1,0,
    1,1,0,   -1,1,0, 1,-1,0
  };
  vector<GLfloat> texcoords = {
    0,0, 1,0, 0,1,
    1,1, 0,1, 1,0
  };
  
  //framebuffer
  GLuint framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  
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
  
  // color attachment
  Texture t;
  t.width = width;
  t.height = height;
  glGenTextures(1, &t.textureId);
  glBindTexture(GL_TEXTURE_2D, t.textureId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, t.textureId, 0);
  
  // tell opengl not to render the color
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    exit(-1);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    //pre
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glViewport(0, 0, width, height);
    
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    preShader.use();
    
    // setup light
    light.position = glm::vec3(5,5,5);
    light.Kq = 0.01;
    light.Kl = 0.01;
    
    preShader.setVector3f("eyePosition", light.position);
    light.setInShader(preShader);
    
    // setup model/view/projection
    glm::mat4 view = glm::lookAt(light.position, glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 10.0f);//glm::perspective(glm::radians(35.0f), aspectRatio, 0.1f, 50.0f);
    preShader.setMatrix4("view", view);
    preShader.setMatrix4("projection", projection);
    
    body.draw(preShader);
    
    //post
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, width, height);
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    postShader.use();
    
    // eye
    glm::vec3 eye = glm::vec3(0,0,5) / zoom;
    view = glm::lookAt(eye, glm::vec3(0,0,0), glm::vec3(0,1,0));
    projection = glm::perspective(glm::radians(35.0f), aspectRatio, 0.1f, 50.0f);
    postShader.setMatrix4("view", view);
    postShader.setMatrix4("projection", projection);
    postShader.setVector3f("eyePosition", light.position);
    
    //light
    light.setInShader(postShader);
    
    //texture
    //t.activate(postShader);
    //GLint loc = glGetUniformLocation(postShader.program, "textureObject");
    //glUniform1i(loc, 0);
    
    //draw
    body.draw(preShader);
    body.rotation.y += 0.01;
    
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
      cout << hex << err << endl;
    }
    
    glfwSwapBuffers(window);
  }
  
  glDeleteFramebuffers(1, &framebuffer);
  glDeleteBuffers(1, &verticesVBO);
  glDeleteBuffers(1, &texcoordsVBO);
  glDeleteVertexArrays(1, &VAO);
  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    zoom += 0.15;
  if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    zoom -= 0.15;
}
