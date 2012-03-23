/*
 * OscService.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "OscService.h"

OscService::OscService() {
	connected = false;
}

OscService::OscService(string ip, int port, string host_name){
	connect(ip,port,host_name);
}

OscService::~OscService(){

}

void OscService::connect(string _ip, int _port, string _host_name){
	ip = _ip;
	port = _port;
	host_name = _host_name;
	osc.setup(ip,port);
	connected = true;
}

void OscService::update(){
	if(connected){
		sendMessages();
	}
}

string OscService::getIp(){
	return ip;
}

int OscService::getPort(){
	return port;
}

void OscService::close(){
	connected = false;
}

bool OscService::isConnected(){
	return connected;
}

string OscService::getHostName() const
{
    return host_name;
}


