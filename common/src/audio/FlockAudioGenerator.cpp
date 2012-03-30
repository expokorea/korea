/*
 * FlockAudioGenerator.cpp
 *
 *  Created on: 21/03/2012
 *      Author: arturo
 */

#include "FlockAudioGenerator.h"
#include "float.h"

double envelope[] = {0,0,1,25,0,50};

FlockAudioGenerator::FlockAudioGenerator() {
	// TODO Auto-generated constructor stub

}

FlockAudioGenerator::~FlockAudioGenerator() {
	// TODO Auto-generated destructor stub
}

void FlockAudioGenerator::setup(){
	sampleRate 			= 44100; /* Sampling Rate */
	initialBufferSize	= 256;	/* Buffer Size. you have to fill this buffer with sound*/
	//lAudio.resize(initialBufferSize,0);/* outputs */
	//rAudio.resize(initialBufferSize,0);/* outputs */

	minFreq.set("min freq.",40,0,1000);
	maxFreq.set("max freq.",400,0,2000);
	resonance.set("resonance.",9,0,100);
	freq = minFreq;
	lamp=ramp=0;
	randomSineFreq = ofRandom(1);
	randomSineAmp = ofRandom(1);
	tickCount = 0;
	targetSineAmp = 0;
	sinefreq=0;
	sineamp=0;
	envelopeBeep.amplitude=0;
}

void FlockAudioGenerator::process(){
	//short * output = &buffer[0];
	float tempf,sinef=0;
	int bufferSize = buffer.size();
	/*if(triggerBeep){
		targetSineAmp = ofNoise(float(tickCount*bufferSize)/float(bufferSize*20),randomSineAmp);
		targetSineFreq = ofNoise(float(tickCount*bufferSize)/float(bufferSize*200),randomSineFreq)*1000+1000;
		sampleStartTrigger = tickCount*bufferSize;
		triggerDuration = float(maxiSettings::sampleRate) * ofRandom(.2,1.5);
		sinefreq=1000;
		triggerBeep = false;
		beepTriggered = true;

	}*/
	for (int i = 0; i < bufferSize; i++){
		freq = freq*.99 + ofMap(freqIn,0,1,minFreq,maxFreq,true)*.01;
		sinefreq = sinefreq*.99 + targetSineFreq*.01;

		tempf=mySwitchableOsc.noise();

		tempf=myFilter.lores(myFilter.hires(tempf,freq,resonance),freq,resonance) * amp;
		//sinef = sineOsc.sinewave(sinefreq) *	envelopeBeep.line(6,envelope);
		/*if(beepTriggered){
			sinef = sineOsc.saw(sinefreq) * sineamp;
			if(targetSineFreq-sinefreq<FLT_EPSILON){
				targetSineAmp = 0;
				sampleStartTrigger = tickCount*bufferSize+i;
			}else{
				sinefreq = ofMap(tickCount*bufferSize+i,sampleStartTrigger,sampleStartTrigger+triggerDuration,1000,targetSineFreq);
			}
			sineamp = ofMap(tickCount*bufferSize+i,sampleStartTrigger,sampleStartTrigger+triggerDuration,0,targetSineAmp);
			if(sineamp<FLT_EPSILON){
				beepTriggered=false;
			}
		}*/
		buffer[i] = ((tempf) * 32767.5f) -.5f;
	}
	tickCount++;
}

void  FlockAudioGenerator::update(){
	/*float trigger = ofNoise(ofGetElapsedTimef()/100.,randomSineAmp);
	if((trigger>.7 && prevTrigger<=.7) || (trigger<.3 && prevTrigger>=.3) || (trigger<.2 && prevTrigger>=.2) || (trigger<.8 && prevTrigger>=.8)){
		triggerBeep = true;
		cout << "trigger" << endl;
		//targetSineFreq = ofNoise(float(tickCount*buffer.size())/float(buffer.size()*200),randomSineFreq)*1000+1000;
		//envelopeBeep.trigger(0,0);
	}
	prevTrigger = trigger;
	if(envelopeBeep.amplitude>0){
		cout << envelopeBeep.amplitude << endl;
	}*/
}

//--------------------------------------------------------------
void FlockAudioGenerator::audioOut 	(float * output, int bufferSize, int nChannels, int deviceID, long unsigned long tickCount  ){
	//long unsigned long time = ofGetElapsedTimeMicros();
	float tempf;
	freq = freq*.9 + ofMap(freqIn,0,1,minFreq,maxFreq,true)*.1;
	for (int i = 0; i < bufferSize; i++){

		//CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.

		//if (CurrentCount<5)//simple if statement

		tempf=mySwitchableOsc.noise() * .8;


		/*else if (CurrentCount>=5)//and the 'else' bit.

			myOscOutput=mySwitchableOsc.noise();//one osc object can produce whichever waveform you want.*/


		//	maxFreq = myEnvelope.line(6, myEnvelopeData);

		tempf=myFilter.lores(tempf,freq,resonance);//lores takes an audio input, a frequency and a resonance factor (1-100)
		//ofMap(ofNoise((double(tickCount)*double(bufferSize)+double(i))/44100.*noiseFreq)

		//myFilteredOutput=compressor.compressor(myFilteredOutput,ratio,threshold,attack,release);

		//myFilteredOutput=chorus.chorus(myFilteredOutput,800,.5,1,.7);

		//outputs[0]=outputs[1]=myFilteredOutput;//point me at your speakers and fire.


		*output++ += tempf* lamp; /* You may end up with lots of outputs. add them here */
		*output++ += tempf* ramp;
	}

	//reverb->processStereo (&lAudio[0], &rAudio[0], bufferSize);
	/*if(activateCompressor){
		for(int i=0;i<bufferSize;i++){
			output[i*2] += compressor.compressor(lAudio[i],ratio,threshold,attack,release);
			output[i*2+1] += compressor.compressor(rAudio[i],ratio,threshold,attack,release);
		}

	}else{*/
		/*for(int i=0;i<bufferSize;i++){
			output[i*2] += lAudio[i] * lamp;
			output[i*2+1] += rAudio[i] * ramp;
		}*/
	//}
	//juceFilter.makeBandPass(44100,ofMap(ofNoise((double(tickCount)*double(bufferSize))/44100.*noiseFreq),0,1,minFreq,maxFreq),resonance,2);
	//juceFilter.processSamples(output,bufferSize*nChannels);
	//time = ofGetElapsedTimeMicros() - time;
	//cout << time << endl;
}
