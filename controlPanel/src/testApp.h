#pragma once

#include "ofMain.h"
#include "ofxAvahiClient.h"
#include "OscPlayerClient.h"
#include "OscKinectServer.h"
#include "ofGui.h"
#include "OscContoursClient.h"

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

		vector<ofPtr<OscPlayerClient> > videoClients;
		vector<ofPtr<OscKinectServer> > kinectServers;
		ofxAvahiClientBrowser avahi;

		ofPanel gui;
		ofPanel guiMapping;
		ofToggle startMapping;

		OscContoursClient contours;
};
