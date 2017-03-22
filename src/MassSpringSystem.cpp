//
//  MassSpringSystem.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/20/17.
//
//

#include "MassSpringSystem.hpp"
using namespace std;
using namespace cv;

MassSpringSystem::MassSpringSystem(int numParticles,
                                   MSForceFunction forceFunction,
                                   ParticleInitFunction particleInitFunction,
                                   UpdateFunction updateFunction,
                                   SpringInitFunction springInitFunction,
                                   float lifeMax) : ParticleSystem(numParticles, ForceFunction(), particleInitFunction, lifeMax)
{
//  this->numParticles = numParticles;
//  this->forceFunction = forceFunction;
//  this->particleInitFunction = particleInitFunction;
  this->updateFunction = updateFunction;
  this->springInitFunction = springInitFunction;
  springInitFunction(this->particles, this->springs);
//  this->lifeMax = lifeMax;
//  
//  for(int i = 0; i < numParticles; i++)
//  {
//    Particle p;
//    particleInitFunction(p, i);
//    this->particles.push_back(p);
//  }
//  
//  this->clock = 0;
}

void MassSpringSystem::update(float dt)
{
  updateFunction(*this, dt);
//  else
//    ParticleSystem::update(dt);
//  // timing
//  dt = dt/2;
//  this->clock += dt;
//  
//  // get forces
//  vector<glm::vec3> forces = forceFunction(this->particles, this->clock);
//  
//  // update particles
//  for(int i = 0; i < particles.size(); i++)
//  {
//    Particle *it = &particles[i];
//    it->velocity += dt * (forces[i] / it->mass);
//    it->position += dt * it->velocity;
//    it->life -= dt;
//    if(it->life < 0)
//      particleInitFunction(*it, i);
//  }
}


//void MassSpringSystem::getPositons(vector<GLfloat>& positions)
//{
//  positions.clear();
//  for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
//  {
//    positions.insert(positions.end(), glm::value_ptr(it->position), glm::value_ptr(it->position)+3);
//  }
//}

