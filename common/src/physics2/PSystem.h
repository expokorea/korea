#pragma once

#include "ofMain.h"
#include "RibbonParticle.h"
#include "EatableParticleField.h"

class PSystem{

	void setup();
	void update();
	void draw();
	void updateBoundingBox(BoundingBox3D bbUser);
	
	vector<RibbonParticle> particles;
	EatableParticleField field;
	BoundingBox3D bb;
	ofxParameter<float> bbW,bbH,bbD;

};
