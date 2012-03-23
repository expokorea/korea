#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);
	ofHideCursor();

	xml.loadFile("settings.xml");
	string servicename = xml.getValue("settings:service","oscvideoplayer");
	string videoname = xml.getValue("settings:videofile","video.mov");
	bool loop = xml.getValue("settings:loop",0);
	cout << servicename << endl;

	int port = ofRandom(5555,9999);
	osc.setup(port);
	avahi.start(servicename,"_oscit._udp",port);

	player.loadMovie(videoname);
	if(loop) player.setLoopState(OF_LOOP_NORMAL);
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
