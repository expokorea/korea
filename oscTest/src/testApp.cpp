#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	osc.setup("localhost", 6666);
	playing = false;
	ofSetFrameRate(10);
}

//--------------------------------------------------------------
void testApp::update(){
	ofxOscMessage msg;
	if(playing){
		msg.setAddress("play");
	}else{
		msg.setAddress("stop");
	}
	osc.sendMessage(msg);
}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' '){
		playing = !playing;
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
