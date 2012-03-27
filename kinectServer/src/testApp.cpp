#include "testApp.h"


#define ANGLE_FROM_ACCEL 1

#if ANGLE_FROM_PCL_GROUND_PLANE
#undef Success
#include "segmentation.h"
#include "ofxPCL.h"
#endif

using namespace ofxCv;

static ofMesh vertexData;


bool insideBB3D(const ofPoint & p, const ofPoint & position, const ofPoint & size){
	return p.x > position.x - size.x * .5 && p.x < position.x + size.x * .5 &&
			p.y > position.y - size.y * .5 && p.y < position.y + size.y * .5 &&
			p.z > position.z - size.z * .5 && p.z < position.z + size.z * .5;
}

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
	//ofSetLogLevel(OF_LOG_VERBOSE);
	kinect.init(false,true,true);
	kinect.setRegistration(true);
	kinect.open();

	kinect.setDepthClipping(3000,6500);

	contourFinder.setMinAreaRadius(5);
	contourFinder.setMaxAreaRadius(160*120/3);

	oscConfig.setup(6667);
	avahi.start("osckinectserverconfig","_oscit._udp",6667);


	ofAddListener(avahiBrowser.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahiBrowser.serviceRemoveE,this,&testApp::removedAvahiService);

	avahiBrowser.lookup("_oscit._udp");

	//ofSetFrameRate(40);
	ofSetVerticalSync(true);
	frame = 0;

	gui.setup("","settings.xml",1034,10);
	gui.add(tilt.set("tilt",0,-31,31));
	gui.add(farThreshold.set("farThreshold",0,0,1));
	gui.add(nearThreshold.set("nearThreshold",1,0,1));
	gui.add(bbW.set("bbW",1024,0,10000));
	gui.add(bbH.set("bbH",768,0,10000));
	gui.add(bbD.set("bbD",1024,0,10000));
	gui.add(bbX.set("bbX",512,0,6000));
	gui.add(bbY.set("bbY",512,-3000,3000));
	gui.add(bbZ.set("bbZ",512,0,6000));
	gui.add(correctAngle.set("correctAngle",false));
	gui.add(usePlayer.set("usePlayer",false));
	gui.add(paused.set("paused",false));
	gui.add(ortho.set("ortho",false));
	gui.add(useFbo.set("useFbo",false));
	gui.add(applyBB.set("applyBB",false));
	gui.add(pitch.set("pitch",0,-180,180));
	gui.add(roll.set("roll",0,-180,180));
	gui.add(yaw.set("yaw",0,-180,180));
	gui.add(rollZero.set("rollZero",0,-180,180));
	gui.add(tiltZero.set("tiltZero",0,-180,180));
	gui.add(cameraX.set("cameraX",0,-6000,2000));
	gui.add(cameraY.set("cameraY",-3000,-6000,2000));
	gui.add(cameraZ.set("cameraZ",-2500,-6000,2000));
	gui.add(vCameraTilt.set("cameraTilt",90,-180,180));
	/*gui.add(minX.set("minX",2000,-2000,2000));
	gui.add(minY.set("minY",2000,-2000,2000));
	gui.add(minZ.set("minZ",2000,-2000,2000));
	gui.add(maxX.set("maxX",0,-2000,2000));
	gui.add(maxY.set("maxY",0,-2000,2000));
	gui.add(maxZ.set("maxZ",0,-2000,2000));*/
	gui.add(mainViewport.set("mainViewport",0,0,3));
	gui.add(positionFilter.set("positionFilter",.2,0,1));
	gui.add(sizeFilter.set("sizeFilter",.3,0,1));
	gui.add(gpuFilterPasses.set("gpuFilterPasses",3,0,10));
	gui.add(drawViewports.set("drawViewports",false));
	gui.add(captureBgBtn.setup("captureBg"));
	gui.add(rotZeroBtn.setup("set zero rotation"));
	gui.add(fps.set("fps",60,0,120));

	recording = false;
	usePlayer = false;
	player.load("2012-03-05-16-50-52-613");

	/*nearThresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	farThresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	thresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	background.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	diff.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	thres8Bit.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	fg.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	gaussCurrent.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	rgbDepth.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_COLOR);*/

	captureBg = true;

	tilt.addListener(this,&testApp::tiltChanged);
	captureBgBtn.addListener(this,&testApp::captureBgPressed);
	rotZeroBtn.addListener(this,&testApp::rotZeroPressed);


	player.setLoop(true);

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


	tracker.setMaximumAge(2);
	/*bgSubstractor.history=10;
	bgSubstractor.nmixtures=8;
	bgSubstractor.backgroundRatio=.3;
	bgSubstractor.noiseSigma = .1;*/

	camTransform.setFov(58);
	camTop.setFov(58);
	camLeft.setFov(58);
	camFront.setFov(58);


	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.setUsage(GL_STREAM_DRAW);
	int step = 3;
	for(int y = 0; y < 480; y +=step) {
		for(int x = 0; x < 640; x +=step) {
			mesh.addVertex(ofVec3f());
			mesh.addTexCoord(ofVec2f(x,y));
		}
	}

}

void testApp::captureBgPressed(bool & pressed){
	//background.set(0);
	captureBg = true;
}

void testApp::rotZeroPressed(bool & pressed){
	rollZero = roll.getValue();
	tiltZero = pitch.getValue();
}

void testApp::tiltChanged(int & tilt){
	kinect.setCameraTiltAngle(tilt);
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

template<class Kinect>
void testApp::updateAnalisys(Kinect & kinect){

	if(!paused) kinect.update();
	if(paused || kinect.isFrameNew()){
		static int counter = 0;
		int w = 640;
		int h = 480;

#if ANGLE_FROM_ACCEL
		//ofVec3f worldOffset(worldX,worldY,worldZ);
		ofMatrix4x4 m = camTransform.getModelViewMatrix();;
		int i = 0;
		int step = 3;
		for(int y = 0; y < h; y +=step) {
			for(int x = 0; x < w; x +=step) {
				const short & distance = kinect.getRawDepthPixelsRef()[x+y*w];
				//if(distance > 0) {
					ofVec3f v = kinect.getWorldCoordinateAt(x, y, distance);
					if(correctAngle){
						v = m*v;
						//v += worldOffset;
						/*v += ofVec3f(-ofGetViewportWidth()*.5,-ofGetViewportHeight()*.5,0);
						v.rotate(pitch-tiltZero,ofVec3f(1,0,0));
						v.rotate(roll-rollZero,ofVec3f(0,0,1));
						v.rotate(yaw,ofVec3f(0,1,0));
						v += ofVec3f(ofGetViewportWidth()*.5,ofGetViewportHeight()*.5,0);*/
						/*if(v.x<minX) minX = v.x;
						if(v.y<minY) minY = v.y;
						if(v.z<minZ) minZ = v.z;
						if(v.x>maxX) maxX = v.x;
						if(v.y>maxY) maxY = v.y;
						if(v.z>maxZ) maxZ = v.z;*/
					}
					if(!applyBB || insideBB3D(v,ofVec3f(bbX,bbY,bbZ), ofVec3f(bbW, bbH, bbD))){
						mesh.getVertices()[i] = v;
					}else{
						mesh.getVertices()[i].set(0,0,0);
					}
				//}
				i++;
			}
		}
#elif ANGLE_FROM_PCL_GROUND_PLANE
		pcl::PointCloud<pcl::PointXYZ>::Ptr pcPtr = pcl::PointCloud<pcl::PointXYZ>::Ptr(new pcl::PointCloud<pcl::PointXYZ>);
		for(int y = 0; y < h; y += step) {
			for(int x = 0; x < w; x += step) {
				//if(kinect.getDistanceAt(x, y) > 0) {
					float z = kinect.getDistanceAt(x, y);
					pcPtr->push_back(pcl::PointXYZ(x,y,z));
				//}
			}
		}
		pcPtr->width=640;
		pcPtr->height=480;

		pcl::ModelCoefficients::Ptr planeCoeffs = fitPlane<pcl::PointXYZ>(pcPtr,10,5);
		plane.set(planeCoeffs->values[0],planeCoeffs->values[1],planeCoeffs->values[2],planeCoeffs->values[3]);

		for(int i=0;i<pcPtr->points.size();i++){
			ofVec3f v(pcPtr->points[i].x,pcPtr->points[i].y,pcPtr->points[i].z);
			if(plane.distanceToPoint(v)>60)
				mesh.addVertex( kinect.getWorldCoordinateAt(v.x, v.y, v.z) );
		}

		//pcPtr = findAndSubtractPlane<pcl::PointXYZ>(pcPtr,60,5);
		//ofxPCL::toOf(pcPtr,mesh,1,1,1);
#elif CV_ANALISYS
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
					oscContours[j]->sendBlob(polylines[i],z);
				}
			}
			frame++;

			if(recording){

				//convertColor(kinect.getDepthPixelsRef(),rgbDepth,CV_GRAY2RGB);
				recorder.addFrame(kinect.getRawDepthPixelsRef());
			}
		}
#endif
	}
}

//--------------------------------------------------------------
void testApp::update(){
	fps = ofGetFrameRate();
	camTransform.resetTransform();
	camTransform.tilt(pitch-tiltZero);
	camTransform.roll(roll-rollZero);
	camTransform.pan(yaw);

	if(usePlayer){
		updateAnalisys(player);
	}else{
		updateAnalisys(kinect);
		pitch = pitch*.9 + .1*kinect.getPitch();
		roll = roll*.9 + .1*kinect.getRoll();
	}

	while(oscConfig.hasWaitingMessages()){
		ofxOscMessage msg;
		oscConfig.getNextMessage(&msg);

		if(msg.getAddress()=="tilt" && tilt!=msg.getArgAsInt32(0)){
			tilt=msg.getArgAsInt32(0);
			kinect.setCameraTiltAngle(tilt);
		}else if(msg.getAddress()=="farThreshold"){
			farThreshold=msg.getArgAsInt32(0);
		}else if(msg.getAddress()=="nearThreshold"){
			nearThreshold=msg.getArgAsInt32(0);
		}else if(msg.getAddress()=="record"){
			recording = msg.getArgAsInt32(0);
			if(recording && !usePlayer){
				recorder.setup(ofGetTimestampString(),kinect.getWidth(),kinect.getHeight(),30,true,OF_IMAGE_FORMAT_PNG,16);
			}else{
				//recorder.encodeVideo();
			}
		}else if(msg.getAddress()=="player"){
			usePlayer = msg.getArgAsInt32(0);
			if(usePlayer){
				player.load("2012-03-05-16-50-52-613");
			}
		}else if(msg.getAddress()=="clipping"){
			float nearClip = msg.getArgAsFloat(0);
			float farClip = msg.getArgAsFloat(1);
			if(usePlayer) player.setDepthClipping(nearClip,farClip);
			else kinect.setDepthClipping(nearClip,farClip);
		}
	}
}

void testApp::drawPointCloud() {
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, 1, -1);
	//ofTranslate(0, 0, -1000); // center the points a bit
	if(!useFbo)
		kinect.getTextureReference().bind();
	mesh.draw();
	if(!useFbo)
		kinect.getTextureReference().unbind();
	ofPopMatrix();
}

float trackingDistance(const testApp::Blob& a, const testApp::Blob& b) {
	float dx = a.pos.x - b.pos.x;
	float dy = a.pos.y - b.pos.y;
	return sqrtf(dx * dx + dy * dy);
}

void testApp::drawScene(float xRot, bool drawBB){
	glEnable(GL_DEPTH_TEST);
	drawPointCloud();
	glDisable(GL_DEPTH_TEST);

}

//--------------------------------------------------------------
void testApp::draw(){

	float w=1024,h=768;

	//camTransform.setupPerspective(true,58);

	if(drawViewports){
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
		drawPointCloud();
		camFront.end();
		fboViewportFront.end();
		ofPopView();
		glDisable(GL_DEPTH_TEST);


		if(mainViewport==1) fboViewportFront.draw(0,0,1024,768);

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
		camTop.draw();
		camFront.draw();
		if(!useFbo){
			ofSetColor(255,0,0);
			ofNoFill();
			ofBox(ofVec3f(bbX,bbY,-bbZ),bbW,bbH,bbD);
			ofSetColor(255);
		}
		//glMultMatrixf(camTransform.getModelViewMatrix().getPtr());
		//ofTranslate(camFront.getPosition());
		ofBox(30);
		drawPointCloud();
		camLeft.end();
		fboViewportLeft.end();
		ofPopView();
		glDisable(GL_DEPTH_TEST);

		if(mainViewport==3) fboViewportLeft.draw(0,0,1024,768);
	}

	if(drawViewports || useFbo){

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
		camLeft.draw();
		camFront.draw();
		if(!useFbo){
			ofSetColor(255,0,0);
			ofNoFill();
			ofBox(ofVec3f(bbX,bbY,-bbZ),bbW,bbH,bbD);
			ofSetColor(255);
		}
		//glMultMatrixf(camTransform.getModelViewMatrix().getPtr());
		//ofTranslate(camFront.getPosition());
		ofBox(30);
		drawPointCloud();
		camTop.end();
		fboViewportTop.end();
		ofPopView();
		glDisable(GL_DEPTH_TEST);

		if(mainViewport==2) fboViewportTop.draw(0,0,1024,768);
	}


	ofSetColor(255);

	if(useFbo){
		for(int i=1;i<gpuFilterPasses;i++){
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
		if(captureBg && !paused){
			cv::Mat kernel;
			cv::Mat thresMat = toCv(thres8Bit);
			cv::Point anchor(-1,-1);
			//erode(thresMat,thresMat,kernel,anchor,1);
			dilate(thresMat,thresMat,kernel,anchor,4);
			cv::threshold(thresMat,backgroundMat, 1, 255, THRESH_BINARY);
			captureBg = false;
		}

		cv::Mat kernel;
		cv::Mat thresMat = toCv(thres8Bit);
		/*cv::Point anchor(-1,-1);
		erode(thresMat,thresMat,kernel,anchor,1);
		dilate(thresMat,thresMat,kernel,anchor,3);*/
		thresMat = thresMat - backgroundMat;
		cv::threshold(thresMat,thresMat, 1, 255, THRESH_BINARY);


		contourFinder.findContours(thres8Bit);


		polylines = contourFinder.getPolylines();
		blobs.resize(polylines.size());
		for(int i=0;i<(int)polylines.size();i++){
			blobs[i].pos = polylines[i].getCentroid2D()*ofVec2f(1024/320,768/240);
			blobs[i].size = polylines[i].getPerimeter()/TWO_PI;
		}
		tracker.track(blobs);
		for(int i=0;i<(int)polylines.size();i++){
			if(tracker.existsPrevious(tracker.getLabelFromIndex(i))){
				tracker.getCurrent(tracker.getLabelFromIndex(i)).pos = tracker.getPrevious(tracker.getLabelFromIndex(i)).pos *(1-positionFilter) + tracker.getCurrent(tracker.getLabelFromIndex(i)).pos *positionFilter;
				tracker.getCurrent(tracker.getLabelFromIndex(i)).size = tracker.getPrevious(tracker.getLabelFromIndex(i)).size *(1-sizeFilter) + tracker.getCurrent(tracker.getLabelFromIndex(i)).size *sizeFilter;
			}
		}


		for(int i=0;i<oscContours.size();i++){
			oscContours[i]->newFrame(frame);
		}

		const vector<unsigned int> & labels = tracker.getCurrentLabels();
		for(int i=0;i<(int)labels.size();i++){
			const Blob & blob = tracker.getCurrent(tracker.getLabelFromIndex(i));
			for(int j=0;j<oscContours.size();j++){
				oscContours[j]->sendBlob(labels[i], blob.pos, blob.size);
			}
		}
		frame++;

		for(int i=0;i<(int)polylines.size();i++){
			if(tracker.getAge(tracker.getLabelFromIndex(i))>2){
				const Blob & blob = tracker.getCurrent(tracker.getLabelFromIndex(i));
				polylines[i].draw();
				ofCircle(blob.pos,blob.size);
				ofDrawBitmapString(ofToString(tracker.getLabelFromIndex(i)),blob.pos);
			}
		}

		ofSetColor(255);
	}

	ofSetColor(255);

	if(drawViewports){
		ofFill();
		ofEnableAlphaBlending();
		ofSetColor(255,150);
		ofRect(ofGetWidth()-340,0,340,ofGetHeight());
		ofDisableAlphaBlending();
		ofNoFill();

		ofSetColor(255);
		ofDrawBitmapString("front",ofGetWidth()-330,15);
		ofRect(ofGetWidth()-330,20,320,240);
		fboViewportFront.draw(ofGetWidth()-330,20,320,240);

		ofDrawBitmapString("top",ofGetWidth()-330,275);
		ofRect(ofGetWidth()-330,280,320,240);
		fboViewportTop.draw(ofGetWidth()-330,280,320,240);

		ofDrawBitmapString("fbo",ofGetWidth()-330,550);
		fbo2.draw(ofGetWidth()-330,550,fbo.getWidth(),fbo.getHeight());


		ofDrawBitmapString("bg",ofGetWidth()-330,550+fbo.getHeight()+20);
		bgTex.loadData(background);
		bgTex.draw(ofGetWidth()-330,550+fbo.getHeight()+20,fbo.getWidth(),fbo.getHeight());


		ofDrawBitmapString("diff thresh",ofGetWidth()-330+fbo.getWidth()+10,550);
		texThres.loadData(thres8Bit);
		texThres.draw(ofGetWidth()-330+fbo.getWidth()+10,550,fbo.getWidth(),fbo.getHeight());


		ofDrawBitmapString("depth img",ofGetWidth()-330+fbo.getWidth()+10,550+fbo.getHeight()+20);
		if(usePlayer){
			player.drawDepth(ofGetWidth()-330+fbo.getWidth()+10,550+fbo.getHeight()+20,fbo.getWidth(),fbo.getHeight());
		}else{
			kinect.drawDepth(ofGetWidth()-330+fbo.getWidth()+10,550+fbo.getHeight()+20,fbo.getWidth(),fbo.getHeight());
		}
	}

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
