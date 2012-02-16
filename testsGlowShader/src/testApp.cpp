#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);

	ofSetSphereResolution(10);

	ofBackground(0);
	pSystem.setup(200);
	
	kSystem.setup(200);
	
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
	gui.add(&pSystem.gui);

	lightOn.addListener(this,&testApp::lightOnChanged);

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

	if(demo){
		pSystemDemo.update();
	}else{
		//pSystem.updateAll(10);
		//pSystem.calculate();
		kSystem.update();
		float circleForce=5;
		float circleForceRadius=100;
		//kSystem.vectorField.addInwardCircle((float)mouseX, (float)mouseY, circleForceRadius, circleForce);
		
	}
	
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
		/*glPushMatrix();
			glTranslatef(ofGetWidth()/2.f ,ofGetHeight()/2.f ,0.f);
			pSystem.drawForGlow();
		glPopMatrix();*/
		kSystem.drawForGlow();
	}

	glow.end();

	ofSetColor(255);
	glow.draw(0,0);

	if(demo){
		pSystemDemo.draw();
	}else{
		/*glPushMatrix();
			glTranslatef(ofGetWidth()/2.f ,ofGetHeight()/2.f ,0.f);
			pSystem.drawAll();
		glPopMatrix();*/
		kSystem.draw();
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
