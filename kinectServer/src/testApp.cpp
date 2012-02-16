#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	kinect.init(false,false,true);
	kinect.open();

	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(kinect.getWidth()*kinect.getHeight()/3);

	oscConfig.setup(6667);
	avahi.start("osckinectserverconfig","_oscit._udp",6667);


	ofAddListener(avahiBrowser.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahiBrowser.serviceRemoveE,this,&testApp::removedAvahiService);

	avahiBrowser.lookup("_oscit._udp");

	ofSetFrameRate(40);
	frame = 0;
	tilt = 0;
	farThreshold = 0;
	nearThreshold = 255;

	nearThresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	farThresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	thresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
}


void testApp::newAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "new service" << service.name << "at" << service.host_name << service.ip + ":" << service.port;
	if(service.name=="oscservercontours"){
		oscContours.push_back(ofPtr<OscContourServer>(new OscContourServer(service.ip,service.port,service.host_name)));
	}
}

void testApp::removedAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "service removed" << service.name << "at" << service.host_name << service.ip + ":" << service.port;
	if(service.name=="oscservercontours"){
		for(int i=0; i<oscContours.size(); i++){
			if(oscContours[i]->getHostName()==service.host_name){
				oscContours.erase(oscContours.begin()+i);
				return;
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
	if(kinect.isFrameNew()){
		threshold(kinect.getDepthPixelsRef(),nearThresPix,nearThreshold, true);
		threshold(kinect.getDepthPixelsRef(),farThresPix,farThreshold);
		toCv(thresPix) = toCv(nearThresPix) & toCv(farThresPix);
		contourFinder.findContours(thresPix);

		for(int i=0;i<oscContours.size();i++){
			oscContours[i]->newFrame(frame);
		}


		polylines = contourFinder.getPolylines();
		for(int i=0;i<(int)polylines.size();i++){
			ofPoint centroid = polylines[i].getCentroid2D();
			polylines[i] = polylines[i].getResampledByCount(16);
			float z = kinect.getDistanceAt(centroid);
			for(int j=0;j<oscContours.size();j++){
				oscContours[j]->sendContour(polylines[i],z);
			}
		}
		frame++;
	}


	while(oscConfig.hasWaitingMessages()){
		ofxOscMessage msg;
		oscConfig.getNextMessage(&msg);

		if(msg.getAddress()=="tilt" && tilt!=msg.getArgAsInt32(0)){
			tilt=msg.getArgAsInt32(0);
			kinect.setCameraTiltAngle(tilt);
		}else if(msg.getAddress()=="farThreshold"){
			farThreshold=msg.getArgAsInt32(0);
		}else if(msg.getAddress()=="nearThreshold"){
			nearThreshold=msg.getArgAsInt32(0);
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	thresPix.update();
	kinect.drawDepth(0,0);
	for(int i=0;i<(int)polylines.size();i++){
		polylines[i].draw();
	}

	thresPix.draw(660,0);

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
