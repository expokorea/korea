/*
 * ParticleSystemDemo.h
 *
 *  Created on: 16/02/2012
 *      Author: arturo
 */

#pragma once

#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"

class ParticleSystemDemo {
public:
	ParticleSystemDemo();

	void setup();
	void update();
	void drawForGlow();
	void draw();

	void drawPath();

	ofxIntSlider r,g,b;
	ofxFloatSlider speed;
	ofxFloatSlider radius;

	int minAlpha, maxAlpha;
	ofPolyline path;

private:
	float t;
	ofVboMesh particles;
	ofTexture tex;
};

