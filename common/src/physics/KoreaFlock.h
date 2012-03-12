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

typedef struct flockGroup{
	
	int groupFlag;
	int userID;
	bool bFollowing;
	ofVec3f pos;
	
};

typedef struct distIds{
	float dist;
	int id;
};


typedef struct userDistances{
	vector<distIds> distData;
};

class KoreaFlock{
	
public:
	
		
	void setup( int total, int worldWidth = 1024, int worldHeight = 768, int worldDepth = 600);
	void setupInGroups( int worldWidth = 1024, int worldHeight = 768, int worldDepth = 600);

	void update();
	
	void draw();
	void drawForGlow();
	void drawDebug();
	
	// user data
	void assignUserTargets( vector<KUserData> users);
	void debugUserCenter(KUserData & myUser);
	
	
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
	
	
	// debugging 
	ofPoint user1;
	

};