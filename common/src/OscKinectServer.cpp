/*
 * OscKinectServer.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "OscKinectServer.h"

OscKinectServer::OscKinectServer()
:OscService()
{
	init();
}


OscKinectServer::OscKinectServer(string ip, int port, string host_name)
:OscService(ip,port,host_name)
{
	init();
}

void OscKinectServer::init(){
	gui.setup("kinect at " + getHostName() + ":" + ofToString(getPort()));
	gui.add(tilt.setup("tilt",0,-30,30,300));
	gui.add(nearThreshold.setup("nearThreshold",0,0,255,300));
	gui.add(farThreshold.setup("farThreshold",0,0,255,300));

	tilt.addListener(this,&OscKinectServer::tiltChanged);
	nearThreshold.addListener(this,&OscKinectServer::nearThresholdChanged);
	farThreshold.addListener(this,&OscKinectServer::farThresholdChanged);
}

OscKinectServer::~OscKinectServer() {
	tilt.removeListener(this,&OscKinectServer::tiltChanged);
	nearThreshold.removeListener(this,&OscKinectServer::nearThresholdChanged);
	farThreshold.removeListener(this,&OscKinectServer::farThresholdChanged);
}




void OscKinectServer::tiltChanged(int & tilt){
	if(!isConnected()) return;

	ofxOscMessage msg;

	msg.setAddress("tilt");
	msg.addIntArg(tilt);
	osc.sendMessage(msg);

}

void OscKinectServer::nearThresholdChanged(int & t){
	if(!isConnected()) return;

	ofxOscMessage msg;

	msg.setAddress("nearThreshold");
	msg.addIntArg(nearThreshold);
	osc.sendMessage(msg);
}

void OscKinectServer::farThresholdChanged(int & t){
	if(!isConnected()) return;

	ofxOscMessage msg;

	msg.setAddress("farThreshold");
	msg.addIntArg(farThreshold);
	osc.sendMessage(msg);
}
