/*
 *  userData.h
 *  testInteract
 *
 *  Created by Chris on 2/29/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class KUserData{

	public:

	KUserData(){};
	
	ofVec3f pos;
	vector<ofPoint> contour;
	float timeLastSeen;
	
	void setup();
	void drawUser();
	
	//
	// void setContour(vector<ofPoint> pts);
	// void setPosition(ofVec3f pos);
	
	void debugSetUserContour();		// sets fake contour points for debugging
	void debugSetUserPosFromMouse(float mouseX, float mouseY, float z = 0);
	
	ofVec3f getPosition(){ return pos; };
	float getTimeLastSeen(){ return timeLastSeen; }
	vector<ofPoint> getContour(){ return contour; }

	ofxFloatSlider targetMovement,targetForce;
	ofVec3f target;
	float prevMouseX, prevMouseY;
};
