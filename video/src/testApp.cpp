#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofDisableTextureEdgeHack();
	video.load("Overlay2.avi",true);
	video.play();
	ofBackground(0);
	ofSetVerticalSync(true);
	gui.setup("");
	gui.add(x.set("x",0,-2000,2000));
	gui.add(y.set("y",0,-2000,2000));
	gui.add(z.set("z",0,-2000,2000));
	//fbo.allocate(ofGetWidth(),ofGetHeight(),GL_RGB);
}

int vfps = 0;
int vframesOneSec=0;
int vlastFrame = ofGetElapsedTimeMillis();
//--------------------------------------------------------------
void testApp::update(){
	video.update();
	if(video.isFrameNew()){
		vframesOneSec++;
		if(ofGetElapsedTimeMillis()-vlastFrame>=1000){
			vlastFrame = ofGetElapsedTimeMillis();
			vfps = vframesOneSec;
			vframesOneSec = 0;
		}
	}
	//glRasterPos2i(x,y);
}

//--------------------------------------------------------------
void testApp::draw(){
//video.draw(0,0);
	/*glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho((GLdouble)0,(GLdouble)ofGetWidth(),(GLdouble)ofGetHeight(),(GLdouble)0,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();*/
	//fbo.begin();
	//glDrawPixels(video.getWidth(),video.getHeight(),GL_RGB, GL_UNSIGNED_BYTE, video.getPixels());
	video.draw();
	gui.draw();
	//fbo.end();
	//fbo.draw(0,0);
	//glBitmap(video.getWidth(),video.getHeight(),0,0,0,0,video.getPixels());
/*ofSetColor(255);
for(int i=0;i<10000;i++){
	ofCircle(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()),ofRandom(10));
}
ofSetColor(255,0,0);*/
ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
ofDrawBitmapString(ofToString(vfps),20,40);
//video.draw(0,0);
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
