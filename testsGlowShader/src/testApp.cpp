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
	gui.add(kernelSize.setup("kernelSize",6,1,20));
	gui.add(brightness.setup("brightness",1.4,1,20));
	gui.add(framerate.setup("framerate",0,0,80));
	gui.add(speed.setup("speed",8,1,80));
	gui.add(radius.setup("radius",3,1,10));
	gui.add(r.setup("r",16,0,255));
	gui.add(g.setup("g",0,0,255));
	gui.add(b.setup("b",230,0,255));
	gui.add(lightOn.setup("light",false));

	lightOn.addListener(this,&testApp::lightOnChanged);

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

	glEnable(GL_DEPTH_TEST);
	ofSetColor(ofColor(r,g,b));
	ofSetRectMode(OF_RECTMODE_CENTER);
	fbo1.begin();
	ofClear(0);
	ofSeedRandom(0);
	float t = ofGetElapsedTimef()*ofMap(speed,1,80,0,1)*ofMap(speed,1,80,0,1);
	for(int i=0;i<100;i++){
		ofColor color(r,g,b);
		color.setBrightness(ofMap(ofNoise(i/100.*ofRandom(1),ofGetElapsedTimef()/5.,ofRandom(1)),0,1,20,255));
		ofSetColor(color);
		ofSphere(ofNoise(i/100.,t,ofRandom(1))*ofGetWidth(),
				ofNoise(ofRandom(1),i/100.,t)*ofGetHeight(),
				ofNoise(i/100.,ofRandom(1),t)*ofGetHeight()*.4,
				radius);
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
	ofDisableLighting();
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
