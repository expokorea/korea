#pragma once

#include "ofMain.h"
//#include "ofxPCL.h"
#include "ofxKinect.h"
#include "ofxGui.h"

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
		
		void loadTemplate(int & templateN);


		ofxKinect kinect;
		ofMesh mesh1,mesh2,meshorig;
		ofVboMesh normalsMesh;
		ofEasyCam cam;
		ofxPanel gui;
		ofxParameter<bool> voxelFilter;
		ofxParameter<bool> removeDistantePoints;
		ofxParameter<bool> align;
		ofxParameter<bool> computeNormals;
		ofxParameter<bool> showNormals;
		ofxParameter<bool> computeFeatures;
		ofxParameter<bool> draworig;
		ofxParameter<bool> draw1;
		ofxParameter<bool> draw2;
		ofxParameter<float> maxDistance;
		ofxParameter<float> rot;
		ofxParameter<int> templates;
};
