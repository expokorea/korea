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
		if(times[i]<time && times[i]>=previousTime){
			return true;
		}
	}
	return false;
}

int triggerEvent(vector<Timming::ParticlesOutEvent> & times,float time, float previousTime){
	for(int i=0;i<times.size();i++){
		if(times[i]<time && times[i]>=previousTime){
			return times[i].target;
		}
	}
	return 0;
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
		timesParticlesout.push_back(ParticlesOutEvent(times.getValue("particlesout",0,i),times.getAttribute("particlesout","target",1,i)));
	}
	int nummappingin = times.getNumTags("mappingin");
	cout << nummappingin << "mapping in" << endl;
	for(int i=0;i<nummappingin;i++){
		timesMappingin.push_back(times.getValue("mappingin",0,i));
	}
	int nummappingout = times.getNumTags("mappingout");
	cout << nummappingout << "mapping in" << endl;
	for(int i=0;i<nummappingout;i++){
		timesMappingout.push_back(times.getValue("mappingout",0,i));
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
	int particlesOutTarget=triggerEvent(timesParticlesout,time,prevTime);
	if(particlesOutTarget){
		cout << "particlesout" << time <<  endl;
		ofNotifyEvent(particlesOut,particlesOutTarget);
	}
	if(triggerEvent(timesMappingin,time,prevTime)){
		cout << "mappingin" << time << endl;
		ofNotifyEvent(mappingIn,eventArgs);
	}
	if(triggerEvent(timesMappingout,time,prevTime)){
		cout << "mappingout" << time << endl;
		ofNotifyEvent(mappingOut,eventArgs);
	}
	prevTime=time;
}
