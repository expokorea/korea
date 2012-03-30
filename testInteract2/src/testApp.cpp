#include "testApp.h"
#include "BoundingBox3D.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);
	ofEnableBlendMode(OF_BLENDMODE_ADD);


	RibbonParticle::generateTexture(RibbonParticle::Voronoi);
	gui.setup();
	gui.add(RibbonParticle::r.set("r",16,0,255));
	gui.add(RibbonParticle::g.set("g",0,0,255));
	gui.add(RibbonParticle::b.set("b",230,0,255));
	gui.add(RibbonParticle::rTex.set("r texture",16,0,255));
	gui.add(RibbonParticle::gTex.set("g texture",0,0,255));
	gui.add(RibbonParticle::bTex.set("b texture",230,0,255));
	gui.add(RibbonParticle::rLines.set("r lines",255,0,255));
	gui.add(RibbonParticle::gLines.set("g lines",255,0,255));
	gui.add(RibbonParticle::bLines.set("b lines",255,0,255));
	gui.add(EatableParticleField::r.set("r",16,0,255));
	gui.add(EatableParticleField::g.set("g",0,0,255));
	gui.add(EatableParticleField::b.set("b",230,0,255));
	gui.add(EatableParticleField::sameQuadrantProbability.set("sameQuadrantProbability",.001,0,.1));
	gui.add(RibbonParticle::speedFactor.set("speed factor",25,0,200));
	gui.add(RibbonParticle::accelFactor.set("accel factor",1,0,2));
	gui.add(RibbonParticle::friction.set("friction",.08,0,1));
	gui.add(RibbonParticle::strengthRunnawayForce.set("strengthRunnawayForce",10,0,30));
	gui.add(RibbonParticle::maxDistanceRunAway.set("maxDistanceRunAway",2000,0,3000));
	gui.add(RibbonParticle::jitterFreq.set("jitterFreq (Hz)",1,0,10));
	gui.add(RibbonParticle::jitterAmp.set("jitterAmp",.015,0,.1));
	gui.add(RibbonParticle::headSize.set("headSize",4,0,10));
	gui.add(RibbonParticle::thicknessMin.set("thicknessMin",3,1,10));
	gui.add(RibbonParticle::thicknessMax.set("thicknessMax",5,5,20));
	gui.add(RibbonParticle::lengthMin.set("lengthMin",40,1,100));
	gui.add(RibbonParticle::lengthMax.set("lengthMax",70,1,200));
	gui.add(RibbonParticle::highlightDuration.set("highlightDuration (s)",3,.1,10));
	gui.add(RibbonParticle::highlightLenPct.set("highlightLenPct",.3,.1,1));
	gui.add(RibbonParticle::fastSpeedFactor.set("fastSpeedFactor",2,1,10));
	gui.add(RibbonParticle::fastSpeedProbability.set("fastSpeedProbability",.1,0,1));



	particles.setup(20);

	gui.add(EatableParticleField::eatDistance.set("eatDistance",300,0,500));
	gui.add(bbW.set("bb width",350,0,2000));
	gui.add(bbH.set("bb height",540,0,2000));
	gui.add(bbD.set("bb depth",600,0,2000));
	gui.add(bbZ.set("bb z",-bbD*.5,-1000,1000));

	glow.setup();
	gui.add(glow.passes.set("glow passes",4,0,6));

	gui.loadFromFile("settings.xml");

	fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB);
	gui.add(recording.set("recording",false));
	recording.addListener(this,&testApp::record);

	gui.add(fps.set("fps",60,0,120));

#ifdef USE_AUDIO
	soundManager.setup(particles);
#endif

}

void testApp::record(bool & record){
	if(record){
		recorder.setup(ofGetTimestampString()+".mov",ofGetWidth(),ofGetHeight(),30);
		ofSetFrameRate(30);
	}else{
		recorder.encodeVideo();
		ofSetFrameRate(60);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	fps = ofGetFrameRate();
	particles.update(BoundingBox3D(mouseX-bbW*.5,mouseY-bbH*.5,bbZ+bbD*.5,bbW,bbH,bbD));
	soundManager.update();
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	gui.draw();
	if(recording){
		fbo.begin();
		ofClear(0);
	}
	glow.begin(true);
	particles.drawForGlow();
	glow.end();
	glow.draw(0,0);
	particles.draw();

	ofSetColor(255);
	if(recording){
		fbo.end();
		fbo.readToPixels(pixels);
		recorder.addFrame(pixels);
		fbo.draw(0,0);
	}
	//RibbonParticle::tex.draw(0,0);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if(key==' '){
		prevSpeedFactor = RibbonParticle::speedFactor;
		RibbonParticle::speedFactor=0;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	if(key==' '){
		RibbonParticle::speedFactor=prevSpeedFactor;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	particles.runAway();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	particles.goBack();
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
