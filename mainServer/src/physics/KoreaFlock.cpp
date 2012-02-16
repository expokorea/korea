/*
 *  KoreaFlock.cpp
 *  mainServer
 *
 *  Created by Chris on 2/16/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#include "KoreaFlock.h"

void KoreaFlock::setup( int total)
{

	KoreaParticle tempParticle;
	
	// setup random
	for( int i = 0; i < total; i++)
	{
		ofVec3f pos = ofVec3f( ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight() ), 0);
		ofVec3f vel = ofVec3f( ofRandom(-1,1), ofRandom(-1,1), 0);
		tempParticle.setup( pos, vel, .1f );
		particles.push_back(tempParticle);
	}
	
}

void KoreaFlock::update()
{

	for( int i = 0; i < particles.size(); i++)
	{
		particles[i].update();
	}

}

void KoreaFlock::draw()
{
	for( int i = 0; i < particles.size(); i++)
		particles[i].draw();

}