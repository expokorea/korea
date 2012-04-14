/*
 * KinectController.cpp
 *
 *  Created on: 23/03/2012
 *      Author: arturo
 */

#include "KinectController.h"
#define ANGLE_FROM_ACCEL 1

#if ANGLE_FROM_PCL_GROUND_PLANE
#undef Success
#include "segmentation.h"
#include "ofxPCL.h"
#endif


bool insideBB3D(const ofPoint & p, const ofPoint & position, const ofPoint & size){
	return p.x > position.x - size.x * .5 && p.x < position.x + size.x * .5 &&
			p.y > position.y - size.y * .5 && p.y < position.y + size.y * .5 &&
			p.z > position.z - size.z * .5 && p.z < position.z + size.z * .5;
}

KinectController::KinectController(int device) {
	setup(device);
}

KinectController::~KinectController() {
	// TODO Auto-generated destructor stub
}


void KinectController::setup(int device){

	kinect.init(false,true,true);
	kinect.setRegistration(true);
	kinect.open(device);


	oscConfig.setup(6666 + device);
	avahi.start("osckinectserverconfig","_oscit._udp",6666 + device);

	gui.setup("","settings_kinectdevice"+ofToString(device)+".xml",1000+200*(device+1),10);
	gui.add(tilt.set("tilt",0,-31,31));
	gui.add(nearClip.set("nearClip",3000,0,10000));
	gui.add(farClip.set("farClip",6500,0,10000));
	gui.add(x.set("x",0,-5000,5000));
	gui.add(y.set("y",0,-5000,5000));
	gui.add(z.set("z",0,-5000,5000));
	gui.add(correctAngle.set("correctAngle",false));
	gui.add(applyBB.set("applyBB",false));
	gui.add(pitch.set("pitch",0,-180,180));
	gui.add(roll.set("roll",0,-180,180));
	gui.add(yaw.set("yaw",0,-180,180));
	gui.add(rollZero.set("rollZero",0,-180,180));
	gui.add(tiltZero.set("tiltZero",0,-180,180));
	gui.add(rotZeroBtn.setup("set zero rotation"));
	gui.add(useColor.set("useColor",true));

	gui.loadFromFile("settings_kinectdevice"+ofToString(device)+".xml");


	kinect.setDepthClipping(nearClip,farClip);

	tilt.addListener(this,&KinectController::tiltChanged);
	rotZeroBtn.addListener(this,&KinectController::rotZeroPressed);
	nearClip.addListener(this,&KinectController::clippingChanged);
	farClip.addListener(this,&KinectController::clippingChanged);

	camTransform.setFov(58);

	mesh.setMode(OF_PRIMITIVE_POINTS);
	mesh.setUsage(GL_STREAM_DRAW);
	step = 2;
	for(int y = 0; y < 480; y +=step) {
		for(int x = 0; x < 640; x +=step) {
			mesh.addVertex(ofVec3f());
			mesh.addTexCoord(ofVec2f(x,y));
		}
	}
}

void KinectController::rotZeroPressed(bool & pressed){
	rollZero = roll.getValue();
	tiltZero = pitch.getValue();
}

void KinectController::tiltChanged(int & tilt){
	kinect.setCameraTiltAngle(tilt);
}

void KinectController::clippingChanged(float & clip){
	kinect.setDepthClipping(nearClip,farClip);
}

void KinectController::updateAnalisys(ofxKinect & kinect, float bbX, float bbY, float bbZ, float bbW, float bbH, float bbD){

	if(kinect.isFrameNew()){
		static int counter = 0;
		int w = 640;
		int h = 480;

#if ANGLE_FROM_ACCEL
		//ofVec3f worldOffset(worldX,worldY,worldZ);
		ofMatrix4x4 m = camTransform.getModelViewMatrix();
		int i = 0;
		for(int y = 0; y < h; y +=step) {
			for(int x = 0; x < w; x +=step) {
				const short & distance = kinect.getRawDepthPixelsRef()[x+y*w];
				//if(distance > 0) {
					const ofVec3f  & v = kinect.getWorldCoordinateAt(x, y, distance);
					/*if(correctAngle){
						v = v*m;
					}
					if(!applyBB || insideBB3D(v,ofVec3f(bbX,bbY,bbZ), ofVec3f(bbW, bbH, bbD))){*/
						mesh.getVertices()[i] = v;
					/*}else{
						mesh.getVertices()[i].set(0,0,0);
					}*/
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


void KinectController::update(float bbX, float bbY, float bbZ, float bbW, float bbH, float bbD){
	pitch = pitch*.9 + .1*kinect.getPitch();
	roll = roll*.9 + .1*kinect.getRoll();

	camTransform.resetTransform();
	camTransform.tilt(-pitch+tiltZero);
	camTransform.roll(-roll+rollZero);
	camTransform.pan(-yaw);
	camTransform.setPosition(x,y,z);

	camTransformView.resetTransform();
	camTransformView.tilt(pitch-tiltZero);
	camTransformView.roll(roll-rollZero);
	camTransformView.pan(yaw);
	camTransformView.setPosition(x,y,z);

	kinect.update();
	updateAnalisys(kinect, bbX,  bbY,  bbZ,  bbW,  bbH,  bbD);


	while(oscConfig.hasWaitingMessages()){
		ofxOscMessage msg;
		oscConfig.getNextMessage(&msg);

		if(msg.getAddress()=="tilt" && tilt!=msg.getArgAsInt32(0)){
			tilt=msg.getArgAsInt32(0);
			kinect.setCameraTiltAngle(tilt);
		}else if(msg.getAddress()=="clipping"){
			float nearClip = msg.getArgAsFloat(0);
			float farClip = msg.getArgAsFloat(1);
			kinect.setDepthClipping(nearClip,farClip);
		}
	}
}

bool KinectController::isFrameNew(){
	return kinect.isFrameNew();
}

void KinectController::drawPointCloud() {
	glPointSize(3*step);
	ofPushMatrix();
	if(correctAngle) glMultMatrixf(camTransformView.getModelViewMatrix().getPtr());
	// the projected points are 'upside down' and 'backwards'
	ofScale(1, 1, -1);
	//ofTranslate(0, 0, -1000); // center the points a bit
	if(useColor)
		kinect.getTextureReference().bind();
	mesh.draw();
	if(useColor)
		kinect.getTextureReference().unbind();
	ofPopMatrix();
}

void KinectController::drawGui(){
	gui.draw();
}

void KinectController::drawDepth(float x, float y, float w, float h){
	kinect.drawDepth(x,y,w,h);
}

void KinectController::drawRGB(float x, float y, float w, float h){
	kinect.draw(x,y,w,h);
}
