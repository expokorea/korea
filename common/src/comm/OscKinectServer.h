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
#include "ofxPanel.h"
#include "ofxSlider.h"

class OscKinectServer: public OscService {
public:
	OscKinectServer();
	OscKinectServer(string ip, int port, string host_name);
	virtual ~OscKinectServer();

	void tiltChanged(int & tilt);
	void nearThresholdChanged(int & t);
	void farThresholdChanged(int & t);
	void recordPressed(bool & r);
	void usePlayer(bool & p);
	void clipChanged(float & c);

	ofxPanel gui;
	ofxIntSlider tilt;
	ofxIntSlider nearThreshold;
	ofxIntSlider farThreshold;
	ofxFloatSlider nearClip;
	ofxFloatSlider farClip;
	ofxToggle	 record, player;
private:
	void init();
};

#endif /* OSCKINECTSERVER_H_ */
