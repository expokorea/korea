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
}


void ParticleSystemDemo::update(){
	t = ofGetElapsedTimef()*ofMap(speed,1,80,0,1)*ofMap(speed,1,80,0,1);
}

void ParticleSystemDemo::drawForGlow(){
	ofSeedRandom(0);
	for(int i=0;i<100;i++){
		ofColor color(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,minAlpha,maxAlpha));
		ofSetColor(color);
		//color.setBrightness();
		//ofSetColor(r,g,b,a*.5);
		ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius);
	}
}

void ParticleSystemDemo::draw(){
	ofSeedRandom(0);
	for(int i=0;i<100;i++){
		ofColor color(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,minAlpha*3,maxAlpha));
		ofSetColor(color);
		//color.setBrightness();
		//ofSetColor(r,g,b,a*.5);
		ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius*.4);
	}
}
