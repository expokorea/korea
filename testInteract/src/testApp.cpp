#include "testApp.h"
#include "ofxTimeUtils.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	//ofSetFrameRate(60);

	KoreaParticle::model.loadModel("blobFish.obj");
	ofSetSphereResolution(5);

	ofBackground(0);

	gui.setup("blur");
	gui.add(passes.setup("passes",glow.passes,1,4));
	gui.add(brightness.setup("brightness",glow.brightness,1,20));
	gui.add(framerate.setup("framerate",0,0,80));
	gui.add(pSystemDemo.speed.setup("speed",8,1,80));
	gui.add(pSystemDemo.radius.setup("radius",3,1,10));
	gui.add(pSystemDemo.r.setup("r",16,0,255));
	gui.add(pSystemDemo.g.setup("g",0,0,255));
	gui.add(pSystemDemo.b.setup("b",230,0,255));
	gui.add(KoreaParticle::r.setup("r",16,0,255));
	gui.add(KoreaParticle::g.setup("g",0,0,255));
	gui.add(KoreaParticle::b.setup("b",230,0,255));
	gui.add(KoreaParticle::debug.setup("debug",false));
	gui.add(KoreaParticle::thickness.setup("thickness",3,0,16));
	gui.add(lightOn.setup("light",false));
	gui.add(drawGlow.setup("drawGlow",true));
	gui.add(KoreaParticle::useModel.setup("use model",false));
	gui.add(lightCutoff.setup("light cutoff",45,0,100));
	gui.add(lightExponent.setup("light exponent",0,0,3));
	gui.add(lightConstant.setup("light constant",2,0,10));
	gui.add(lightLinear.setup("light linear",1,0,10));
	gui.add(lightQuad.setup("light quad",0.5,0,10));
	gui.add(record.setup("record",false));
	//gui.add(&pSystem.gui);
	bShowGui = true;
	
	lightOn.addListener(this,&testApp::lightOnChanged);
	record.addListener(this,&testApp::recordPressed);
	light.setPosition(ofVec3f(0,0,0));
	fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB,4);


	pSystem.setup(100);

	//kSystem.setup(100,(1024*3)-200,768-200);
	kSystem.setup(200,1024*3,768,600);
	gui.add(&kSystem.gui);
	
	user1.setup();


	glow.setup();
	glow.brightness = 1;
	glow.passes = 4;
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	
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


void testApp::recordPressed(bool & l){
	if(l && !record){
		ofxTimeUtils::setMode(ofxTimeUtils::Frame,60);
		ofSetFrameRate(60);
		recorder.setup(ofGetTimestampString()+".mp4",ofGetWidth(),ofGetHeight(),60);
	}else if(!l && record){
		ofxTimeUtils::setMode(ofxTimeUtils::Time,60);
		ofSetFrameRate(60);
		recorder.encodeVideo();
	}
}

//--------------------------------------------------------------
void testApp::update(){
	//blurAmnt = ofMap(sin(ofGetElapsedTimef()),-1,1,.75f,1.25f);
	framerate = ofGetFrameRate();

	//pSystemDemo.update();
	kSystem.update();
	//kSystem.color.set(pSystemDemo.r,pSystemDemo.g,pSystemDemo.b);
	kSystem.debugUserCenter(user1);

	user1.debugSetUserPosFromMouse(mouseX,mouseY);
	user1.debugSetUserContour();
}

//--------------------------------------------------------------
void testApp::draw(){
	
	//gui.draw();
	//return;
	//ofEnableBlendMode(OF_BLENDMODE_ADD);
	
	if(lightOn){
		ofEnableLighting();
		//light.setSpotlight( lightCutoff, lightExponent );
		//light.setDirectional();
		//light.setAttenuation(lightConstant, lightLinear, lightQuad);
	}

	float sizeW = 1024*3;
	float sizeH = 768;
	



	if(record){
		fbo.begin();
		ofClear(0);
	}

	if(!drawGlow){
		cam.begin();
		//light.setPosition(cam.getPosition());
		if(KoreaParticle::debug){
			ofNoFill();
			ofPushMatrix();
				ofRect(-sizeW*.5,-sizeH*.5,sizeW,sizeH);
				ofTranslate(0,0,-1000);
				ofRect(-sizeW*.5,-sizeH*.5,sizeW,sizeH);
			ofPopMatrix();
		}

		ofFill();
		kSystem.draw();
		if(KoreaParticle::debug) user1.drawUser();

		/*ofPushMatrix();
			ofTranslate(-ofGetWidth()*.5,-ofGetHeight()*.5,0);
			pSystemDemo.draw();
		ofPopMatrix();*/

		cam.end();


	}else{
		ofFill();
		glow.begin(false);
		cam.begin();
		light.setPosition(cam.getPosition());

		ofClear(0,0);

		ofFill();
		kSystem.drawForGlow();

		/*ofPushMatrix();
			ofTranslate(-ofGetWidth()*.5,-ofGetHeight()*.5,0);
			pSystemDemo.drawForGlow();
		ofPopMatrix();*/

		cam.end();
		glow.end();

		ofSetColor(255);
		if(!record){
			ofPushMatrix();
			glScalef(1,-1,1);
			glow.draw(0,-ofGetHeight());
		}else{
			glow.draw(0,0);
		}

		if(!record){
			ofPopMatrix();
		}


		cam.begin();

		if(KoreaParticle::debug){
			ofNoFill();
			ofPushMatrix();
				ofRect(-sizeW*.5,-sizeH*.5,sizeW,sizeH);
				ofTranslate(0,0,-1000);
				ofRect(-sizeW*.5,-sizeH*.5,sizeW,sizeH);
			ofPopMatrix();
		}

		ofFill();
		kSystem.draw();
		if(KoreaParticle::debug) user1.drawUser();

		/*ofPushMatrix();
			ofTranslate(-ofGetWidth()*.5,-ofGetHeight()*.5,0);
			pSystemDemo.draw();
		ofPopMatrix();*/

		cam.end();
	}

	if(record){
		fbo.end();
		ofSetColor(255);
		fbo.draw(0,0);
		fbo.readToPixels(pixRecord);
		recorder.addFrame(pixRecord);
	}
	
	ofSetColor(255);
	ofDisableLighting();
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
