/*
 * OscPlayerClient.h
 *
 *  Created on: 07/02/2012
 *      Author: arturo
 */

#ifndef OSCPLAYERCLIENT_H_
#define OSCPLAYERCLIENT_H_

#include "ofxOscSender.h"
#include "ofConstants.h"

class OscPlayerClient {
public:
	OscPlayerClient();
	OscPlayerClient(string ip, int port);

	void connect(string ip, int port);
	void close();

	void update();

	void play();
	void stop();

	void togglePlaying();
	bool isPlaying();

	string getIp();
	int getPort();

	bool isConnected();

private:
	ofxOscSender osc;
	string ip;
	int port;
	bool connected;
	bool playing;
};

#endif /* OSCPLAYERCLIENT_H_ */
