#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Glow.h"
#include "particleSystem.h"
#include "ParticleSystemDemo.h"
#include "ofxVideoRecorder.h"
#include "KoreaFlock.h"

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
		void initCameraMovementPressed(bool & m);
		void demoPressed(bool & m);


		ofShader shader, shaderBokeh;
		ofFbo fbo;
		ofxPanel gui;
		ofxFloatSlider brightness;
		ofxIntSlider passes;
		ofxFloatSlider framerate;
		ofxToggle lightOn;
		ofxToggle demo;
		ofxFloatSlider lightX, lightY, lightZ;
		ofxFloatSlider posCameraPct,lookAtPct;
		ofxToggle drawNotBlurred;
		ofxToggle record;
		ofxToggle useCamera;
		ofxButton initCameraMovement;

		ofLight light;
		Glow glow;
		ofShader glowMultiply;
		ParticleSystem pSystem;
		ParticleSystemDemo pSystemDemo;

		ofxVideoRecorder recorder;
		ofPixels pixRecord;

		bool hideGui;
		
		KoreaFlock kSystem;
		ofCamera camera;
		ofPoint prevCameraPos;
		ofNode lookAt;
		int timeInitCamMovement;
};
