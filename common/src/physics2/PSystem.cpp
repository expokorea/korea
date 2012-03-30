#include "PSystem.h"


void PSystem::setup(int numParticles)
{
	field.setup();
	particles.resize(numParticles);
	for(int i=0;i<particles.size();i++){
		particles[i].setup();
	}
	flocker.setup(particles.size());
}



void PSystem::update(const BoundingBox3D & bb)
{
	field.update(ofGetLastFrameTime(),bb);
	flocker.update(ofGetLastFrameTime(),particles);
	for(int i=0;i<particles.size();i++){
		if(!flocker.getIsFollowing(i))
			particles[i].target.set(field.getParticle(&particles[i]));
		particles[i].update(ofGetLastFrameTime(),bb);
	}
}



void PSystem::draw()
{
	for(int i=0;i<particles.size();i++){
		particles[i].draw();
	}
	ofSetColor(RibbonParticle::r,RibbonParticle::g,RibbonParticle::b);
	field.draw();
	//flocker.draw(particles);
}

void PSystem::drawForGlow()
{
	for(int i=0;i<particles.size();i++){
		particles[i].drawForGlow();
	}
	ofSetColor(RibbonParticle::r,RibbonParticle::g,RibbonParticle::b);
	field.draw();
}


void PSystem::runAway(){
	for(int i=0;i<particles.size();i++){
		particles[i].runAway();
	}
}

void PSystem::goBack(){
	for(int i=0;i<particles.size();i++){
		particles[i].goBack();
	}
}

