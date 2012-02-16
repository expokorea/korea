/*
 *  KoreaParticle.h
 *  mainServer
 *
 *  Created by Chris on 2/15/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"



class KoreaParticle {
	
	public:
	
	ofVec3f  pos;
	ofVec3f  vel;
	float    damping;
	ofVec3f  target;
	float    targetForce;
	bool     bUseTarget;
	float    noiseForce;
	bool     bUseNoise;
	bool     bFlocking;
	
	float rt;
	
	float sepDist,alnDist,cohDist,separation,cohesion,alignment;
	float countCoh,countSep,countAlign;
	ofVec3f sumCoh,sumSep,sumAlign;
	
	void setup(ofVec3f pos = ofVec3f(0,0,0),ofVec3f vel = ofVec3f(0,0,0), float damping = .99f );
	void update();
	void draw();
	void drawForGlow();

	void setTarget(ofVec3f targ, float targetForce);
	
	void resetFlocking();
	void applyForces();
	void addForFlocking(KoreaParticle * sister);
	
};
