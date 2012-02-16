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
	void begin();
	void end();

	void draw(float x, float y);

	ofParameter<float> brightness;
	ofParameter<int> passes;

private:
	ofFbo fbo1, fbo2;
	ofShader shader;
};

