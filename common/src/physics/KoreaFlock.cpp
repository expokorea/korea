/*
 *  Korea3DFlock.cpp
 *  testGlowShader
 *
 *  Created by Chris on 2/20/12.
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
		ofVec3f pos = ofVec3f( ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight() ), ofRandom(0,100));
		ofVec3f vel = ofVec3f( 0,0,0);	//ofRandom(-1,1), ofRandom(-1,1), 0);
		tempParticle.setup( pos, vel, .01f );
		tempParticle.bUseTarget = true;
		tempParticle.rt = i;
		particles.push_back(tempParticle);
	}
	
	gui.setup("particles");
	gui.add(speed.setup("speed",.025,0,.1));
	gui.add(useTrails.setup("trails",true));
	//gui.add(sep.setup("sep",.025,0,.5));
	
	color.set(255,255,255);
}

void KoreaFlock::update()
{
	ofSeedRandom(0);
	
	for( int i = 0; i < particles.size(); i++)
	{
		
		particles[i].resetFlocking();
		for( int j = 0; j < particles.size(); j++){
			if(i!=j)particles[i].addForFlocking(&particles[j]);
		}
		particles[i].applyForces();
		particles[i].update();
	}
	
	
	
	for( int i = 0; i < particles.size(); i++)
	{	
		particles[i].targetForce = speed;
		particles[i].bDrawTrails = useTrails;
	}
	
	
}

void KoreaFlock::draw()
{
	for( int i = 0; i < particles.size(); i++)
		particles[i].draw();
	
}

void KoreaFlock::drawForGlow(){
	ofSetColor(color);
	for(int i = 0; i < particles.size(); i++)
		particles[i].drawForGlow();
}