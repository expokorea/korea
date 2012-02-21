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
		tempParticle.bUseTarget = true;
		tempParticle.targetForce = .001;
		tempParticle.bUseNoise = false;
		tempParticle.setTarget(ofVec3f(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight() ), 0), .025);

		particles.push_back(tempParticle);
	}
	
	vectorField.setupField(60,40,ofGetWidth(), ofGetHeight());
	vectorField.randomizeField(2.0);
	bUseVectorField = true;
	
	for( int i = 0; i < 20; i++)
	{
		ofVec3f pos = ofVec3f( ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight() ), 0);
		ofVec3f vel = ofVec3f( ofRandom(-1,1), ofRandom(-1,1), 0);
		tempParticle.setup( pos, vel, .1f );
		tempParticle.noiseForce = 0;
		tempParticle.setTarget(ofVec3f(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight() ), 0), .2);
		vParticles.push_back(tempParticle);
	}
	
	
	gui.setup("particles");
	gui.add(speed.setup("speed",.025,0,1));
	gui.add(useTrails.setup("trails",true));
	
	
}

void KoreaFlock::update()
{

	for( int i = 0; i < particles.size(); i++) particles[i].update();
	
	for( int i = 0; i < vParticles.size(); i++) vParticles[i].update();
	
	for( int i = 0; i < particles.size(); i++)
	{
		ofVec2f frc;
		frc = vectorField.getForceFromPos(particles[i].pos.x, particles[i].pos.y);
		particles[i].vel.x += frc.x;
		particles[i].vel.y += frc.y;
	}
	
	vectorField.fadeField(.9);
	
	for( int i = 0; i < vParticles.size(); i++)
	{
		if(i%2 == 0) vectorField.addInwardCircle(vParticles[i].pos.x,vParticles[i].pos.y, 60, .51);
		else vectorField.addOutwardCircle(vParticles[i].pos.x,vParticles[i].pos.y, 80, 1);
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
		
	//vectorField.draw();
}

void KoreaFlock::drawForGlow(){
	for(int i = 0; i < particles.size(); i++)
		particles[i].drawForGlow();
}