/*
 * EatableParticleField.cpp
 *
 *  Created on: 28/03/2012
 *      Author: arturo
 */

#include "EatableParticleField.h"

ofxParameter<float> EatableParticleField::eatDistance=100;
ofxParameter<int> EatableParticleField::r,EatableParticleField::g,EatableParticleField::b;
ofxParameter<float> EatableParticleField::sameQuadrantProbability;


static int randomDifferent(int low, int high, int old) {
	int cur = ofRandom(low, high - 1);
	if(cur >= old) {
		cur++;
		cur = cur % high;
	}
	return cur;
}


void EatableParticleField::setup(){
	quadrants.resize(8);
	state = Distributed;
	//ofDisableArbTex();
	dot.loadImage("dot.png");
	//ofEnableArbTex();
	//cout << "dot target" << dot.getTextureReference().getTextureData().textureTarget << endl;
}

void EatableParticleField::update(float dt, const BoundingBox3D & bb){
	bb3d = bb;

	quadrants[0].x = bb.x;
	quadrants[0].y = bb.y;
	quadrants[0].z = bb.z;
	quadrants[0].width = bb.width*.5;
	quadrants[0].height = bb.height*.5;
	quadrants[0].depth = bb.depth*.5;

	quadrants[1] = quadrants[0];
	quadrants[1].x = bb.x+bb.width*.5;
	quadrants[1].y = bb.y;

	quadrants[2] = quadrants[0];
	quadrants[2].x = bb.x+bb.width*.5;
	quadrants[2].y = bb.y+bb.height*.5;

	quadrants[3] = quadrants[0];
	quadrants[3].x = bb.x;
	quadrants[3].y = bb.y+bb.height*.5;

	quadrants[4] = quadrants[0];
	quadrants[4].z = bb.z+bb.depth*.5;

	quadrants[5] = quadrants[1];
	quadrants[5].z = bb.z+bb.depth*.5;

	quadrants[6] = quadrants[2];
	quadrants[6].z = bb.z+bb.depth*.5;

	quadrants[7] = quadrants[3];
	quadrants[7].z = bb.z+bb.depth*.5;

	switch(state){
	case Distributed:{
		if(ofRandom(1)<sameQuadrantProbability){
			state = SameQuadrant;
			int nextQuadrant = ofRandom(quadrants.size()-1);
			ofVec3f p(ofRandom(quadrants[nextQuadrant].x,quadrants[nextQuadrant].x+quadrants[nextQuadrant].width),
									ofRandom(quadrants[nextQuadrant].y,quadrants[nextQuadrant].y+quadrants[nextQuadrant].height),
									ofRandom(quadrants[nextQuadrant].z,quadrants[nextQuadrant].z+quadrants[nextQuadrant].depth));
			map<RibbonParticle*,ofVec3f>::iterator it;
			for(it=particles.begin();it!=particles.end();it++){
				prevParticles[it->first] = it->second;
				it->second.set(p);
				it->first->setHuntting(true);
			}
			particlesReachedSameQuadrant = 0;
		}
	}break;
	case SameQuadrant:{
		if(particlesReachedSameQuadrant == particles.size()){
			state = Distributed;
		}
	}break;
	}

	vector<list<EatenParticle>::iterator> toDelete;
	list<EatenParticle>::iterator it;
	for(it=eatenParticles.begin();it!=eatenParticles.end();it++){
		it->life -= dt;
		if(it->life<=0){
			toDelete.push_back(it);
		}else{
			float d = it->born+it->life;
			it->pos += ofVec3f(ofSignedNoise(d),ofSignedNoise(d),ofSignedNoise(d));
		}
	}
	for(int i=0;i<toDelete.size();i++){
		eatenParticles.erase(toDelete[i]);
	}
}


void EatableParticleField::draw(){
	/*glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	//glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(30);
	dot.getTextureReference().bind();
	//ofEnablePointSprites();
	glBegin(GL_POINTS);
	glVertex3f(ofGetMouseX(),ofGetMouseY(),0);*/
	list<EatenParticle>::iterator it;
	for(it=eatenParticles.begin();it!=eatenParticles.end();it++){
		float alpha = it->life/it->maxLife * 255;
		ofSetColor(r,g,b,alpha*.8);
		//glVertex3f(it->pos.x,it->pos.y,0);//it->pos.z);
		ofCircle(it->pos,it->radius*.8);
	}
	/*glEnd();
	dot.getTextureReference().unbind();
	ofDisablePointSprites();*/

	/*for(it=eatenParticles.begin();it!=eatenParticles.end();it++){
		float alpha = it->life/it->maxLife * 255;
		ofSetColor(r,g,b,alpha);
		ofCircle(it->pos,it->radius);
	}*/
	//ofSetColor(255,0,0);
	//ofNoFill();
	//bb3d.draw();
}

void EatableParticleField::drawForGlow(){
	/*glEnable(GL_POINT_SPRITE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	//glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(30);
	dot.getTextureReference().bind();
	//ofEnablePointSprites();
	glBegin(GL_POINTS);
	glVertex3f(ofGetMouseX(),ofGetMouseY(),0);*/
	list<EatenParticle>::iterator it;
	for(it=eatenParticles.begin();it!=eatenParticles.end();it++){
		float alpha = it->life/it->maxLife * 255;
		ofSetColor(r,g,b,alpha);
		//glVertex3f(it->pos.x,it->pos.y,0);//it->pos.z);
		ofCircle(it->pos,it->radius*1.3);
	}
	/*glEnd();
	dot.getTextureReference().unbind();
	ofDisablePointSprites();*/

	/*for(it=eatenParticles.begin();it!=eatenParticles.end();it++){
		float alpha = it->life/it->maxLife * 255;
		ofSetColor(r,g,b,alpha);
		ofCircle(it->pos,it->radius);
	}*/
	//ofSetColor(255,0,0);
	//ofNoFill();
	//bb3d.draw();
}

ofVec3f EatableParticleField::getParticle(RibbonParticle * p){
	
	bool bTargetInside = bb3d.inside(particles[p]);
	
	if(!bTargetInside || particles.find(p)==particles.end() || p->getPos().squareDistance(particles[p])<eatDistance || p->getState()==RibbonParticle::GoingBack){
		int nextQuadrant = ofRandom(0,quadrants.size()-1);
		if(prevParticles.find(p)!=particles.end()){
			for(int i=0;i<quadrants.size();i++){
				if(quadrants[i].inside(prevParticles[p])){
					nextQuadrant = randomDifferent(0,quadrants.size()-1,i);
					break;
				}
			}
		}
		eatenParticles.push_back(EatenParticle(particles[p],ofGetElapsedTimef(),ofRandom(1.5),ofRandom(3)));
		prevParticles[p] = particles[p];
		particles[p].set(ofRandom(quadrants[nextQuadrant].x,quadrants[nextQuadrant].x+quadrants[nextQuadrant].width),
				ofRandom(quadrants[nextQuadrant].y,quadrants[nextQuadrant].y+quadrants[nextQuadrant].height),
				ofRandom(quadrants[nextQuadrant].z,quadrants[nextQuadrant].z+quadrants[nextQuadrant].depth));
		if(state==SameQuadrant){
			particlesReachedSameQuadrant++;
			p->setHuntting(false);
		}
	}
	return particles[p];
}

