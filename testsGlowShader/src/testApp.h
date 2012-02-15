#pragma once

#include "ofMain.h"
#include "ofGui.h"

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
		

		ofShader shader, shaderBokeh;
		ofFbo fbo1, fbo2;
		ofPanel gui;
		ofFloatSlider blurAmnt;
		ofFloatSlider brightness;
		ofIntSlider kernelSize;
		ofFloatSlider framerate;
		ofIntSlider r,g,b;
};
