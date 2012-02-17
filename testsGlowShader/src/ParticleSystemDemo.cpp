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
	model.loadModel("particle.obj");
	particleMesh = model.getMesh(0);
}


void ParticleSystemDemo::update(){
	t = ofGetElapsedTimef()*ofMap(speed,1,80,0,1)*ofMap(speed,1,80,0,1);
}

void ParticleSystemDemo::drawForGlow(){
	ofSeedRandom(0);
	for(int i=0;i<1000;i++){
		ofColor color(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,minAlpha,maxAlpha));
		ofSetColor(color);
		//color.setBrightness();
		//ofSetColor(r,g,b,a*.5);
		ofPushMatrix();
		ofTranslate(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4);
		ofRotate(ofNoise(i/100.,t,ofRandom(1))*360,1,0,0);
		ofScale(radius,radius,radius);
		particleMesh.draw();
		ofPopMatrix();
		/*ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius);*/
	}
}

void ParticleSystemDemo::draw(){
	ofSeedRandom(0);
	for(int i=0;i<1000;i++){
		ofColor color(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,minAlpha*3,maxAlpha));
		ofSetColor(color);
		//color.setBrightness();
		//ofSetColor(r,g,b,a*.5);

		ofPushMatrix();
		ofTranslate(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4);
		ofRotate(ofNoise(i/100.,t,ofRandom(1))*360,1,0,0);
		ofScale(radius*.4,radius*.4,radius*.4);
		particleMesh.draw();
		ofPopMatrix();
		/*ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius*.4);*/
	}
}
