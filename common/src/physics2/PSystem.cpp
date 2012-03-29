#include "ofMain.h"
#include "PSystem.h"


void PSystem::setup()
{
	RibbonParticle::generateTexture(RibbonParticle::Voronoi);
	particles.resize(20);
	for(int i=0;i<particles.size();i++){
		particles[i].setup();
	}
	
	field.setup();
	
}

void PSystem::update()
{
	field.update(ofGetLastFrameTime(),bb);
	for(int i=0;i<particles.size();i++){
		particles[i].target.set(field.getParticle(&particles[i]));
		particles[i].update(ofGetLastFrameTime(),bb);
	}
}

void PSystem::draw()
{
	for(int i=0;i<particles.size();i++){
		particles[i].drawForGlow();
	}
}
	
void PSystem::updateBoundingBox(BoundingBox3D bbUser)
{
	bb = bbUser;
}

	
