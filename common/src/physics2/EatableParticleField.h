#pragma once

#include "ofConstants.h"
#include "ofVectorMath.h"
#include "ofPoint.h"
#include "BoundingBox3D.h"
#include "RibbonParticle.h"
#include "ofxGui.h"
#include <list>

class EatableParticleField{
public:
	void setup();
	void update(float t, const BoundingBox3D & bb);
	void draw();

	ofVec3f getParticle(RibbonParticle * p);
	
	static ofxParameter<float> eatDistance;
	static ofxParameter<int> r,g,b;
	static ofxParameter<float> sameQuadrantProbability;

private:
	struct EatenParticle{
		EatenParticle(const ofVec3f & pos, float born, float t, float r)
		:pos(pos)
		,born(born)
		,life(t)
		,maxLife(t)
		,radius(r){}

		ofVec3f pos;
		float born;
		float life,maxLife;
		float radius;
	};
	map<RibbonParticle*,ofVec3f> particles;
	map<RibbonParticle*,ofVec3f> prevParticles;
	list<EatenParticle> eatenParticles;
	vector<BoundingBox3D> quadrants;

	// 3d bounding box, 2 points: x,y,z  w,h,d
	BoundingBox3D bb3d;

	enum State{
		Distributed,
		SameQuadrant
	}state;
	int particlesReachedSameQuadrant;
};
