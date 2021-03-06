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

float zoom = 0.35;

float Theta = M_PI_4;
float Phi = M_PI_4;
float Radius = 6.0;

glm::vec3 calcCameraPosition(glm::vec3 target = glm::vec3(0)) {
  float x = cos(Phi) * sin(Theta);
  float y = sin(Phi);
  float z = cos(Phi) * cos(Theta);
  return (glm::vec3(x, y, z) * Radius) + target;
}

void opticalFlow(const cv::UMat& prevRGB, const cv::UMat& currentRGB, cv::UMat& outputFlow) {
  cv::UMat prev, current;
  cv::Mat flow;
  cv::cvtColor(prevRGB, prev, CV_BGR2GRAY);
  cv::cvtColor(currentRGB, current, CV_BGR2GRAY);
  double pyramidScale = 0.5;
  int levels = 3;
  int winSize = 11;
  int iterations = 3;
  int polyN = 5;
  double polySigma = 1.1;
  int flags = 0;
  cv::calcOpticalFlowFarneback(prev, current, flow, pyramidScale, levels, winSize, iterations, polyN, polySigma, flags);
  
  // format flow image
  double minVal, maxVal;
  cv::minMaxLoc(flow, &minVal, &maxVal);
  
  int pixelStep = 10;
  for(int row = 0; row < outputFlow.rows; row += pixelStep) {
    for(int col = 0; col < outputFlow.cols; col += pixelStep) {
      cv::Vec2f pixelFlow = flow.at<cv::Vec2f>(row, col);// * 10;
      if(cv::norm(pixelFlow) < 2) continue;
      cv::Point origin(col, row);
      cv::Point2f dest(round(col + pixelFlow[0]), round(row + pixelFlow[1]));
      
      cv::line(outputFlow, origin, dest, cv::Scalar(0,255,255));
    }
  }
}


int main(int argc, char * argv[])
{
  bool shouldClose = false;
  int width;// = 640;
  int height;// = 480;
  
  // create window
  GLFWwindow* window = glGetWindow(320, 240, "none", false);
  glfwGetFramebufferSize(window, &width, &height);
  glEnable(GL_MULTISAMPLE);
  
  glfwSetKeyCallback(window, keyCallback);
  
  // set viewport
  glViewport(0, 0, width, height);
  
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
  
  // shader
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  shader.use();
  
  // setup light
  Light light;
  light.position = glm::vec3(2,10,10);
  light.Kq = 0.001;
  light.Kl = 0.003;
  light.setInShader(shader);
  
  
  GLNode wheel1 = GLShapes::createCylinder(0.3, 0.2);
  wheel1.rotation.x = M_PI_2;
  wheel1.position = glm::vec3(-1.2, -0.3, 0.8);
  
  GLNode wheel2 = wheel1;
  wheel2.position = glm::vec3(1.2, -0.3, 0.8);
  
  GLNode wheel3 = wheel1;
  wheel3.position = glm::vec3(-1.2, -0.3, -0.8);
  
  GLNode wheel4 = wheel1;
  wheel4.position = glm::vec3(1.2, -0.3, -0.8);
  
  GLNode body = GLShapes::createCube();
  body.scale = glm::vec3(3, 0.75, 1.5);
  body.position = glm::vec3(0, 0, 0);
  
  GLNode top = GLShapes::createCube();
  top.scale = glm::vec3(1.5, 0.5, 1.48);
  top.position = glm::vec3(-0.3, 0.5, 0);
  
  
  Material blackMaterial = Material(glm::vec3(0.01), glm::vec3(0.01), glm::vec3(0.01), 0.001);
  Material blueMaterial = Material(glm::vec3(0.7, 0.01, 0.01), glm::vec3(0.1), glm::vec3(0.01), 0.1);
  wheel1.setMaterial(blackMaterial);
  wheel2.setMaterial(blackMaterial);
  wheel3.setMaterial(blackMaterial);
  wheel4.setMaterial(blackMaterial);
  body.setMaterial(blueMaterial);
  
  GLNode object;
  object.addChild(&body);
  object.addChild(&top);
  object.addChild(&wheel1);
  object.addChild(&wheel2);
  object.addChild(&wheel3);
  object.addChild(&wheel4);
  
  cv::namedWindow("frambuffer render");
  
  GLbyte *pixelData = new GLbyte[width*height*3];
  cv::UMat previousFrame = cv::UMat::zeros(height, width, CV_8UC3);
  
  int timingCount = 0;
  double timingAcc = 0;
  string fpsText = "___ ms per frame";
  
  do {
    double startTime = glfwGetTime();
    glfwPollEvents();
    
    // render
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7, 0.7, 0.7, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::vec3 eye = calcCameraPosition();//glm::vec3(0,-10,3);
    glm::mat4 view = glm::lookAt(eye / zoom, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(width)/height, 0.1f, 50.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    shader.setVector3f("eyePosition", eye);

    float t = glfwGetTime();
    float radius = 3.0f;
    
    object.position = glm::vec3(radius * cos(t), 0.0f, radius * sin(t)) + glm::vec3(cos((t-0.5)*2), 1.0f, sin((t-0.5)*2));
    object.draw(shader);
    
    // read pixels
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
    
    // update cv Mat
    cv::UMat currentFrame = cv::Mat(height, width, CV_8UC3, pixelData).getUMat(CV_STORAGE_READ);
    cv::flip(currentFrame, currentFrame, 0);
    cv::UMat flow = currentFrame.clone();
    
    opticalFlow(previousFrame, currentFrame, flow);
    cv::putText(flow, fpsText, cv::Point(10,flow.rows-10), CV_FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0,255,0));
    
    // timing
    double endTime = glfwGetTime();
    
    timingAcc += endTime - startTime;
    timingCount++;
    if(timingCount == 30) {
      fpsText = cv::format("%3.0f ms per frame", timingAcc * 1000 / 30);
      timingAcc = 0;
      timingCount = 0;
    }
    
    cv::imshow("frambuffer render", flow);
    int key = cv::waitKey(30);
    switch(key) {
        
      case 63232: Phi += 0.1; break;
        
      case 63233: Phi -= 0.1; break;
        
      case 63234: Theta -= 0.1; break;
        
      case 63235: Theta += 0.1; break;
        
      case int('i'): Radius -= 1; break;

      case int('o'): Radius += 1; break;
        
      case 27: shouldClose = true;
        
      default: break;
    }
    
    currentFrame.copyTo(previousFrame);
    
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
   if(key == GLFW_KEY_UP && action == GLFW_PRESS)
     zoom += 0.11;
   if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
     zoom -= 0.11;
}
