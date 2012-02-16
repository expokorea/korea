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
	
	target.set(0,0,0);
	targetForce = 1;
	bUseTarget = false;
	noiseForce = .01;
	bUseNoise = true;
	bFlocking = false;

	rt = ofRandom(0,1);
	
	sepDist = 300;
	alnDist = 380;
	cohDist = 380;
	
	separation = .51;
    cohesion  =  .351;
    alignment =  .351;
}

void KoreaParticle::update()
{
	

	pos += vel;
	
	vel.x -= damping*vel.x;
	vel.y -= damping*vel.y;
	vel.z -= damping*vel.z;
	
	if(bUseNoise)
	{
		vel.x += noiseForce*ofSignedNoise(ofGetElapsedTimef()*rt,0,0);
		vel.y += noiseForce*ofSignedNoise(0,ofGetElapsedTimef()*rt,0);
		vel.z += noiseForce*ofSignedNoise(0,0,ofGetElapsedTimef()*rt);
	}
	
	
	// target attractions
	// ----------- (1) make a vector of where this position is:
	
	if(bUseTarget)
	{
	
	
	ofVec2f posOfForce;
	posOfForce.set(target.x,target.y);
	
	// ----------- (2) calculate the difference & length
	
	ofVec2f diff	= posOfForce-pos;
	float length	= diff.length();
	float radius	= 10;
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
	ofVec2f frc;
	if (bAmCloseEnough == false){
		float pct = 1;// - (length / radius);  // stronger on the inside
		diff.normalize();
		frc.set(0,0);
		frc.x = diff.x * targetForce;// * pct;
        frc.y = diff.y * targetForce;// * pct;
		vel.x+=frc.x;
		vel.y+=frc.y;
		//cout << "ok" <<endl;
    }
	else{
		target.set(ofRandom(0,ofGetWidth()), ofRandom(0,ofGetHeight() ), 0);
	}
	
	}
	 
}

void KoreaParticle::draw()
{
	ofCircle(pos.x, pos.y, 3);
	//ofCircle(target.x, target.y, 1);
}


void KoreaParticle::drawForGlow() {
		ofSphere(pos,10);
}

void KoreaParticle::setTarget(ofVec3f targ, float targetForce)
{
	target = targ;
	bUseTarget = true;
	this->targetForce = targetForce;
}
	

void KoreaParticle::addForFlocking(KoreaParticle * sister)
{
	
	
	float distSep   = sepDist;
	float distAlign = alnDist;
	float distCoh   = cohDist;
	
	
	ofVec3f diff, direction;
	float d, attention_factor;
	
	
	// add for seperation
	diff = sister->pos - pos;
	d 	 = diff.length();
	//diff.normalize();
	
	
	// attention angle factor
	direction = vel;
	direction.normalize();
	attention_factor = diff.dot( direction );
	
	
	const float CUTOFF = .651;//PI;
	attention_factor -= CUTOFF;
	attention_factor /= (1.0f-CUTOFF);
	attention_factor = 1;
	
	
	if( attention_factor > 0.0f )
	{
		if( d > 0 && d < distSep )
		{
			sumSep += (diff / d) * attention_factor;
			countSep++;
		}
		
		// add for alignment
		if( d > 0 && d < distAlign )
		{
			sumAlign += sister->vel.normalized()  * attention_factor;
			countAlign++;
		}
		
		//add for cohesion
		if( d > 0 && d < distCoh )
		{
			sumCoh += sister->pos * attention_factor;
			countCoh++;
        }
		
	}
	
}


void KoreaParticle::applyForces()
{
	
	// seperation
	if(countSep > 0)
	{
		sumSep /= (float)countSep;
	}
	
	
	// alignment
	if(countAlign > 0)
	{
		sumAlign /= (float)countAlign;
	}
	
	// cohesion
	if(countCoh > 0)
	{
		
		sumCoh /= (float)countCoh;
		sumCoh -= pos;
	}
	
	float pct = 1.0f;
	
	float sepFrc 	= separation;
	float cohFrc 	= cohesion;
	float alignFrc 	= alignment;
	
	vel += sumSep.normalized() 	* sepFrc;
	vel += sumAlign.normalized() * alignFrc;
	vel += sumCoh.normalized() 	* cohFrc;
		
	
}

void KoreaParticle::resetFlocking()
{
    countCoh 	= 0;
	countSep	= 0;
	countAlign	= 0;
	
	sumCoh.set(0,0,0);
	sumSep.set(0,0,0);
	sumAlign.set(0,0,0);
}