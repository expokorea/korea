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
	bool     bFlocking;
	bool	 bDrawTrails;
	
	float rt;
	float t;
	
	vector<ofVec3f> trails;
	
	// flocking params
	float sepDist,alnDist,cohDist,separation,cohesion,alignment;
	float countCoh,countSep,countAlign;
	ofVec3f sumCoh,sumSep,sumAlign;
	
	void setup(ofVec3f pos = ofVec3f(0,0,0), ofVec3f vel = ofVec3f(0,0,0), float damping = .99f );
	void update();
	void draw();
	void draw3D();
	void drawForGlow();

	void setTarget(ofVec3f targ, float targetForce);
	void applyTargetAttraction();
	
	void resetFlocking();
	void applyForces();
	void addForFlocking(KoreaParticle * sister);
	
};
