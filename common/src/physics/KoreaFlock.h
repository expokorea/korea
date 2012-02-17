/*
 *  KoreaFlock.h
 *  mainServer
 *
 *  Created by Chris on 2/16/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "KoreaParticle.h"
#include "vectorField.h"
#include "ofGui.h"

class KoreaFlock{

	public:

		vector<KoreaParticle> particles;
		vector<KoreaParticle> vParticles;
		
		void setup( int total);
		void update();
		void draw();
		void drawForGlow();
		
		VectorField vectorField;
		bool bUseVectorField;
		
		ofFloatSlider speed;
		ofFloatSlider vFieldForce;
		ofToggle useTrails;
		ofPanel gui;
		
};