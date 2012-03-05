/*
 * ofxKinectSequencePlayer.h
 *
 *  Created on: 05/03/2012
 *      Author: arturo
 */

#pragma once

#include "ofConstants.h"
#include "ofPixels.h"
#include "ofTexture.h"

class ofxKinectSequencePlayer {
public:
	ofxKinectSequencePlayer();
	virtual ~ofxKinectSequencePlayer();

	void load(string movie);

	void update();
	bool isFrameNew();

	/// get the pixels of the most recent depth frame
	ofPixels & getDepthPixelsRef();       ///< grayscale values
	ofShortPixels & getRawDepthPixelsRef();   ///< raw 11 bit values

	/// get the distance in millimeters to a given point as a float array
	ofFloatPixels & getDistancePixelsRef();

	void enableDepthNearValueWhite(bool bEnabled);
	bool isDepthNearValueWhite();
	float getNearClipping();
	float getFarClipping();
	void setDepthClipping(float nearClip=500, float farClip=4000);
    /// get the calulated distance for a depth point
	float getDistanceAt(int x, int y);
	float getDistanceAt(const ofPoint & p);

	void drawDepth(float x, float y);
	void drawDistance(float x, float y);

private:
	void updateDepthLookupTable();

	ofShortPixels rawPixels;
	ofFloatPixels distancePixels;
	ofPixels depthPixels;
	ofDirectory folder;
	int currentImage;
	int lastFrameMillis;
	vector<unsigned char> depthLookupTable;
	bool bNearWhite;

	float nearClipping, farClipping;
	bool newFrame;

	ofTexture texDepth,texDistance;
};

