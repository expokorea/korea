#pragma once
#include "ofConstants.h"
#include "ofVectorMath.h"
#include "ofxGui.h"
#include "ofNode.h"

class BoundingBox3D;

class RibbonParticle{
public:
	void setup();
	void update(float dt,const BoundingBox3D & bb);
	void draw();
	void drawForGlow();


	enum State{
		Flocking,
		ReachingTarget,
		TargetReached,
		RunningAway,
		GoingBack
	} state;

	void flock();
	void runAway();
	void goBack();
	State getState(){
		return state;
	}


	enum TexMode{
		Delaunay,
		Voronoi
	}texMode;
	static void generateTexture(TexMode mode);

	ofVec3f getPos(){ return pos; }

	ofVec3f 	target;



	static ofxParameter<int> r,g,b;
	static ofxParameter<int> rTex,gTex,bTex;
	static ofxParameter<int> rLines,gLines,bLines;
	static ofxParameter<float> speedFactor;
	static ofxParameter<float> friction;
	static ofxParameter<float> strengthRunnawayForce;
	static ofxParameter<float> maxDistanceRunAway;
	static ofxParameter<float> jitterFreq;
	static ofxParameter<float> jitterAmp;
	static ofxParameter<float> accelFactor;
	static ofxParameter<float> headSize;
	static ofxParameter<float> thicknessMin;
	static ofxParameter<float> thicknessMax;
	static ofxParameter<int> lengthMin;
	static ofxParameter<int> lengthMax;

	static ofFbo tex;
	static ofImage head;
private:
	void setupTrails();
	void lengthChanged(int & length);
	void thicknessChanged(float & thickness);

	// trails
	ofVboMesh trailStrip,trailStripForGlow,texturedStrip,trailStripLineL,trailStripLineR, headMesh;
	vector<ofVec3f> trails;
	// node debug
	ofVec3f		vel, accel;
	ofVec3f  	pos;
	float thisThickness;
	int length;
	float thisSpeedFactor, thisFriction;
	float noiseSeed;
	ofVec3f prevTarget;
	ofVec3f interpTarget;
	ofColor thisRGB;
	ofColor targetColor;
	float jitterPhase;
	float tTargetChanged;
	
	// highlight color
	float higlightCounter;
	
	enum SpeedState{
		Fast,
		Slow
	}speedState;

	static int headRibbonCoordsWidth;
	static ofPoint headRibbonCoordsZero;

};
