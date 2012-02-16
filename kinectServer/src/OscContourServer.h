/*
 * OscContourServer.h
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#pragma once

#include "OscService.h"
#include "ofConstants.h"
#include "ofPolyline.h"

class OscContourServer: public OscService {
public:
	OscContourServer();
	OscContourServer(string ip, int port, string host_name);

	void newFrame(int frame);
	void sendContour(const ofPolyline & contour, float z);

};

