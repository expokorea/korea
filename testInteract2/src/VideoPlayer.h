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

class VideoPlayer {
public:
	void load(string path, bool usePBO=true);
	void play();
	void update();
	void draw(float x, float y, float w, float h);

	bool isFrameNew();

	int getWidth();
	int getHeight();

	float getPosition();
	float getDuration();
	unsigned long getCurrentFrame();
private:
	ofVideoPlayer player;
	ofPBO pbo;
	ofTexture tex;
	ofFbo fbo;
	bool usePBO;
};

