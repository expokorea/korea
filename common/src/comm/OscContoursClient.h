/*
 * OscContoursClient.h
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#pragma once

#include "ofConstants.h"
#include "ofxOsc.h"
#include "ofxAvahiClient.h"

class OscContoursClient {
public:
	OscContoursClient();

	void setup(int port);
	void update();

	struct Blob{
		Blob(unsigned int id, const ofPoint & pos, float size)
		:id(id)
		,pos(pos)
		,size(size){}

		unsigned int id;
		ofPoint pos;
		float size;
	};

	vector<Blob> & getBlobs();
	unsigned int size();
	Blob & operator[](int i);

private:
	ofxOscReceiver osc;
	ofxAvahiClientService avahi;
	vector<Blob> blobs;
};

