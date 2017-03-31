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

void MassSpringSystem::createSurface(const MassSpringSystem& mss, int width, int height,
                                     vector<GLfloat>& vertices, vector<GLfloat>& normals)
{
  // form vertices and normals
  vertices.clear();
  normals.clear();
  
  for(int z = 0; z < height - 1; z++)
  {
    for(int x = 0; x < width - 1; x++)
    {
      // get triangle 1
      int tlIdx = z * width + x;
      int blIdx = (z + 1) * width + x;
      int trIdx = z * width + x + 1;
      int brIdx = (z + 1) * width + x + 1;
      glm::vec3 tl = mss.particles[tlIdx].position;
      glm::vec3 bl = mss.particles[blIdx].position;
      glm::vec3 tr = mss.particles[trIdx].position;
      glm::vec3 br = mss.particles[brIdx].position;
      vertices.insert(vertices.end(), glm::value_ptr(tl), glm::value_ptr(tl)+3);//top left
      vertices.insert(vertices.end(), glm::value_ptr(bl), glm::value_ptr(bl)+3);//bottom left
      vertices.insert(vertices.end(), glm::value_ptr(br), glm::value_ptr(br)+3);//bottom right
      
      // normal
      glm::vec3 l1 = bl - tl;
      glm::vec3 l2 = tr - tl;
      glm::vec3 normal = glm::normalize(glm::cross(l1, l2));//note the order of the cross
      for(int i = 0; i < 3; i++)
        normals.insert(normals.end(), glm::value_ptr(normal), glm::value_ptr(normal)+3);
      
      // get triangle 2
      vertices.insert(vertices.end(), glm::value_ptr(br), glm::value_ptr(br)+3);//bottom right
      vertices.insert(vertices.end(), glm::value_ptr(tr), glm::value_ptr(tr)+3);//top right
      vertices.insert(vertices.end(), glm::value_ptr(tl), glm::value_ptr(tl)+3);//top left
      
      // normal
      l1 = tr - br;
      l2 = bl - br;
      normal = glm::normalize(glm::cross(l1, l2));//note the order of the cross
      for(int i = 0; i < 3; i++)
        normals.insert(normals.end(), glm::value_ptr(normal), glm::value_ptr(normal)+3);
    }
  }
}


//void MassSpringSystem::getPositons(vector<GLfloat>& positions)
//{
//  positions.clear();
//  for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
//  {
//    positions.insert(positions.end(), glm::value_ptr(it->position), glm::value_ptr(it->position)+3);
//  }
//}

