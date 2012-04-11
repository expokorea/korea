#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxAvahiClient.h"
#include "OscPlayerClient.h"
#include "ofxGui.h"
#include "ofxOsc.h"
#include <gst/net/gstnet.h>

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
		void movieEOS(ofEventArgs & eos);

		void initPipeline();
		void resetPipeline();
		void resetClock();

		ofxXmlSettings xml;
		ofVideoPlayer player;
		vector<ofPtr<OscPlayerClient> > videoClients;
		ofxOscSender oscSender;
		ofxOscMessage stopMsg,playMsg;
		ofxAvahiClientBrowser avahi;
		ofxAvahiClientService avahiClock;
		ofMutex mutex;
		int playerFinishedMs;
		int millistostartloop;
		string servicename;
		GstClock* clock;
		GstElement * pipeline;
		GstNetTimeProvider * netClock;
		bool restartPipelineNextFrame;
		string videoname;
		ofPtr<ofGstVideoPlayer> gstPlayer;
		int syncport, clockport;
};
