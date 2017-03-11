//
//  GLNode.cpp
//  playground-modern
//
//  Created by Terna Kpamber on 2/22/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "GLNode.hpp"
using namespace std;

GLNode::GLNode()
{
  
}

GLNode::GLNode(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals)
{
  assert(vertices.size() > 0 && (normals.size() == vertices.size()));
  //initialize
  init(vertices, normals);
}

GLNode::GLNode(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals, const std::vector<GLuint>& indices)
{
  assert(vertices.size() > 0 && (normals.size() == indices.size() * 3));
  //expand vertices
  vector<GLfloat> expandedVertices;
  for(auto index: indices) {
    expandedVertices.push_back(vertices[index * 3]);
    expandedVertices.push_back(vertices[index * 3 + 1]);
    expandedVertices.push_back(vertices[index * 3 +2]);
  }
  //initialize
  init(expandedVertices, normals);
}

GLNode::~GLNode()
{
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);
}

void GLNode::init(const std::vector<GLfloat>& vertices, const std::vector<GLfloat>& normals)
{
  // set number of primitives for draw call
  this->numTriangles = GLsizei(vertices.size() / 3);
  
  //  generate buffers
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  
  // pack vertices and normals
  vector<GLfloat> verticesAndNormals;
  verticesAndNormals.insert(verticesAndNormals.end(), vertices.begin(), vertices.end());
  verticesAndNormals.insert(verticesAndNormals.end(), normals.begin(), normals.end());

  auto byteLength = sizeof(GLfloat) * verticesAndNormals.size();
  auto normalsByteOffset = sizeof(GLfloat) * vertices.size();
  
  glBindVertexArray(VAO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, byteLength, verticesAndNormals.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)normalsByteOffset);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
}

void GLNode::setUniformsInShader(const Shader& shader, const glm::mat4& model)
{
  //model
  shader.setMatrix4("model", model);
  
  //normal transform
  glm::mat3 normalTransform = glm::mat3(inverse(transpose(model)));
  shader.setMatrix4("normalTransform", normalTransform);
  
  //material
  this->material.setInShader(shader);
}

glm::mat4 GLNode::computeModel(const glm::mat4& parentModel)
{
  glm::mat4 model = glm::translate(parentModel, position);
  model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
  model = glm::scale(model, scale);
  return model;
}

void GLNode::draw(const Shader& shader, glm::mat4 parentModel)
{
  glm::mat4 model = computeModel(parentModel);
  
  if(this->numTriangles > 0)
  {
    //update the parent model to this node's model for the children
    this->setUniformsInShader(shader, model);
    
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numTriangles * 3 );
    glBindVertexArray(0);
  }
  
  for(GLNode* child: children)
  {
    child->draw(shader, model);
  }
}
