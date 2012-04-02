#include "testApp.h"
#include "BoundingBox3D.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	ratio = 1600./float(1024*3);
	gui.setGlow(glow);
	gui.setup(10, 768*ratio + 10);
	gui.load("settings.xml");

	RibbonParticle::generateTexture(RibbonParticle::Voronoi);
	particles.setup(20);

	
	

#ifdef USE_AUDIO
	soundManager.setup(particles);
#endif

	cam.setFov(60);
	gui.overlap = 152;
	viewport.set(0,0,1024*3-gui.overlap*2,768);
	ofFbo::Settings settings;
	settings.width = 1024*3-gui.overlap*2;
	settings.height = 768;
	settings.internalformat = GL_RGBA;
	settings.numSamples = 4;
	settings.useDepth = false;
	settings.useStencil = false;
	fbo.allocate(1024*3-gui.overlap*2,768,GL_RGBA);

	if(gui.viewportsInFbo)
		glow.setup(1024*3-gui.overlap*2,768);
	else
		glow.setup(1024,768);

	viewportQuads.resize(3);
	for(int i=0;i<3;i++){
		viewportQuads[i].addVertex(ofVec3f(i*1024*ratio,0));
		viewportQuads[i].addVertex(ofVec3f((i*1024+1024)*ratio,0));
		viewportQuads[i].addVertex(ofVec3f((i*1024+1024)*ratio,768*ratio));
		viewportQuads[i].addVertex(ofVec3f((i*1024)*ratio,768*ratio));

		viewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap,0));
		viewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap+1024,0));
		viewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap+1024,768));
		viewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap,768));
	}

	eachViewportQuads.resize(3);
	for(int i=0;i<3;i++){
		eachViewportQuads[i].addVertex(ofVec3f(0,0));
		eachViewportQuads[i].addVertex(ofVec3f(1024,0));
		eachViewportQuads[i].addVertex(ofVec3f(1024,768));
		eachViewportQuads[i].addVertex(ofVec3f(0,768));

		eachViewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap,0));
		eachViewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap+1024,0));
		eachViewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap+1024,768));
		eachViewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap,768));
	}
	gui.recording.addListener(this,&testApp::record);
}

void testApp::record(bool & record){
	if(record){
		recorder.setup(ofGetTimestampString()+".mov",ofGetWidth(),ofGetHeight(),25);
		ofSetFrameRate(25);
	}else if(gui.recording){
		recorder.encodeVideo();
		ofSetFrameRate(60);
	}
}

//--------------------------------------------------------------
void testApp::update(){
	cam.setFov(gui.fov);
	gui.fps = ofGetFrameRate();
	ofVec3f userPos;
	if(!gui.viewportsInFbo){
		int viewportDisplacement=0;
		viewport.x = -gui.viewportNum*1024+gui.viewportNum*gui.overlap;
		if(gui.viewportNum==0) viewportDisplacement = -1024;
		if(gui.viewportNum==1) viewportDisplacement = -gui.overlap;
		if(gui.viewportNum==2) viewportDisplacement = 1024-gui.overlap*2;
		userPos.set(mouseX-PSystem::bbW*.5-ofGetWidth()*.5+viewportDisplacement,
					mouseY-PSystem::bbH*.5-ofGetHeight()*.5,
					PSystem::bbZ+PSystem::bbD*.5-ofGetWidth());
	}else{
		gui.virtualMouseX = float(mouseX)/ratio - fbo.getWidth()*.5;
		gui.virtualMouseY = - fbo.getHeight()*.5;//float(mouseY)/ratio - fbo.getHeight()*.5;
		userPos.set(gui.virtualMouseX,	gui.virtualMouseY,	PSystem::bbZ+PSystem::bbD*.5-ofGetWidth());
	}

	particles.update(userPos);
#ifdef USE_SOUND
	soundManager.update();
#endif
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	gui.draw();
	glow.begin(false);
	cam.begin(viewport);
	particles.drawForGlow();
	cam.end();
	glow.end();

	if(gui.viewportsInFbo){
		fbo.begin(true);
		ofClear(0);
	}
		glow.draw(0,0);

		cam.begin(viewport);
		particles.draw();
		cam.end();

	if(gui.viewportsInFbo){
		fbo.end();
		//fbo.readToPixels(pixels);
		//recorder.addFrame(pixels);
		//fbo.draw(-viewportNum*1024+viewportNum*overlap,0);
	}

	ofSetColor(255);
	if(gui.drawAllViewports){
		if(gui.drawOverlap){
			fbo.getTextureReference().bind();
			for(int i=0;i<3;i++){
				viewportQuads[i].setMode(OF_PRIMITIVE_TRIANGLE_FAN);
				viewportQuads[i].draw();
			}
			fbo.getTextureReference().unbind();
		}else{
			fbo.draw(0,0,fbo.getWidth()*ratio,fbo.getHeight()*ratio);
		}

		if(gui.drawViewports){
			for(int i=0;i<3;i++){
				viewportQuads[i].setMode(OF_PRIMITIVE_LINE_LOOP);
				viewportQuads[i].drawWireframe();
			}
		}

		if(gui.drawOverlapMarks){
			ofSetColor(255,0,0,100);
			for(int i=1; i<3; i++){
				float x = (1024*i-gui.overlap)*ratio;
				ofLine(x,0,x,fbo.getHeight()*ratio);
			}
			ofSetColor(0,255,0,100);
			for(int i=1; i<3; i++){
				float x = (1024*i+gui.overlap)*ratio;
				ofLine(x,0,x,fbo.getHeight()*ratio);
			}
		}
	}else{
		ofPushMatrix();
		ofTranslate(gui.guiRender.getPosition().x+gui.guiRender.getWidth()+10,0);
		fbo.getTextureReference().bind();
		eachViewportQuads[gui.viewportNum].setMode(OF_PRIMITIVE_TRIANGLE_FAN);
		eachViewportQuads[gui.viewportNum].draw();
		fbo.getTextureReference().unbind();
		if(gui.drawViewports){
			eachViewportQuads[gui.viewportNum].setMode(OF_PRIMITIVE_LINE_LOOP);
			eachViewportQuads[gui.viewportNum].drawWireframe();
		}
		ofPopMatrix();
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

	if(key=='r'){
		gui.recording = !gui.recording;
	}

	if(key==OF_KEY_RIGHT) gui.viewportNum++;
	if(key==OF_KEY_LEFT) gui.viewportNum--;
	
	if( key == 'h')
	{
		particles.hide();
	}
	if( key == 'H') particles.goBack();
	
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
