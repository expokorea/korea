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
#include "ofAppRunner.h"

class Glow {
public:
	Glow();
	virtual ~Glow();

	void setup(float w=ofGetWidth(), float h=ofGetHeight());
	void begin(bool setPerspective);
	void end();

	void draw(float x, float y);
	void draw(float x, float y, float w, float h);

	ofxParameter<int> passes;

private:
	ofFbo fbo1, fbo2;
	ofShader shader;
};

