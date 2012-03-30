#pragma once

#include "ofConstants.h"
#include "RibbonParticle.h"
#include "EatableParticleField.h"

class PSystem{
public:
	void setup(int numParticles);
	void update(const BoundingBox3D & bb);
	void draw();
	void drawForGlow();

	void runAway();
	void goBack();

private:
	vector<RibbonParticle> particles;
	EatableParticleField field;
};