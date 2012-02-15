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

	vector<ofPolyline> & getContours();
	unsigned int size();
	ofPolyline & operator[](int i);

private:
	ofxOscReceiver osc;
	ofxAvahiClientService avahi;
	vector<ofPolyline> contours;
};

