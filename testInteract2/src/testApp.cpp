#include "testApp.h"
#include "BoundingBox3D.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(0);
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	ofSetWindowShape(1024*2,768);

	float ratio = 1600./float(1024*3);
	gui.setGlow(glow);
	gui.setContoursClient(contoursClient);
	gui.setup(10, 768*ratio + 10);
	gui.load("settings.xml");

	RibbonParticle::generateTexture(RibbonParticle::Voronoi);
	particles.resize(2);

	for(int i=0;i<particles.size();i++){
		particles[i].setup(20);
	}




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

	if(gui.viewportsInFbo){
		glow.setup(1024*3-gui.overlap*2,768);
		fbo.allocate(1024*3-gui.overlap*2,768,GL_RGBA);
	}else{
		glow.setup(1024*3,768);
		fbo.allocate(1024*3,768,GL_RGBA);
	}


	viewportQuads.resize(3);
	eachViewportQuads.resize(3);
	gui.recording.addListener(this,&testApp::record);
	gui.ratio.addListener(this,&testApp::setupMeshes);
	setupMeshes(ratio);


	for(int i=0;i<particles.size();i++){
		particles[i].hide();
	}

	showGui = true;

	//player.load("file:///F:\\0803yuv.avi");
	player.load("Overlay2.mov");
	player.play();

	contoursClient.setup(5555);
	mappingOsc.setup("192.168.3.255",6667);
	RibbonParticle::depthAlphaMax = 0;
	RibbonParticle::depthAlphaMin = -600;

    timming.setup(player);
	ofAddListener(timming.particlesIn,this,&testApp::particlesIn);
	ofAddListener(timming.particlesOut,this,&testApp::particlesOut);
	ofAddListener(timming.mappingIn,this,&testApp::mappingIn);
}

void testApp::setupMeshes(float & ratio){
	for(int i=0;i<3;i++){
		viewportQuads[i].clear();
		viewportQuads[i].addVertex(ofVec3f(i*1024*gui.ratio,0));
		viewportQuads[i].addVertex(ofVec3f((i*1024+1024)*gui.ratio,0));
		viewportQuads[i].addVertex(ofVec3f((i*1024+1024)*gui.ratio,768*gui.ratio));
		viewportQuads[i].addVertex(ofVec3f((i*1024)*gui.ratio,768*gui.ratio));

		viewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap,0));
		viewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap+1024,0));
		viewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap+1024,768));
		viewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap,768));
	}
	for(int i=0;i<3;i++){
		eachViewportQuads[i].clear();
		eachViewportQuads[i].addVertex(ofVec3f(0,0));
		eachViewportQuads[i].addVertex(ofVec3f(1024,0));
		eachViewportQuads[i].addVertex(ofVec3f(1024,768));
		eachViewportQuads[i].addVertex(ofVec3f(0,768));

		eachViewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap,0));
		eachViewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap+1024,0));
		eachViewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap+1024,768));
		eachViewportQuads[i].addTexCoord(ofVec3f(i*1024-i*gui.overlap,768));
	}

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

void testApp::particlesIn(bool & p){
	for(int i=0;i<particles.size();i++){
		particles[i].goBack();
	}
}

void testApp::particlesOut(bool & p){
	for(int i=0;i<particles.size();i++){
		particles[i].hide();
	}
}

void testApp::mappingIn(bool & m){
	ofxOscMessage msg;
	msg.setAddress("play");
	mappingOsc.sendMessage(msg);
}

//--------------------------------------------------------------
void testApp::update(){
	if(gui.showVideo){
		player.update();
	}
	if(gui.runawayEvents) timming.update();

	cam.setFov(gui.fov);
	gui.fps = ofGetFrameRate();
	ofVec3f userPos;
	if(gui.mouseUser){
		gui.virtualMouseX = float(mouseX)/gui.ratio - fbo.getWidth()*.5;
				gui.virtualMouseY = - fbo.getHeight()*.5;//float(mouseY)/ratio - fbo.getHeight()*.5;
				userPos.set(gui.virtualMouseX,	gui.virtualMouseY,	PSystem::bbZ+PSystem::bbD*.5-1024);
		particles[0].setUserPosition(userPos);
		particles[1].setUserPosition(userPos);
	}else{
		contoursClient.update();
		for(int server=0;server<2;server++){
			for(int i=0;i<1 && i<contoursClient.size(server);i++){
				if(server){
					gui.virtualMouseX = (1024-contoursClient.getBlobs(server)[i].pos.x)*fbo.getWidth()*.5/1024. + gui.offsetRight;
				}else{
					gui.virtualMouseX = (1024-contoursClient.getBlobs(server)[i].pos.x)*fbo.getWidth()*.5/1024. - fbo.getWidth()*.5 + gui.offsetLeft;
				}
				gui.virtualMouseY = - fbo.getHeight()*.5;
				userPos.set(gui.virtualMouseX,	gui.virtualMouseY,	PSystem::bbZ+PSystem::bbD*.5-1024);
				if(contoursClient.getBlobs(server)[i].suddenChange){
					particles[server].runAway();
				}

				particles[server].setUserPosition(userPos);
			}
		}
	}

	for(int i=0;i<particles.size();i++){
		particles[i].update();
	}
	/*if(!gui.viewportsInFbo){
		userPos.set(mouseX-PSystem::bbW*.5-ofGetWidth()*.5,
					mouseY-PSystem::bbH*.5-ofGetHeight()*.5,
					PSystem::bbZ+PSystem::bbD*.5-ofGetWidth());
	}else{
		gui.virtualMouseX = float(mouseX)/gui.ratio - fbo.getWidth()*.5;
		gui.virtualMouseY = - fbo.getHeight()*.5;//float(mouseY)/ratio - fbo.getHeight()*.5;
		userPos.set(gui.virtualMouseX,	gui.virtualMouseY,	PSystem::bbZ+PSystem::bbD*.5-1024);
	}*/

#ifdef USE_SOUND
	soundManager.update();
#endif

}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(255);
	if(showGui) gui.draw();
	glow.begin(false);
	cam.begin(viewport);
	for(int i=0;i<particles.size();i++){
		particles[i].drawForGlow();
	}
	cam.end();
	glow.end();

	if(gui.showVideo && !gui.overlapVideo){
		player.draw(0,0,player.getWidth()*gui.ratio,player.getHeight()*gui.ratio);
	}

	if(gui.viewportsInFbo){
		fbo.begin(true);
		ofClear(0);
		if(gui.showVideo && gui.overlapVideo){
			player.draw(0,0,player.getWidth(),player.getHeight());
		}
		glow.draw(0,0);
	}else{
		glow.draw(0,768,1024*3,-768);
	}

	cam.begin(viewport);
	for(int i=0;i<particles.size();i++){
		particles[i].draw();
	}
	cam.end();


	if(gui.drawGrid){
		ofSetLineWidth(4);
		ofSetColor(255);
		for(int x=0; x<1024*3*gui.ratio; x+=1024*3/30*gui.ratio){
			ofLine(x,0,x,fbo.getHeight()*gui.ratio);
		}
		for(int y=0; y<768*gui.ratio; y+=768/10*gui.ratio){
			ofLine(0,y,1024*3*gui.ratio,y);
		}
		ofSetLineWidth(1);
	}

	if(gui.viewportsInFbo){
		fbo.end();
		//fbo.readToPixels(pixels);
		//recorder.addFrame(pixels);
		//fbo.draw(-viewportNum*1024+viewportNum*overlap,0);
	}


	ofSetColor(255);
	if(gui.viewportsInFbo){
		if(gui.drawAllViewports){
			if(gui.drawOverlap){
				fbo.getTextureReference().bind();
				for(int i=0;i<3;i++){
					viewportQuads[i].setMode(OF_PRIMITIVE_TRIANGLE_FAN);
					viewportQuads[i].draw();
				}
				fbo.getTextureReference().unbind();
			}else{
				fbo.draw(0,0,fbo.getWidth()*gui.ratio,fbo.getHeight()*gui.ratio);
			}

			if(gui.drawViewports){
				ofSetLineWidth(4);
				ofSetColor(255);
				for(int i=0;i<3;i++){
					viewportQuads[i].setMode(OF_PRIMITIVE_LINE_LOOP);
					viewportQuads[i].drawWireframe();
				}
				ofSetLineWidth(1);
			}

			if(gui.drawOverlapMarks){
				ofSetLineWidth(4);
				ofSetColor(255,0,0,100);
				for(int i=1; i<3; i++){
					float x = (1024*i-gui.overlap)*gui.ratio;
					ofLine(x,0,x,fbo.getHeight()*gui.ratio);
				}
				ofSetColor(0,255,0,100);
				for(int i=1; i<3; i++){
					float x = (1024*i+gui.overlap)*gui.ratio;
					ofLine(x,0,x,fbo.getHeight()*gui.ratio);
				}
				ofSetLineWidth(1);
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
	}

	if(showGui){
		ofDrawBitmapString("video sec. " + ofToString(player.getDuration()*player.getPosition(),2),20,20);
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

		for(int i=0;i<particles.size();i++){
			particles[i].hide();
		}
	}
	if( key == 'H')
		for(int i=0;i<particles.size();i++){
			particles[i].goBack();
		}

	if(key=='g'){
		showGui = !showGui;
		if(showGui)
			ofShowCursor();
		else
			ofHideCursor();
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
	//particles.runAway();
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	//particles.goBack();
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
