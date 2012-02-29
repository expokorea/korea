/*
 *  Korea3DFlock.cpp
 *  testGlowShader
 *
 *  Created by Chris on 2/20/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#include "KoreaFlock.h"

void KoreaFlock::setup( int total, int worldWidth, int worldHeight, int worldDepth)
{
	
	//model.loadModel("blobFish.obj");
	
	KoreaParticle tempParticle;
	
	this->worldWidth  = worldWidth;
	this->worldHeight = worldHeight;
	this->worldDepth  = worldDepth;
	
	float halfWidth = worldWidth * .5;
	float halfHeight = worldHeight * .5;
	float halfDepth = worldDepth * .5;
	
	// setup random
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

void KoreaFlock::update()
{
	ofSeedRandom(0);
	
	for( int i = 0; i < particles.size(); i++)
	{
		float halfWidth = worldWidth * .5;
		float halfHeight = worldHeight * .5;
		float halfDepth = worldDepth * .5;
	
		if(particles[i].particleState == KPARTICLE_FLOCKING)
			particles[i].target.set(
					   ofNoise(particles[i].rt/100.,particles[i].t,ofRandom(1))*worldWidth-halfWidth,
					   ofNoise(ofRandom(1),particles[i].rt/100.,particles[i].t)*worldHeight-halfHeight,
					   ofNoise(particles[i].rt/100.,ofRandom(1),particles[i].t)*worldDepth-halfDepth);
					   
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

//void KoreaFlock::debugUserCenter(ofPoint p)
void KoreaFlock::debugUserCenter(KUserData & myUser)
{
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
	
	float distRange = userRadius;
	for( int i = 0; i < particles.size(); i++)
	{	
		ofVec3f diff = user1-particles[i].pos;
		float d = diff.length();
		if(d < distRange)
		{
			particles[i].setState(KPARTICLE_TARGET);
			int index = i % tPts ;
			particles[i].target = myUser.contour[index];
			particles[i].targetForce = speed*4;
		}else{
			particles[i].setState(KPARTICLE_FLOCKING);
		}
		
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
	{
		particles[i].drawForGlow();
		
		/*ofSetColor(0,0,255,255);

		ofPushMatrix();
			glTranslatef(particles[i].pos.x,particles[i].pos.y,particles[i].pos.z);
			glRotatef(90,1,0,0);
			glRotatef(90,0,0,1);
			//ofRotate(angle, axis.x, axis.y, axis.z);  
			glScalef(.1,.1,.1);
			model.draw(OF_MESH_FILL);
		ofPopMatrix();*/
		
		
	}		
	//ofSphere(user1,10);
	

	
}