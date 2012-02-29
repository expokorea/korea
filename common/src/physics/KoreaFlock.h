/*
 *  Korea3DFlock.h
 *  testGlowShader
 *
 *  Created by Chris on 2/20/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

/*
 *  KoreaFlock.h
 *  mainServer
 *
 *  Created by Chris on 2/16/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "KoreaParticle.h"
#include "ofxGui.h"
#include "userData.h"

class KoreaFlock{
	
public:
	
	vector<KoreaParticle> particles;
	
	void setup( int total, int worldWidth = 1024, int worldHeight = 768, int worldDepth = 600);
	void update();
	void draw();
	void drawForGlow();
	
	//void debugUserCenter(ofPoint p);
	void debugUserCenter(KUserData & myUser);
	
	ofxFloatSlider speed;
	ofxFloatSlider sep;
	ofxFloatSlider coh;
	ofxFloatSlider aln;
	ofxToggle useTrails;
	ofxIntSlider userRadius;
	ofxPanel gui;
	ofColor color;
	
	
	float worldWidth,worldHeight,worldDepth;
	ofPoint user1;
	
	ofxAssimpModelLoader model;


};