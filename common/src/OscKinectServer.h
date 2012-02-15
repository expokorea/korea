/*
 * OscKinectServer.h
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#ifndef OSCKINECTSERVER_H_
#define OSCKINECTSERVER_H_

#include "ofConstants.h"
#include "OscService.h"
#include "ofPanel.h"
#include "ofSlider.h"

class OscKinectServer: public OscService {
public:
	OscKinectServer();
	OscKinectServer(string ip, int port, string host_name);
	virtual ~OscKinectServer();

	void tiltChanged(int & tilt);
	void nearThresholdChanged(int & t);
	void farThresholdChanged(int & t);

	ofPanel gui;
	ofIntSlider tilt;
	ofIntSlider nearThreshold;
	ofIntSlider farThreshold;

private:
	void init();
};

#endif /* OSCKINECTSERVER_H_ */
