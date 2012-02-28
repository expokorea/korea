#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "ofxGui.h"

class ParticleSystem {
	public:

	
	vector<Particle> particles;
	
	void setup(int numParticles);
	void drawAll();
	void drawForGlow();
	void updateAll(float turbulence);
	void updateAll();
	
	void calculate();
	
	ofVec3f avgPosition;
	ofVec3f avgVelocity;
	ofVec3f stdDevPosition;
	ofVec3f stdDevVelocity;
	
	float pctLocked;
	
	//unused now
	ofVec3f center;
	
	//gui
	bool bShowGui;
	ofxPanel gui;
	ofxFloatSlider guiSpread;
	ofxFloatSlider guiSpeed;
	ofxFloatSlider guiVisc;
	ofxFloatSlider radius;
	
	void drawGui();
	
	
};
