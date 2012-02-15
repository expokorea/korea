#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	shader.load("","glow.frag");
	shaderBokeh.load("","bokeh.frag");
	ofSetVerticalSync(true);

	ofFbo::Settings settings;
	settings.depthAsTexture = true;
	settings.width = ofGetWidth();
	settings.height = ofGetHeight();
	settings.internalformat = GL_RGBA;

	fbo1.allocate(settings);
	fbo2.allocate(settings);
	ofBackground(0);

	gui.setup("blur");
	gui.add(blurAmnt.setup("blurAmnt",1,0,5));
	gui.add(kernelSize.setup("kernelSize",8,1,20));
	gui.add(brightness.setup("brightness",1.4,1,20));
	gui.add(framerate.setup("framerate",0,0,80));
	gui.add(r.setup("r",255,0,255));
	gui.add(g.setup("g",255,0,255));
	gui.add(b.setup("b",255,0,255));

}

//--------------------------------------------------------------
void testApp::update(){
	blurAmnt = ofMap(sin(ofGetElapsedTimef()),-1,1,.75f,1.25f);
	framerate = ofGetFrameRate();
}

//--------------------------------------------------------------
void testApp::draw(){
	glEnable(GL_DEPTH_TEST);
	ofSetColor(ofColor(r,g,b));
	ofSetRectMode(OF_RECTMODE_CENTER);
	fbo1.begin();
	ofClear(0);
	ofSeedRandom(0);
	for(int i=0;i<100;i++){
		ofCircle(ofNoise(i,ofGetElapsedTimef()/5.,ofRandom(1))*ofGetWidth(),ofNoise(ofRandom(1),i,ofGetElapsedTimef()/5.)*ofGetHeight(),3);
	}
	fbo1.end();




	ofSetRectMode(OF_RECTMODE_CORNER);

	fbo2.begin();
	ofClear(0);
	shader.begin();
	shader.setUniformTexture("src_tex_unit0",fbo1.getTextureReference(),0);
	shader.setUniform1f("blurAmnt",blurAmnt);
	shader.setUniform1i("direction",0);
	shader.setUniform1i("kernelSize",kernelSize);
	shader.setUniform1f("brightness",brightness);
	fbo1.draw(0,0);
	shader.end();
	fbo2.end();

	shader.begin();
	shader.setUniformTexture("src_tex_unit0",fbo2.getTextureReference(),0);
	shader.setUniform1f("blurAmnt",blurAmnt);
	shader.setUniform1i("direction",1);
	shader.setUniform1i("kernelSize",kernelSize);
	shader.setUniform1f("brightness",brightness);
	fbo2.draw(0,0);
	shader.end();

	glDisable(GL_DEPTH_TEST);
	ofSetRectMode(OF_RECTMODE_CORNER);
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
