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
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"

typedef enum{
	KPARTICLE_TARGET,
	KPARTICLE_FLOCKING,
	KPARTICLE_EATING
}koreaParticleState;


class KoreaParticle {
	
	public:
	
	ofVec3f  pos;
	ofVec3f  vel;
	ofVec3f  acc;
	float    damping;
	ofVec3f  target;
	float    targetForce;
	float	 targetSpeed;
	bool     bUseTarget;
	bool     bFlocking;
	bool	 bDrawTrails;
	bool	 bEating;
	
	// vars for noise / time
	float rt;
	float t;
	
	// trails
	ofVboMesh trailStrip,trailStripForGlow;
	vector<ofVec3f> trails;
	vector<float> angles;
	vector<float> zangles;
	
	// flocking params
	float sepDist,alnDist,cohDist;
	float separation,cohesion,alignment;
	float countCoh,countSep,countAlign;
	ofVec3f sumCoh,sumSep,sumAlign;
	
	// current state
	koreaParticleState particleState;
	float worldWidth,worldHeight,worldDepth;
	
	// node debug
	ofNode node;
	
	
	static ofxIntSlider r,g,b;
	static ofxToggle debug;
	static ofxToggle useModel;
	static ofxFloatSlider thickness;
	static ofxIntSlider length;
	static float speedFactor;
	
	// for group flocking (only flock with others of my group)
	unsigned int groupFlag;
	
	static ofxAssimpModelLoader model;
	
	
	KoreaParticle();
	KoreaParticle(unsigned int flag){ groupFlag = flag; };
	
	void setup(ofVec3f pos = ofVec3f(0,0,0), ofVec3f vel = ofVec3f(0,0,0), float damping = .99f );
	void update(float dt = 1);
	
	void draw();
	void drawForGlow();
	void drawDebug();

	// target methods
	void setTarget(ofVec3f targ, float targetForce);
	void applyTargetAttraction();
	void repelFrom(ofVec3f pt, float force, float dist);

	// flocking methods
	void resetFlocking();
	void applyForces();
	void addForFlocking(KoreaParticle * sister);
	void setFlockingParams();
	
	// states / behaviors
	void setState(koreaParticleState state);
	
	
};
