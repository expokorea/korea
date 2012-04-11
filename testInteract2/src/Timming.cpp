/*
 * Timming.cpp
 *
 *  Created on: 10/04/2012
 *      Author: arturo
 */

#include "Timming.h"

Timming::Timming() {
	player = 0;
}

bool triggerEvent(vector<int> & times,float time, float previousTime){
	for(int i=0;i<times.size();i++){
		if(time>=times[i] && previousTime<times[i]){
			return true;
		}
	}
	return false;
}

void Timming::setup(VideoPlayer & _player){
	player = &_player;
	times.loadFile("times.xml");
	int numparticlesin = times.getNumTags("particlesin");
	cout << numparticlesin << "particles in" << endl;
	for(int i=0;i<numparticlesin;i++){
		timesParticlesin.push_back(times.getValue("particlesin",0,i));
	}
	int numparticlesout = times.getNumTags("particlesout");
	cout << numparticlesout << "particles out" << endl;
	for(int i=0;i<numparticlesout;i++){
		timesParticlesout.push_back(times.getValue("particlesout",0,i));
	}
	int nummappingin = times.getNumTags("mappingin");
	cout << nummappingin << "mapping in" << endl;
	for(int i=0;i<nummappingin;i++){
		timesMappingin.push_back(times.getValue("mappingin",0,i));
	}
	prevTime=0;
}

void Timming::update(){
	static bool eventArgs = true;
	float time = player->getPosition()*player->getDuration();
	if(triggerEvent(timesParticlesin,time,prevTime)){
		cout << "particlesin" << time << endl;
		ofNotifyEvent(particlesIn,eventArgs);
	}
	if(triggerEvent(timesParticlesout,time,prevTime)){
		cout << "particlesout" << time <<  endl;
		ofNotifyEvent(particlesOut,eventArgs);
	}
	if(triggerEvent(timesMappingin,time,prevTime)){
		cout << "mappingin" << time << endl;
		ofNotifyEvent(mappingIn,eventArgs);
	}
	prevTime=time;
}
