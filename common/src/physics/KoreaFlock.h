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

#define USE_TIME_BASED

typedef struct flockGroup{
	
	int groupFlag;
	int userId;
	bool bFollowing;
	ofVec3f pos;
	ofVec3f target;
	
};

// to keep track of which group is closest to user
typedef struct distIds{
	float dist;
	int id;
};


typedef struct userDistances{
	vector<distIds> distData;
};

class KoreaFlock{
	
public:
	
		
	// not in use (some vars not inited here...)
	void setup( int total, int worldWidth = 1024, int worldHeight = 768, int worldDepth = 600);
	
	// create the groups
	void setupInGroups( int worldWidth = 1024, int worldHeight = 768, int worldDepth = 600);

	void update();
	
	void draw();
	void drawForGlow();
	void drawDebug();
	
	// here is where most of the following is set. searches for closest
	// group to a user, assigns targets and sets new modes
	void assignUserTargets( vector<KUserData> users);
	
	void debugUserCenter(KUserData & myUser);
	
	// called periodically from update to create the illision of chasing prey
	void setRandomEating();
	
	vector<KoreaParticle> particles;
	vector<flockGroup> groups;
	
	ofxFloatSlider speed;
	ofxFloatSlider sep;
	ofxFloatSlider coh;
	ofxFloatSlider aln;
	ofxToggle useTrails;
	ofxIntSlider userRadius;
	ofxPanel gui;
	ofColor color;
	
	
	float worldWidth,worldHeight,worldDepth;
	float timeLastUpdate;
	
	// debugging 
	ofPoint user1;
	
	// vars to create a variable eat event
	float lastEatTime, eatWaitTime;

};