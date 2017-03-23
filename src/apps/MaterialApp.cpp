//
//  MaterialApp.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/6/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "Material.hpp"
#include "ResourceManager.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

vector<GLfloat> appendNormalsToVerticesWithIndices(const vector<GLfloat>& vertices,
                                                  const vector<GLuint>& indices,
                                                  const vector<GLfloat>& normals);

int main(int argc, char * argv[])
{
  
  GLFWwindow *window = glGetWindow();
  
  //callback
  glfwSetKeyCallback(window, keyCallback);
  
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  
  /* shader */
  Shader materialShader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  
  Material material;
  
  /* object to render */
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
  
  vector<GLfloat> objectData = appendNormalsToVerticesWithIndices(vertices, indices, normals);
  long normalsOffset = indices.size() * 3 * sizeof(GLfloat);
  
  /* object buffers */
  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 objectData.size() * sizeof(GLfloat),
                 objectData.data(),
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)normalsOffset);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
  /* render loop */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    /* select and shader */
    materialShader.use();
    
    /* setup model/view/projection */
    glm::vec3 eye = glm::vec3(-1,1,3);
    glm::mat4 model(1.0);
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
    materialShader.setMatrix4("model", model);
    materialShader.setMatrix4("view", view);
    materialShader.setMatrix4("projection", projection);
    
    /* setup normal */
    glm::mat4 normalTransform = glm::mat3(inverse(transpose(model)));
    materialShader.setMatrix4("normalTransform", normalTransform);
    
    /* setup material */
    material.setInShader(materialShader, "material");
    
    /* setup light */
    glm::vec3 lightColor = glm::vec3(1.0);
    glm::vec3 ligthPosition = glm::vec3(0,0.7,0.7);
    materialShader.setVector3f("eyePosition", eye);
    materialShader.setVector3f("lightColor", lightColor);
    materialShader.setVector3f("lightPosition", ligthPosition);
    
    /* render */
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

vector<GLfloat> appendNormalsToVerticesWithIndices(const vector<GLfloat>& vertices,
                                                  const vector<GLuint>& indices,
                                                  const vector<GLfloat>& normals)
{
  vector<GLfloat> vertNor;// = vertices;
  for(auto index: indices) {
    vertNor.push_back(vertices[index * 3]);
    vertNor.push_back(vertices[index * 3 + 1]);
    vertNor.push_back(vertices[index * 3 +2]);
  }
  
  if(!normals.empty())
    vertNor.insert(vertNor.end(), normals.begin(), normals.end());
  
  return vertNor;
}
