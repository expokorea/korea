/*
 * SoundManager.h
 *
 *  Created on: 22/03/2012
 *      Author: arturo
 */

#pragma once

#include "PSystem.h"
#include "Poco/Condition.h"
#include "AL/alc.h"

class SoundManager: public ofThread {
public:
	SoundManager();
	virtual ~SoundManager();

	void setup(vector<PSystem> & flock);
	void update();
	void createEffect(int effect);

private:
	void threadedFunction();
	vector<PSystem> * flock;
	static ALCdevice * alDevice;
	static ALCcontext * alContext;
	unsigned int  uiEffectSlot;
	unsigned int  uiEffect;
	Poco::Condition started;

	class SourcesUpdater: public ofThread{
	public:
		void setup(vector<PSystem> & flock);
		void threadedFunction();

	private:
		vector<PSystem> * flock;
	} soundUpdater;
};

