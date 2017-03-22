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

// hold indices of the particles connected to this spring
typedef struct Spring {
  float rest;//rest length
  float Ks;	// damping constant
  float Kd;	// spring constant
  int p1;		// particle index of the connected point
  int p2;		// particle index of the other connected point
  Spring();
  Spring(float rest, float Ks, float Kd, int p1, int p2) {
    this->rest = rest;
    this->Kd = Kd;
    this->Ks = Ks;
    this->p1 = p1;
    this->p2 = p2;
  }
} Spring;

// updates the postions of the particles
class MassSpringSystem;
typedef std::function<void(MassSpringSystem&, float)> MSForceFunction;
typedef std::function<void(MassSpringSystem&, float)> UpdateFunction;
typedef std::function<void(const std::vector<Particle>&, std::vector<Spring>&)> SpringInitFunction;

class MassSpringSystem : public ParticleSystem
{
public:
  std::vector<Spring> springs;
//  float clock;// current time of the system
//  std::vector<Particle> particles;
//  ForceFunction forceFunction;//force evaluation function
//  ParticleInitFunction particleInitFunction;
  MSForceFunction msForceFunction;
  UpdateFunction updateFunction;
  SpringInitFunction springInitFunction;
  
  MassSpringSystem(int numParticles,
                   MSForceFunction forceFunction,
                   ParticleInitFunction particleInitFunction,
                   UpdateFunction updateFunction,
                   SpringInitFunction springInitFunction,
                   float lifeMax = 1.0);
  void update(float dt);
//  void getPositons(std::vector<GLfloat>& positions);
//  void setLifeMax(float lifeMax) { assert(lifeMax > 0); this->lifeMax = lifeMax; }
//  float getLifeMax(){ return lifeMax; }
};

#endif /* MassSpringSystem_hpp */
