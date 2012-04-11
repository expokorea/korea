/*
 * OscContourServer.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "OscContourServer.h"

OscBlobServer::OscBlobServer() {
}

OscBlobServer::OscBlobServer(string ip, int port, string host_name)
:OscService(ip,port,host_name){

}

void OscBlobServer::newFrame(int servernum, int frame){
	if(!isConnected()) return;
	ofxOscMessage msg;
	msg.setAddress("frame");
	msg.addIntArg(servernum);
	msg.addIntArg(frame);
	osc.sendMessage(msg);
}

void OscBlobServer::sendBlob(int servernum, int id, const ofPoint & pos, float size){
	if(!isConnected()) return;
	ofxOscMessage msg;
	msg.setAddress("blob");
	msg.addIntArg(servernum);
	msg.addIntArg(id);
	msg.addFloatArg(pos.x);
	msg.addFloatArg(pos.y);
	msg.addFloatArg(size);
	osc.sendMessage(msg);
}

