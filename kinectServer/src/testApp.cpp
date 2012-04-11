#include "testApp.h"


using namespace ofxCv;
ofMesh vertexData;
//----------------------------------------
void ofBox(ofVec3f size){
	ofPushMatrix();
	if(ofGetCoordHandedness() == OF_LEFT_HANDED){
		ofScale(1, 1, -1);
	}

	float w = size.x * .5;
	float h = size.y * .5;
	float d = size.z * .5;

	vertexData.clear();
	if(ofGetStyle().bFill){
		ofVec3f vertices[] = {
			ofVec3f(+w,-h,+d), ofVec3f(+w,-h,-d), ofVec3f(+w,+h,-d), ofVec3f(+w,+h,+d),
			ofVec3f(+w,+h,+d), ofVec3f(+w,+h,-d), ofVec3f(-w,+h,-d), ofVec3f(-w,+h,+d),
			ofVec3f(+w,+h,+d), ofVec3f(-w,+h,+d), ofVec3f(-w,-h,+d), ofVec3f(+w,-h,+d),
			ofVec3f(-w,-h,+d), ofVec3f(-w,+h,+d), ofVec3f(-w,+h,-d), ofVec3f(-w,-h,-d),
			ofVec3f(-w,-h,+d), ofVec3f(-w,-h,-d), ofVec3f(+w,-h,-d), ofVec3f(+w,-h,+d),
			ofVec3f(-w,-h,-d), ofVec3f(-w,+h,-d), ofVec3f(+w,+h,-d), ofVec3f(+w,-h,-d)
		};
		vertexData.addVertices(vertices,24);

		static ofVec3f normals[] = {
			ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0),
			ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0),
			ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1),
			ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0),
			ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0),
			ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1)
		};
		vertexData.addNormals(normals,24);

		static ofVec2f tex[] = {
			ofVec2f(1,0), ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1),
			ofVec2f(1,1), ofVec2f(1,0), ofVec2f(0,0), ofVec2f(0,1),
			ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0), ofVec2f(0,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0)
		};
		vertexData.addTexCoords(tex,24);

		static ofIndexType indices[] = {
			0,1,2, // right top left
			0,2,3, // right bottom right
			4,5,6, // bottom top right
			4,6,7, // bottom bottom left
			8,9,10, // back bottom right
			8,10,11, // back top left
			12,13,14, // left bottom right
			12,14,15, // left top left
			16,17,18, // ... etc
			16,18,19,
			20,21,22,
			20,22,23
		};
		vertexData.addIndices(indices,36);
		vertexData.setMode(OF_PRIMITIVE_TRIANGLES);
		ofGetCurrentRenderer()->draw(vertexData,vertexData.usingColors(),vertexData.usingTextures(),vertexData.usingNormals());
	} else {
		ofVec3f vertices[] = {
			ofVec3f(+w,+h,+d),
			ofVec3f(+w,+h,-d),
			ofVec3f(+w,-h,+d),
			ofVec3f(+w,-h,-d),
			ofVec3f(-w,+h,+d),
			ofVec3f(-w,+h,-d),
			ofVec3f(-w,-h,+d),
			ofVec3f(-w,-h,-d)
		};
		vertexData.addVertices(vertices,8);

		static float n = sqrtf(3);
		static ofVec3f normals[] = {
			ofVec3f(+n,+n,+n),
			ofVec3f(+n,+n,-n),
			ofVec3f(+n,-n,+n),
			ofVec3f(+n,-n,-n),
			ofVec3f(-n,+n,+n),
			ofVec3f(-n,+n,-n),
			ofVec3f(-n,-n,+n),
			ofVec3f(-n,-n,-n)
		};
		vertexData.addNormals(normals,8);

		static ofIndexType indices[] = {
			0,1, 1,3, 3,2, 2,0,
			4,5, 5,7, 7,6, 6,4,
			0,4, 5,1, 7,3, 6,2
		};
		vertexData.addIndices(indices,24);

		vertexData.setMode(OF_PRIMITIVE_LINES);
		ofGetCurrentRenderer()->draw(vertexData, vertexData.usingColors(),vertexData.usingTextures(),vertexData.usingNormals());
	}


	ofPopMatrix();
}

//----------------------------------------
void ofBox(const ofPoint& position, float w, float h, float d){
	ofPushMatrix();
	ofTranslate(position);
	ofBox(ofVec3f(w,h,d));
	ofPopMatrix();
}

void MeshToPointCloud(const ofMesh & pc, int w, int h, ofMesh & mesh){

	for(int y = 0; y < h; y ++) {
		for(int x = 0; x < w; x ++) {

		}
	}
}

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);

	kinects.push_back(new KinectController(0));
	//kinects.push_back(new KinectController(1));

	contourFinder.setMinAreaRadius(5);
	contourFinder.setMaxAreaRadius(160*120/3);


	ofAddListener(avahiBrowser.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahiBrowser.serviceRemoveE,this,&testApp::removedAvahiService);

	avahiBrowser.lookup("_oscit._udp");

	ofSetFrameRate(60);
	ofSetVerticalSync(false);
	frame = 0;

	gui.setup("","settings.xml",1000,10);
	gui.add(bbW.set("bbW",1024,0,10000));
	gui.add(bbH.set("bbH",768,0,10000));
	gui.add(bbD.set("bbD",1024,0,10000));
	gui.add(bbX.set("bbX",512,0,6000));
	gui.add(bbY.set("bbY",512,-3000,3000));
	gui.add(bbZ.set("bbZ",512,0,6000));
	//gui.add(ortho.set("ortho",false));
	ortho = false;
	gui.add(cameraX.set("cameraX",0,-10000,10000));
	gui.add(cameraY.set("cameraY",-3000,-10000,2000));
	gui.add(cameraZ.set("cameraZ",-2500,-10000,2000));
	gui.add(vCameraTilt.set("cameraTilt",90,-180,180));
	gui.add(useFbo.set("useFbo",false));
	/*gui.add(minX.set("minX",2000,-2000,2000));
	gui.add(minY.set("minY",2000,-2000,2000));
	gui.add(minZ.set("minZ",2000,-2000,2000));
	gui.add(maxX.set("maxX",0,-2000,2000));
	gui.add(maxY.set("maxY",0,-2000,2000));
	gui.add(maxZ.set("maxZ",0,-2000,2000));*/
	//gui.add(mainViewport.set("mainViewport",0,0,3));
	//gui.add(positionFilter.set("positionFilter",.2,0,1));
	//gui.add(sizeFilter.set("sizeFilter",.3,0,1));
	mainViewport = 0;
	//gui.add(gpuFilterPasses.set("gpuFilterPasses",3,0,10));
	gui.add(servernum.set("left/right",0,0,1));
	gui.add(drawViewports.set("drawViewports",false));
	gui.add(drawRGB.set("drawRGB",false));
	gui.add(drawDepth.set("drawDepth",false));
	gui.add(drawFbo.set("drawFbo",false));
	gui.add(captureBgBtn.setup("captureBg"));
	gui.add(fps.set("fps",60,0,120));
	gui.loadFromFile("settings.xml");


	captureBg = true;

	captureBgBtn.addListener(this,&testApp::captureBgPressed);


	/*recording = false;
	player.setLoop(true);*/

	background.allocate(320,240, OF_IMAGE_GRAYSCALE);
	thres8Bit.allocate(320,240, OF_IMAGE_GRAYSCALE);
	background.set(0);
	fbo.allocate(160,120,GL_LUMINANCE);
	fbo2.allocate(320,240,GL_LUMINANCE);
	fboViewportFront.allocate(1024,768,GL_RGB);
	fboViewportTop.allocate(1024,768,GL_RGB);
	fboViewportLeft.allocate(1024,768,GL_RGB);
	bgTex.allocate(background);
	texThres.allocate(background);
	ofBackground(0);


	//tracker.setMaximumAge(2);
	/*bgSubstractor.history=10;
	bgSubstractor.nmixtures=8;
	bgSubstractor.backgroundRatio=.3;
	bgSubstractor.noiseSigma = .1;*/

	camTop.setFov(58);
	camLeft.setFov(58);
	camFront.setFov(58);

	camTop.setNearClip(0);
	camTop.setFarClip(15000);

	camLeft.setNearClip(0);
	camLeft.setFarClip(15000);

	camFront.setNearClip(0);
	camFront.setFarClip(15000);

	bb3dShader.load("bb3d.vert","");
	isFrameNew = false;

	ofxXmlSettings settings;
	settings.loadFile("settings.xml");
	string serverip = settings.getValue("serverip","192.168.3.1");
	int serverport = settings.getValue("serverport",5555);
	oscContours.push_back(ofPtr<OscBlobServer>(new OscBlobServer(serverip,serverport,"yeosuserver1")));
}

void testApp::captureBgPressed(bool & pressed){
	//background.set(0);
	captureBg = true;
}

void testApp::newAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "new service" << service.name << "at" << service.host_name << service.ip + ":" << service.port;
	if(service.name=="oscservercontours"){
		oscContours.push_back(ofPtr<OscBlobServer>(new OscBlobServer(service.ip,service.port,service.host_name)));
	}
}

void testApp::removedAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "service removed" << service.name << "at" << service.host_name << service.ip + ":" << service.port;
	if(service.name=="oscservercontours"){
		for(int i=0; i<oscContours.size(); i++){
			if(oscContours[i]->getHostName()==service.host_name){
				oscContours.erase(oscContours.begin()+i);
				return;
			}
		}
	}
}

/*void testApp::contourFind(){
	cv::Mat backgroundMat = toCv(background);
	if(captureBg>0){
		backgroundMat += toCv(kinect.getDistancePixelsRef());
		captureBg--;
	}else if(captureBg==0){
		backgroundMat /= 10;
		cv::GaussianBlur(backgroundMat,backgroundMat,Size(11,11),10);
		captureBg=-1;
	}else{
		// difference threshold
		cv::Mat diffMat = toCv(diff);
		cv::Mat currentMat = toCv(kinect.getDistancePixelsRef());
		cv::Mat gaussCurrentMat = toCv(gaussCurrent);
		cv::GaussianBlur(currentMat,gaussCurrentMat,Size(11,11),10);
		cv::absdiff(backgroundMat, gaussCurrentMat, diffMat);
		//diffMat = toCv(background) - toCv(kinect.getDistancePixelsRef());
		threshold(diff,thresPix,.001);
		thres8Bit = thresPix;

		cv::Mat kernel;
		cv::Mat thresMat = toCv(thres8Bit);
		cv::Point anchor(-1,-1);
		erode(toCv(thres8Bit),thresMat,kernel,anchor,10);

		dilate(toCv(thres8Bit),thresMat,kernel,anchor,5);


		cv::Mat fgMat = toCv(fg);
		bgSubstractor(toCv(thres8Bit),fgMat);
		contourFinder.findContours(fg);

		for(int i=0;i<oscContours.size();i++){
			oscContours[i]->newFrame(frame);
		}


		polylines = contourFinder.getPolylines();
		for(int i=0;i<(int)polylines.size();i++){
			ofPoint centroid = polylines[i].getCentroid2D();
			polylines[i] = polylines[i].getResampledByCount(16);
			float z = kinect.getDistanceAt(centroid);
			for(int j=0;j<oscContours.size();j++){
				oscContours[j]->sendContour(polylines[i],z);
			}
		}
		frame++;

		if(recording){

			//convertColor(kinect.getDepthPixelsRef(),rgbDepth,CV_GRAY2RGB);
			recorder.addFrame(kinect.getRawDepthPixelsRef());
		}
	}
}*/


//--------------------------------------------------------------
void testApp::update(){
	isFrameNew  = false;
	fps = ofGetFrameRate();
	for(int i=0;i<kinects.size();i++){
		kinects[i]->update(bbX,  bbY,  bbZ,  bbW,  bbH,  bbD);
		isFrameNew |= kinects[i]->isFrameNew();
	}
	float w=1024,h=768;

	//camTransform.setupPerspective(true,58);

	if(isFrameNew && drawViewports){

		glEnable(GL_DEPTH_TEST);
		ofPushView();
		fboViewportFront.begin(false);
		ofClear(0);
		ofViewport(0,0,w,h);
		//camFront.disableOrtho();
		//camFront.setupPerspective(true,58);
		camFront.begin();
		if(!useFbo){
			ofSetColor(255,0,0);
			ofNoFill();
			ofBox(ofVec3f(bbX,bbY,-bbZ),bbW,bbH,bbD);
			ofSetColor(255);
		}
		//glMultMatrixf(camTransform.getModelViewMatrix().getPtr());
		for(int i=0;i<(int)kinects.size();i++){
			if(kinects[i]->applyBB){
				bb3dShader.begin();
				bb3dShader.setUniformMatrix4f("m",kinects[i]->camTransform.getModelViewMatrix());
				bb3dShader.setUniform1f("x1",bbX-bbW*.5);
				bb3dShader.setUniform1f("y1",bbY-bbH*.5);
				bb3dShader.setUniform1f("z1",bbZ-bbD*.5);
				bb3dShader.setUniform1f("x2",bbX+bbW*.5);
				bb3dShader.setUniform1f("y2",bbY+bbH*.5);
				bb3dShader.setUniform1f("z2",bbZ+bbD*.5);
				bb3dShader.setUniform1i("useColor",kinects[i]->useColor);
			}
			kinects[i]->drawPointCloud();
			if(kinects[i]->applyBB){
				bb3dShader.end();
			}

		}
		camFront.end();
		fboViewportFront.end();
		ofPopView();
		glDisable(GL_DEPTH_TEST);




		glEnable(GL_DEPTH_TEST);
		ofPushView();
		fboViewportLeft.begin(false);
		ofClear(0);
		ofViewport(0,0,w,h);
		if(ortho) camLeft.enableOrtho();
		else camLeft.disableOrtho();
		//camLeft.setupPerspective(true,58);
		/*camLeft.tilt(pitch-tiltZero);
		camLeft.roll(roll-rollZero);
		camLeft.pan(yaw);*/
		camLeft.resetTransform();
		camLeft.dolly(cameraZ);
		//cam.boom(cameraY);
		camLeft.truck(-6000);
		camLeft.pan(-vCameraTilt);
		camLeft.begin();
		//camTop.draw();
		//camFront.draw();
		if(!useFbo){
			ofSetColor(255,0,0);
			ofNoFill();
			ofBox(ofVec3f(bbX,bbY,-bbZ),bbW,bbH,bbD);
			ofSetColor(255);
		}
		//glMultMatrixf(camTransform.getModelViewMatrix().getPtr());
		//ofTranslate(camFront.getPosition());
		ofBox(30);
		for(int i=0;i<kinects.size();i++){
			if(kinects[i]->applyBB){
				bb3dShader.begin();
				bb3dShader.setUniformMatrix4f("m",kinects[i]->camTransform.getModelViewMatrix());
				bb3dShader.setUniform1f("x1",bbX-bbW*.5);
				bb3dShader.setUniform1f("y1",bbY-bbH*.5);
				bb3dShader.setUniform1f("z1",bbZ-bbD*.5);
				bb3dShader.setUniform1f("x2",bbX+bbW*.5);
				bb3dShader.setUniform1f("y2",bbY+bbH*.5);
				bb3dShader.setUniform1f("z2",bbZ+bbD*.5);
				bb3dShader.setUniform1i("useColor",kinects[i]->useColor);
			}
			kinects[i]->drawPointCloud();
			if(kinects[i]->applyBB){
				bb3dShader.end();
			}
		}
		camLeft.end();
		fboViewportLeft.end();
		ofPopView();
		glDisable(GL_DEPTH_TEST);




		glEnable(GL_DEPTH_TEST);
		ofPushView();
		fboViewportTop.begin(false);
		ofClear(0);
		ofViewport(0,0,w,h);
		if(ortho) camTop.enableOrtho();
		else camTop.disableOrtho();
		//ofMatrix4x4 m = camFront.getGlobalTransformMatrix();
		//camTop.setupPerspective(true,58);
		/*camTop.tilt(pitch-tiltZero);
		camTop.roll(roll-rollZero);
		camTop.pan(yaw);*/
		camTop.resetTransform();
		camTop.dolly(cameraZ);
		camTop.boom(-cameraY);
		camTop.truck(cameraX);
		camTop.tilt(-vCameraTilt);
		camTop.begin();
		//camLeft.draw();
		//camFront.draw();
		if(!useFbo){
			ofSetColor(255,0,0);
			ofNoFill();
			ofBox(ofVec3f(bbX,bbY,-bbZ),bbW,bbH,bbD);
			ofSetColor(255);
		}
		//glMultMatrixf(camTransform.getModelViewMatrix().getPtr());
		//ofTranslate(camFront.getPosition());
		ofBox(30);
		for(int i=0;i<kinects.size();i++){
			if(kinects[i]->applyBB){
				bb3dShader.begin();
				bb3dShader.setUniformMatrix4f("m",kinects[i]->camTransform.getModelViewMatrix());
				bb3dShader.setUniform1f("x1",bbX-bbW*.5);
				bb3dShader.setUniform1f("y1",bbY-bbH*.5);
				bb3dShader.setUniform1f("z1",bbZ-bbD*.5);
				bb3dShader.setUniform1f("x2",bbX+bbW*.5);
				bb3dShader.setUniform1f("y2",bbY+bbH*.5);
				bb3dShader.setUniform1f("z2",bbZ+bbD*.5);
				bb3dShader.setUniform1i("useColor",kinects[i]->useColor);
			}
			kinects[i]->drawPointCloud();
			if(kinects[i]->applyBB){
				bb3dShader.end();
			}
		}
		camTop.end();
		fboViewportTop.end();
		ofPopView();
		glDisable(GL_DEPTH_TEST);

	}



	ofSetColor(255);

	if(isFrameNew && useFbo && drawViewports){
		/*for(int i=1;i<gpuFilterPasses;i++){
			fbo.begin(true);
			ofClear(0);
			fboViewportTop.draw(0,0,fbo.getWidth(),fbo.getHeight());
			fbo.end();

			fbo2.begin(true);
			ofClear(0);
			fbo.draw(0,0,fbo2.getWidth(),fbo2.getHeight());
			fbo2.end();
		}

		fbo2.readToPixels(thres8Bit);


		cv::Mat backgroundMat = toCv(background);
		if(captureBg){
			cv::Mat kernel;
			cv::Mat thresMat = toCv(thres8Bit);
			cv::Point anchor(-1,-1);
			//erode(thresMat,thresMat,kernel,anchor,1);
			dilate(thresMat,thresMat,kernel,anchor,4);
			cv::threshold(thresMat,backgroundMat, 1, 255, THRESH_BINARY);
			captureBg = false;
		}*/

		/*cv::Mat kernel;
		cv::Mat thresMat = toCv(thres8Bit);
		/*cv::Point anchor(-1,-1);
		erode(thresMat,thresMat,kernel,anchor,1);
		dilate(thresMat,thresMat,kernel,anchor,3);*/
		/*thresMat = thresMat - backgroundMat;
		cv::threshold(thresMat,thresMat, 1, 255, THRESH_BINARY);*/

		fbo2.begin();
		fboViewportFront.draw(0,0,fbo2.getWidth(),fbo2.getHeight());
		fbo2.end();
	}

	if(isFrameNew && useFbo && !drawViewports){
		glEnable(GL_DEPTH_TEST);
		ofPushView();
		fbo2.begin(false);
		ofClear(0);
		ofViewport(0,0,320,240);
		//camFront.disableOrtho();
		//camFront.setupPerspective(true,58);
		camFront.begin();
		/*ofTranslate(512,384);
		ofScale(fbo2.getWidth()/w,fbo2.getHeight()/h,1);
		ofTranslate(-512,-384);*/
		//glMultMatrixf(camTransform.getModelViewMatrix().getPtr());
		for(int i=0;i<(int)kinects.size();i++){
			if(kinects[i]->applyBB){
				bb3dShader.begin();
				bb3dShader.setUniformMatrix4f("m",kinects[i]->camTransform.getModelViewMatrix());
				bb3dShader.setUniform1f("x1",bbX-bbW*.5);
				bb3dShader.setUniform1f("y1",bbY-bbH*.5);
				bb3dShader.setUniform1f("z1",bbZ-bbD*.5);
				bb3dShader.setUniform1f("x2",bbX+bbW*.5);
				bb3dShader.setUniform1f("y2",bbY+bbH*.5);
				bb3dShader.setUniform1f("z2",bbZ+bbD*.5);
				bb3dShader.setUniform1i("useColor",kinects[i]->useColor);
			}
			kinects[i]->drawPointCloud();
			if(kinects[i]->applyBB){
				bb3dShader.end();
			}
		}
		camFront.end();
		fbo2.end();
		ofPopView();
		glDisable(GL_DEPTH_TEST);
	}

	if(isFrameNew && useFbo){
		fbo2.readToPixels(thres8Bit);

		contourFinder.findContours(thres8Bit);


		polylines = contourFinder.getPolylines();
		blobs.resize(polylines.size());
		for(int i=0;i<(int)polylines.size();i++){
			blobs[i].pos = polylines[i].getCentroid2D();
			blobs[i].size = polylines[i].getBoundingBox().height;///TWO_PI;
			blobs[i].index = i;
		}
		tracker.track(blobs);
		/*for(int i=0;i<(int)polylines.size();i++){
			if(tracker.existsPrevious(tracker.getLabelFromIndex(i))){
				tracker.getCurrent(tracker.getLabelFromIndex(i)).pos = tracker.getPrevious(tracker.getLabelFromIndex(i)).pos *(1-positionFilter) + tracker.getCurrent(tracker.getLabelFromIndex(i)).pos *positionFilter;
				tracker.getCurrent(tracker.getLabelFromIndex(i)).size = tracker.getPrevious(tracker.getLabelFromIndex(i)).size *(1-sizeFilter) + tracker.getCurrent(tracker.getLabelFromIndex(i)).size *sizeFilter;
			}
		}*/

		ofNoFill();
		for(int i=0;i<oscContours.size();i++){
			oscContours[i]->newFrame(servernum,frame);
		}

		const vector<unsigned int> & labels = tracker.getCurrentLabels();
		if(!labels.empty()){
			int label=9999;
			for(int i=0;i<labels.size();i++){
				if(labels[i]<label && tracker.getAge(labels[i])>2) label=labels[i];
			}
			if(label!=9999){
				const Blob & blob = tracker.getCurrent(label);
				int index = tracker.getIndexFromLabel(label);

				ofPoint centroid = blob.pos;
				float x = centroid.x;
				float diff = 0;
				for(int j=0;j<polylines[index].size();j++){
					if(fabs(polylines[index][j].x-centroid.x)>diff){
						diff =fabs(polylines[index][j].x-centroid.x);
						x = polylines[index][j].x;
					}
				}
				centroid = ofPoint(x,centroid.y)*ofVec2f(1024/320,768/240);
				for(int j=0;j<oscContours.size();j++){
					oscContours[j]->sendBlob(servernum, label, centroid, blob.size);
				}
			}
		}
		frame++;

	}
}

float trackingDistance(const testApp::Blob& a, const testApp::Blob& b) {
	float dx = a.pos.x - b.pos.x;
	float dy = a.pos.y - b.pos.y;
	return sqrtf(dx * dx + dy * dy);
}


//--------------------------------------------------------------
void testApp::draw(){


	if(useFbo){
		for(int i=0;i<(int)polylines.size();i++){
			if(tracker.getAge(tracker.getLabelFromIndex(i))>2){
				const Blob & blob = tracker.getCurrent(tracker.getLabelFromIndex(i));
				polylines[i].draw();
				ofCircle(blob.pos,blob.size);
				ofDrawBitmapString(ofToString(tracker.getLabelFromIndex(i)),blob.pos);
			}
		}
	}
	ofSetColor(255);

	if(drawViewports){
		/*ofFill();
		ofEnableAlphaBlending();
		ofSetColor(255,150);
		ofRect(ofGetWidth()-340,0,340,ofGetHeight());
		ofDisableAlphaBlending();
		ofNoFill();*/

		ofSetColor(255);
		ofDrawBitmapString("front",20,15);
		ofRect(19,19,482,322);
		fboViewportFront.draw(20,20,480,320);

		ofDrawBitmapString("top",510,15);
		ofRect(509,19,482,322);
		fboViewportTop.draw(510,20,480,320);

		ofDrawBitmapString("left",20,355);
		ofRect(19,359,482,322);
		fboViewportLeft.draw(20,360,480,320);
		if(mainViewport==1) fboViewportFront.draw(0,0,1024,768);
		if(mainViewport==3) fboViewportLeft.draw(0,0,1024,768);
		if(mainViewport==2) fboViewportTop.draw(0,0,1024,768);

		/*ofDrawBitmapString("fbo",ofGetWidth()-330,550);
		fbo2.draw(ofGetWidth()-330,550,fbo.getWidth(),fbo.getHeight());


		ofDrawBitmapString("bg",ofGetWidth()-330,550+fbo.getHeight()+20);
		bgTex.loadData(background);
		bgTex.draw(ofGetWidth()-330,550+fbo.getHeight()+20,fbo.getWidth(),fbo.getHeight());


		ofDrawBitmapString("diff thresh",ofGetWidth()-330+fbo.getWidth()+10,550);
		texThres.loadData(thres8Bit);
		texThres.draw(ofGetWidth()-330+fbo.getWidth()+10,550,fbo.getWidth(),fbo.getHeight());*/


		/*ofDrawBitmapString("depth img",ofGetWidth()-330+fbo.getWidth()+10,550+fbo.getHeight()+20);
		for(int i=0;i<kinects.size();i++){
			kinects[i]->drawDepth(ofGetWidth()-330+fbo.getWidth()+10,550+(fbo.getHeight()+20)*(i+1),fbo.getWidth(),fbo.getHeight());
		}*/
	}

	if(drawFbo){
		ofDrawBitmapString("kinect0 fbo",510,355);
		ofRect(509,359,322,242);
		fbo2.draw(510,360);
	}

	if(drawRGB){
		ofSetColor(255);
		ofDrawBitmapString("kinect0",20,15);
		ofRect(19,19,482,322);
		kinects[0]->drawRGB(20,20,480,320);

		ofDrawBitmapString("kinect1",510,15);
		ofRect(509,19,482,322);
		kinects[1]->drawRGB(510,20,480,320);
	}

	if(drawDepth){
		ofSetColor(255);
		ofDrawBitmapString("kinect0",20,355);
		ofRect(19,359,482,322);
		kinects[0]->drawDepth(20,360,480,320);

		ofDrawBitmapString("kinect1",510,355);
		ofRect(509,359,482,322);
		kinects[1]->drawDepth(510,360,480,320);
	}

	gui.draw();
	for(int i=0;i<kinects.size();i++){
		kinects[i]->drawGui();
	}
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
