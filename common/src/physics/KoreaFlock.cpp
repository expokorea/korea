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
		ofVec3f pos = ofVec3f( ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight() ), ofRandom(300,-ofGetHeight()*.5));
		ofVec3f vel = ofVec3f( 0,0,0);	//ofRandom(-1,1), ofRandom(-1,1), 0);
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
	
	
	//KoreaParticle::targetForce = speed;
	for( int i = 0; i < particles.size(); i++)
	{	
		if(particles[i].particleState == KPARTICLE_FLOCKING)
		{
			particles[i].targetForce = speed;
			particles[i].separation = sep;
			particles[i].cohesion = coh;
			particles[i].alignment = aln;
		}else{
			particles[i].targetForce = speed;
			particles[i].separation = sep;
			particles[i].cohesion = 0;
			particles[i].alignment = 0;			
		}
		
		particles[i].bDrawTrails = useTrails;
	}
	
	
}

void KoreaFlock::debugUserCenter(ofPoint p)
{
	// calculate all that are within range of user
	p.y  = ofGetHeight()-p.y;
	
	p.x = ofMap(p.x,0,ofGetWidth(),ofGetWidth()*.5-270,ofGetWidth()*.5+270);
	p.y = ofMap(p.y,0,ofGetHeight(),ofGetHeight()*.5-270,ofGetHeight()*.5+270);
	
	float distRange = 300;
	for( int i = 0; i < particles.size(); i++)
	{	
		ofVec3f diff = ofVec3f(p.x,p.y,300)-particles[i].pos;
		float d = diff.length();
		if(d < 200)
		{
			particles[i].setState(KPARTICLE_TARGET);
			particles[i].target = ofVec3f(p.x,p.y,300);
			particles[i].targetForce = speed*4;
		}else{
			particles[i].setState(KPARTICLE_FLOCKING);
		}
		
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
		
	//ofSphere(ofVec3f(ofGetWidth()*.5+270,ofGetHeight()*.5,300),10);
}