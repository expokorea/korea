#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "OscContourServer.h"
#include "ofxVideoRecorder.h"
#include "ofxGui.h"
#include "ofxPlane.h"
#include "KinectController.h"

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
		

		void captureBgPressed(bool & pressed);

		void newAvahiService(ofxAvahiService & service);
		void removedAvahiService(ofxAvahiService & service);

		/*ofxKinectSequencePlayer player;
		ofxVideoRecorder recorder;
		bool recording;*/

		vector<KinectController*> kinects;

		vector<ofPolyline> polylines;
		vector<ofPtr<OscBlobServer> > oscContours;
		ofxAvahiClientBrowser avahiBrowser;

		ofxCv::ContourFinder contourFinder;

		/*cv::BackgroundSubtractorMOG bgSubstractor;*/
		int frame;
		ofxParameter<float> bbW, bbH, bbD, bbX, bbY, bbZ;
		ofxParameter<bool> ortho;
		ofxParameter<int> gpuFilterPasses;
		ofxParameter<float> positionFilter, sizeFilter;
		ofxParameter<float> cameraX, cameraY, cameraZ;
		ofxParameter<int> vCameraTilt;
		ofxParameter<bool> drawViewports;
		ofxParameter<bool> drawRGB;
		ofxParameter<bool> drawDepth;
		ofxParameter<bool> drawFbo;
		ofxParameter<bool> drawContours;
		ofxParameter<int> mainViewport;
		ofxParameter<bool> useFbo;
		ofxParameter<int> fps;
		ofxParameter<int> servernum;
		ofxButton captureBgBtn;
		ofxPanel gui;

		ofPixels nearThresPix, farThresPix, rgbDepth;
		ofPixels background, diff, gaussCurrent;
		ofPixels thres8Bit;
		ofTexture texThres, bgTex;

		bool captureBg;

		ofCamera camFront,camTop,camLeft;

		ofFbo fbo, fbo2, fboViewportFront, fboViewportTop, fboViewportLeft;


		struct Blob{
			ofPoint pos;
			float size;
			int index;
			int getIndex(){return index;}
		};
		ofxCv::Tracker<Blob> tracker;
		vector<Blob> blobs;

		ofShader bb3dShader;
		bool isFrameNew;

};
