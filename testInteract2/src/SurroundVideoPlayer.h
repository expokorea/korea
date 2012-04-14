/*
 * SurroundVideoPlayer.h
 *
 *  Created on: 12/04/2012
 *      Author: arturo
 */

#pragma once
#include "ofGstVideoPlayer.h"
#include "ofBaseTypes.h"
#include "ofSoundStream.h"
#include "ofxAndroidCircBuffer.h"
#include <queue>

class SurroundVideoPlayer: public ofGstVideoPlayer, public ofBaseSoundOutput {
public:
	SurroundVideoPlayer();
	virtual ~SurroundVideoPlayer();

	bool loadMovie(string name);
	void play();

	void setDeviceFront(string name);
	void setDeviceRear(string name);
	void setDeviceSubLFE(string name);

	void newBufferCB(GstBuffer * buffer);
	void audioOut( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount  );

	int getDeviceIDByName(string name);
private:
	ofSoundStream frontOut;
	ofSoundStream rearOut;
	ofSoundStream LFESubOut;
	int deviceRear, deviceFront, deviceLFESub;
	string nameFront,nameRear,nameSubLFE;
	ofxAndroidCircBuffer<float> bufferFront,bufferRear,bufferLFESub;
};

