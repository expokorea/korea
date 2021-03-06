#include "testApp.h"
#include "ofxTimeUtils.h"


//--------------------------------------------------------------
void testApp::setup(){
	

	ofSetVerticalSync(true);
	//ofSetFrameRate(30);
	ofBackground(0);

	//KoreaParticle::model.loadModel("blobFish.obj");
	ofSetSphereResolution(5);

	gui.setup("blur");
	gui.add(passes.setup("passes",glow.passes,1,4));
	gui.add(brightness.setup("brightness",glow.brightness,1,20));
	gui.add(framerate.setup("framerate",0,0,80));
	///gui.add(pSystemDemo.speed.setup("speed",8,1,80));
	///gui.add(pSystemDemo.radius.setup("radius",3,1,10));
	///gui.add(pSystemDemo.r.setup("r",16,0,255));
	///gui.add(pSystemDemo.g.setup("g",0,0,255));
	///gui.add(pSystemDemo.b.setup("b",230,0,255));
	gui.add(KoreaParticle::r.setup("r",16,0,255));
	gui.add(KoreaParticle::g.setup("g",0,0,255));
	gui.add(KoreaParticle::b.setup("b",230,0,255));
	gui.add(KoreaParticle::rTex.setup("r texture",16,0,255));
	gui.add(KoreaParticle::gTex.setup("g texture",0,0,255));
	gui.add(KoreaParticle::bTex.setup("b texture",230,0,255));
	gui.add(KoreaParticle::rLines.setup("r lines",255,0,255));
	gui.add(KoreaParticle::gLines.setup("g lines",255,0,255));
	gui.add(KoreaParticle::bLines.setup("b lines",255,0,255));
	gui.add(particlesTexMode.setup("delaunay/voronoi",true));
	gui.add(KoreaParticle::debug.setup("debug",false));
	////gui.add(KoreaParticle::thickness.setup("thickness",3,0,16));
	gui.add(KoreaParticle::flockAlpha.setup("flock alpha",.3,0,1));
	gui.add(lightOn.setup("light",false));
	gui.add(drawGlow.setup("drawGlow",true));
	///gui.add(KoreaParticle::useModel.setup("use model",false));
	gui.add(lightCutoff.setup("light cutoff",45,0,100));
	gui.add(lightExponent.setup("light exponent",0,0,3));
	gui.add(lightConstant.setup("light constant",2,0,10));
	gui.add(lightLinear.setup("light linear",1,0,10));
	gui.add(lightQuad.setup("light quad",0.5,0,10));
	gui.add(record.setup("record",false));
	///gui.add(&pSystem.gui);
	
	ofxXmlSettings xml;
	xml.loadFile("settings.xml");
	gui.loadFromXml(xml);
	
	bShowGui = true;
	
	
	lightOn.addListener(this,&testApp::lightOnChanged);
	record.addListener(this,&testApp::recordPressed);
	light.setPosition(ofVec3f(0,0,0));
	fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB,4);

	KoreaParticle::r.addListener(this,&testApp::particleRGBChanged);
	KoreaParticle::g.addListener(this,&testApp::particleRGBChanged);
	KoreaParticle::b.addListener(this,&testApp::particleRGBChanged);

	KoreaParticle::rTex.addListener(this,&testApp::particleTexRGBChanged);
	KoreaParticle::gTex.addListener(this,&testApp::particleTexRGBChanged);
	KoreaParticle::bTex.addListener(this,&testApp::particleTexRGBChanged);

	KoreaParticle::rLines.addListener(this,&testApp::particleLinesRGBChanged);
	KoreaParticle::gLines.addListener(this,&testApp::particleLinesRGBChanged);
	KoreaParticle::bLines.addListener(this,&testApp::particleLinesRGBChanged);

	particlesTexMode.addListener(this,&testApp::particleTexModeChanged);

	//KoreaParticle::generateTexture(KoreaParticle::Voronoi);
	kSystem.setupInGroups(1024*3,768,600);
	gui.add(&kSystem.gui);
	
	// debugging
	user1.setup();
	users.push_back(user1);
	///users.push_back(user1);

	// shaders
	glow.setup();
	glow.brightness = 1;
	glow.passes = 4;
	ofEnableBlendMode(OF_BLENDMODE_ADD);
}


void testApp::particleRGBChanged(int & rgb){

}

void testApp::particleTexRGBChanged(int & rgb){

}

void testApp::particleLinesRGBChanged(int & rgb){

}

void testApp::particleTexModeChanged(bool & mode){
	/*if(mode){
		KoreaParticle::generateTexture(KoreaParticle::Voronoi);
	}else{
		KoreaParticle::generateTexture(KoreaParticle::Delaunay);
	}*/
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
		string filename = ofToDataPath(ofGetTimestampString());
		recorder.setup(filename,ofGetWidth(),ofGetHeight(),30);
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
	kSystem.assignUserTargets(users);
	kSystem.update();
	//kSystem.debugUserCenter(user1);
	

	//user1.debugSetUserPosFromMouse(mouseX,mouseY);
	//user1.debugSetUserContour();
	users[0].debugSetUserPosFromMouse(mouseX,mouseY,300);
	users[0].debugSetUserContour();
	
	//users[1].debugSetUserPosFromMouse(mouseX+300,mouseY,-100);
	//users[1].debugSetUserContour();
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
		if(KoreaParticle::debug){
			for(int i = 0; i < (int)users.size(); i++)
				users[i].drawUser();
				//user1.drawUser();
		}
		if(KoreaParticle::debug) kSystem.drawDebug();

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
		//for(int i = 0; i < users.size(); i++)
		//	users[i].drawUser();
			
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
		
		/*ofSetRectMode(OF_RECTMODE_CENTER);
		ofNoFill();
		ofRect( ofGetWidth()/2-1,ofGetHeight()/2-1,642,482);
		ofSetRectMode(OF_RECTMODE_CORNER);*/

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
		case ' ': 
			if(!kSystem.bFadeOut) kSystem.setFadeOut();
			else kSystem.setFadeIn();
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
