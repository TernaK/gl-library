//
//  MassSpringSystem.hpp
//  gl_library
//
//  Created by Terna Kpamber on 3/20/17.
//
//

#ifndef MassSpringSystem_hpp
#define MassSpringSystem_hpp

#include <opencv2/opencv.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ParticleSystem.hpp"

// updates the postions of the particles
typedef std::function<void(std::vector<Particle>&, float)> UpdateFunction;

class MassSpringSystem : public ParticleSystem
{
public:
//  float clock;// current time of the system
//  std::vector<Particle> particles;
//  ForceFunction forceFunction;//force evaluation function
//  ParticleInitFunction particleInitFunction;
  UpdateFunction updateFunction;
  
  MassSpringSystem(int numParticles,
                   ForceFunction forceFunction,
                   ParticleInitFunction particleInitFunction,
                   UpdateFunction updateFunction,
                   float lifeMax = 1.0);
  void update(float dt);
//  void getPositons(std::vector<GLfloat>& positions);
//  void setLifeMax(float lifeMax) { assert(lifeMax > 0); this->lifeMax = lifeMax; }
//  float getLifeMax(){ return lifeMax; }
};

#endif /* MassSpringSystem_hpp */
