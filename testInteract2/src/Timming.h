/*
 * Timming.h
 *
 *  Created on: 10/04/2012
 *      Author: arturo
 */

#pragma once

#include "ofEvents.h"
#include "VideoPlayer.h"
#include "ofxXmlSettings.h"

class Timming {
public:
	Timming();

	void setup(VideoPlayer & player);
	void update();

	ofEvent<bool> particlesIn;
	ofEvent<bool> particlesOut;
	ofEvent<bool> mappingIn;

private:
	VideoPlayer * player;
	ofxXmlSettings times;
	vector<int> timesParticlesin;
	vector<int> timesParticlesout;
	vector<int> timesMappingin;
	float prevTime;
};

