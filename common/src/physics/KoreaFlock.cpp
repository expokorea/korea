/*
 *  Korea3DFlock.cpp
 *  testGlowShader
 *
 *  Created by Chris on 2/20/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#include "KoreaFlock.h"
#include "ofxTimeUtils.h"



bool sort_dist_compare( distIds a, distIds b ) {
	
	return (a.dist < b.dist);
}
 

void KoreaFlock::setup( int total, int worldWidth, int worldHeight, int worldDepth)
{
		
	
	this->worldWidth  = worldWidth;
	this->worldHeight = worldHeight;
	this->worldDepth  = worldDepth;
	
	float halfWidth = worldWidth * .5;
	float halfHeight = worldHeight * .5;
	float halfDepth = worldDepth * .5;
	
	// setup random
	KoreaParticle tempParticle;
	for( int i = 0; i < total; i++)
	{
		ofVec3f pos = ofVec3f( ofRandom(-halfWidth,halfWidth), ofRandom(-halfHeight,halfHeight), ofRandom(-halfDepth,halfDepth));
		ofVec3f vel = ofVec3f( 0,0,0);
		tempParticle.setup( pos, vel, .01f );
		tempParticle.bUseTarget = true;
		tempParticle.rt = i;
		particles.push_back(tempParticle);
	}
	
	gui.setup("particles");
	gui.add(speed.setup("t force",.010,0,.05));
	gui.add(useTrails.setup("trails",true));
	gui.add(sep.setup("sep",.05,0,.1));
	gui.add(coh.setup("coh",.01,0,.1));
	gui.add(aln.setup("aln",.01,0,.1));
	gui.add(userRadius.setup("user radius",200,0,500));
	
	color.set(255,255,255);
}

void KoreaFlock::setupInGroups( int worldWidth, int worldHeight, int worldDepth)
{
	this->worldWidth  = worldWidth;
	this->worldHeight = worldHeight;
	this->worldDepth  = worldDepth;
	
	float halfWidth  = worldWidth  * .5;
	float halfHeight = worldHeight * .5;
	float halfDepth  = worldDepth  * .5;
	
	// create particles in groups with unique identifier
	int groupMin = 6;
	int groupMax = 20;
	int totalGroup = 10;
		
	flockGroup tempGroup;
	tempGroup.bFollowing = false;
	tempGroup.userId = -1;
	
	for( int j = 0; j < totalGroup; j++)
	{
		int thisGroup = ofRandom(groupMin,groupMax);
		for( int i = 0; i<thisGroup; i++)
			particles.push_back(KoreaParticle(j));	
		
		tempGroup.groupFlag = j;
		tempGroup.pos.set(0,0,0);
		groups.push_back(tempGroup);
		
	}
	
	// set groups up in thirds of the screen
	float thirdHW = .5 * (worldWidth / 3.0f);
	
	for( int i = 0; i < particles.size(); i++)
	{
		ofVec3f pos;
		if( particles[i].groupFlag <= 3 )
			pos = ofVec3f( ofRandom(-thirdHW-50,-thirdHW+50), ofRandom(-50,50), ofRandom(-50,50));
		else if( particles[i].groupFlag <= 6 )
			pos = ofVec3f( ofRandom(-50,50), ofRandom(-50,50), ofRandom(-50,50));
		else 
			pos = ofVec3f( ofRandom(thirdHW-50,thirdHW+50), ofRandom(-50,50), ofRandom(-50,50));
		
		particles[i].setup( pos, ofVec3f( 0,0,0), .01f );
		particles[i].bUseTarget = true;
		particles[i].rt = i;
	}
	
	
	// set interface
	gui.setup("particles");
	gui.add(speed.setup("t force",.010,0,.05));
	gui.add(useTrails.setup("trails",true));
	gui.add(sep.setup("sep",.05,0,.1));
	gui.add(coh.setup("coh",.01,0,.1));
	gui.add(aln.setup("aln",.01,0,.1));
	gui.add(userRadius.setup("user radius",200,0,500));
	
	color.set(255,255,255);
	
	lastEatTime = 0;
	eatWaitTime = 0;
	
}

void KoreaFlock::update()
{
	ofSeedRandom(0);
	
	float dt = ofGetElapsedTimef() - timeLastUpdate;
	timeLastUpdate = ofGetElapsedTimef();
	
	
	for( int i = 0; i < particles.size(); i++)
	{
		float halfWidth  = worldWidth  * .5;
		float halfHeight = worldHeight * .5;
		float halfDepth  = worldDepth  * .5;
	
		if(particles[i].particleState == KPARTICLE_FLOCKING){
			particles[i].target.set(
					   ofNoise(particles[i].rt/100.,particles[i].t,ofRandom(1))*worldWidth-halfWidth,
					   ofNoise(ofRandom(1),particles[i].rt/100.,particles[i].t)*worldHeight-halfHeight,
					   ofNoise(particles[i].rt/100.,ofRandom(1),particles[i].t)*worldDepth-(1.25*halfDepth));
		}
		
		particles[i].resetFlocking();
		for( int j = 0; j < particles.size(); j++){
			if(i!=j)particles[i].addForFlocking(&particles[j]);
		}
		particles[i].applyForces();
		
		#ifdef USE_TIME_BASED
		particles[i].update(dt*100);
		#else
		particles[i].update();
		#endif

	}
	
	
	//KoreaParticle::targetForce = speed;
	for( int i = 0; i < particles.size(); i++)
	{	
		
		// note: check if this is conflicting with settings in particle
		if(particles[i].particleState == KPARTICLE_FLOCKING)
		{
			particles[i].targetForce = speed;
			particles[i].separation = sep;
			particles[i].cohesion = coh;
			particles[i].alignment = aln;
		}else if( particles[i].particleState == KPARTICLE_TARGET){
			particles[i].targetForce = speed;
			particles[i].separation = sep;
			particles[i].cohesion = 0;
			particles[i].alignment = 0;			
		}else{
			particles[i].separation = 0;
			particles[i].cohesion = 0;
			particles[i].alignment = 0;	
		}
		
		
		particles[i].bDrawTrails = useTrails;
	}
	
	if( ofGetElapsedTimef() - lastEatTime > eatWaitTime )
	{
		setRandomEating();
		lastEatTime = ofGetElapsedTimef();
		eatWaitTime = ofRandom(.5,2);
	}
	
}

//void KoreaFlock::debugUserCenter(ofPoint p)
void KoreaFlock::debugUserCenter(KUserData & myUser)
{
	return;
	/*
	// calculate all that are within range of user
	p.y  = ofGetHeight()-p.y;
	
	// mapping mouse
	p.x = ofMap(p.x,0,ofGetWidth(),-270,270);
	p.y = ofMap(p.y,0,ofGetHeight(),-198,198);
	p.z = 300;
	*/
	
	user1 = myUser.pos;
	int tPts = myUser.contour.size()-1;
	
	if(tPts > 0 )
	{
	
	ofSeedRandom(0);
	float distRange = userRadius;
	for( int i = 0; i < particles.size(); i++)
	{	
		float diff = abs(user1.x-particles[i].pos.x);
		//float d = diff.length();
		if(diff < distRange)
		{
			particles[i].setState(KPARTICLE_TARGET);
			int index = (i + int(ofxTimeUtils::getElapsedTimef())) % tPts;
			particles[i].target = myUser.contour[index];
			particles[i].targetForce = speed*2*myUser.targetForce;
		}else{
			particles[i].setState(KPARTICLE_FLOCKING);
		}
		
	}
	
	}
}

void KoreaFlock::draw()
{
	//glEnable(GL_DEPTH_TEST);
	for( int i = 0; i < particles.size(); i++)
			particles[i].draw();
			
	//glDisable(GL_DEPTH_TEST);

}

void KoreaFlock::drawDebug()
{
	for( int i = 0; i < groups.size(); i++)
		ofSphere(groups[i].pos, 5);

}

void KoreaFlock::drawForGlow(){
	
	ofSetColor(color);
	
	for(int i = 0; i < particles.size(); i++)
	{
		particles[i].drawForGlow();
	}		
	
}


void KoreaFlock::assignUserTargets( vector<KUserData> users)
{
	
	// create a vector for groups with boolean to mark if it following a user or not
	vector<bool> isFollowing;
	for( int i = 0; i < groups.size(); i++) isFollowing.push_back(false);
	
	// for each flock calculate average position
	for( int i = 0; i < groups.size(); i++)
	{
		// reset user id
		groups[i].userId = -1;
		
		groups[i].pos.set(0,0,0);
		int tInGroup = 0;
		for( int j = 0; j < particles.size(); j++)
		{
			if(particles[j].groupFlag == groups[i].groupFlag )
			{
				groups[i].pos += particles[j].pos;
				tInGroup++;
			}
			
			
			
		}
		groups[i].pos.x /= (float)tInGroup;
		groups[i].pos.y /= (float)tInGroup;
		groups[i].pos.z /= (float)tInGroup;
		
	}
	
	vector<userDistances> userDist;
	userDistances tempDist;
	
	// for each user, calc distances to flock groups
	for( int i = 0; i < users.size(); i++)
	{
		userDist.push_back(tempDist);
		
		// for each flock, calc distance, if in range record
		for(int j = 0; j < groups.size(); j++)
		{
			distIds tempDistId;
			float distSq = (groups[j].pos-users[i].pos).lengthSquared();
			tempDistId.dist = distSq;
			tempDistId.id = j;
			userDist[i].distData.push_back(tempDistId);
		}
		
		// sort distances	
		sort( userDist[i].distData.begin(), userDist[i].distData.end(), sort_dist_compare );

	}
	
	
	// for each user, choose closest flock
	for( int i = 0; i < users.size(); i++)
	{
		//try to pick closest
		for( int j = 0; j < userDist[i].distData.size(); j++)
		{
			int groupId = userDist[i].distData[j].id;
			if( !isFollowing[groupId] )
			{
				isFollowing[groupId] = true;
				groups[groupId].userId = i; 
				break;
			}
			
		}
	}
	
	for( int i = 0; i < groups.size(); i++)
	{
		int userId = groups[i].userId;
		if(userId >= 0)
		{
			int tPts = users[ userId ].contour.size()-1;
			if(tPts > 0 ){
				int index = (i + int(ofxTimeUtils::getElapsedTimef()*1)) % tPts;//(int)(ofRandom(0,tPts));
				groups[i].target = users[ userId ].contour[index];
			}
		}
	}
		
	// make all in groups that have follow id, follow that user
	for( int i = 0; i < particles.size(); i++)
	{
		if(particles[i].particleState == KPARTICLE_EATING) continue;
		
		int groupId = particles[i].groupFlag;
		int userId = groups[groupId].userId;
		int tPts = users[ userId ].contour.size()-1;
		if( userId >= 0 && tPts > 0 )
		{
			particles[i].setState(KPARTICLE_TARGET);
			int index = (groupId + int(ofxTimeUtils::getElapsedTimef()*2)) % tPts;
			if(users[userId].targetMovement < 2)
				particles[i].target = groups[groupId].target;//users[ userId ].contour[index];
			else
				particles[i].target = users[ userId ].pos;//contour[index];
			
			if(users[userId].targetMovement > 10){
				users[ userId ].targetForce = (float(3-users[ userId ].targetMovement)/6.);
			}
			particles[i].targetForce = speed*2*users[ userId ].targetForce;
		}else{
			particles[i].setState(KPARTICLE_FLOCKING);
		}
	}
	
	
	/*for( int i = 0; i < particles.size(); i++)
	{
		if(particles[i].particleState == KPARTICLE_FLOCKING)
		{
			for( int j = 0; j < users.size(); j++)
			{
				particles[i].repelFrom(  users[ j ].pos, -1, 200100);
			}
			
		}
	}*/
	
}

void KoreaFlock::setRandomEating()
{
	ofSeedRandom();
	
	// pick a random particle
	int i = ofRandom(0,particles.size()-1);
	
	// set its state
	particles[i].setState(KPARTICLE_EATING);
	
	// set its targetForce
	particles[i].targetForce = .1;
	
	// pick target in range
	float radius = ofRandom(30,50);
	float angle = ofRandom(0,TWO_PI);
	particles[i].target.set( particles[i].pos.x+radius * cos(angle), particles[i].pos.y+radius * sin(angle), particles[i].pos.z+ ofRandom(-5,5) );
	
	
}

