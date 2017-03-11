//
//  ResourceManagerApp.cpp
//  playground-modern
//
//  Created by Terna Kpamber on 3/5/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char * argv[])
{
  
  GLFWwindow *window = glGetWindow();
  
  //callback
  glfwSetKeyCallback(window, keyCallback);
  
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  
  /* resource manager */
  ResourceManager resMan;
  resMan.addShader(Shader("resources/shaders/rm_vshader.glsl",
                          "resources/shaders/rm_fshader.glsl"),
                   "shader");
  resMan.addTexture(Texture("resources/textures/awesomeface.png"), "texture");
  
  /* object to render */
  vector<GLfloat> objectData
  {	//vertex		 		//texture
    -0.5, -0.5, 0,	0,1,
    0.5, 	-0.5, 0,  1,1,
    -0.5,	0.5,  0,	0,0,
    
    0.5, 	-0.5, 0,  1,1,
    -0.5,	0.5,  0,	0,0,
    0.5,	0.5,  0,	1,0,
  };
  
  
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
  
  Texture texture = resMan.getTexture("texture");
  Shader shader = resMan.getShader("shader");
  
  /* render loop */
  while(!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2, 0.2, 0.5, 1.0);
    
    /* select shader */
    shader.use();
    
    /* setup texture */
    texture.activate(shader);
    shader.setInteger("textureImage", 0);
    
    /* setup model/view/projection */
    glm::mat4 model(1.0);
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,5), glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 10.0f);
    shader.setMatrix4("model", model);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    /* render */
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
}
