/*
 *  ParticleFlocker.h
 *  testInteract
 *
 *  Created by Chris on 3/30/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "RibbonParticle.h"

class ParticleFlocker{

	public:
	
		void setup( int nParticles );
		void update( float dt, vector<RibbonParticle> & particles );
		void draw( vector<RibbonParticle> & particles );
		
		bool getIsFollowing( int id ){
			return isFollowing[id];
		}
		
		static ofxParameter<int> totalToFlock;
		
	private:
		
		// who is following
		vector<bool>isFollowing;	
		vector<bool>isLeader;
		vector<float>followingTime;
		vector<int> whoIFollow;
		
		float timeDec;
};