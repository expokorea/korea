/*
 * SoundManager.cpp
 *
 *  Created on: 22/03/2012
 *      Author: arturo
 */

#include "SoundManager.h"
#include "ofConstants.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>



ALCdevice * SoundManager::alDevice = 0;
ALCcontext * SoundManager::alContext = 0;

static ALvoid (AL_APIENTRY *p_alGenFilters)(ALsizei,ALuint*);
static ALvoid (AL_APIENTRY *p_alDeleteFilters)(ALsizei,ALuint*);
static ALvoid (AL_APIENTRY *p_alFilteri)(ALuint,ALenum,ALint);
static ALvoid (AL_APIENTRY *p_alGenEffects)(ALsizei,ALuint*);
static ALvoid (AL_APIENTRY *p_alGenAuxiliaryEffectSlots)(ALsizei,ALuint*);
static ALvoid (AL_APIENTRY *p_alDeleteAuxiliaryEffectSlots)(ALsizei,ALuint*);
static ALvoid (AL_APIENTRY *p_alAuxiliaryEffectSloti)(ALuint,ALenum,ALint);
static ALvoid (AL_APIENTRY *p_alAuxiliaryEffectSlotf)(ALuint,ALenum,ALfloat);
static ALvoid (AL_APIENTRY *p_alDeleteEffects)(ALsizei,ALuint*);
static ALvoid (AL_APIENTRY *p_alEffecti)(ALuint,ALenum,ALint);
static ALvoid (AL_APIENTRY *p_alEffectf)(ALuint,ALenum,ALfloat);


SoundManager::SoundManager() {
	flock = NULL;
}

SoundManager::~SoundManager() {
	// TODO Auto-generated destructor stub
}

void SoundManager::setup(KoreaFlock & flock){
	if(!alDevice){
		if (alGetString(AL_VERSION))
			cout << "OpenAL version: "    << alGetString(AL_VERSION)    << endl;
		if (alGetString(AL_RENDERER))
			cout << "OpenAL renderer: "   << alGetString(AL_RENDERER)   << endl;
		if (alGetString(AL_VENDOR))
			cout << "OpenAL vendor: "     << alGetString(AL_VENDOR)     << endl;
		if (alGetString(ALC_EXTENSIONS))
			cout << "OpenAL extensions: " << alGetString(ALC_EXTENSIONS) << endl;
		if(alcIsExtensionPresent(NULL,"ALC_ENUMERATION_EXT")){
			const char *s = (const char *) alcGetString(NULL, ALC_DEVICE_SPECIFIER);
			while (*s != '\0')
			{
				cout << "OpenAL available device: " << s << endl;
				while (*s++ != '\0');
			};
		}
		// Print default device name
		cout << "OpenAL default device: "
		     << (const char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER)
			 << endl;

		alDevice = alcOpenDevice("ALSA Default"	);

		//maxiSettings::sampleRate = 156000;
		//int samplerate;
		alcGetIntegerv(alDevice, ALC_FREQUENCY, 1, &maxiSettings::sampleRate);
		cout << "OpenAL device samplerate: "
		     << maxiSettings::sampleRate
			 << endl;

		alContext = alcCreateContext(alDevice,NULL);
		alcMakeContextCurrent (alContext);
		alListener3f(AL_POSITION, 0,0,300);
		alListenerf(AL_METERS_PER_UNIT,.005);

		if (alcIsExtensionPresent(alDevice, "ALC_EXT_EFX") == ALC_FALSE)
		{
			ofLog(OF_LOG_ERROR,"EFX extention is not found");
		}

		p_alGenFilters = (ALvoid (*)(ALsizei,ALuint*)) alGetProcAddress("alGenFilters");
		p_alDeleteFilters = (ALvoid (*)(ALsizei,ALuint*)) alGetProcAddress("alDeleteFilters");
		p_alFilteri = (ALvoid (*)(ALuint,ALenum,ALint)) alGetProcAddress("alFilteri");
		p_alGenEffects = (ALvoid (*)(ALsizei,ALuint*)) alGetProcAddress("alGenEffects");
		p_alGenAuxiliaryEffectSlots = (ALvoid (*)(ALsizei,ALuint*)) alGetProcAddress("alGenAuxiliaryEffectSlots");
		p_alDeleteAuxiliaryEffectSlots = (ALvoid (*)(ALsizei,ALuint*)) alGetProcAddress("alDeleteAuxiliaryEffectSlots");
		p_alAuxiliaryEffectSloti = (ALvoid (*)(ALuint,ALenum,ALint)) alGetProcAddress("alAuxiliaryEffectSloti");
		p_alAuxiliaryEffectSlotf = (ALvoid (*)(ALuint,ALenum,ALfloat)) alGetProcAddress("alAuxiliaryEffectSlotf");
		p_alDeleteEffects = (ALvoid (*)(ALsizei,ALuint*)) alGetProcAddress("alDeleteEffects");
		p_alEffecti = (ALvoid (*)(ALuint,ALenum, ALint)) alGetProcAddress("alEffecti");
		p_alEffectf = (ALvoid (*)(ALuint,ALenum, ALfloat)) alGetProcAddress("alEffectf");
	}



	//ofLogNotice() << "creating reverb";
	//createEffect(AL_EFFECT_REVERB);
	for(int i=0;i<flock.particles.size();i++){
		flock.particles[i].audioGenerator.bufferIDs.resize(4);
		alGenBuffers(flock.particles[i].audioGenerator.bufferIDs.size(),&flock.particles[i].audioGenerator.bufferIDs[0]);
		alGenSources(1,&flock.particles[i].audioGenerator.sourceID);


		alSourcef (flock.particles[i].audioGenerator.sourceID, AL_PITCH,    1.0f);
		alSourcef (flock.particles[i].audioGenerator.sourceID, AL_GAIN,     1.0f);
	    alSourcef (flock.particles[i].audioGenerator.sourceID, AL_ROLLOFF_FACTOR,  .01);
	    alSourcei (flock.particles[i].audioGenerator.sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
	    //alSourcef (flock.particles[i].audioGenerator.sourceID, AL_REFERENCE_DISTANCE,  5);
	    alSourcei (flock.particles[i].audioGenerator.sourceID, AL_DISTANCE_MODEL, AL_INVERSE_DISTANCE);

		alSourcefv(flock.particles[i].audioGenerator.sourceID, AL_POSITION,(flock.particles[i].pos-ofVec3f(0,0,300)).getPtr());

		flock.particles[i].audioGenerator.buffer.resize(1024);
		for(int j=0;j<flock.particles[i].audioGenerator.bufferIDs.size();j++){
			flock.particles[i].audioGenerator.process();
			alBufferData(flock.particles[i].audioGenerator.bufferIDs[j],AL_FORMAT_MONO16,&flock.particles[i].audioGenerator.buffer[0],flock.particles[i].audioGenerator.buffer.size()*2,maxiSettings::sampleRate);
			ALenum err = alGetError();
			if ( err != AL_NO_ERROR){
				ofLogError()<< "ofOpenALSoundPlayer: error creating buffer " << err;
				//return;
			}
		}
		alSourceQueueBuffers(flock.particles[i].audioGenerator.sourceID,flock.particles[i].audioGenerator.bufferIDs.size(),&flock.particles[i].audioGenerator.bufferIDs[0]);

		//alSource3i(flock.particles[i].audioGenerator.sourceID, AL_AUXILIARY_SEND_FILTER, uiEffectSlot, 0, AL_FILTER_NULL);
		alSourcePlayv(1,&flock.particles[i].audioGenerator.sourceID);
	}


	/*ofLogNotice() << "creating equalizer";
	createEffect(AL_EFFECT_ECHO);
	ofLogNotice() << "creating compressor";
	createEffect(AL_EFFECT_COMPRESSOR);*/
	/*Effect fx2;
    fx2.addEffect(AL_EFFECT_ECHO);
    fx2.setEffectGain(1.0f);
    gunshot.assignEffect(fx2.getEffect(),fx2.getEffectID());*/

	this->flock = &flock;
	startThread(true,false);
}

void SoundManager::createEffect(int effect){
	// Generate an Auxiliary Effect Slot
	p_alGenAuxiliaryEffectSlots(1, &uiEffectSlot);

	// The EFX Extension includes support for global effects, such as Reverb.  To use a global effect,
	// you need to create an Auxiliary Effect Slot to store the Effect ...
	if (alGetError()==AL_NO_ERROR)
	{
			// Once we have an Auxiliary Effect Slot, we can generate an Effect Object, set its Type
			// and Parameter Values, and then load the Effect into the Auxiliary Effect Slot ...
			p_alGenEffects(1, &uiEffect);
			if (alGetError() == AL_NO_ERROR)
			{
					// Set the Effect Type
					p_alEffecti(uiEffect, AL_EFFECT_TYPE, effect);
					if (alGetError() != AL_NO_ERROR)
						ofLog(OF_LOG_ERROR,"Failed to generate Effect");
					else
						p_alAuxiliaryEffectSloti(uiEffectSlot, AL_EFFECTSLOT_EFFECT, uiEffect);
			}

	}
	else
	{
			ofLog(OF_LOG_ERROR,"Failed to generate an Auxilary Effect Slot");
	}
}

void SoundManager::update(){

	for(int i=0;i<flock->particles.size();i++){
		alSourcefv(flock->particles[i].audioGenerator.sourceID,AL_POSITION,(flock->particles[i].pos-ofVec3f(0,0,300)).getPtr());
		flock->particles[i].audioGenerator.update();
	}
}

void SoundManager::threadedFunction(){
	while(isThreadRunning()){
		for(int i=0; i<flock->particles.size(); i++){
			int processed;
			alGetSourcei(flock->particles[i].audioGenerator.sourceID, AL_BUFFERS_PROCESSED, &processed);
			if(processed<=0) continue;
			while(processed)
			{
				flock->particles[i].audioGenerator.process();
				int numFrames = flock->particles[i].audioGenerator.buffer.size();
				ALuint albuffer;
				alSourceUnqueueBuffers(flock->particles[i].audioGenerator.sourceID, 1, &albuffer);
				alBufferData(albuffer,AL_FORMAT_MONO16,&flock->particles[i].audioGenerator.buffer[0],numFrames*2,maxiSettings::sampleRate);
				alSourceQueueBuffers(flock->particles[i].audioGenerator.sourceID, 1, &albuffer);
				ALenum err = alGetError();
				if ( err != AL_NO_ERROR){
                    ofLogError() << "Error buffering " << err;
                }
				processed--;
			}
			ALint state;
			alGetSourcei(flock->particles[i].audioGenerator.sourceID,AL_SOURCE_STATE,&state);
			if(state != AL_PLAYING){
				alSourcePlayv(1,&flock->particles[i].audioGenerator.sourceID);
				cout << "underrun" << endl;
				//cout << flock->particles[i].audioGenerator.sourceID << " playing" << endl;
			}


		}
		//ofSleepMillis(1);
	}
}

