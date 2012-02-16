/*
 *  KoreaParticle.h
 *  mainServer
 *
 *  Created by Chris on 2/15/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */
#pragma once

#include "ofMain.h"

class KoreaParticle {
	
	public:
	
	ofVec3f pos;
	ofVec3f vel;
	float damping;
	
		
	void setup(ofVec3f pos = ofVec3f(0,0,0),ofVec3f vel = ofVec3f(0,0,0), float damping = .99f );
	void update();
	void draw();
	
};
