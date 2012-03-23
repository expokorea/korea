/*
 * ofxPlane.h
 *
 *  Created on: 27/03/2010
 *      Author: arturo
 */

#ifndef OFXPLANE_H_
#define OFXPLANE_H_

#include "ofVec3f.h"
#include "ofMain.h"
#include <math.h>
#include <float.h>

class ofxPlane {
public:
	float a,b,c,d;
	ofxPlane():a(0),b(0),c(0),d(0){};

	ofxPlane(float a, float b, float c, float d)
	:a(a),b(b),c(c),d(d){}

	ofxPlane(const ofVec3f & normal, const ofVec3f & point)
	:a(normal.x)
	,b(normal.y)
	,c(normal.z)
	,d(-normal.dot(point)){}

	ofxPlane(const ofVec3f & p1, const ofVec3f & p2, const ofVec3f & p3){
		set(p1,p2,p3);
	}

	virtual ~ofxPlane(){}

	void set(float _a, float _b, float _c, float _d){
		a = _a;
		b = _b;
		c = _c;
		d = _d;
	}

	void set(const ofVec3f & normal, const ofVec3f & point){
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = -normal.dot(point);
		//cout << d << endl;
	}

	void set(const ofVec3f & p1, const ofVec3f & p2, const ofVec3f & p3){
		a = p1.y *(p2.z - p3.z) + p2.y *(p3.z - p1.z) + p3.y *(p1.z - p2.z);
		b = p1.z *(p2.x - p3.x) + p2.z *(p3.x - p1.x) + p3.z *(p1.x - p2.x);
		c = p1.x *(p2.y - p3.y) + p2.x *(p3.y - p1.y) + p3.x *(p1.y - p2.y);
		d = -(p1.x * (p2.y *p3.z - p3.y *p2.z) + p2.x * (p3.y *p1.z - p1.y *p3.z) + p3.x * (p1.y *p2.z - p2.y* p1.z));
	}

	bool pointInPlane(const ofVec3f & point) const{
		return (a*point.x + b*point.y + c*point.z + d == 0);
	}

	ofVec3f lineIntersection(const ofVec3f & p1, const ofVec3f & p2) const{
		ofVec3f normal(a,b,c);
		float denom = normal.dot(p2-p1);
		if(denom == 0) return FLT_MAX; // parallel
		float numer = -d - normal.dot(p1);
		float pct = numer / denom;
		return ofVec3f(ofLerp(p1.x,p2.x,pct), ofLerp(p1.y,p2.y,pct), ofLerp(p1.z,p2.z,pct));
	}

	ofVec3f lineSegmentIntersection(const ofVec3f & p1, const ofVec3f & p2) const{
		ofVec3f normal(a,b,c);
		float denom = normal.dot(p2-p1);
		if(denom == 0) return FLT_MAX; // parallel
		float numer = -d - normal.dot(p1);
		float pct = numer / denom;
		if(pct>=0 && pct<=1)
			return ofVec3f(ofLerp(p1.x,p2.x,pct), ofLerp(p1.y,p2.y,pct), ofLerp(p1.z,p2.z,pct));
		else
			return FLT_MAX;
	}

	bool lineInPlane(const ofVec3f & p1, const ofVec3f & p2) const{
		ofVec3f normal(a,b,c);
		float denom = normal.dot(p2-p1);
		float numer = -d - normal.dot(p1);
		return (denom == 0 && numer == 0);
	}

	bool isLineParallel(const ofVec3f & p1, const ofVec3f & p2) const{
		ofVec3f normal(a,b,c);
		float denom = normal.dot(p2-p1);
		if(denom == 0) return true;
		else return false;
	}

	float distanceToPoint(const ofVec3f & p) const{
		return fabs(a*p.x + b*p.y + c*p.z + d) / sqrt(a*a + b*b + c*c);
	}

	float distanceToOrigin() const{
		return d/sqrt(a*a + b*b + c*c);
	}

	// <0 in the oposite side of the normal
	// >0 in the side  of the normal
	float signedDistanceToPoint(const ofVec3f & p) const{
		return (a*p.x + b*p.y + c*p.z + d) / sqrt(a*a + b*b + c*c);
	}

	ofVec3f nearestPointInPlane(const ofVec3f & p){

	}

	ofVec3f getNormal() const{
		return ofVec3f(a,b,c);
	}

	ofVec3f getP0() const{
		return ofVec3f(0,0,0);//ofVec3f(-d/a,-d/b,-d/c);
	}

	float angle(const ofxPlane & p) const{
		return acos( (a*p.a + b*p.b + c*p.c) / (sqrt(a*a + b*b + c*c) * sqrt(p.a + p.b + p.c)));
	}

	void rotate(float angle, const ofVec3f & pivot, const ofVec3f & axis){
		ofVec3f rotatedNormal = getNormal().getRotated(angle,pivot,axis);
		ofVec3f rotatedP0 = getP0().getRotated(angle,pivot,axis);
		set(rotatedNormal, rotatedP0);
	}

	ofxPlane getRotated(float angle, const ofVec3f & pivot, const ofVec3f & axis){
		ofxPlane rotated = *this;
		rotated.rotate(angle, pivot, axis);
		return rotated;
	}

	void operator+=(const ofVec3f & t){
		ofVec3f translatedNormal = getNormal() + t;
		ofVec3f translatedP0 = getP0() + t;
		set(translatedNormal,translatedP0);
	}

	ofxPlane operator+(const ofVec3f & t) const{
		ofxPlane plane = *this;
		plane += t;
		return plane;
	}

	void operator-=(const ofVec3f & t){
		*this += -t;
	}

	ofxPlane operator-(const ofVec3f & t) const{
		ofxPlane plane = *this;
		plane -= t;
		return plane;
	}

	void translate(const ofVec3f & t){
		*this += t;
	}

	ofxPlane getTranslated(const ofVec3f & t){
		return *this + t;
	}
};

#endif /* OFXPLANE_H_ */
