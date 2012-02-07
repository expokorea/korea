#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	kinect.init(false,false,true);
	kinect.open();
	contourFinder.setMinAreaRadius(10);
	contourFinder.setMaxAreaRadius(200);
	osc.setup("localhost",6666);
	ofSetFrameRate(40);
	frame = 0;
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
	if(kinect.isFrameNew()){
		ofxOscMessage msg;
		contourFinder.findContours(kinect.getDepthPixelsRef());

		msg.setAddress("frame");
		msg.addIntArg(frame);
		osc.sendMessage(msg);

		polylines = contourFinder.getPolylines();
		for(int i=0;i<(int)polylines.size();i++){
			msg.clear();
			polylines[i] = polylines[i].getResampledByCount(16);
			msg.setAddress("contour");
			for(int j=0;j<(int)polylines[i].size();j++){
				msg.addFloatArg(polylines[i][j].x);
				msg.addFloatArg(polylines[i][j].y);
			}
			osc.sendMessage(msg);
		}
		frame++;
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	kinect.drawDepth(0,0);
	for(int i=0;i<(int)polylines.size();i++){
		polylines[i].draw();
	}

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
