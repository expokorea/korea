/*
 * Glow.cpp
 *
 *  Created on: 15/02/2012
 *      Author: arturo
 */

#include "Glow.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"

Glow::Glow() {
	// TODO Auto-generated constructor stub

}

Glow::~Glow() {
	// TODO Auto-generated destructor stub
}

void Glow::setup(){
	shader.load("","glow.frag");

	ofFbo::Settings settings;
	settings.depthAsTexture = true;
	settings.width = ofGetWidth();
	settings.height = ofGetHeight();
	settings.internalformat = GL_RGBA;
	settings.dethInternalFormat = GL_DEPTH_COMPONENT32;
	settings.useStencil = false;

	fbo1.allocate(settings);
	fbo2.allocate(settings);
}

void Glow::begin(){
	fbo1.begin();
}

void Glow::end(){
	fbo1.end();

	ofSetColor(255);
	for(int i=0;i<passes;i++){
		fbo2.begin();
		ofClear(0,0);
		shader.begin();
		shader.setUniformTexture("src_tex_unit0",fbo1.getTextureReference(),0);
		shader.setUniform1i("direction",0);
		//shader.setUniform1i("kernelSize",kernelSize);
		shader.setUniform1f("brightness",brightness);
		fbo1.draw(0,0);
		shader.end();
		fbo2.end();

		fbo1.begin();
		ofClear(0,0);
		shader.begin();
		shader.setUniformTexture("src_tex_unit0",fbo2.getTextureReference(),0);
		shader.setUniform1i("direction",1);
		//shader.setUniform1i("kernelSize",kernelSize);
		shader.setUniform1f("brightness",brightness);
		fbo2.draw(0,0);
		shader.end();
		fbo1.end();
	}
}

void Glow::draw(float x, float y){
	fbo1.draw(x,y);
}
