/*
 *  ParticleFlocker.cpp
 *  testInteract
 *
 *  Created by Chris on 3/30/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#include "ParticleFlocker.h"
#include "EatableParticleField.h"

ofxParameter<int>ParticleFlocker::totalToFlock;

void ParticleFlocker::setup( int nParticles )
{
	for(int i = 0; i < nParticles; i++) isFollowing.push_back(false);
	for(int i = 0; i < nParticles; i++) followingTime.push_back(0.f);
	for(int i = 0; i < nParticles; i++) isLeader.push_back(false);
	for(int i = 0; i < nParticles; i++)  whoIFollow.push_back(0);

	timeDec = 1;
	totalToFlock = 5;
}

void ParticleFlocker::update( float dt, vector<RibbonParticle> & particles )
{
	
	// check timers and update
	for( int i = 0; i < followingTime.size(); i++)
	{
		if(followingTime[i] > 0 ) followingTime[i] -= dt * timeDec;
		else isFollowing[i] = false;
	}
	
	for(int i=0;i<particles.size();i++){
		if(isFollowing[i] && particles[i].getPos().squareDistance(particles[i].target)<EatableParticleField::eatDistance){
			isFollowing[i] = false;
			followingTime[i] = 0;
		}
	}

	for( int i = 0; i < isLeader.size(); i++) isLeader[i] = false;
	
	for( int i = 0; i < isFollowing.size(); i++)
	{
		if(isFollowing[i])
		{
			int mom = whoIFollow[i];
			isLeader[mom] = true;
		}
		
	}
	
	int totalFollowers = 0;
	for( int i = 0; i < isFollowing.size(); i++)
		if(isFollowing[i]) totalFollowers++;
	
	// keep at least 5 following at all times
	int totalToAdd = MAX(0,totalToFlock-totalFollowers);
	
	vector<int>availables;
	for( int i = 0; i < particles.size(); i++)
		if(!isLeader[i] && !isFollowing[i] && particles[i].state != RibbonParticle::RunningAway) availables.push_back(i);
	
	if(availables.size() > 1)
	{
	
		for( int i = 0; i < totalToAdd; i++)
		{
			// pick follower from random available
			int rand = ofRandom(0,availables.size()-1);
			int me = availables[rand];
			
			// remove from available
			availables.erase(availables.begin() + rand );
			
			// set is following and reset follow timer
			isFollowing[me] = true;
			followingTime[me] = ofRandom(10,30);
			
			// pick leader to follow from non-followers
			rand = ofRandom(0,particles.size()-1);
			if( rand == me && rand > 0 ) rand--;
			else if(rand == me && rand < particles.size()-2) rand++;
			
			int mom =rand;
			isLeader[mom] = true;
			whoIFollow[me] = mom;
			
			if(availables.size() <= 0 ) break;
		}
	
	}
	
	// set targets
	for( int i = 0; i < particles.size(); i++)
	{
		if( isFollowing[i] )
		{
			int mom = whoIFollow[i];
			float A = ofRandom(0,TWO_PI);
			float B = ofRandom(0,TWO_PI);
			float R = 15;
			particles[i].target.set( 
						particles[mom].getPos().x + R*sin(A)*cos(B),
						particles[mom].getPos().y + R*sin(A)*sin(B),
						particles[mom].getPos().z + R*cos(A));
		}
	}
	
	
	
}


void ParticleFlocker::draw( vector<RibbonParticle> & particles ){
	
	for( int i = 0; i < particles.size(); i++)
	{
		//if( isFollowing[i] )
		//{
		if(!isFollowing[i]) continue;
			int mom = whoIFollow[i];
			ofPoint meP = particles[i].getPos();
			ofPoint momP = particles[i].target;//particles[mom].getPos();//
			
			//ofSetColor(255,0,0);
			//ofSphere(meP,2);
			//ofSetColor(0,255,0);
			//ofSphere(momP,2);
			
			ofSetColor(255,255,255);
			ofLine(meP,momP);
		//}
	}
}

