#pragma once

#include "ofMain.h"
#include "Glow.h"
#include "particleSystem.h"
#include "ParticleSystemDemo.h"
#include "KoreaFlock.h"
#include "userData.h"

#ifdef TARGET_OSX
#include "ofxQtVideoSaver.h"
#else
#include "ofxVideoRecorder.h"
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
		
		void lightOnChanged(bool & l);
		void recordPressed(bool & l);

		void particleRGBChanged(int & rgb);
		void particleTexRGBChanged(int & rgb);
		void particleLinesRGBChanged(int & rgb);
		void particleTexModeChanged(bool & mode);


		ofShader shader, shaderBokeh;
		ofFbo fbo1, fbo2;
		ofxPanel gui;
		ofxFloatSlider brightness;
		ofxIntSlider passes;
		ofxFloatSlider framerate;
		ofxToggle lightOn;
		ofxToggle drawGlow;
		ofxFloatSlider lightCutoff, lightExponent;
		ofxFloatSlider lightConstant, lightLinear, lightQuad;
		ofxToggle particlesTexMode;

		ofLight light;
		Glow glow;
		ParticleSystem pSystem;
		ParticleSystemDemo pSystemDemo;
		
		KoreaFlock kSystem;
		
		ofEasyCam cam;
		bool bShowGui;
		
		
		KUserData user1;
		vector<KUserData> users;
		
		ofPixels pixRecord;
		
		
		ofFbo fbo;
		ofxToggle record;


#ifdef TARGET_OSX
		ofxQtVideoSaver movieSaver;
#else
		ofxVideoRecorder recorder;
#endif
};
