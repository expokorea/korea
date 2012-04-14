/*
 * Gui.h
 *
 *  Created on: 31/03/2012
 *      Author: arturo
 */

#pragma once
#include "ofConstants.h"
#include "ofxGui.h"
#include "Glow.h"
#include "OscContoursClient.h"
#include "VideoPlayer.h"

class Gui {
public:
	void setGlow(Glow & glow);
	void setContoursClient(OscContoursClient & contoursClient);
	void setPlayer(VideoPlayer & _player);
	void setup(int x, int y);
	void load(string file);
	void draw();

	ofxPanel guiParticles;
	ofxPanel guiPSystem;
	ofxPanel guiField;
	ofxPanel guiRender;
	ofxPanel guiAudio;



	ofxParameter<float> fps;
	ofxParameter<bool> recording;
	ofxParameter<int> viewportNum;
	ofxParameter<float> overlap;
	ofxParameter<bool> viewportsInFbo;
	ofxParameter<bool> drawGrid;
	ofxParameter<bool> drawViewports;
	ofxParameter<bool> drawOverlap;
	ofxParameter<bool> drawOverlapMarks;
	ofxParameter<bool> drawAllViewports;
	ofxParameter<bool> showVideo;
	ofxParameter<bool> overlapVideo;
	ofxParameter<float> ratio;
	ofxParameter<float> virtualMouseX,virtualMouseY;
	ofxParameter<float> minLeft,maxLeft,minRight,maxRight;
	ofxParameter<int> fov;
	ofxParameter<bool> runawayEvents;
	ofxParameter<bool> mouseUser;

private:
	Glow * glow;
	OscContoursClient * contoursClient;
	VideoPlayer * player;
};

