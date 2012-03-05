/*
 * ofxKinectSequencePlayer.cpp
 *
 *  Created on: 05/03/2012
 *      Author: arturo
 */

#include "ofxKinectSequencePlayer.h"
#include "ofImage.h"

ofxKinectSequencePlayer::ofxKinectSequencePlayer() {
	// TODO Auto-generated constructor stub

}

ofxKinectSequencePlayer::~ofxKinectSequencePlayer() {
	// TODO Auto-generated destructor stub
}

void ofxKinectSequencePlayer::load(string movie){
	folder.listDir(movie);
	folder.sort();
	rawPixels.allocate(640,480,OF_IMAGE_GRAYSCALE);
	distancePixels.allocate(640,480,OF_IMAGE_GRAYSCALE);
	depthPixels.allocate(640,480,OF_IMAGE_GRAYSCALE);

	texDepth.allocate(640,480,GL_LUMINANCE);
	texDistance.allocate(640,480,ofGetGlInternalFormat(distancePixels));

	bNearWhite = true;
	setDepthClipping();

	currentImage = 0;
	newFrame=false;
	lastFrameMillis = ofGetElapsedTimeMillis();

}

void ofxKinectSequencePlayer::update(){
	if(ofGetElapsedTimeMillis()-lastFrameMillis>=1000./30. && currentImage<folder.size()){
		ofLoadImage(rawPixels,folder.getFile(currentImage,ofFile::Reference).getAbsolutePath());

		int n = 640 * 480;

		distancePixels = rawPixels;
		for(int i = 0; i < n; i++) {
			depthPixels[i] = depthLookupTable[rawPixels[i]];
		}
		texDepth.loadData(depthPixels);
		texDistance.loadData(distancePixels);
		newFrame = true;
		currentImage++;
	}else{
		newFrame = false;
	}
}

bool ofxKinectSequencePlayer::isFrameNew(){
	return newFrame;
}

/// get the pixels of the most recent depth frame
ofPixels & ofxKinectSequencePlayer::getDepthPixelsRef(){
	return depthPixels;
}
ofShortPixels & ofxKinectSequencePlayer::getRawDepthPixelsRef(){
	return rawPixels;
}

/// get the distance in millimeters to a given point as a float array
ofFloatPixels & ofxKinectSequencePlayer::getDistancePixelsRef(){
	return distancePixels;
}
//---------------------------------------------------------------------------
void ofxKinectSequencePlayer::updateDepthLookupTable() {
	unsigned char nearColor = bNearWhite ? 255 : 0;
	unsigned char farColor = bNearWhite ? 0 : 255;
	unsigned int maxDepthLevels = 10000;
	depthLookupTable.resize(maxDepthLevels);
	depthLookupTable[0] = 0;
	for(int i = 1; i < maxDepthLevels; i++) {
		depthLookupTable[i] = ofMap(i, nearClipping, farClipping, nearColor, farColor, true);
	}
}
//---------------------------------------------------------------------------
void ofxKinectSequencePlayer::enableDepthNearValueWhite(bool bEnabled) {
	bNearWhite = bEnabled;
	updateDepthLookupTable();
}

//---------------------------------------------------------------------------
bool ofxKinectSequencePlayer::isDepthNearValueWhite() {
	return bNearWhite;
}

//---------------------------------------------------------------------------
void ofxKinectSequencePlayer::setDepthClipping(float nearClip, float farClip) {
	nearClipping = nearClip;
	farClipping = farClip;
	updateDepthLookupTable();
}

//---------------------------------------------------------------------------
float ofxKinectSequencePlayer::getNearClipping() {
    return nearClipping;
}

//---------------------------------------------------------------------------
float ofxKinectSequencePlayer::getFarClipping() {
    return farClipping;
}

//------------------------------------
float ofxKinectSequencePlayer::getDistanceAt(int x, int y) {
	return rawPixels[y * 640 + x];
}

//------------------------------------
float ofxKinectSequencePlayer::getDistanceAt(const ofPoint & p) {
	return getDistanceAt(p.x, p.y);
}

void ofxKinectSequencePlayer::drawDepth(float x, float y){
	texDepth.draw(x,y);
}

void ofxKinectSequencePlayer::drawDistance(float x, float y){
	texDistance.draw(x,y);
}
