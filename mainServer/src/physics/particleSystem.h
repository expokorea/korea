#pragma once

#include "ofMain.h"
#include "Particle.h"
#include "ofGui.h"

class ParticleSystem {
	public:

	
	vector<Particle> particles;
	
	void setup(int numParticles);
	void drawAll();
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
	ofPanel gui;
	ofFloatSlider guiSpread;
	ofFloatSlider guiSpeed;
	ofFloatSlider guiVisc;
	
	void drawGui();
	
	
};
