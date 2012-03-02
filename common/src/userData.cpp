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
}


void KUserData::debugSetUserContour()
{
	// create contour points in ellipse with some noise
	
	contour.clear();
	
	float radiusX = 30;
	float radiusY = 70;
	
	float angStep = TWO_PI / 20.f;
	
	for( int i = 0; i < 20; i++)
	{
		ofPoint p = ofPoint( 
		pos.x + (radiusX + (ofNoise(ofxTimeUtils::getElapsedTimef()*2,i/20.f)*45) )*sin( i*angStep),
		pos.y + (radiusY + (ofNoise(i/20.f,ofxTimeUtils::getElapsedTimef()*2)*45) )*cos(angStep*i), pos.z);
		contour.push_back(p);
	}
	
	
}

void KUserData::debugSetUserPosFromMouse(float mouseX, float mouseY)
{
	// calculate all that are within range of user
	pos.set(mouseX,ofGetHeight()-mouseY,300);
	targetMovement = targetMovement*.7 + (abs(prevMouseX-mouseX) + abs(prevMouseY-mouseY))*.3;
	prevMouseX = mouseX;
	prevMouseY = mouseY;
	targetForce = float(3-targetMovement)/3.;
	
	// mapping mouse
	pos.x = ofMap(pos.x,0,ofGetWidth(),-270,270);
	pos.y = ofMap(pos.y,0,ofGetHeight(),-198,198);
}


void KUserData::drawUser()
{
	ofSetColor(255);
	
	ofSphere(pos,3);
	
	/*glPointSize(10);
	glBegin(GL_POINTS);
	for( int i = 0; i < contour.size(); i++)
	{
		glVertex3f(contour[i].x,contour[i].y,contour[i].z);
	}
	glEnd();
	glPointSize(10);*/
}

