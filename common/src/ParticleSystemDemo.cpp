/*
 * ParticleSystemDemo.cpp
 *
 *  Created on: 16/02/2012
 *      Author: arturo
 */

#include "ParticleSystemDemo.h"
#include "ofMath.h"

ParticleSystemDemo::ParticleSystemDemo() {
	minAlpha = 20;
	maxAlpha = 200;
	////model.loadModel("particle.obj");
	////particleMesh = model.getMesh(0);
	/*path.curveTo(ofGetWidth()/2,ofGetHeight()/2,0,50);
	path.curveTo(0,0,ofGetWidth()*2,50);
	path.curveTo(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*3.8,50);
	path.curveTo(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*3.9,50);
	path.curveTo(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*4,50);
	path.curveTo(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*4,50);
	path.curveTo(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*4,50);*/
	/*path.bezierTo(ofGetWidth()/2,ofGetHeight()/2,0,ofGetWidth()/2,ofGetHeight()/2,0,ofGetWidth()/2,ofGetHeight()/2,0,1000);
	path.bezierTo(0,0,ofGetWidth()*2,0,0,ofGetWidth()*2,0,0,ofGetWidth()*2,1000);
	path.bezierTo(0,0,ofGetWidth()*2,0,0,ofGetWidth()*2,ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*4,1000);*/
}


void ParticleSystemDemo::update(){
	t = ofGetElapsedTimef()*ofMap(speed,1,80,0,1)*ofMap(speed,1,80,0,1);
}

void ParticleSystemDemo::drawForGlow(){
	ofSeedRandom(0);
	for(int i=0;i<500;i++){
		ofColor color(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,minAlpha,maxAlpha));
		ofSetColor(color);
		//color.setBrightness();
		//ofSetColor(r,g,b,a*.5);
		ofPushMatrix();
		ofTranslate(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetWidth()*4);
		//ofRotate(ofNoise(i/100.,t,ofRandom(1))*360,1,0,0);
		//ofScale(radius,radius,radius);
		//particleMesh.draw();
		ofSphere(radius);
		ofPopMatrix();
		/*ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius);*/
	}
}

void ParticleSystemDemo::draw(){
	ofSeedRandom(0);
	for(int i=0;i<500;i++){
		ofColor color(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,minAlpha*3,maxAlpha));
		ofSetColor(color);
		//color.setBrightness();
		//ofSetColor(r,g,b,a*.5);

		ofPushMatrix();
		ofTranslate(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetWidth()*4);
		//ofRotate(ofNoise(i/100.,t,ofRandom(1))*360,1,0,0);
		//ofScale(radius*.4,radius*.4,radius*.4);
		//particleMesh.draw();
		ofSphere(radius*.4);
		ofPopMatrix();
		/*ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius*.4);*/
	}
}

void ParticleSystemDemo::drawPath(){
	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255);
	path.draw();
	ofPopStyle();
}
