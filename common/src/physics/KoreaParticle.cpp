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
	targetForce = .01;
	bUseTarget = false;
	bFlocking = false;
	bDrawTrails = true;

	rt = ofRandom(0,100);
	
	sepDist = 20;
	alnDist = 80;
	cohDist = 90;
	
	separation = .05;
    cohesion  =  .01;//.1;
    alignment =  .01;
	
	trails.reserve(100);
}

void KoreaParticle::update()
{
	
	vel.x -= damping*vel.x;
	vel.y -= damping*vel.y;
	vel.z -= damping*vel.z;	//vel.z = 0;
	
	if(bUseTarget) applyTargetAttraction();
	
	pos += vel;
	
	t = ofGetElapsedTimef()*ofMap(10,1,80,0,1)*ofMap(10,1,80,0,1);

	if(trails.size() > 100) trails.erase(trails.begin());
	trails.push_back(pos);
	
}

void KoreaParticle::draw3D()
{
	ofSphere(pos,5);
}

void KoreaParticle::draw()
{
	//ofCircle(pos.x, pos.y, 3);

}


void KoreaParticle::drawForGlow() {
	
	ofSphere(pos,5);
		
	if(bDrawTrails)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofNoFill();
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < trails.size(); i++)
		{
			float pct = (float)i / (float)trails.size();
			glColor4f(1,1,1,pct*.75);
			glVertex3f(trails[i].x, trails[i].y,trails[i].z);
		}
		glEnd();
		ofDisableAlphaBlending();
		ofPopStyle();
	}
}

void KoreaParticle::setTarget(ofVec3f targ, float targetForce)
{
	target = targ;
	bUseTarget = true;
	this->targetForce = targetForce;
}
	
void KoreaParticle::applyTargetAttraction()
{
	target.set(
			   ofNoise(rt/100.,t,ofRandom(1))*ofGetWidth(),
			   ofNoise(ofRandom(1),rt/100.,t)*ofGetHeight(),
			   ofNoise(rt/100.,ofRandom(1),t)*ofGetHeight());
	
	ofVec3f posOfForce;
	posOfForce.set(target.x,target.y,target.z);
	
	// ----------- (2) calculate the difference & length
	
	ofVec3f diff	= posOfForce-pos;
	float length	= diff.length();
	float radius	= 10;
	
	// ----------- (3) check close enough
	
	/*bool bAmCloseEnough = false;
    if (radius > 0){
        if (length > 200){
            bAmCloseEnough = false;
        }
    }*/
	
	// ----------- (4) if so, update force
	ofVec3f frc;
	//if (bAmCloseEnough == false){
		diff.normalize();
		frc.set(0,0);
		frc.x = diff.x * targetForce;// * pct;
        frc.y = diff.y * targetForce;// * pct;
		frc.z = diff.z * targetForce;// * pct;
		vel+=frc;
		
    //}
	
	
}

void KoreaParticle::addForFlocking(KoreaParticle * sister)
{
	
	
	ofVec3f diff, direction;
	float d, attention_factor;
	
	
	// add for seperation
	diff = pos-sister->pos;// - pos;
	d 	 = diff.length();
	diff.normalize();
	
	
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
		if( d > 0 && d < sepDist )
		{
			sumSep += (diff) * attention_factor;
			countSep++;
		}
		
		// add for alignment
		if( d > 0 && d < alnDist )
		{
			sumAlign += sister->vel  * attention_factor;
			countAlign++;
		}
		
		//add for cohesion
		if( d > 0 && d < cohDist )
		{
			sumCoh += sister->pos * attention_factor;
			countCoh++;
        }
		
	}
	
}


void KoreaParticle::applyForces()
{
	float sepFrc 	= separation;
	float cohFrc 	= cohesion;
	float alignFrc 	= alignment;
	// seperation
	if(countSep > 0)
	{
		sumSep /= (float)countSep;
		vel += (sumSep.normalized() 	* sepFrc);
	}
	
	
	// alignment
	if(countAlign > 0)
	{
		sumAlign /= (float)countAlign;
		vel += (sumAlign.normalized() * alignFrc);
	}
	
	// cohesion
	if(countCoh > 0)
	{
		
		sumCoh /= (float)countCoh;
		sumCoh -= pos;
		vel += (sumCoh.normalized() 	* cohFrc);
	}
	

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