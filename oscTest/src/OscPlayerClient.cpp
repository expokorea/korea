/*
 * OscPlayerClient.cpp
 *
 *  Created on: 07/02/2012
 *      Author: arturo
 */

#include "OscPlayerClient.h"

OscPlayerClient::OscPlayerClient() {
	connected = false;
	playing = false;
}

OscPlayerClient::OscPlayerClient(string ip, int port){
	playing = false;
	connect(ip,port);
}

void OscPlayerClient::connect(string _ip, int _port){
	ip = _ip;
	port = _port;
	osc.setup(ip,port);
	connected = true;
}

void OscPlayerClient::update(){
	if(connected){
		ofxOscMessage msg;
		if(playing){
			msg.setAddress("play");
		}else{
			msg.setAddress("stop");
		}
		osc.sendMessage(msg);
	}
}

void OscPlayerClient::play(){
	playing = true;
}

void OscPlayerClient::stop(){
	playing = false;
}

void OscPlayerClient::togglePlaying(){
	playing = !playing;
}

bool OscPlayerClient::isPlaying(){
	return playing;
}

string OscPlayerClient::getIp(){
	return ip;
}

int OscPlayerClient::getPort(){
	return port;
}

void OscPlayerClient::close(){
	connected = false;
}

bool OscPlayerClient::isConnected(){
	return connected;
}
