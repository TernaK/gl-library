//
//  ParticleSystem.hpp
//  gl_library
//
//  Created by Terna Kpamber on 3/14/17.
//
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include <opencv2/opencv.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef struct Particle
{
  // members
  glm::vec3 position;
  glm::vec3 velocity;
  float mass;
  float life;
  
  // constructor
  Particle(glm::vec3 pos = glm::vec3(0),
           glm::vec3 vel = glm::vec3(0),
           float mass = 1, float life = 5)
  {
    this->position = pos;
    this->velocity = vel;
    this->mass = mass;
    this->life = life;
  }
} Particle;

// returns an array of forces for the particles
typedef std::function<std::vector<glm::vec3>(const std::vector<Particle>&, float)> ForceFunction;
typedef std::function<void(Particle&, int)> ParticleInitFunction;

class ParticleSystem
{
public:
  float clock;// current time of the system
  std::vector<Particle> particles;
  ForceFunction forceFunction;//force evaluation function
  ParticleInitFunction particleInitFunction;
  
  ParticleSystem(int numParticles, ForceFunction forceFunction, ParticleInitFunction particleInitFunction, float lifeMax = 1.0);
  void update(float dt);
  void getPositons(std::vector<GLfloat>& positions);
  void setLifeMax(float lifeMax) { assert(lifeMax > 0); this->lifeMax = lifeMax; }
  float getLifeMax(){ return lifeMax; }
  
protected:
  int numParticles;
  float lifeMax;
};

#endif /* ParticleSystem_hpp */
