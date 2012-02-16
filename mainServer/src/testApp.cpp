#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	osc.setup(6666);
	
	pSystem.setup(1000);
	
	ofSetFrameRate(60);
	
	
}

//--------------------------------------------------------------
void testApp::update(){
	
	pSystem.updateAll(10);
	pSystem.calculate();
	
	
	ofxOscMessage msg;
	while(osc.hasWaitingMessages()){
		osc.getNextMessage(&msg);
		if(msg.getAddress()=="frame"){
			contours.clear();
			continue;
		}

		if(msg.getAddress()=="contour"){
			int n = msg.getNumArgs();
			contours.push_back(ofPolyline());
			ofPolyline & contour = contours[contours.size()-1];
			for(int i=0;i<n;i+=2){
				ofPoint p(msg.getArgAsFloat(i),msg.getArgAsFloat(i+1));
				contour.addVertex(p);
			}
			contour.close();
		}
	}


}

//--------------------------------------------------------------
void testApp::draw(){
	
	/*
	for(int i=0;i<contours.size();i++){
		contours[i].draw();
	}
	*/
	
	glPushMatrix();
		glTranslatef(ofGetWidth()/2.f ,ofGetHeight()/2.f ,0.f);
		pSystem.drawAll();
	glPopMatrix();
	
	pSystem.drawGui();
	
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
