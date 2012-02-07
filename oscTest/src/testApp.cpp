#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofAddListener(avahi.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahi.serviceRemoveE,this,&testApp::removedAvahiService);

	avahi.lookup("_oscit._udp");
	ofSetFrameRate(10);
}

//--------------------------------------------------------------
void testApp::update(){
	for(int i=0; i<clients.size(); i++){
		clients[i]->update();
	}
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' '){
		for(int i=0; i<clients.size(); i++){
			clients[i]->togglePlaying();
		}
	}
}


void testApp::newAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "new service" << service.ip << ":" << service.port;
	clients.push_back(ofPtr<OscPlayerClient>(new OscPlayerClient(service.ip,service.port)));
}

void testApp::removedAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "removing service" << service.ip << ":" << service.port;
	for(int i=0; i<clients.size(); i++){
		if(clients[i]->getIp()==service.ip){
			clients.erase(clients.begin()+i);
			return;
		}
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
