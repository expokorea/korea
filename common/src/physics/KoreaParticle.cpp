/*
 *  KoreaParticle.cpp
 *  mainServer
 *
 *  Created by Chris on 2/15/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#include "KoreaParticle.h"

/*float KoreaParticle::targetForce,
KoreaParticle::separation,
KoreaParticle::cohesion,
KoreaParticle::alignment;*/

ofxIntSlider KoreaParticle::r;
ofxIntSlider KoreaParticle::g;
ofxIntSlider KoreaParticle::b;
ofxToggle KoreaParticle::debug;
ofxToggle KoreaParticle::useModel;

void KoreaParticle::setup(ofVec3f pos, ofVec3f vel, float damping)
{
	
	model.loadModel("blobFish2.obj");
	
	this->vel = vel;
	this->pos = pos;
	this->damping = damping;
	
	target.set(0,0,0);
	targetForce = .01;
	bUseTarget  = false;
	bFlocking   = false;
	bDrawTrails = true;
	particleState = KPARTICLE_FLOCKING;
	
	rt = ofRandom(0,100);
	
	sepDist = 40;
	alnDist = 70;
	cohDist = 60;
	
	separation =  .025;
    cohesion   =  .005;
    alignment  =  .005;
	
	targetSpeed = 8;
	
	trails.reserve(100);
	
	node.setScale(1);

	thickness = 8;
}

void KoreaParticle::update()
{
	
	vel.x -= damping*vel.x;
	vel.y -= damping*vel.y;
	vel.z -= damping*vel.z;
	
	// apply attractions
	if(bUseTarget) applyTargetAttraction();
	
	// upadte position
	pos += vel;
	
	// for targets and trails
	t = ofGetElapsedTimef()*ofMap(targetSpeed,1,80,0,1)*ofMap(targetSpeed,1,80,0,1);

	if(trails.size() > 100) trails.erase(trails.begin());
	trails.push_back(pos);
	
	node.setPosition(pos);
	if(trails.size()>10)node.lookAt(trails[trails.size()-10]);
}

void KoreaParticle::draw3D()
{
	//ofSphere(pos,5);
}

void KoreaParticle::draw()
{
	if(debug){
		drawDebug();
		return;
	}
	//ofCircle(pos.x, pos.y, 3);
	if(particleState == KPARTICLE_FLOCKING) ofSetColor(r,g,b);
	else ofSetColor(r,g,b);

	//ofFill();
	//ofSphere(pos,10);
	//node.draw();

	if(bDrawTrails)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofNoFill();
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < int(trails.size())-1; i++)
		{
			//float pct = (float)i / (float)trails.size();
			ofSetColor(r,g,b,200);
			ofVec3f p0 = trails[i];
			ofVec3f p1 = trails[i+1];

			ofVec3f up(0, 0, 1);	// arbitrary up vector

			ofVec3f dir = (p1 - p0).normalize();			// normalized direction vector from p0 to p1
			ofVec3f right = dir.cross(up).normalize();	// right vector
			ofVec3f norm = dir.cross(up);
			right *= .5 * ofClamp(thickness*ofNoise(float(i)/float(trails.size()))*ofNoise(float(i)/float(trails.size()))*ofNoise(float(i)/float(trails.size()))*sin(float(i)/float(trails.size())*PI)+2,0,thickness);

			ofVec3f p = trails[i] -right;
			glVertex3f(p.x, p.y, p.z);

			p = trails[i] + right;
			glVertex3f(p.x, p.y, p.z);

			p = trails[i+1] - right;
			glVertex3f(p.x, p.y, p.z);

			p = trails[i+1] + right;
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
		ofDisableAlphaBlending();
		ofPopStyle();

		/*glBegin(GL_LINE_STRIP);
		glVertex3f(pos.x, pos.y,pos.z);
		glVertex3f(target.x, target.y,target.z);
		glEnd();*/
	}

}


void KoreaParticle::drawForGlow() {
	
	if(debug){
		return;
	}
	
	float pct = ofMap(pos.z,-200,200,.5,1);
	
	if(!KoreaParticle::useModel) pct = 1;
	
	if(particleState == KPARTICLE_FLOCKING) ofSetColor(r*pct,g*pct,b*pct);
	else ofSetColor(r*pct,g*pct,b*pct);
	
	//ofFill();
	//ofSphere(pos,10);
	//node.draw();
	
	if(KoreaParticle::useModel){
		ofVec3f axis;
		float angle;  
		node.getOrientationQuat().getRotate(angle, axis);  
		
		ofPushMatrix();
			glTranslatef(pos.x,pos.y,pos.z);
			glRotatef(90,1,0,0);
			glRotatef(90,0,0,1);
			ofRotate(angle, axis.x, axis.y, axis.z);  
			glScalef(.075,.075,.075);
			model.draw(OF_MESH_FILL);
		ofPopMatrix();
	}
		
	if(bDrawTrails)
	{
		ofPushStyle();
		ofEnableAlphaBlending();
		ofNoFill();
		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < int(trails.size())-1; i++)
		{
			float pct = (float)i / (float)trails.size();
			ofSetColor(r,g,b,200*pct);
			ofVec3f p0 = trails[i];
			ofVec3f p1 = trails[i+1];

			ofVec3f up(0, 0, 1);	// arbitrary up vector

			ofVec3f dir = (p1 - p0).normalize();			// normalized direction vector from p0 to p1
			ofVec3f right = dir.cross(up).normalize();	// right vector
			ofVec3f norm = dir.cross(up);
			right *= ofClamp(thickness*ofNoise(float(i)/float(trails.size()))*ofNoise(float(i)/float(trails.size()))*ofNoise(float(i)/float(trails.size()))*sin(float(i)/float(trails.size())*PI)+2,0,thickness);

			ofVec3f p = trails[i] -right;
			glVertex3f(p.x, p.y, p.z);

			p = trails[i] + right;
			glVertex3f(p.x, p.y, p.z);

			p = trails[i+1] - right;
			glVertex3f(p.x, p.y, p.z);

			p = trails[i+1] + right;
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
		ofDisableAlphaBlending();
		ofPopStyle();
		
		/*glBegin(GL_LINE_STRIP);
		glVertex3f(pos.x, pos.y,pos.z);
		glVertex3f(target.x, target.y,target.z);
		glEnd();*/
	}
}

void KoreaParticle::drawDebug(){
	if(particleState == KPARTICLE_FLOCKING) ofSetColor(100,100,100);
		else ofSetColor(255,255,255);

		//ofFill();
		ofSphere(pos,10);
		node.draw();

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

			/*glBegin(GL_LINE_STRIP);
			glVertex3f(pos.x, pos.y,pos.z);
			glVertex3f(target.x, target.y,target.z);
			glEnd();*/
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
	
	
	ofVec3f diff	= target-pos;
	//float length	= diff.length();
	//float radius	= 10;
	
	/*bool bAmCloseEnough = false;
    if (radius > 0){
        if (length > 200){
            bAmCloseEnough = false;
        }
    }*/
	
	ofVec3f frc = ofVec3f(0,0,0);
	
	diff.normalize();
		
	frc.x = diff.x * targetForce;
	frc.y = diff.y * targetForce;
	frc.z = diff.z * targetForce;
	
	vel+=frc;

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

void KoreaParticle::setState(koreaParticleState state)
{
	particleState = state;
}
