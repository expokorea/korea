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
	ofEvent<int> particlesOut;
	ofEvent<bool> mappingIn;
	ofEvent<bool> mappingOut;

	struct ParticlesOutEvent{
		ParticlesOutEvent(int s, int t):seconds(s),target(t){}
		int seconds;
		int target;

		bool operator>=(int t){
			return seconds>=t;
		}
		bool operator<(int t){
			return seconds<t;
		}
	};
private:
	VideoPlayer * player;
	ofxXmlSettings times;
	vector<int> timesParticlesin;
	vector<ParticlesOutEvent> timesParticlesout;
	vector<int> timesMappingin;
	vector<int> timesMappingout;
	float prevTime;
};

