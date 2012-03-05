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
	gui.add(nearClip.setup("nearClip",3000,0,10000,300));
	gui.add(farClip.setup("farClip",6500,0,10000,300));
	gui.add(record.setup("record",false));
	gui.add(player.setup("use player",false));

	tilt.addListener(this,&OscKinectServer::tiltChanged);
	nearThreshold.addListener(this,&OscKinectServer::nearThresholdChanged);
	farThreshold.addListener(this,&OscKinectServer::farThresholdChanged);
	nearClip.addListener(this,&OscKinectServer::clipChanged);
	farClip.addListener(this,&OscKinectServer::clipChanged);
	record.addListener(this,&OscKinectServer::recordPressed);
	player.addListener(this,&OscKinectServer::usePlayer);
}

OscKinectServer::~OscKinectServer() {
	tilt.removeListener(this,&OscKinectServer::tiltChanged);
	nearThreshold.removeListener(this,&OscKinectServer::nearThresholdChanged);
	farThreshold.removeListener(this,&OscKinectServer::farThresholdChanged);
}


void OscKinectServer::recordPressed(bool & r){
	if(!isConnected()) return;

	ofxOscMessage msg;

	msg.setAddress("record");
	msg.addIntArg(r);
	osc.sendMessage(msg);

}

void OscKinectServer::usePlayer(bool & p){

	if(!isConnected()) return;

	ofxOscMessage msg;

	msg.setAddress("player");
	msg.addIntArg(p);
	osc.sendMessage(msg);
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

void OscKinectServer::clipChanged(float & c){
	if(!isConnected()) return;

	ofxOscMessage msg;

	msg.setAddress("clipping");
	msg.addFloatArg(nearClip);
	msg.addFloatArg(farClip);
	osc.sendMessage(msg);

}
