#include "testApp.h"
#include "BoundingBox3D.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);
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


	field.setup();
	gui.add(field.eatDistance.set("eatDistance",300,0,500));
	gui.add(bbW.set("bb width",350,0,2000));
	gui.add(bbH.set("bb height",540,0,2000));
	gui.add(bbD.set("bb depth",600,0,2000));


	particles.resize(20);
	for(int i=0;i<particles.size();i++){
		particles[i].setup();
	}

	glow.setup();
	gui.add(glow.passes.set("glow passes",4,0,6));
	ofEnableBlendMode(OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void testApp::update(){
	BoundingBox3D bb(mouseX-bbW*.5,mouseY-bbH*.5,-bbD*.5,bbW,bbH,bbD);
	field.update(ofGetLastFrameTime(),bb);
	for(int i=0;i<particles.size();i++){
		particles[i].target.set(field.getParticle(&particles[i]));
		particles[i].update(ofGetLastFrameTime(),bb);
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	gui.draw();
	glow.begin(true);
	ofPushMatrix();
	//cam.begin();
	for(int i=0;i<particles.size();i++){
		particles[i].drawForGlow();
	}
	ofSetColor(RibbonParticle::r,RibbonParticle::g,RibbonParticle::b);
	field.draw();
	ofPopMatrix();
	glow.end();
	glow.draw(0,0);
	for(int i=0;i<particles.size();i++){
		particles[i].draw();
	}
	ofSetColor(RibbonParticle::r,RibbonParticle::g,RibbonParticle::b);
	field.draw();
	//cam.end();
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

	for(int i=0;i<particles.size();i++){
		particles[i].runAway();
	}
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

	for(int i=0;i<particles.size();i++){
		particles[i].goBack();
	}
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
