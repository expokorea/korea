/*
 * OscContoursClient.cpp
 *
 *  Created on: 14/02/2012
 *      Author: arturo
 */

#include "OscContoursClient.h"

OscContoursClient::OscContoursClient() {
	// TODO Auto-generated constructor stub

}


void OscContoursClient::setup(int port){
	blobs.resize(2);
	prevBlobs.resize(2);
	osc.setup(port);
#ifdef USE_ZEROCONF
	avahi.start("oscservercontours","_oscit._udp",port);
#endif
}

void OscContoursClient::update(){
	ofxOscMessage msg;
	while(osc.hasWaitingMessages()){
		osc.getNextMessage(&msg);
		if(msg.getAddress()=="frame"){
			int server=msg.getArgAsInt32(0);
			prevBlobs[server] = blobs[server];
			blobs[server].clear();
			continue;
		}

		if(msg.getAddress()=="blob"){
			int server=msg.getArgAsInt32(0);
			blobs[server].push_back(Blob(server,msg.getArgAsInt32(1),
				ofPoint(msg.getArgAsFloat(2), msg.getArgAsFloat(3)),
				msg.getArgAsFloat(4)));

			for(int i=0;i<prevBlobs[server].size();i++){
				if(blobs[server].back().id==prevBlobs[server][i].id){
					blobs[server].back().sizeHist = prevBlobs[server][i].sizeHist;
					break;
				}
			}
			float max=-1, min=99;

			deque<float>::iterator it;
			for(it=blobs[server].back().sizeHist.begin();it!=blobs[server].back().sizeHist.end();it++){
				if(max<*it) max=*it;
				if(min>*it) min=*it;
			}

			blobs[server].back().sizeHist.push_back(blobs[server].back().size);
			if(blobs[server].back().sizeHist.size()>runAwayFramesThreshold){
				blobs[server].back().sizeHist.pop_front();
			}

			if(blobs[server].back().sizeHist.size()>5 && blobs[server].back().size-(max+min)*0.5 > (max+min)*0.5 * (1-runAwaySensitivity)){
				blobs[server].back().suddenChange = true;
			}else{ // if(!eyesOpened && avg-(max+min)*0.5 > .2){
				blobs[server].back().suddenChange = false;
			}

			//cout <<msg.getArgAsFloat(1) << msg.getArgAsFloat(2) << endl;
		}

		/*if(msg.getAddress()=="contour"){
			int n = msg.getNumArgs();
			contours.push_back(ofPolyline());
			ofPolyline & contour = contours.back();
			float z = msg.getArgAsFloat(0);
			for(int i=1;i<n;i+=2){
				ofPoint p(msg.getArgAsFloat(i),msg.getArgAsFloat(i+1));
				contour.addVertex(p);
			}
			contour.close();
		}*/
	}
}

vector<OscContoursClient::Blob> & OscContoursClient::getBlobs(int server){
	return blobs[server];
}

unsigned int OscContoursClient::size(int server){
	return blobs[server].size();
}
