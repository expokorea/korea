#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "ofxAvahiClient.h"
#include "OscContourServer.h"
#include "ofxVideoRecorder.h"
#include "ofxKinectSequencePlayer.h"
#include "ofxGui.h"
#include "ofxPlane.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void drawPointCloud();

		template<class Kinect>
		void updateAnalisys(Kinect & kinect);

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		

		void newAvahiService(ofxAvahiService & service);
		void removedAvahiService(ofxAvahiService & service);

		void tiltChanged(int & tilt);
		void captureBgPressed(bool & pressed);
		void rotZeroPressed(bool & pressed);

		void drawScene(float xRot,bool drawBB);

		ofxKinect kinect;
		ofxKinectSequencePlayer player;
		vector<ofPolyline> polylines;
		vector<ofPtr<OscBlobServer> > oscContours;
		ofxOscReceiver oscConfig;
		ofxAvahiClientService avahi;
		ofxAvahiClientBrowser avahiBrowser;

		ofxCv::ContourFinder contourFinder;

		/*cv::BackgroundSubtractorMOG bgSubstractor;*/
		int frame;

		ofxParameter<int> tilt;
		ofxParameter<float> farThreshold;
		ofxParameter<float> nearThreshold;
		ofxParameter<float> bbW, bbH, bbD, bbX, bbY, bbZ;
		ofxParameter<bool> correctAngle;
		ofxParameter<bool> usePlayer;
		ofxParameter<bool> paused;
		ofxParameter<bool> ortho;
		ofxParameter<float> pitch, roll, yaw;
		ofxParameter<float> cameraX, cameraY, cameraZ;
		ofxParameter<int> vCameraTilt;
		ofxParameter<float> maxX, maxY, maxZ;
		ofxParameter<float> minX, minY, minZ;
		ofxParameter<bool> useFbo;
		ofxParameter<bool> applyBB;
		ofxParameter<int> gpuFilterPasses;
		ofxParameter<float> positionFilter, sizeFilter;
		ofxParameter<float> rollZero, tiltZero;
		ofxParameter<bool> drawViewports;
		ofxParameter<int> mainViewport;
		ofxParameter<int> fps;
		ofxButton captureBgBtn, rotZeroBtn;
		ofxPanel gui;



		ofPixels nearThresPix, farThresPix, rgbDepth;
		ofPixels background, diff, gaussCurrent;
		ofPixels thres8Bit;
		ofTexture texThres, bgTex;


		ofxVideoRecorder recorder;
		bool recording;

		bool captureBg;

		ofCamera camFront,camTop,camLeft,camTransform;
		ofVboMesh mesh;
		ofxPlane plane;
		bool planeFound;

		ofFbo fbo, fbo2, fboViewportFront, fboViewportTop, fboViewportLeft;


		struct Blob{
			ofPoint pos;
			float size;
		};
		ofxCv::Tracker<Blob> tracker;
		vector<Blob> blobs;

};
