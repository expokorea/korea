#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);

	int port = ofRandom(5555,9999);
	osc.setup(port);
	avahi.start("oscvideoplayer","_oscit._udp",port);

	player.loadMovie("fingers.mov");
}

//--------------------------------------------------------------
void testApp::update(){
	while(osc.hasWaitingMessages()){
		ofxOscMessage msg;
		osc.getNextMessage(&msg);

		if(msg.getAddress()=="play" && player.isPaused()){
			player.play();
		}
		if(msg.getAddress()=="stop" && !player.isPaused()){
			player.setFrame(0);
			player.stop();
		}
	}
	if(!player.isPaused()) player.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	if(!player.isPaused()) player.draw(0,0);
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
