#include "testApp.h"
using namespace ofxCv;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	kinect.init(false,false,true);
	kinect.open();

	kinect.setDepthClipping(3000,6500);

	contourFinder.setMinAreaRadius(30);
	contourFinder.setMaxAreaRadius(kinect.getWidth()*kinect.getHeight()/3);

	oscConfig.setup(6667);
	avahi.start("osckinectserverconfig","_oscit._udp",6667);


	ofAddListener(avahiBrowser.serviceNewE,this,&testApp::newAvahiService);
	ofAddListener(avahiBrowser.serviceRemoveE,this,&testApp::removedAvahiService);

	avahiBrowser.lookup("_oscit._udp");

	ofSetFrameRate(40);
	frame = 0;
	tilt = 0;
	farThreshold = 0;
	nearThreshold = 1;

	recording = false;
	usePlayer = true;
	player.load("2012-03-05-16-50-52-613");

	nearThresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	farThresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	thresPix.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	background.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	diff.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	thres8Bit.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	fg.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_GRAYSCALE);
	rgbDepth.allocate(kinect.getWidth(), kinect.getHeight(), OF_IMAGE_COLOR);

	texThres.allocate(kinect.getWidth(), kinect.getHeight(), ofGetGlInternalFormat(fg));
	captureBg = 10;
	bgSubstractor.history=10;
	bgSubstractor.nmixtures=8;
	bgSubstractor.backgroundRatio=.3;
	bgSubstractor.noiseSigma = .1;
}


void testApp::newAvahiService(ofxAvahiService & service){
	ofLogVerbose() << "new service" << service.name << "at" << service.host_name << service.ip + ":" << service.port;
	if(service.name=="oscservercontours"){
		oscContours.push_back(ofPtr<OscContourServer>(new OscContourServer(service.ip,service.port,service.host_name)));
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

template<class Kinect>
void testApp::updateAnalisys(Kinect & kinect){

	kinect.update();
	if(kinect.isFrameNew()){
		if(captureBg>0){
			toCv(background) += toCv(kinect.getDistancePixelsRef());
			captureBg--;
		}else if(captureBg==0){
			toCv(background) /= 10;
			captureBg=-1;
		}else{
			// difference threshold
			cv::Mat diffMat = toCv(diff);
			cv::absdiff(toCv(background), toCv(kinect.getDistancePixelsRef()), diffMat);
			//diffMat = toCv(background) - toCv(kinect.getDistancePixelsRef());
			threshold(diff,thresPix,.01);
			thres8Bit = thresPix;

			/*cv::Mat kernel;
			cv::Mat thresMat = toCv(thres8Bit);
			cv::Point anchor(-1,-1);
			erode(toCv(thres8Bit),thresMat,kernel,anchor,10);

			dilate(toCv(thres8Bit),thresMat,kernel,anchor,5);*/

			// near far threshold
			/*threshold(kinect.getDistancePixelsRef(),nearThresPix,nearThreshold, true);
			threshold(kinect.getDistancePixelsRef(),farThresPix,farThreshold);
			toCv(thresPix) = toCv(nearThresPix) & toCv(farThresPix);*/


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
	}
}

//--------------------------------------------------------------
void testApp::update(){
	if(usePlayer){
		updateAnalisys(player);
	}else{
		updateAnalisys(kinect);
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

//--------------------------------------------------------------
void testApp::draw(){
	texThres.loadData(fg);
	if(usePlayer){
		player.drawDistance(0,0);
	}else{
		kinect.drawDistance(0,0);
	}
	for(int i=0;i<(int)polylines.size();i++){
		polylines[i].draw();
	}

	texThres.draw(660,0);

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
