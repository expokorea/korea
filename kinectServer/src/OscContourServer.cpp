/*
 * OscContourServer.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "OscContourServer.h"

OscContourServer::OscContourServer() {
}

OscContourServer::OscContourServer(string ip, int port, string host_name)
:OscService(ip,port,host_name){

}

void OscContourServer::newFrame(int frame){
	if(!isConnected()) return;
	ofxOscMessage msg;
	msg.setAddress("frame");
	msg.addIntArg(frame);
	osc.sendMessage(msg);
}

void OscContourServer::sendContour(const ofPolyline & contour, float z){
	if(!isConnected()) return;
	ofxOscMessage msg;
	msg.setAddress("contour");
	msg.addFloatArg(z);
	for(int j=0;j<(int)contour.size();j++){
		msg.addFloatArg(contour[j].x);
		msg.addFloatArg(contour[j].y);
	}
	osc.sendMessage(msg);
}

