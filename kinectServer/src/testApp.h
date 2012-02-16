#pragma once

#include "ofMain.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "ofxOsc.h"
#include "ofxAvahiClient.h"
#include "OscContourServer.h"

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
		

		void newAvahiService(ofxAvahiService & service);
		void removedAvahiService(ofxAvahiService & service);

		ofxKinect kinect;
		ContourFinder contourFinder;
		vector<ofPolyline> polylines;
		vector<ofPtr<OscContourServer> > oscContours;
		ofxOscReceiver oscConfig;
		ofxAvahiClientService avahi;
		ofxAvahiClientBrowser avahiBrowser;
		int frame;

		int tilt;
		int farThreshold;
		int nearThreshold;

		ofPixels nearThresPix, farThresPix;
		ofImage thresPix;
};
