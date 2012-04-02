#include "PSystem.h"

ofxParameter<float> PSystem::bbW,PSystem::bbH,PSystem::bbD,PSystem::bbZ;
ofxParameter<bool> PSystem::drawBB;

void PSystem::setup(int numParticles)
{
	field.setup();
	particles.resize(numParticles);
	for(int i=0;i<particles.size();i++){
		particles[i].setup();
	}
	flocker.setup(particles.size());
}



void PSystem::update(const ofVec3f & userPosition)
{
	bb = BoundingBox3D(userPosition.x,userPosition.y,userPosition.z,bbW,bbH,bbD);
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

	ofSetColor(255);
	ofNoFill();
	if(drawBB){
		bb.draw();
	}
	ofFill();
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

void PSystem::hide(){
	for(int i=0;i<particles.size();i++){
		particles[i].hide();
	}
}
