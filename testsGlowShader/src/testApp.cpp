#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(30);

	ofSetSphereResolution(10);

	ofBackground(0);
	pSystem.setup(1000);

	glow.setup();
	glow.brightness = 1;
	glow.passes = 2;

	gui.setup("blur");
	gui.add(passes.setup("passes",glow.passes,1,4));
	gui.add(brightness.setup("brightness",glow.brightness,1,20));
	gui.add(framerate.setup("framerate",0,0,80));
	gui.add(pSystemDemo.speed.setup("speed",8,1,80));
	gui.add(pSystemDemo.radius.setup("radius",3,1,10));
	gui.add(pSystemDemo.r.setup("r",16,0,255));
	gui.add(pSystemDemo.g.setup("g",0,0,255));
	gui.add(pSystemDemo.b.setup("b",230,0,255));
	gui.add(lightOn.setup("light",false));
	gui.add(demo.setup("demo p system",false));
	gui.add(lightX.setup("light X",ofGetWidth()/2,0,ofGetWidth()));
	gui.add(lightY.setup("light Y",ofGetHeight()/2,0,ofGetHeight()));
	gui.add(lightZ.setup("light Z",0,-ofGetWidth(),ofGetWidth()));
	gui.add(drawNotBlurred.setup("drawNotBlurred",false));
	gui.add(record.setup("record",false));
	gui.add(&pSystem.gui);

	lightOn.addListener(this,&testApp::lightOnChanged);
	record.addListener(this,&testApp::recordPressed);

	fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB);

	ofEnableAlphaBlending();
	hideGui = false;
    //light.enable();
}

void testApp::lightOnChanged(bool & l){
	if(l){
		light.enable();
	}else{
		light.disable();
	}
}

void testApp::recordPressed(bool & l){
	if(l && !record){
		recorder.setup(ofGetTimestampString()+".mp4",ofGetWidth(),ofGetHeight(),30);
	}else if(!l && record){
		recorder.encodeVideo();
	}
}

//--------------------------------------------------------------
void testApp::update(){
	//blurAmnt = ofMap(sin(ofGetElapsedTimef()),-1,1,.75f,1.25f);
	framerate = ofGetFrameRate();

	if(demo){
		pSystemDemo.update();
	}else{
		pSystem.updateAll(10);
		pSystem.calculate();
	}

	light.setPosition(lightX,lightY,lightZ);
}

//--------------------------------------------------------------
void testApp::draw(){
	if(lightOn)
		ofEnableLighting();


	glow.begin();
	ofClear(0,0);

	//glEnable(GL_DEPTH_TEST);
	if(demo){
		pSystemDemo.drawForGlow();
	}else{
		glPushMatrix();
			glTranslatef(ofGetWidth()/2.f ,ofGetHeight()/2.f ,0.f);
			pSystem.drawForGlow();
		glPopMatrix();
	}

	glow.end();

	ofSetColor(255);
	if(record){
		fbo.begin();
		ofClear(0);
	}
	glow.draw(0,0);

	if(drawNotBlurred){
		if(demo){
			pSystemDemo.draw();
		}else{
			glPushMatrix();
				glTranslatef(ofGetWidth()/2.f ,ofGetHeight()/2.f ,0.f);
				pSystem.drawAll();
			glPopMatrix();
		}
	}
	if(record){
		fbo.end();
		ofSetColor(255);
		fbo.draw(0,0);
		fbo.readToPixels(pixRecord);
		recorder.addFrame(pixRecord);
	}


	if(!hideGui){
		ofSetColor(255);
		//glDisable(GL_DEPTH_TEST);
		ofDisableLighting();
		gui.draw();
		//light.draw();
	}

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key=='f'){
		ofToggleFullscreen();
	}
	if(key=='h'){
		hideGui=!hideGui;
		if(hideGui){
			ofHideCursor();
		}else{
			ofShowCursor();
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
	fbo.allocate(w,h,GL_RGB);
	glow.setup();
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
