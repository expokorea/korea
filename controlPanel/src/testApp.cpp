#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofAddListener(avahi.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahi.serviceRemoveE,this,&testApp::removedAvahiService);


	contours.setup(6668);

	ofSetFrameRate(10);
	ofBackground(0);

	gui.setup("control panel");
	guiMapping.setup("mapping players");
	guiMapping.add(startMapping.setup("start mapping",false));

	gui.add(&guiMapping);

	avahi.lookup("_oscit._udp");
}

//--------------------------------------------------------------
void testApp::update(){
	for(int i=0; i<videoClients.size(); i++){
		videoClients[i]->update();
	}

	for(int i=0; i<kinectServers.size(); i++){
		kinectServers[i]->update();
	}

	contours.update();
}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetColor(255);
	gui.draw();

	ofRect(320,10,640,480);
	ofSetColor(0);
	ofPushMatrix();
	ofTranslate(320,10);
	for(int i=0;i<contours.size();i++){
		contours[i].draw();
	}
	ofPopMatrix();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
}


void testApp::newAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "new service" << service.name << "in" << service.host_name << service.ip << ":" << service.port;
	if(service.name=="oscvideoplayer"){
		videoClients.push_back(ofPtr<OscPlayerClient>(new OscPlayerClient(service.ip,service.port,service.host_name)));
		videoClients.back()->playing = startMapping.value;
	}else if(service.name=="osckinectserverconfig"){
		kinectServers.push_back(ofPtr<OscKinectServer>(new OscKinectServer(service.ip,service.port,service.host_name)));
		gui.add(&kinectServers.back()->gui);
	}
}

void testApp::removedAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "removing service" << service.name << "in" << service.host_name << service.ip << ":" << service.port;
	if(service.name=="oscvideoplayer"){
		for(int i=0; i<videoClients.size(); i++){
			if(videoClients[i]->getHostName()==service.host_name){
				videoClients.erase(videoClients.begin()+i);
				return;
			}
		}
	}
	if(service.name=="osckinectserverconfig"){
		for(int i=0; i<kinectServers.size(); i++){
			if(kinectServers[i]->getHostName()==service.host_name){
				kinectServers.erase(kinectServers.begin()+i);
				return;
			}
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
