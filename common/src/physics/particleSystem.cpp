#include "particleSystem.h"



void ParticleSystem::setup(int numParticles) {
	
	
	
	center.set(0,0,0);
	
	Particle::globalOffset.set(0, 1. / 3, 2. / 3);
	Particle::speed = 30;
	Particle::spread = 100;
	Particle::viscosity = .924;
	Particle::independence = .75;
	Particle::neighborhood = 1700;
	Particle::targetForce = .1;
	Particle::noiseScaleInput	= .177;
	Particle::noiseScaleOutput	= .5;
	 
	gui.setup("particles");
	gui.add(guiSpread.setup("spread",Particle::spread,10,2000));
	gui.add(guiSpeed.setup("speed",Particle::speed,1,100));
	gui.add(guiVisc.setup("viscosity",Particle::viscosity,0,1));
	
	bShowGui = true;
	 //
	/*
	 Particle::spread			= panel.getValueF("particle_spread");
	 Particle::viscosity			= panel.getValueF("particle_viscosity");
	 Particle::independence		= panel.getValueF("particle_independence");
	 Particle::neighborhood		= panel.getValueF("particle_neighborhood");
	 Particle::targetForce		= panel.getValueF("particle_targetForce");	
	 Particle::noiseScaleInput	= panel.getValueF("noise_scale_input");
	 Particle::noiseScaleOutput	= panel.getValueF("noise_scale_output");
	 */
	 
	
	float radius = 550;
	for(int i = 0; i < numParticles; i++){
		particles.push_back(Particle(radius));
	}
}

void ParticleSystem::calculate(){
	
	avgPosition.set(0,0,0);
	avgVelocity.set(0,0,0);
	stdDevPosition.set(0,0,0);
	stdDevVelocity.set(0,0,0);
	
	int nVisible = 0;
	
	for(int i = 0; i < particles.size(); i++){
		if (particles[i].bVisible == true){
			
			avgPosition += particles[i].position;
			avgVelocity += particles[i].velocity;
			nVisible++;
		}
	}

	if (nVisible > 0){
		avgPosition /= (float)nVisible;
		avgVelocity /= (float)nVisible;
	}
	
	ofVec3f diffPosition, diffVelocity;
	for (int i = 0; i < particles.size(); i++){
		if (particles[i].bVisible == true){
			diffPosition = particles[i].position - avgPosition;
			diffVelocity = particles[i].velocity - avgVelocity;
			stdDevPosition += diffPosition * diffPosition;
			stdDevVelocity += diffVelocity * diffVelocity;
		}
	}
	
	if (nVisible > 0){
		stdDevPosition /= (float)nVisible;
		stdDevVelocity /= (float)nVisible;
		
		stdDevPosition.x = sqrt(stdDevPosition.x);
		stdDevPosition.y = sqrt(stdDevPosition.y);
		stdDevPosition.z = sqrt(stdDevPosition.z);
		
		stdDevVelocity.x = sqrt(stdDevVelocity.x);
		stdDevVelocity.y = sqrt(stdDevVelocity.y);
		stdDevVelocity.z = sqrt(stdDevVelocity.z);
	}
				
	
	
	float avgTargetForce = 0;
	
	for (int i = 0; i < particles.size(); i++){
		if (particles[i].bVisible == true){
			avgTargetForce += particles[i].lockedPct;
		}
	}
	
	if (nVisible > 0){
		avgTargetForce /= (float)nVisible;
	}
	
	pctLocked = ofClamp(avgTargetForce, 0,0.6) / 0.6f;
	
	
}


void ParticleSystem::drawAll() {
	for(int i = 0; i < particles.size(); i++)
		particles[i].draw();

}

void ParticleSystem::drawForGlow(){
	for(int i = 0; i < particles.size(); i++)
		particles[i].drawForGlow();
}

void ParticleSystem::updateAll(){
	/*
	Particle::avg.set(0, 0, 0);
	ofxVec3f sum;
	for(int i = 0; i < particles.size(); i++) {
		//particles[i].update();
		sum += particles[i].position;
	}
	Particle::avg = sum / particles.size();
	 */
}

void ParticleSystem::updateAll(float turbulence) {
	Particle::avg.set(0, 0, 0);
	ofVec3f sum;
	int nVisible = 0;
	float timeNow = ofGetElapsedTimef();
	for(int i = 0; i < particles.size(); i++) {
		if (particles[i].bVisible) {
			particles[i].update();
			sum += particles[i].position;
			nVisible++;
		}
			particles[i].updateQueue(timeNow);
			particles[i].center = center;
	}
	Particle::avg = sum / nVisible;
	Particle::globalOffset += turbulence / Particle::neighborhood;
	
	
	
	//
	Particle::spread = guiSpread;
	Particle::speed = guiSpeed;
	Particle::viscosity = guiVisc;
}


void ParticleSystem::drawGui(){
	if(bShowGui) gui.draw();
}

