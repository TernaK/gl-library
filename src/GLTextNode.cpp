//
//  GLTextureNode.cpp
//  playground-modern
//
//  Created by Terna Kpamber on 2/25/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "GLTextNode.hpp"
using namespace std;
using namespace cv;

GLTextNode::GLTextNode() {
  
}

GLTextNode::GLTextNode(std::string text, GLfloat midHeight) {
  // generate texture
  int font = FONT_HERSHEY_SIMPLEX;
  double fontScale = 3.0;
  double thickness = 6.0;
  int baseline;
  Size textSize = getTextSize(text, font, fontScale, thickness, &baseline);
  
  Size imageSize; // the final texture size
  imageSize.height = textSize.height + baseline + thickness; // account for baseline & thickness
  imageSize.width = textSize.width + (4 - (textSize.width % 4)); // width MUST be multiple of 4
  
  // obtain text origin
  Point origin = Point((imageSize.width - textSize.width)/2, imageSize.height - baseline); // position in the center
  
  // generate texture image
  Mat textImage = Mat(imageSize, CV_8UC3, Scalar(0,0,0));
  putText(textImage, text, origin, font, fontScale, Scalar(255,255,255), thickness);
  
  // generate texture from image data
  this->texture = Texture(textImage.data, textImage.size().width, textImage.size().height, textImage.channels());
  
  // vertices and texture coordinates
  vector<GLfloat> vertices = {
    -1,-1,0, 1,-1,0, -1,1,0,
    1,1,0,   -1,1,0, 1,-1,0
    -1,-1,0, 1,-1,0, -1,1,0,
    1,1,0,   -1,1,0, 1,-1,0
  };
  vector<GLfloat> texcoords = {
    0,1, 1,1, 0,0,
    1,0, 0,0, 1,1
  };
  
  // scale by the height
  float aspect = float(textImage.size().width) / float(textImage.size().height);
  for(int i = 0; i < vertices.size() / 3; i++) {
    vertices[i * 3] *= midHeight * aspect;
    vertices[i * 3 + 1] *= midHeight;
  }
  
  // set size
  this->size = glm::vec2(midHeight * aspect * 2, midHeight * 2);
  
  // gl bindings and upload
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &this->VBO);
  
  // pack vertices and texture coordinates
  vector<GLfloat> verticesAndTexcoords;
  verticesAndTexcoords.insert(verticesAndTexcoords.end(), vertices.begin(), vertices.end());
  verticesAndTexcoords.insert(verticesAndTexcoords.end(), texcoords.begin(), texcoords.end());
  
  long byteLength = sizeof(GLfloat) * verticesAndTexcoords.size();
  long texcoordsOffset = sizeof(GLfloat) * vertices.size();
  
  glBindVertexArray(VAO);
  {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, byteLength, verticesAndTexcoords.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (GLvoid*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (GLvoid*)texcoordsOffset);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }
  glBindVertexArray(0);
}

void GLTextNode::draw(Shader shader) {
  this->texture.activate(shader);
  shader.setInteger("texture", 0);
  shader.setFloat("scale", this->scale);
  shader.setVector2f("translation", this->position);
  
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}

