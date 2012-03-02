#pragma once

#include "ofMain.h"

class ofxTimeUtils{
public:
	enum Mode{
		Time,
		Frame
	};
	static void setMode(Mode _mode, float _fps=-1){
		mode = _mode;
		if(_fps!=-1)fps = _fps;
		else fps=ofGetFrameRate();
		oneFrame = 1./fps;
	}

	static float getElapsedTimef(){
		if(mode==Time){
			return ofGetElapsedTimef();
		}else{
			return oneFrame*ofGetFrameNum();
		}
	}

private:
	static Mode mode;
	static float fps;
	static float oneFrame;
};
