/*
 * OscContoursClient.h
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#pragma once

#include "ofConstants.h"
#include "ofxOsc.h"
#include "ofxGui.h"
#include <deque>

#ifdef TARGET_LINUX
#define USE_ZEROCONF
#endif

#ifdef USE_ZEROCONF
#include "ofxAvahiClient.h"
#endif

class OscContoursClient {
public:
	OscContoursClient();

	void setup(int port);
	void update();

	struct Blob{
		Blob(int servernum, unsigned int id, const ofPoint & pos, float size)
		:servernum(servernum)
		,id(id)
		,pos(pos)
		,size(size){}

		int servernum;
		unsigned int id;
		ofPoint pos;
		float size;
		deque<float> sizeHist;
		bool suddenChange;
	};

	vector<Blob> & getBlobs(int server);
	unsigned int size(int server);

	ofxParameter<float> runAwaySensitivity;
	ofxParameter<int> runAwayFramesThreshold;

private:
	ofxOscReceiver osc;
#ifdef USE_ZEROCONF
	ofxAvahiClientService avahi;
#endif
	vector<vector<Blob> >blobs;
	vector<vector<Blob> >prevBlobs;
};

