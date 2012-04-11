/*
 * VideoPlayer.cpp
 *
 *  Created on: 08/04/2012
 *      Author: arturo
 */

#include "VideoPlayer.h"

void VideoPlayer::load(string path, bool _usePBO){
	usePBO = _usePBO;
	player.setUseTexture(false);
	player.loadMovie(path);
	player.setLoopState(OF_LOOP_NORMAL);

	if(usePBO){
		tex.allocate(player.getPixelsRef());
		pbo.allocate(tex,1);
	}else{
		fbo.allocate(player.getWidth(),player.getHeight(),GL_RGB);
		tex = fbo.getTextureReference();
	}

}

void VideoPlayer::play(){
	player.play();
}


void VideoPlayer::update(){
	player.update();
}

bool VideoPlayer::isFrameNew(){
	return player.isFrameNew();
}

void VideoPlayer::draw(float x, float y, float w, float h){
	if(player.isFrameNew()){
		if(usePBO){
			//int time = ofGetElapsedTimeMicros();
			pbo.loadData(player.getPixelsRef());
			//cout << ofGetElapsedTimeMicros() - time << endl;
			//time = ofGetElapsedTimeMicros();
			pbo.updateTexture();
			//cout << "time tex" << ofGetElapsedTimeMicros() - time << endl;
		}else{
			//int time = ofGetElapsedTimeMicros();
			fbo.begin();
			glDrawPixels(player.getWidth(),player.getHeight(),GL_RGB, GL_UNSIGNED_BYTE, player.getPixels());
			fbo.end();
			//cout << ofGetElapsedTimeMicros() - time << endl;
		}
	}
	tex.draw(x,y,w,h);
}

int VideoPlayer::getWidth(){
	return player.getWidth();
}

int VideoPlayer::getHeight(){
	return player.getHeight();
}

float VideoPlayer::getPosition(){
	return player.getPosition();
}

float VideoPlayer::getDuration(){
	return player.getDuration();
}

unsigned long VideoPlayer::getCurrentFrame(){
	return player.getCurrentFrame();
}
