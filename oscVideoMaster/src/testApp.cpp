#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);
	ofHideCursor();
	ofAddListener(avahi.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahi.serviceRemoveE,this,&testApp::removedAvahiService);

	xml.loadFile("settings.xml");
	servicename = xml.getValue("settings:service","oscvideoplayer");
	string videoname = xml.getValue("settings:videofile","video.mov");
	bool loop = xml.getValue("settings:loop",0);
	millistostartloop = xml.getValue("settings:millistostartloop",2000);
	cout << servicename << endl;

	avahi.lookup("_oscit._udp");

	player.loadMovie(videoname);
	if(loop) player.setLoopState(OF_LOOP_NORMAL);
	player.play();
	playerFinishedMs = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	mutex.lock();
	player.update();
	if(player.getIsMovieDone()){
		playerFinishedMs = ofGetElapsedTimeMillis();
		player.setFrame(0);
		player.setPaused(true);
		for(int i=0; i<(int)videoClients.size(); i++){
			videoClients[i]->playing = false;
		}
	}
	if(player.isPaused() && ofGetElapsedTimeMillis()-playerFinishedMs>=millistostartloop){
		player.play();
		for(int i=0; i<(int)videoClients.size(); i++){
			videoClients[i]->playing = true;
		}
	}
	for(int i=0; i<(int)videoClients.size(); i++){
		videoClients[i]->update();
	}
	mutex.unlock();
}

//--------------------------------------------------------------
void testApp::draw(){
	if(!player.isPaused()) player.draw(0,0);
}

void testApp::newAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "new service" << service.name << "in" << service.host_name << service.ip << ":" << service.port;
	mutex.lock();
	if(service.name.find(servicename)==0){
		videoClients.push_back(ofPtr<OscPlayerClient>(new OscPlayerClient(service.ip,service.port,service.host_name)));
		videoClients.back()->playing = !player.isPaused();
	}
	mutex.unlock();
}

void testApp::removedAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "removing service" << service.name << "in" << service.host_name << service.ip << ":" << service.port;
	mutex.lock();
	if(service.name.find(servicename)==0){
		for(int i=0; i<(int)videoClients.size(); i++){
			if(videoClients[i]->getHostName()==service.host_name){
				videoClients.erase(videoClients.begin()+i);
				return;
			}
		}
	}
	mutex.unlock();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
