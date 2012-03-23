/*
 * Glow.h
 *
 *  Created on: 15/02/2012
 *      Author: arturo
 */

#pragma once

#include "ofFbo.h"
#include "ofShader.h"
#include "ofParameter.h"

class Glow {
public:
	Glow();
	virtual ~Glow();

	void setup();
	void begin(bool setPerspective);
	void end();

	void draw(float x, float y);
	void draw(float x, float y, float w, float h);

	ofxParameter<float> brightness;
	ofxParameter<int> passes;

private:
	ofFbo fbo1, fbo2;
	ofShader shader;
};

