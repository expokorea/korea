#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Glow.h"
#include "particleSystem.h"
#include "ParticleSystemDemo.h"
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


		ofShader shader, shaderBokeh;
		ofFbo fbo1, fbo2;
		ofxPanel gui;
		ofxFloatSlider brightness;
		ofxIntSlider passes;
		ofxFloatSlider framerate;
		ofxToggle lightOn;
		ofxToggle drawGlow;

		ofLight light;
		Glow glow;
		ParticleSystem pSystem;
		ParticleSystemDemo pSystemDemo;
		
		KoreaFlock kSystem;
		
		ofEasyCam cam;
		bool bShowGui;
};
