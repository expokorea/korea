/*
 * FlockAudioGenerator.h
 *
 *  Created on: 21/03/2012
 *      Author: arturo
 */

#pragma once

#include "ofConstants.h"
#include "maximilian.h"
#include "ofxGui.h"

class FlockAudioGenerator {
public:
	FlockAudioGenerator();
	virtual ~FlockAudioGenerator();

	void setup();

	void audioOut 	( float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount  ); /* output method */
	void process();
	void update();

	void paramsChanged(float & p);

	vector<short>  buffer;

	vector<unsigned int> bufferIDs;
	unsigned int sourceID;

	int		initialBufferSize; /* buffer size */
	int		sampleRate;

	static ofxParameter<float> minFreq, maxFreq, noiseFreq, resonance,globalAmp;
	ofxParameter<float> freqIn;
	float lamp,ramp,amp;
	ofVec3f pos;
	/* stick you maximilian stuff below */
private:
	//double wave,sample,outputs[2];
	maxiOsc mySwitchableOsc,sineOsc;//
	maxiEnvelope envelopeBeep;
	float randomSineFreq,randomSineAmp;
	int tickCount;
	float thisMinFreq,thisMaxFreq,thisResonance;
	//int CurrentCount;//
	//float myOscOutput,myFilteredOutput;//
	//maxiEnvelope myEnvelope;
	maxiFilter myFilter;
	float freq;
	float prevTrigger;
	bool triggerBeep,beepTriggered;
	float targetSineAmp, targetSineFreq,sinefreq,sineamp;
	int sampleStartTrigger, triggerDuration;
};

