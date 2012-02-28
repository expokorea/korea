#include "testApp.h"
#include "ofTween.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	ofSetSphereResolution(10);

	ofBackground(0);
	pSystem.setup(200);
	
	kSystem.setup(100);
	
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
	//gui.add(z.setup("z",665.108,0,ofGetWidth()*4));
	gui.add(posCameraPct.setup("posCameraPct",0,0,1));
	gui.add(lookAtPct.setup("lookAtPct",0,0,1));
	gui.add(drawNotBlurred.setup("drawNotBlurred",true));
	gui.add(record.setup("record",false));
	gui.add(useCamera.setup("useCamera",false));
	gui.add(initCameraMovement.setup("initCameraMovement"));
	//gui.add(&pSystem.gui);
	gui.add(&kSystem.gui);
	gui.loadFromFile("settings.xml");
	pSystemDemo.speed = 8;

	lightOn.addListener(this,&testApp::lightOnChanged);
	record.addListener(this,&testApp::recordPressed);
	initCameraMovement.addListener(this,&testApp::initCameraMovementPressed);

	fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB);

	ofEnableAlphaBlending();
	hideGui = false;
	ofEnableAlphaBlending();
	
	camera.setupPerspective();
	//prevCameraPos = pSystemDemo.path[0];

	timeInitCamMovement = ofGetElapsedTimef();

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
		recorder.setup(ofGetTimestampString()+".mp4",ofGetWidth(),ofGetHeight(),20);
	}else if(!l && record){
		recorder.encodeVideo();
	}
}

void testApp::initCameraMovementPressed(bool & m){
	if(!m){
		timeInitCamMovement = ofGetElapsedTimef();
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
		pSystemDemo.update();
		kSystem.update();
		kSystem.color.set(pSystemDemo.r,pSystemDemo.g,pSystemDemo.b);

	}

	light.setPosition(lightX,lightY,lightZ);

	//z = ofMap(ofGetElapsedTimef(),0,20,0,pSystemDemo.path.size()-1,true);
	posCameraPct = ofMap(ofGetElapsedTimef()-timeInitCamMovement,0,20,0,1,&ofEasing::quadOut,true);
	camera.setupPerspective();
	//camera.setPosition(ofGetWidth()/2, ofGetHeight()/2,  z); //665.108 +
	//camera.setPosition(pSystemDemo.path[z]);
	//lookAt.setPosition(ofGetWidth()/2,ofGetHeight()/2,ofLerp(0,ofGetWidth()*4,posCameraPct));
	//camera.lookAt(ofVec3f(ofGetWidth()/2,ofGetHeight()/2,0));
	camera.pan(-180+posCameraPct*180);

			/*ofBezierPoint(ofPoint(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*4),
			ofPoint(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*2),
			ofPoint(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*2),
			pSystemDemo.path[0],
			posCameraPct));*/
	prevCameraPos = ofBezierPoint(ofPoint(ofGetWidth()/2,ofGetHeight()/2,0),
			ofPoint(ofGetWidth()/6,ofGetHeight()/6,ofGetWidth()*1.5),
			ofPoint(ofGetWidth()/6,ofGetHeight()/6*3,ofGetWidth()*2.5),
			ofPoint(ofGetWidth()/2,ofGetHeight()/2,ofGetWidth()*4),
			posCameraPct);
	prevCameraPos.x += ofSignedNoise(prevCameraPos.x/100.)*100;
	prevCameraPos.y += ofSignedNoise(prevCameraPos.y/100.)*100;
	camera.setPosition(prevCameraPos);


}

//--------------------------------------------------------------
void testApp::draw(){
	if(lightOn)
		ofEnableLighting();


	glow.begin(false);
	if(useCamera)camera.begin();
	ofClear(0,0);

	//glEnable(GL_DEPTH_TEST);
	if(demo){
		pSystemDemo.drawForGlow();
	}else{
		/*glPushMatrix();
			glTranslatef(ofGetWidth()/2.f ,ofGetHeight()/2.f ,0.f);
			pSystem.drawForGlow();
		glPopMatrix();*/
		pSystemDemo.drawForGlow();

		kSystem.drawForGlow();
	}
	if(useCamera)camera.end();
	glow.end();

	ofSetColor(255);
	if(record){
		fbo.begin();
		ofClear(0);
	}
	glow.draw(0,0);

	if(drawNotBlurred){
		if(useCamera)camera.begin();
		if(demo){
			pSystemDemo.draw();
		}else{
			/*glPushMatrix();
				glTranslatef(ofGetWidth()/2.f ,ofGetHeight()/2.f ,0.f);
				pSystem.drawAll();
			glPopMatrix();*/
			kSystem.draw();
		}
		if(useCamera)camera.end();
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

		/*if(useCamera)camera.begin();
		pSystemDemo.drawPath();
		if(useCamera)camera.end();*/
		//light.draw();

		/*camera.begin();
		pSystemDemo.path.draw();
		camera.end();*/

		stringstream pos;
		pos << camera.getPosition();
		ofDrawBitmapString("camera pos:" + pos.str(),ofGetWidth()-500,20);
		stringstream lookAt;
		lookAt << camera.getLookAtDir();
		ofDrawBitmapString("camera look at:" + lookAt.str(),ofGetWidth()-500,40);
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
