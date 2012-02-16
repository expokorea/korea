#include "Particle.h"
#include "ofGraphics.h"


ofVec3f
	Particle::centeringForce,
	Particle::globalOffset,
	Particle::avg;


float
	Particle::speed,
	Particle::spread,
	Particle::viscosity,
	Particle::independence,
	Particle::neighborhood,
	Particle::targetForce,
	Particle::noiseScaleInput,
	Particle::noiseScaleOutput,
	Particle::curTime;
	
	int Particle::curFrame;

Particle::Particle(float radius) {
    randomize(localOffset);
  	randomize(position);
  	position *= radius;
	bVisible = true;
	color.set(1,1,1,1);
	state = PARTICLE_FLOCKING;//PARTICLE_FREE;
	lockedPct = 0;
	lastPosition = position;
	size = ofRandom(3,5);
}

void Particle::drawForGlow() {
	if(bVisible){
		//ofSetColor(color.x,color.y,color.z,color.w);
		ofSphere(position,	size);
	}
}

void Particle::draw() {
	if(bVisible){
		//ofSetColor(color.x,color.y,color.z,color.w);
		ofSphere(position,	float(size)*.4);
	}
}

void Particle::applyFlockingForce(bool bAccountForTargetForce){
	
	float basex = (position.x / neighborhood)*Particle::noiseScaleInput;
	float basey = (position.y / neighborhood)*Particle::noiseScaleInput;
	float basez = (position.z / neighborhood)*Particle::noiseScaleInput;
	
	ofVec3f addToForce;
	
    addToForce.x +=
	ofSignedNoise(
				  basex + (globalOffset.x + localOffset.x * independence),
				  basey,
				  basez)*Particle::noiseScaleOutput;
	addToForce.y +=
	ofSignedNoise(
				  basex,
				  basey + (globalOffset.y  + localOffset.y * independence),
				  basez)*Particle::noiseScaleOutput;
	addToForce.z +=
	ofSignedNoise(
				  basex,
				  basey,
				  basez + (globalOffset.z + localOffset.z * independence))*Particle::noiseScaleOutput;


	if( bAccountForTargetForce ){
		addToForce *= (1-targetForce);
	}
	
	force += addToForce;
	
}

float Particle::getCurrentTime(){
	if( ofGetFrameNum() != curFrame ){
		curFrame = ofGetFrameNum();
		curTime  = ofGetElapsedTimef();
	}
	return curTime;
}

void Particle::applyViscosityForce() {
    force += velocity * -viscosity;
}

void Particle::applyTargetForce() {
	ofVec3f diff = targetPosition - position;
	//diff.normalize();
	diff /= 100.0;
	
	force += (diff * targetForce);
}


void Particle::applyCenteringForce() {
    centeringForce.set(position);
    centeringForce -= avg;
    float distanceToCenter = centeringForce.length();
    centeringForce.normalize();
    centeringForce *= -distanceToCenter / (spread * spread);
    force += centeringForce;
}

void Particle::clearQueueState(){
	stateQueue.clear();
}

void Particle::queueState(particleState stateIn,  float timeInState){
	stateQueue.push_back(timedState(stateIn, timeInState + Particle::getCurrentTime()));
}

void Particle::updateQueue(float timeInF){
	if(	stateQueue.size() ){
		if( stateQueue.front().timeTill <= timeInF ){
			startState(stateQueue.front().state);
			stateQueue.erase(stateQueue.begin() );
		}
	}
}

void Particle::startState(particleState newState){
	state = newState;
	//printf("state is now %i\n", state); 
	
	//if( newState == PARTICLE_EXPLODE ){
	//	force = explodeForce;
	///}
	if( newState == PARTICLE_FREE ){
		//something?
	}	
	if( newState == PARTICLE_FLOCKING ){
		//something?
	}	
		
}

void Particle::update(){

//   force.set(0, 0, 0);
//    applyFlockingForce();
//    applyViscosityForce();
//	applyCenteringForce();
//	applyTargetForce();

	lastPosition = position;
	
	/*if( state == PARTICLE_EXPLODE ){
		force    *= 0.95;
		velocity *= 0.987;
		
		lockedPct *= 0.98;
		
	}else{	*/
		force.set(0, 0, 0);
		applyViscosityForce();
		applyCenteringForce();
		
		lockedPct *= 0.98;
		
	//}
	 
	if( state == PARTICLE_FLOCKING ){	
		applyFlockingForce(false);
		
		lockedPct *= 0.98;
		
	}else if( state == PARTICLE_TARGET ){	
		applyTargetForce();
		applyFlockingForce(true);	
		
		lockedPct = 0.95 * lockedPct + 0.05 * 1.0;
		
	}	
	
    velocity += force; // mass = 1
    position += velocity * speed;
}
