#pragma once

#include "ofConstants.h"
#include "RibbonParticle.h"
#include "EatableParticleField.h"
#include "ParticleFlocker.h"

class PSystem{
public:
	void setup(int numParticles);
	void update(const ofVec3f & userPosition);
	void draw();
	void drawForGlow();

	void runAway();
	void goBack();
	void hide();

	vector<RibbonParticle> particles;
	static ofxParameter<float> bbW,bbH,bbD,bbZ;
	static ofxParameter<bool> drawBB;

private:
	EatableParticleField field;
	BoundingBox3D bb;
	ParticleFlocker flocker;
};
