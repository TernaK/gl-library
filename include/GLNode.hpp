//
//  GLNode.hpp
//  playground-modern
//
//  Created by Terna Kpamber on 2/22/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#ifndef GLNode_hpp
#define GLNode_hpp

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include "Shader.hpp"
#include "Material.hpp"

class GLNode
{
protected:
  /* methods */
  void init(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals);
  void setUniformsInShader(const Shader& shader, const glm::mat4& model);
  glm::mat4 computeModel(const glm::mat4& parentModel);
  
  /* members */
  GLsizei numTriangles = 0;
  GLuint VAO, VBO;
  /* members */
  Material material;

public:
  /* methods */
  GLNode();
  GLNode(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals);
  GLNode(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices);
  ~GLNode();
  
  void setMaterial(const Material& material) { this->material = material; };
  void draw(const Shader& shader, glm::mat4 parentModel = glm::mat4(1.0f));
  void addChild(GLNode *child) { this->children.push_back(child); };
  void detachChildren() { this->children.clear(); };
  
  GLuint getVAO() { return this->VAO; };
  GLuint getVBO() { return this->VBO; };
  
  /* members */
  glm::vec3 rotation = glm::vec3(0,0,0);
  glm::vec3 position = glm::vec3(0,0,0);
  glm::vec3 scale = glm::vec3(1,1,1);
  std::vector<GLNode*> children;
};


std::vector<GLfloat> UNIT_CUBE_VERTICES =
{
  -0.5f, -0.5f, 0.5f,
  0.5f, -0.5, 0.5f,
  -0.5f, 0.5f, 0.5f,
  0.5f, 0.5f, 0.5f,
  
  -0.5f, -0.5f, -0.5f,
  0.5f, -0.5f, -0.5f,
  -0.5f, 0.5f, -0.5f,
  0.5f, 0.5f, -0.5f,
};

std::vector<GLuint> UNIT_CUBE_INDICES  =
{
  0,1,2, 3,2,1,//front
  5,4,7, 6,7,4,//back
  4,0,6, 2,6,0,//left
  1,5,3, 7,3,5,//rigt
  2,3,6, 7,6,3,//top
  4,5,0, 1,0,5//bottom
};

std::vector<GLfloat> UNIT_CUBE_NORMALS =
{
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

/******************* GLShapes ********************/

class GLShapes
{
public:
  static GLNode createCube(float side = 1.0);
  
  static GLNode createSphere(float radius = 0.5, int slices = 11, int stacks = 11);
};

#endif /* GLNode_hpp */
