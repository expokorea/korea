/*
 * OscContoursClient.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "OscContoursClient.h"

OscContoursClient::OscContoursClient() {
	// TODO Auto-generated constructor stub

}


void OscContoursClient::setup(int port){
	osc.setup(port);
	avahi.start("oscservercontours","_oscit._udp",port);
}

void OscContoursClient::update(){
	ofxOscMessage msg;
	while(osc.hasWaitingMessages()){
		osc.getNextMessage(&msg);
		if(msg.getAddress()=="frame"){
			contours.clear();
			continue;
		}

		if(msg.getAddress()=="contour"){
			int n = msg.getNumArgs();
			contours.push_back(ofPolyline());
			ofPolyline & contour = contours.back();
			float z = msg.getArgAsFloat(0);
			for(int i=1;i<n;i+=2){
				ofPoint p(msg.getArgAsFloat(i),msg.getArgAsFloat(i+1));
				contour.addVertex(p);
			}
			contour.close();
		}
	}
}

vector<ofPolyline> & OscContoursClient::getContours(){
	return contours;
}

unsigned int OscContoursClient::size(){
	return contours.size();
}

ofPolyline & OscContoursClient::operator[](int i){
	return contours[i];
}
