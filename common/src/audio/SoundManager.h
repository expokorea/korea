/*
 * SoundManager.h
 *
 *  Created on: 22/03/2012
 *      Author: arturo
 */

#pragma once

#include "KoreaFlock.h"

class SoundManager: public ofThread {
public:
	SoundManager();
	virtual ~SoundManager();

	void setup(KoreaFlock & flock);
	void update();
	void createEffect(int effect);

	void threadedFunction();
private:
	KoreaFlock * flock;
	static ALCdevice * alDevice;
	static ALCcontext * alContext;
	unsigned int  uiEffectSlot;
	unsigned int  uiEffect;
};

