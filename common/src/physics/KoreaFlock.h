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

class KoreaFlock{

	public:

		vector<KoreaParticle> particles;
		vector<KoreaParticle> vParticles;
		
		void setup( int total);
		void update();
		void draw();
		
		VectorField vectorField;
		bool bUseVectorField;
	
};