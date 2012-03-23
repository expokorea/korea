/*
 * OscPlayerClient.h
 *
 *  Created on: 07/02/2012
 *      Author: arturo
 */

#ifndef OSCPLAYERCLIENT_H_
#define OSCPLAYERCLIENT_H_

#include "ofConstants.h"
#include "OscService.h"
#include "ofxToggle.h"

class OscPlayerClient: public OscService {
public:
	OscPlayerClient();
	OscPlayerClient(string ip, int port, string host_name);

	ofxParameter<bool> playing;

	void sendMessages();

private:
	void init();
};

#endif /* OSCPLAYERCLIENT_H_ */
