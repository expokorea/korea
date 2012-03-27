/*
 *  userData.cpp
 *  testInteract
 *
 *  Created by Chris on 2/29/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#include "userData.h"
#include "ofxTimeUtils.h"

void KUserData::setup()
{
	pos.set(0,0,0);
	timeLastSeen = 0;
	contour.reserve(20);
	bIsTooActive = false;
	bIsVeryStill = false;
}


void KUserData::debugSetUserContour()
{
	// create contour points in ellipse with some noise
	
	contour.clear();
	
	float radiusX = 50;//15;//30;
	float radiusY = 80;//35;//70;
	float radiusZ = 80;
	
	float angStep = TWO_PI / 20.f;
	
	for( int i = 0; i < 20; i++)
	{
		//float A = ofRandom(0,TWO_PI);
		//float B = ofRandom(0,TWO_PI);
		//float R = ofRandom(30,50);//15,25);
		ofPoint p = ofPoint( 
		pos.x + (radiusX + (ofNoise(ofxTimeUtils::getElapsedTimef()*4,i/20.f)*25) )*sin( i*angStep),
		pos.y + (radiusY + (ofNoise(i/20.f,ofxTimeUtils::getElapsedTimef()*4)*45) )*cos(angStep*i), 
		pos.z + (radiusZ + (ofNoise(i/20.f,ofxTimeUtils::getElapsedTimef()*4)*25) ) 
		//pos.x + (R*sin(A)*cos(B)),
		//pos.y + ((1.2*R)*sin(A)*sin(B)),
		//pos.z + (R*cos(A))
		);
		contour.push_back(p);
	}
	
}

void KUserData::debugSetUserPosFromMouse(float mouseX, float mouseY, float z)
{
	// calculate all that are within range of user
	pos.set(mouseX,ofGetHeight()-mouseY,z);
	targetMovement = targetMovement*.7 + (abs(prevMouseX-mouseX) + abs(prevMouseY-mouseY))*.3;
	prevMouseX = mouseX;
	prevMouseY = mouseY;
	targetForce = 1;//(float(3-targetMovement)/3.);
	
	// mapping mouse
	pos.x = ofMap(pos.x,0,ofGetWidth(),-270,270);
	pos.y = ofMap(pos.y,0,ofGetHeight(),-198,198);
	
	if(targetMovement < 2) bIsVeryStill = true;
	else bIsVeryStill = false;
	
	if(targetMovement > 10) bIsTooActive = true;
	else bIsTooActive = false;
}


void KUserData::drawUser()
{
	ofSetColor(255);
	
	ofSphere(pos,3);
	
	glPointSize(10);
	glBegin(GL_POINTS);
	for( int i = 0; i < contour.size(); i++)
	{
		glVertex3f(contour[i].x,contour[i].y,contour[i].z);
		
		//glVertex3f(pos.x + 20*sin(ofGetElapsedTimef()),contour[i].y,pos.z + 20*cos(ofGetElapsedTimef()));
	}
	glEnd();
	glPointSize(10);
}

