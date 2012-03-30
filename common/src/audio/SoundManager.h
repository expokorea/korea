/*
 * SoundManager.h
 *
 *  Created on: 22/03/2012
 *      Author: arturo
 */

#pragma once

#include "PSystem.h"
#include "Poco/Condition.h"

class SoundManager: public ofThread {
public:
	SoundManager();
	virtual ~SoundManager();

	void setup(PSystem & flock);
	void update();
	void createEffect(int effect);

private:
	void threadedFunction();
	PSystem * flock;
	static ALCdevice * alDevice;
	static ALCcontext * alContext;
	unsigned int  uiEffectSlot;
	unsigned int  uiEffect;
	Poco::Condition started;

	class SourcesUpdater: public ofThread{
	public:
		void setup(PSystem & flock);
		void threadedFunction();

	private:
		PSystem * flock;
	} soundUpdater;
};

