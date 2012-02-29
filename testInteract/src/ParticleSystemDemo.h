/*
 * ParticleSystemDemo.h
 *
 *  Created on: 16/02/2012
 *      Author: arturo
 */

#pragma once

#include "ofGui.h"

class ParticleSystemDemo {
public:
	ParticleSystemDemo();

	void update();
	void drawForGlow();
	void draw();

	ofIntSlider r,g,b;
	ofFloatSlider speed;
	ofFloatSlider radius;

	int minAlpha, maxAlpha;

private:
	float t;
};

