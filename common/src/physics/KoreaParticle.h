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
	KPARTICLE_EATING,
	KPARTICLE_HOVER,
	KPARTICLE_FADE_OUT
}koreaParticleState;


class KoreaParticle {
	
	public:
	
	ofVec3f  pos;
	ofVec3f  vel;

	float    damping;
	ofVec3f  target;
	float    targetForce;
	float	 targetSpeed;
	bool     bUseTarget;
	bool     bFlocking;
	bool	 bDrawTrails;
	bool	 bEating;
	bool	 bNeedHoverTarget;
	bool	 bArrivedAtTarget;
	
	
	// vars for noise / time
	float rt;
	float t;
	
	// trails
	ofVboMesh trailStrip,trailStripForGlow,texturedStrip,trailStripLineL,trailStripLineR;
	vector<ofVec3f> trails;
	
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
	
	// hovering
	int contourIndex;
	
	// eating
	float lastEatTime, eatWaitTime;
	
	// alpha changes
	float fadeAlpha; // for fadingaway at transition
	
	// state stats (not using yet...)
	float stateStartTime;
	
	// variation in form
	float myThickness;
	float myLength;
	
	static ofxIntSlider r,g,b;
	static ofxIntSlider rTex,gTex,bTex;
	static ofxIntSlider rLines,gLines,bLines;
	static ofxToggle debug;
	static ofxToggle useModel;
	float thickness;
	float length;
	static float speedFactor;
	static ofxFloatSlider flockAlpha; // less transparent when not following
	
	// for group flocking (only flock with others of my group)
	unsigned int groupFlag;
	
	static ofxAssimpModelLoader model;
	static ofFbo tex;
	
	
	KoreaParticle();
	KoreaParticle(unsigned int flag){ groupFlag = flag; };
	
	enum TexMode{
		Delaunay,
		Voronoi
	}texMode;
	static void generateTexture(TexMode mode);

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
	
	// states / behaviors
	void setState(koreaParticleState state);
	
	
};
