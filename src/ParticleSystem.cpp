//
//  ParticleSystem.cpp
//  gl_library
//
//  Created by Terna Kpamber on 3/14/17.
//
//

#include "ParticleSystem.hpp"
using namespace std;
using namespace cv;

ParticleSystem::ParticleSystem(int numParticles, ForceFunction forceFunction)
{
  this->numParticles = numParticles;
  this->forceFunction = forceFunction;
  
  for(int i = 0; i < numParticles; i++)
  {
    Particle p;
    initParticle(p);
    this->particles.push_back(p);
  }
  
  this->clock = 0;
}

void ParticleSystem::initParticle(Particle& particle)
{
  glm::vec3 position = glm::vec3(0,-0.5,0);
  position.x += ((float(arc4random() % 100) / 100.0f) - 0.5) * 0.1;
  position.y += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.05;
  position.z += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.1;
  
  glm::vec3 velocity = glm::vec3(0,5,0);
  velocity.x += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 3;
  velocity.y += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 1;
  velocity.z += ((float(arc4random() % 100) / 100.0f) - 0.5)  * 0.1;
  
  particle.position = position;
  particle.velocity = velocity;
  particle.life = this->lifeMax - 1.0 + (float(arc4random() % 100) / 100.0f);
}


void ParticleSystem::update(float dt)
{
  // timing
  dt = dt/2;
  this->clock += dt;
  
  // get forces
  vector<glm::vec3> forces = forceFunction(this->particles, this->clock);
  
  // update particles
  for(int i = 0; i < particles.size(); i++)
  {
    Particle *it = &particles[i];
    it->velocity += dt * (forces[i] / it->mass);
    it->position += dt * it->velocity;
    it->life -= dt;
    if(it->life < 0)
      initParticle(*it);
  }
}


void ParticleSystem::getPositons(vector<GLfloat>& positions)
{
  positions.clear();
  for(vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
  {
    positions.insert(positions.end(), glm::value_ptr(it->position), glm::value_ptr(it->position)+3);
  }
}

/*
 cv::Mat getChange(const std::vector<glm::vec3>& forces, float dt);
 void updateParticlesFromState();
 void updateStateAtIndex(int index, const Particle& particle);
 */
/*
void ParticleSystem::updateStateAtIndex(int index, const Particle& particle)
{
  for(int j = 0; j < 3; j++)
  {
    state.at<float>(index,j) = particle.position[j];
    state.at<float>(index,j+3) = particle.velocity[j];
  }
}

cv::Mat ParticleSystem::getChange(const std::vector<glm::vec3>& forces, float dt)
{
  Mat change = Mat::zeros(numParticles, 6, CV_32FC1);
  for(int i = 0; i < particles.size(); i++)
  {
    glm::vec3 acceleration = forces[i] / particles[i].mass;
    for(int j = 0; j < 3; j++)
    {
      change.at<float>(i,j) = particles[i].velocity[j];
      change.at<float>(i,j+3) = acceleration[j];
    }
  }
  return change;
}
*/
