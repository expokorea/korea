/*
 * OscPlayerClient.cpp
 *
 *  Created on: 07/02/2012
 *      Author: arturo
 */

#include "OscPlayerClient.h"

OscPlayerClient::OscPlayerClient()
:OscService()
{
	init();
}

OscPlayerClient::OscPlayerClient(string ip, int port, string host_name)
:OscService(ip,port,host_name)
{
	init();
}

void OscPlayerClient::init(){
	playing = false;
}


void OscPlayerClient::sendMessages(){
	ofxOscMessage msg;
	if(playing){
		msg.setAddress("play");
	}else{
		msg.setAddress("stop");
	}
	osc.sendMessage(msg);
}
