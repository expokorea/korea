#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);

	ofBackground(0);

	glow.setup();
	glow.brightness = 1;
	glow.passes = 2;

	stencil.setup();

	gui.setup("blur");
	gui.add(passes.setup("passes",glow.passes,1,4));
	gui.add(brightness.setup("brightness",glow.brightness,1,20));
	gui.add(framerate.setup("framerate",0,0,80));
	gui.add(speed.setup("speed",8,1,80));
	gui.add(radius.setup("radius",3,1,10));
	gui.add(r.setup("r",16,0,255));
	gui.add(g.setup("g",0,0,255));
	gui.add(b.setup("b",230,0,255));
	gui.add(lightOn.setup("light",false));

	lightOn.addListener(this,&testApp::lightOnChanged);

	randomcoeffs.resize(100,ofRandom(1));
	ofEnableAlphaBlending();

    //light.enable();
}

void testApp::lightOnChanged(bool & l){
	if(l){
		light.enable();
	}else{
		light.disable();
	}
}

//--------------------------------------------------------------
void testApp::update(){
	//blurAmnt = ofMap(sin(ofGetElapsedTimef()),-1,1,.75f,1.25f);
	framerate = ofGetFrameRate();
}

//--------------------------------------------------------------
void testApp::draw(){
	if(lightOn)
		ofEnableLighting();

	float t = ofGetElapsedTimef()*ofMap(speed,1,80,0,1)*ofMap(speed,1,80,0,1);

	glow.begin();
	ofClear(0,0);

	//glEnable(GL_DEPTH_TEST);
	ofSeedRandom(0);
	for(int i=0;i<100;i++){
		ofColor color(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,20,200));
		ofSetColor(color);
		//color.setBrightness();
		//ofSetColor(r,g,b,a*.5);
		ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius);
	}


	glow.end();
	ofSetColor(255);
	glow.draw(0,0);
	ofSeedRandom(0);
	for(int i=0;i<100;i++){
		ofColor color(r,g,b,ofMap(ofNoise(i/100.*ofRandom(1),t*ofRandom(1,500),ofRandom(1)),0,1,100,200));
		ofSetColor(color);
		//color.setBrightness(ofMap(ofNoise(i/100.*ofRandom(1),t,ofRandom(1)),0,1,100,255));
		//ofSetColor(r,g,b,a);
		ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius*.4);
	}

	ofSetColor(255);
	//glDisable(GL_DEPTH_TEST);
	ofDisableLighting();
	gui.draw();
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
