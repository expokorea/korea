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
#include "ofGui.h"

class KoreaFlock{
	
public:
	
	vector<KoreaParticle> particles;
	
	void setup( int total);
	void update();
	void draw();
	void drawForGlow();
	
	ofFloatSlider speed;
	ofToggle useTrails;
	ofPanel gui;
	ofColor color;
};