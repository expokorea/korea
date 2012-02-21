/*
 * ParticleSystemDemo.h
 *
 *  Created on: 16/02/2012
 *      Author: arturo
 */

#pragma once

#include "ofGui.h"
#include "ofxAssimpModelLoader.h"

class ParticleSystemDemo {
public:
	ParticleSystemDemo();

	void update();
	void drawForGlow();
	void draw();

	void drawPath();

	ofIntSlider r,g,b;
	ofFloatSlider speed;
	ofFloatSlider radius;

	int minAlpha, maxAlpha;
	ofPolyline path;

private:
	float t;
	ofxAssimpModelLoader model;
	ofVboMesh particleMesh;
};

