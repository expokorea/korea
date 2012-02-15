/*
 * OscService.h
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#ifndef OSCSERVICE_H_
#define OSCSERVICE_H_

#include "ofxOscSender.h"
#include "ofConstants.h"

class OscService {
public:
	OscService();
	OscService(string ip, int port, string host_name);
	virtual ~OscService();

	void connect(string ip, int port, string host_name);
	void close();

	void update();

	string getIp();
	int getPort();
    string getHostName() const;

	bool isConnected();

protected:
	virtual void sendMessages(){};
	ofxOscSender osc;

private:
	string ip;
	string host_name;
	int port;
	bool connected;
};

#endif /* OSCSERVICE_H_ */
