/*
 *  KoreaParticle.cpp
 *  mainServer
 *
 *  Created by Chris on 2/15/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#include "KoreaParticle.h"

void KoreaParticle::setup(ofVec3f pos, ofVec3f vel, float damping)
{
	this->vel = vel;
	this->pos = pos;
	this->damping = damping;

}

void KoreaParticle::update()
{
	

	pos += vel;
	
	vel.x -= damping*vel.x;
	vel.y -= damping*vel.y;
	vel.z -= damping*vel.z;
	
	 
}

void KoreaParticle::draw()
{
	ofCircle(pos.x, pos.y, 3);
}

