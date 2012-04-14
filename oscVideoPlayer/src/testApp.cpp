#include "testApp.h"
#include <gst/net/gstnet.h>

static char clockname[256];

void testApp::initPipeline(){
	gstPlayer=ofPtr<ofGstVideoPlayer> (new ofGstVideoPlayer);
	gstPlayer->loadMovie(videoname);
	pipeline = gstPlayer->getGstVideoUtils()->getPipeline();
	if(netclock){
		GstElement * videosink = gstPlayer->getGstVideoUtils()->getSink();
		g_object_set(videosink,"async",0,NULL);
		g_object_set(videosink,"sync",1,NULL);
		g_object_set(videosink,"max-lateness",(gint64).08*GST_SECOND,NULL);
	}
	player.setPlayer(gstPlayer);
}

void testApp::resetPipeline(){
	player.setFrame(0);
	GstState state = GST_STATE_PAUSED;
	gst_element_get_state(pipeline,&state,NULL,2*GST_SECOND);
	player.setPaused(true);
	state = GST_STATE_PAUSED;
	gst_element_get_state(pipeline,&state,NULL,2*GST_SECOND);
}

void testApp::resetClock(){
	player.play();
	GstState state = GST_STATE_PLAYING;
	gst_element_get_state(pipeline,&state,NULL,2*GST_SECOND);
	if(clock) g_object_unref((GObject*)clock);
	clock = NULL;
	cout << "waiting clock" << endl;
	while(!GST_IS_CLOCK(clock)){
		clock = gst_net_client_clock_new (clockname, clockIp.c_str(),clockPort,0);
		ofSleepMillis(10);
	}
	cout <<"clock found" << endl;
	gst_pipeline_use_clock((GstPipeline*)pipeline,clock);
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	//ofSetEscapeQuitsApp(false);
	ofBackground(0);
	ofHideCursor();
	ofSetWindowTitle("player");
	ofAddListener(avahiClock.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahiClock.serviceRemoveE,this,&testApp::removedAvahiService);

	sprintf(clockname,"netclock");
	xml.loadFile("settings.xml");
	servicename = xml.getValue("settings:service","oscvideoplayer");
	videoname = xml.getValue("settings:videofile","video.mov");
	syncport = xml.getValue("settings:syncport",6667);
	netclock = xml.getValue("settings:netclock",0);


	clock = 0;
	serverGone = false;

	int port = ofRandom(7777,9999);
	osc.setup(syncport);

	if(netclock){
		initPipelineNextFrame = false;
		avahi.start(servicename,"_oscit._udp",port);
		avahiClock.lookup("_oscit._udp");
	}else{
		initPipelineNextFrame = true;
	}
}

//--------------------------------------------------------------
void testApp::update(){
	if(initPipelineNextFrame){
		initPipeline();
		if(netclock) resetClock();
		resetPipeline();
		initPipelineNextFrame = false;
	}
	if(!gstPlayer) return;
	while(osc.hasWaitingMessages()){
		ofxOscMessage msg;
		osc.getNextMessage(&msg);

		if(msg.getAddress()=="play" && player.isPaused()){
			if(netclock){
				GstClock * clock = gst_pipeline_get_clock((GstPipeline*)pipeline);
				GstClockTime clockTime = gst_clock_get_time((GstClock*)clock);
				GstClockTime  delay =  msg.getArgAsInt64(0) - clockTime;
				gst_pipeline_set_delay ((GstPipeline *)pipeline, delay);
			}
			player.play();
		}
		if(msg.getAddress()=="stop"){
			if(netclock){
				initPipeline();
				resetClock();
			}
			resetPipeline();
		}
	}
	if(serverGone){
		initPipeline();
		serverGone = false;
	}
	player.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	if(!gstPlayer) return;
	//if(!player.isPaused())
	player.draw(0,0);
	//GstClock * clock = gst_pipeline_get_clock((GstPipeline*)pipeline);
	//ofDrawBitmapString(ofToString(gst_clock_get_time((GstClock*)clock) + 200*GST_MSECOND),20,20);
}


void testApp::exit(){
	cout << "player stop" << endl;
	player.stop();
	cout << "player close" << endl;
	player.close();
	cout << "player reset" << endl;
	if(gstPlayer) gstPlayer.reset();
	cout << "player exit" << endl;
	if(netclock){
		avahi.close();
		avahiClock.close();
	}
}

void testApp::newAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "new service" << service.name << "in" << service.host_name << service.ip << ":" << service.port;
	if(service.name.find("clock"+servicename)==0){
		clockIp = service.ip;
		clockPort = service.port;
		initPipelineNextFrame = true;
		/*if(!clock){
			mutex.lock();
			condition.signal();
			mutex.unlock();
		}*/
	}
}

void testApp::removedAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "removing service" << service.name << "in" << service.host_name << service.ip << ":" << service.port;
	if(service.name.find("clock"+servicename)==0){
		serverGone = true;
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	 if(key==' '){
		 gchar *name;
			GstClock * clock = gst_pipeline_get_clock((GstPipeline*)pipeline);
		 g_object_get((GObject*)clock,"name",&name,NULL);
		 cout << name << endl;
	 }
	/*if(key==OF_KEY_ESC){
		std::exit(0);
	}*/
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
