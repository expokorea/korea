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

class OscBlobServer: public OscService {
public:
	OscBlobServer();
	OscBlobServer(string ip, int port, string host_name);

	void newFrame(int servernum, int frame);
	void sendBlob(int servernum, int id, const ofPoint & pos, float size);

};

