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


/******************* GLShapes ********************/

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

/**** cube ****/
GLNode GLShapes::createCube(float side)
{
  assert(side > 0.001);
  
  // directly scale the vertices of the unit cubeif the scale is anything other than 1
  vector<GLfloat> vertices;
  std::for_each(UNIT_CUBE_VERTICES.begin(), UNIT_CUBE_VERTICES.end(),
                 [&vertices, side](GLfloat v){
                   return vertices.push_back(v * side);
                 });
  GLNode cube = GLNode(vertices, UNIT_CUBE_NORMALS, UNIT_CUBE_INDICES);
  return cube;
}
/**** sphere ****/
GLNode GLShapes::createSphere(float radius, int slices, int stacks)
{
  using std::vector;
  using std::cerr;
  
  assert( (radius > 0.001) && (slices > 3) && (stacks > 3) && (stacks % 2 == 1));
  
  vector<GLfloat> vertices;
  vector<GLfloat> normals;
  for(int st = 0; st < stacks; st++)
  {
    float v0 = (M_PI/2) - float(st)/stacks * M_PI;
    float v1 = v0 - M_PI/stacks;
    
    if(st == 0 || st == (stacks-1)) // top and bottom stacks
    {
      // peak
      bool topPeak = (st == 0);
      glm::vec3 peak = topPeak ? glm::vec3(0,radius,0) : glm::vec3(0,-radius,0);
      
      float v = topPeak ?  (M_PI/2.0f) - (M_PI/stacks) : -(M_PI/2.0f) + (M_PI/stacks) ;
      
      for(int sl = 0; sl < slices; sl++)
      {
        float u1 = float(sl)/slices * (2 * M_PI);
        float u2 = u1 + (2 * M_PI)/slices;
        
//        printf("(%.0f,%.0f,%.0f)\n", peak.x, peak.y, peak.z);
//        printf("(%.0f,%.0f)<->", glm::degrees(u1), glm::degrees(v));
//        printf("(%.0f,%.0f)\n\n", glm::degrees(u2), glm::degrees(v));
        
        // slice1
        GLfloat x1 = radius * cos(v) * sin(u1);
        GLfloat y1 = radius * sin(v);
        GLfloat z1 = radius * cos(v) * cos(u1);
        
        // slice 2
        GLfloat x2 = radius * cos(v) * sin(u2);
        GLfloat y2 = radius * sin(v);
        GLfloat z2 = radius * cos(v) * cos(u2);
        
        glm::vec3 slice1 = topPeak ? glm::vec3(x1,y1,z1) : glm::vec3(x2,y2,z2); //if top use this as slice origin
        glm::vec3 slice2 = topPeak ? glm::vec3(x2,y2,z2) : glm::vec3(x1,y1,z1); //if bottom use this as slice origin
        
        // get triangle 1
        vertices.insert(vertices.end(), glm::value_ptr(peak), glm::value_ptr(peak)+3);
        vertices.insert(vertices.end(), glm::value_ptr(slice1), glm::value_ptr(slice1)+3);
        vertices.insert(vertices.end(), glm::value_ptr(slice2), glm::value_ptr(slice2)+3);
        
        // normal
        glm::vec3 l1 = slice1 - peak;
        glm::vec3 l2 = slice2 - peak;
        glm::vec3 normal = glm::normalize(glm::cross(l1, l2));//note the order of the cross
        for(int i = 0; i < 3; i++)
          normals.insert(normals.end(), glm::value_ptr(normal), glm::value_ptr(normal)+3);
      }
    }
    else
    {
      // generate parameters u -> longitude, v -> lattitude
      for(int sl = 0; sl < slices; sl++)
      {
        float u0 = float(sl)/slices * (2 * M_PI);
        float u1 = u0 + (2 * M_PI)/slices;
        
        // compute (x,y,z) based using the parametric form
        // top left
        GLfloat x00 = radius * cos(v0) * sin(u0);
        GLfloat y00 = radius * sin(v0);
        GLfloat z00 = radius * cos(v0) * cos(u0);
        glm::vec3 topLeft = glm::vec3(x00,y00,z00);
        
        // bottom left
        GLfloat x01 = radius * cos(v1) * sin(u0);
        GLfloat y01 = radius * sin(v1);
        GLfloat z01 = radius * cos(v1) * cos(u0);
        glm::vec3 bottomLeft = glm::vec3(x01,y01,z01);
        
        // top right
        GLfloat x10 = radius * cos(v0) * sin(u1);
        GLfloat y10 = radius * sin(v0);
        GLfloat z10 = radius * cos(v0) * cos(u1);
        glm::vec3 topRight = glm::vec3(x10,y10,z10);
        
        // bottom right
        GLfloat x11 = radius * cos(v1) * sin(u1);
        GLfloat y11 = radius * sin(v1);
        GLfloat z11 = radius * cos(v1) * cos(u1);
        glm::vec3 bottomRight = glm::vec3(x11,y11,z11);
        
        /* get triangle 1 */
        vertices.insert(vertices.end(), glm::value_ptr(topLeft), glm::value_ptr(topLeft)+3);//top left
        vertices.insert(vertices.end(), glm::value_ptr(bottomLeft), glm::value_ptr(bottomLeft)+3);//bottom left
        vertices.insert(vertices.end(), glm::value_ptr(bottomRight), glm::value_ptr(bottomRight)+3);//bottom right
        
        /* normal */
        glm::vec3 l1 = bottomRight - bottomLeft;
        glm::vec3 l2 = topLeft - bottomLeft;
        glm::vec3 normal = glm::normalize(glm::cross(l1, l2));//note the order of the cross
        for(int i = 0; i < 3; i++)
          normals.insert(normals.end(), glm::value_ptr(normal), glm::value_ptr(normal)+3);
        
        /* get triangle 2 */
        vertices.insert(vertices.end(), glm::value_ptr(bottomRight), glm::value_ptr(bottomRight)+3);//bottom right
        vertices.insert(vertices.end(), glm::value_ptr(topRight), glm::value_ptr(topRight)+3);//top right
        vertices.insert(vertices.end(), glm::value_ptr(topLeft), glm::value_ptr(topLeft)+3);//top left
        
        /* normal */
        l1 = topLeft - topRight;
        l2 = bottomRight - topRight;
        normal = glm::normalize(glm::cross(l1, l2));//note the order of the cross
        for(int i = 0; i < 3; i++)
          normals.insert(normals.end(), glm::value_ptr(normal), glm::value_ptr(normal)+3);
      }
    }
  }
  
  return GLNode(vertices, normals);
}
