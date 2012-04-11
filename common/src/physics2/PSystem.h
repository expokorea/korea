#pragma once

#include "ofConstants.h"
#include "RibbonParticle.h"
#include "EatableParticleField.h"
#include "ParticleFlocker.h"

class PSystem{
public:
	void setup(int numParticles);
	void setUserPosition(const ofVec3f & userPosition);
	void update();
	void draw();
	void drawForGlow();

	void runAway();
	void goBack();
	void hide(const ofVec3f & target);

	vector<RibbonParticle> particles;
	static ofxParameter<float> bbW,bbH,bbD,bbZ,bbY;
	static ofxParameter<bool> drawBB;

private:
	EatableParticleField field;
	BoundingBox3D bb;
	ParticleFlocker flocker;
	ofVec3f userPosition;
};
