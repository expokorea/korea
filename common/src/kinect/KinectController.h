/*
 * KinectController.h
 *
 *  Created on: 23/03/2012
 *      Author: arturo
 */

#pragma once
#include "ofxGui.h"
#include "ofxOsc.h"
#include "ofxAvahiClient.h"
#include "ofxKinect.h"
#include "ofCamera.h"

class KinectController {
public:
	KinectController(int device);
	virtual ~KinectController();

	void update(float bbX, float bbY, float bbZ, float bbW, float bbH, float bbD);
	void drawGui();
	void drawPointCloud();

	bool isFrameNew();

	void tiltChanged(int & tilt);
	void rotZeroPressed(bool & pressed);
	void clippingChanged(float & clip);

	void drawDepth(float x, float y, float w, float h);
	void drawRGB(float x, float y, float w, float h);

	ofxPanel gui;
	ofxParameter<int> tilt;
	ofxParameter<float> nearClip, farClip;
	ofxParameter<bool> correctAngle;
	ofxParameter<float> pitch, roll, yaw;
	ofxParameter<float> rollZero, tiltZero;
	ofxButton rotZeroBtn;
	ofxParameter<bool> applyBB;
	ofxParameter<bool> useColor;
	ofxParameter<float> x,y,z;
	ofCamera camTransform;
	ofCamera camTransformView;

private:

	void setup(int device);
	void updateAnalisys(ofxKinect & kinect,float bbX, float bbY, float bbZ, float bbW, float bbH, float bbD);

	ofxKinect kinect;
	ofVboMesh mesh;
	ofxOscReceiver oscConfig;
	ofxAvahiClientService avahi;
	int step;
};

