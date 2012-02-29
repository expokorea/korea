#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	//ofSetVerticalSync(true);

	ofSetSphereResolution(10);

	ofBackground(0);
	pSystem.setup(200);
	
	//kSystem.setup(100,(1024*3)-200,768-200);
	kSystem.setup(100,1024*3,768,600);

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
	//gui.add(&pSystem.gui);
	gui.add(&kSystem.gui);
	bShowGui = true;
	
	lightOn.addListener(this,&testApp::lightOnChanged);
	light.setPosition(ofVec3f(ofGetWidth()*.5,ofGetHeight()*.5,0));
	
	ofEnableAlphaBlending();
	
	ofSetFrameRate(60);
    //light.enable();
	
	
	//cam.disableMouseInput();
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

	pSystemDemo.update();
	kSystem.update();
	kSystem.color.set(pSystemDemo.r,pSystemDemo.g,pSystemDemo.b);
	kSystem.debugUserCenter(ofPoint(mouseX,mouseY));

	
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	if(lightOn)
		ofEnableLighting();

	float sizeW = 1024*3;
	float sizeH = 768;
	
		
	glEnable(GL_DEPTH_TEST);

	
	if(demo){
		
		
		cam.begin();
		
		ofNoFill();
		ofPushMatrix();
			ofRect(-sizeW*.5,-sizeH*.5,sizeW,sizeH);
			ofTranslate(0,0,-1000);
			ofRect(-sizeW*.5,-sizeH*.5,sizeW,sizeH);
		ofPopMatrix();
		
		kSystem.drawForGlow();
		
		ofPushMatrix();
			ofTranslate(-ofGetWidth()*.5,-ofGetHeight()*.5,0);
			pSystemDemo.drawForGlow();
		ofPopMatrix();
		
		cam.end();
	
	}else{
		
		glow.begin();
		cam.begin();
		
		ofClear(0,0);
		
		kSystem.drawForGlow();
		
		ofPushMatrix();
			ofTranslate(-ofGetWidth()*.5,-ofGetHeight()*.5,0);
			pSystemDemo.drawForGlow();
		ofPopMatrix();
		
		cam.end();
		glow.end();
		
		ofSetColor(255);
		ofPushMatrix();
			glScalef(1,-1,1);
			glow.draw(0,-ofGetHeight());
		ofPopMatrix();
	}

	
	ofSetColor(255);
	ofDisableLighting();
	glDisable(GL_DEPTH_TEST);
	if(bShowGui){
		gui.draw();
		ofDrawBitmapString("c: reset cam  m: toggle cam mouse  x: toggle gui",10,ofGetHeight()-20);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	switch(key)
	{
		case 'c': cam.reset(); break;
		case 'm': 
			if(cam.getMouseInputEnabled()) cam.disableMouseInput();
			else cam.enableMouseInput();
			break;
	    case 'x':
			bShowGui = !bShowGui;
			break;
		case 'f':
			ofToggleFullscreen();
			break;
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
