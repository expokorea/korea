/*
 * VideoPlayer.h
 *
 *  Created on: 08/04/2012
 *      Author: arturo
 */

#pragma once

#include "ofConstants.h"
#include "ofVideoPlayer.h"
#include "ofPBO.h"
#include "ofFbo.h"
#include "SurroundVideoPlayer.h"
#include "ofxGui.h"

class VideoPlayer {
public:
	void load(string path,  string nameFront, string nameRear, string nameSubLFE, bool usePBO=true);
	void play();
	void update();
	void draw(float x, float y, float w, float h);

	bool isFrameNew();

	int getWidth();
	int getHeight();

	float getPosition();
	float getDuration();
	unsigned long getCurrentFrame();

	void setVolume(float & volume);
	ofxParameter<float> volume;

private:
	ofVideoPlayer player;
	ofPtr<SurroundVideoPlayer> surroundPlayer;
	ofPBO pbo;
	ofTexture tex;
	ofFbo fbo;
	bool usePBO;
	string nameFront;
	string nameRear;
	string nameSubLFE;
	string path;

};

