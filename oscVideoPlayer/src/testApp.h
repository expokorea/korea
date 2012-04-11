#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxAvahiClient.h"
#include "ofxXmlSettings.h"
#include "Poco/Condition.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void exit();

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

		void resetPipeline();
		void resetClock();
		void initPipeline();
		
		enum State{
			Init,
			WaitServer,
			Stopped,
			ServerGone,
			ServerGoneClearScreen,
			Playing
		}state;

		ofxOscReceiver osc;
		ofVideoPlayer player;
		ofxAvahiClientService avahi;
		ofxAvahiClientBrowser avahiClock;
		ofxXmlSettings xml;
		string servicename;
		GstElement * pipeline;
		GstClock * clock, *netClock;
		ofPtr<ofGstVideoPlayer> gstPlayer;
		Poco::Condition condition;
		ofMutex mutex;
		string videoname;
		bool serverGone;
		int clockPort;
		string clockIp;
		bool resetClockNextFrame;
		bool initPipelineNextFrame;
		int syncport;
		bool netclock;
};
