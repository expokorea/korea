#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Glow.h"
#include "PSystem.h"
#include "ofxVideoRecorder.h"

//#define USE_AUDIO

#ifdef USE_AUDIO
#include "SoundManager.h"
#endif

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void record(bool & record);


		ofxPanel gui;

		PSystem particles;
		ofxParameter<float> bbW,bbH,bbD,bbZ;
		ofCamera cam;
		Glow glow;

		float prevSpeedFactor;

		ofxParameter<float> fps;

		ofxVideoRecorder recorder;
		ofFbo fbo;
		ofxParameter<bool> recording;
		ofPixels pixels;

#ifdef USE_AUDIO
		SoundManager soundManager;
#endif
};
