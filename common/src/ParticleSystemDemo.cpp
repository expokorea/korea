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
	particles.getVertices().resize(10000);
	particles.getColors().resize(10000);
	particles.getNormals().resize(10000);
	particles.setMode(OF_PRIMITIVE_POINTS);
	particles.setUsage(GL_STREAM_DRAW);
}

void ParticleSystemDemo::setup(){
    ofDisableArbTex();
    ofLoadImage(tex, "dot.png");
    ofEnableArbTex();
}

void ParticleSystemDemo::update(){
	t = ofGetElapsedTimef()*ofMap(speed,1,80,0,1)*ofMap(speed,1,80,0,1);
	ofSeedRandom(0);
	for(int i=0;i<10000;i++){
		particles.getColors()[i].set(ofColor(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,minAlpha,maxAlpha)));
		particles.getVertices()[i].set(ofVec3f(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetWidth()*4));
		particles.getNormals()[i].set(ofRandom(5,15),0,0);
	}
}

void ParticleSystemDemo::drawForGlow(){
}

void ParticleSystemDemo::draw(){
	//float attenuation[3] = {.5,.5,.5};
	//glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION,attenuation);
	//glPointSize(10);
	//glDepthMask(GL_FALSE);
	ofEnablePointSprites();
	tex.bind();
	particles.draw();
	tex.unbind();


    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	particles.draw();

	ofDisablePointSprites();
	//glDepthMask(GL_TRUE);
}

void ParticleSystemDemo::drawPath(){
	ofPushStyle();
	ofSetLineWidth(3);
	ofSetColor(255);
	path.draw();
	ofPopStyle();
}
