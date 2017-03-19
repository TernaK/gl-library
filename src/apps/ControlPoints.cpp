//
//  ControlPoints.cpp
//  playground-modern
//
//  Created by Terna Kpamber on 3/15/17.
//  Copyright © 2017 Terna Kpamber. All rights reserved.
//

#include "ResourceManager.hpp"
#include "GLNode.hpp"
#include "AppTestUtility.hpp"
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.hpp"

float scale = 0.5;
float peak = 0.7;

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
  GLfloat aspectRatio = GLfloat(width)/height;
  
  // shader
//  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl", "resources/shaders/material_gshader.glsl");
  Shader shader = Shader("resources/shaders/material_vshader.glsl", "resources/shaders/material_fshader.glsl");
  
  // light
  Light light;
  
  //points
  vector<GLfloat> vertices;
  vector<GLfloat> normals;
  createSurface(vertices, normals, 21, 13, 2);
  
  GLNode surface = GLNode(vertices, normals);
  
//  float clock = 0;
  // render loop
  while(!glfwWindowShouldClose(window))
  {
    //timing
//    float now = glfwGetTime();
//    float dt = now - clock;
//    clock = now;
    
    glfwPollEvents();
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    
    // select and shader
    shader.use();
    
    // setup model/view/projection
    glm::vec3 eye = glm::vec3(10,10,10);
    glm::mat4 view = glm::lookAt(eye, glm::vec3(0), glm::vec3(0,1,0));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 50.0f);
    shader.setMatrix4("view", view);
    shader.setMatrix4("projection", projection);
    
    // setup light
    light.position = glm::vec3(3,3,3);
    shader.setVector3f("eyePosition", eye);
    light.setInShader(shader);

    // render
    surface.scale = glm::vec3(scale);
    surface.draw(shader);
    surface.rotation += glm::vec3(0, 0.01, 0);
    
    glfwSwapBuffers(window);
  }
  
  glfwTerminate();
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  if(key == GLFW_KEY_UP && action == GLFW_PRESS)
    scale += 0.1;
  if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    scale -= 0.1;
}

vector<vector<glm::vec3>> createRandomGrid(int numX, int numZ, int resolution)
{
  assert(resolution >= 2);
  
  // coeffsTransform
  float coeffsTransformData[] =
  {
    1, 	0, 	0, 	0,
    -3, 3,	0,	0,
    3, 	-6,	3, 	0,
    -1,	3, -3,	1
  };
  glm::mat4 coeffsTransform = glm::make_mat4(coeffsTransformData);
  coeffsTransform = glm::transpose(coeffsTransform);
  
  // initial control points
  glm::vec4 controlPoints = glm::vec4(randomFloat(), randomFloat(), randomFloat(), randomFloat());
  
  float x0 = float(numX-1) / -2.0f;
  float z0 = float(numZ-1) / -2.0f;
  // create mesh
  vector<vector<glm::vec3>> grid = vector<vector<glm::vec3>>(numZ, vector<glm::vec3>());
  for(int z = 0; z < numZ; z++)
  {
    for(int x = 0; x < numX; x++)
    {
      //get new coefficients
      glm::vec4 coeffs = coeffsTransform * controlPoints;
      
      //use control points
      float u = 0;
      float uStep = 1.0f/(resolution-1);
      
      //step u form 0 to 1
      for(int c = 0; c < resolution; c++)
      {
        glm::vec4 uVec = glm::vec4(1, u, u*u, u*u*u);
        
        bool skip = x > 0 && c == 0; //avoid writing a location twice
        if(!skip)
        {
          float realX = x + x0 + u;
          float realZ = z + z0;
          float y = glm::dot(uVec, coeffs) * peak;
          grid[z].push_back(glm::vec3(realX, y, realZ));
        }
        
        //increment u
        u += uStep;
      }
      
      // get new control points
      controlPoints = glm::vec4(controlPoints.w, randomFloat(), randomFloat(), randomFloat());
    }
  }
  
  //TODO: insert z resolution
  return grid;
}


void createSurface(vector<GLfloat>& vertices, vector<GLfloat>& normals, int numX, int numZ, int resolution)
{
  /* form grid of locations */
  vector<vector<glm::vec3>> grid = createRandomGrid(numX, numZ, resolution);
  
  /* form vertices */
  vertices.clear();
  normals.clear();
  for(int z = 0; z < grid.size() - 1; z++)
  {
    for(int x = 0; x < grid[0].size() - 1; x++)
    {
      /* get triangle 1 */
      vertices.insert(vertices.end(), glm::value_ptr(grid[z][x]), glm::value_ptr(grid[z][x])+3);//top left
      vertices.insert(vertices.end(), glm::value_ptr(grid[z+1][x]), glm::value_ptr(grid[z+1][x])+3);//bottom left
      vertices.insert(vertices.end(), glm::value_ptr(grid[z+1][x+1]), glm::value_ptr(grid[z+1][x+1])+3);//bottom right
      
      /* normal */
      glm::vec3 l1 = grid[z+1][x] - grid[z][x];
      glm::vec3 l2 = grid[z][x+1] - grid[z][x];
      glm::vec3 normal = glm::normalize(glm::cross(l1, l2));//note the order of the cross
      for(int i = 0; i < 3; i++)
        normals.insert(normals.end(), glm::value_ptr(normal), glm::value_ptr(normal)+3);
      
      /* get triangle 2 */
      vertices.insert(vertices.end(), glm::value_ptr(grid[z+1][x+1]), glm::value_ptr(grid[z+1][x+1])+3);//bottom right
      vertices.insert(vertices.end(), glm::value_ptr(grid[z][x+1]), glm::value_ptr(grid[z][x+1])+3);//top right
      vertices.insert(vertices.end(), glm::value_ptr(grid[z][x]), glm::value_ptr(grid[z][x])+3);//top left
      
      /* normal */
      l1 = grid[z][x] - grid[z][x+1];
      l2 = grid[z+1][x+1] - grid[z][x+1];
      normal = glm::normalize(glm::cross(l1, l2));//note the order of the cross
      for(int i = 0; i < 3; i++)
        normals.insert(normals.end(), glm::value_ptr(normal), glm::value_ptr(normal)+3);
    }
  }
}
