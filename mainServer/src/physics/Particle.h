#pragma once

#include "ofMain.h"
//#include "ofxVec3f.h"
//#include "ofxVec4f.h"
//#include "ofxMSAPerlin.h"

inline void randomize(ofVec3f& v) {
	v.x = ofRandomf();
	v.y = ofRandomf();
	v.z = ofRandomf();
	v.normalize();
}

typedef enum{
	PARTICLE_FREE,
	PARTICLE_FLOCKING,
	PARTICLE_TARGET
}particleState;

struct timedState{
	
	timedState(particleState _state, float time){
		state			= _state;
		timeTill		= time;
	}
	
	particleState state;
	float timeTill;
};

class Particle {
	public:
		
		//------------------------------------------------
		static ofVec3f centeringForce, globalOffset, avg;
		static float speed, spread, viscosity, independence, rebirthRadius, neighborhood, targetForce;
		static float noiseScaleInput, noiseScaleOutput;
		
		static int		curFrame;
		static float	curTime;
		
		//------------------------------------------------
		float		lockedPct;
		ofVec4f color;
		ofVec3f center;
		ofVec3f position, velocity, explodeForce, force, localOffset, lastPosition;
		bool bVisible;
		ofVec3f targetPosition;
		int size;
		
		particleState state;
		
		Particle(float radius);
		void draw();
		void applyFlockingForce(bool bAccountForTargetForce = true);
		void applyViscosityForce();
		void applyCenteringForce();
		void applyTargetForce();
		
		static float getCurrentTime();

		void startState(particleState newState);

		void queueState(particleState stateIn, float timeToStartState);
		void updateQueue(float timeInF);
		void clearQueueState();
		
		float lastTime;
		
		vector <timedState> stateQueue;
		
		void update();
};
