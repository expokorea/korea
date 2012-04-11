#include "testApp.h"

void testApp::initPipeline(){
	gstPlayer=ofPtr<ofGstVideoPlayer> (new ofGstVideoPlayer);
	gstPlayer->loadMovie(videoname);
	pipeline = gstPlayer->getGstVideoUtils()->getPipeline();
	GstElement * videosink = gstPlayer->getGstVideoUtils()->getSink();
	g_object_set(videosink,"async",0,NULL);
	g_object_set(videosink,"sync",1,NULL);
	g_object_set(videosink,"max-lateness",(gint64).08*GST_SECOND,NULL);

	ofAddListener(gstPlayer->getGstVideoUtils()->eosEvent,this,&testApp::movieEOS);
	player.setPlayer(gstPlayer);
}

void testApp::resetPipeline(){
	if(!gstPlayer) return;
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
	clock = gst_element_get_clock(pipeline);
	if(netClock) g_object_unref((GObject*)clock);
	netClock = gst_net_time_provider_new(clock,NULL,clockport);
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetEscapeQuitsApp(false);
	//ofSetLogLevel(OF_LOG_VERBOSE);
	ofBackground(0);
	ofHideCursor();
	ofSetWindowTitle("master");
	ofAddListener(avahi.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahi.serviceRemoveE,this,&testApp::removedAvahiService);

	xml.loadFile("settings.xml");
	servicename = xml.getValue("settings:service","oscvideoplayer");
	videoname = xml.getValue("settings:videofile","video.mov");
	cout << videoname << endl;
	millistostartloop = xml.getValue("settings:millistostartloop",2000);
	clockport = xml.getValue("settings:clockport",9999);
	syncport = xml.getValue("settings:syncport",6667);
	string broadcast = xml.getValue("settings:broadcast","192.168.2.255");
	cout << servicename << endl;


	stopMsg.setAddress("stop");
	playMsg.setAddress("play");

	oscSender.setup(broadcast,syncport);

	clock = NULL;
	netClock = NULL;

	restartPipelineNextFrame = true;

	avahiClock.start("clock"+servicename,"_oscit._udp",clockport);
	avahi.lookup("_oscit._udp");

	// wait 1 second for existing clients
	ofSleepMillis(1000);

}

//--------------------------------------------------------------
void testApp::update(){
	if(restartPipelineNextFrame){
		initPipeline();
		resetClock();
		resetPipeline();
		oscSender.sendMessage(stopMsg);
		playerFinishedMs = ofGetElapsedTimeMillis();
		restartPipelineNextFrame = false;
	}

	player.update();

	if(player.isPaused() && ofGetElapsedTimeMillis()-playerFinishedMs>=millistostartloop){
		playMsg.clear();
		playMsg.setAddress("play");
		GstClock * clock = gst_pipeline_get_clock((GstPipeline*)pipeline);
		gst_pipeline_set_delay ((GstPipeline *)pipeline, 200*GST_MSECOND);
		player.play();
		GstClockTime playtime = gst_clock_get_time((GstClock*)clock) + 200*GST_MSECOND;
		playMsg.addInt64Arg(playtime);
		oscSender.sendMessage(playMsg);
	}
}

void testApp::movieEOS(ofEventArgs & eos){
	restartPipelineNextFrame = true;
}

//--------------------------------------------------------------
void testApp::draw(){
	//if(!player.isPaused())
	player.draw(0,0);
	//GstClock * clock = gst_pipeline_get_clock((GstPipeline*)pipeline);
	//ofDrawBitmapString(ofToString(gst_clock_get_time((GstClock*)clock) + 200*GST_MSECOND),20,20);
}

void testApp::newAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "new service" << service.name << "in" << service.host_name << service.ip << ":" << service.port;
	Poco::ScopedLock<ofMutex> lock(mutex);
	if(service.name.find(servicename)==0){
		videoClients.push_back(ofPtr<OscPlayerClient>(new OscPlayerClient(service.ip,service.port,service.host_name)));
		resetPipeline();
		restartPipelineNextFrame = true;
	}
}

void testApp::removedAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "removing service" << service.name << "in" << service.host_name << service.ip << ":" << service.port;
	Poco::ScopedLock<ofMutex> lock(mutex);
	if(service.name.find(servicename)==0){
		for(int i=0; i<(int)videoClients.size(); i++){
			if(videoClients[i]->getHostName()==service.host_name){
				videoClients.erase(videoClients.begin()+i);
				return;
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' '){
		gchar *name;
		g_object_get((GObject*)clock,"name",&name,NULL);
		cout << name << endl;
	}
	if(key=='p'){
		player.setPaused(!player.isPaused());
		playerFinishedMs = ofGetElapsedTimeMillis()+3000;
	}
	if(key==OF_KEY_ESC){
		std::exit(0);
	}
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
