/*
 * VideoPlayer.cpp
 *
 *  Created on: 08/04/2012
 *      Author: arturo
 */

#include "VideoPlayer.h"

void VideoPlayer::load(string _path, string _nameFront, string _nameRear, string _nameSubLFE, bool _usePBO){
	usePBO = _usePBO;
	path = _path;
	nameFront = _nameFront;
	nameRear = _nameRear;
	nameSubLFE = _nameSubLFE;

	surroundPlayer = ofPtr<SurroundVideoPlayer>(new SurroundVideoPlayer);
	surroundPlayer->setDeviceFront(nameFront);
	surroundPlayer->setDeviceRear(nameRear);
	surroundPlayer->setDeviceSubLFE(nameSubLFE);
	surroundPlayer->loadMovie(path);
	player.setPlayer(surroundPlayer);
	player.setUseTexture(false);
	//player.setVolume(volume);
	volume.addListener(this,&VideoPlayer::setVolume);
	//player.loadMovie(path);
	player.setLoopState(OF_LOOP_NORMAL);

	if(usePBO){
		tex.allocate(player.getPixelsRef());
		pbo.allocate(tex,1);
	}else{
		fbo.allocate(player.getWidth(),player.getHeight(),GL_RGB);
		tex = fbo.getTextureReference();
	}

}

void VideoPlayer::setVolume(float & volume){
	//player.setVolume(volume);
}

void VideoPlayer::play(){
	surroundPlayer->play();
}


void VideoPlayer::update(){
	player.update();
	if(player.getIsMovieDone()){
		player.stop();
		surroundPlayer = ofPtr<SurroundVideoPlayer>(new SurroundVideoPlayer);
		surroundPlayer->setDeviceFront(nameFront);
		surroundPlayer->setDeviceRear(nameRear);
		surroundPlayer->setDeviceSubLFE(nameSubLFE);
		surroundPlayer->loadMovie(path);
		player.setPlayer(surroundPlayer);
		//player.setVolume(volume);
		surroundPlayer->play();
	}
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

