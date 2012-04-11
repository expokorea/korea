/*
 * Gui.cpp
 *
 *  Created on: 31/03/2012
 *      Author: arturo
 */

#include "Gui.h"
#include "RibbonParticle.h"
#include "PSystem.h"
#include "EatableParticleField.h"
#include "ParticleFlocker.h"

void Gui::setGlow(Glow & _glow){
	glow = &_glow;
}

void Gui::setContoursClient(OscContoursClient & _contoursClient){
	contoursClient = &_contoursClient;
}

void Gui::setup(int x, int y){

	guiParticles.setup("particles","settings.xml",x,y);
	guiParticles.add(RibbonParticle::r.set("r",16,0,255));
	guiParticles.add(RibbonParticle::g.set("g",0,0,255));
	guiParticles.add(RibbonParticle::b.set("b",230,0,255));
	guiParticles.add(RibbonParticle::rTex.set("r texture",16,0,255));
	guiParticles.add(RibbonParticle::gTex.set("g texture",0,0,255));
	guiParticles.add(RibbonParticle::bTex.set("b texture",230,0,255));
	guiParticles.add(RibbonParticle::rLines.set("r lines",255,0,255));
	guiParticles.add(RibbonParticle::gLines.set("g lines",255,0,255));
	guiParticles.add(RibbonParticle::bLines.set("b lines",255,0,255));
	guiParticles.add(RibbonParticle::speedFactor.set("speed factor",25,0,200));
	guiParticles.add(RibbonParticle::accelFactor.set("accel factor",1,0,2));
	guiParticles.add(RibbonParticle::friction.set("friction",.08,0,1));
	guiParticles.add(RibbonParticle::strengthRunnawayForce.set("strengthRunnawayForce",10,0,30));
	guiParticles.add(RibbonParticle::maxDistanceRunAway.set("maxDistanceRunAway",2000,0,3000));
	guiParticles.add(RibbonParticle::jitterFreq.set("jitterFreq (Hz)",1,0,10));
	guiParticles.add(RibbonParticle::jitterAmp.set("jitterAmp",.015,0,.1));
	guiParticles.add(RibbonParticle::headSize.set("headSize",4,0,10));
	guiParticles.add(RibbonParticle::thicknessMin.set("thicknessMin",3,1,10));
	guiParticles.add(RibbonParticle::thicknessMax.set("thicknessMax",5,5,20));
	guiParticles.add(RibbonParticle::lengthMin.set("lengthMin",40,1,100));
	guiParticles.add(RibbonParticle::lengthMax.set("lengthMax",70,1,200));
	guiParticles.add(RibbonParticle::highlightDuration.set("highlightDuration (s)",3,.1,10));
	guiParticles.add(RibbonParticle::highlightLenPct.set("highlightLenPct",.3,.1,1));
	guiParticles.add(RibbonParticle::fastSpeedFactor.set("fastSpeedFactor",2,1,10));
	guiParticles.add(RibbonParticle::fastSpeedProbability.set("fastSpeedProbability",.1,0,1));
	guiParticles.add(RibbonParticle::depthAlphaMin.set("depthAlphaMin",-600,-1000,1000));
	guiParticles.add(RibbonParticle::depthAlphaMax.set("depthAlphaMax",100,-1000,1000));


	guiPSystem.setup("p. system","settings.xml",x+guiParticles.getWidth()+10,y);
	guiPSystem.add(PSystem::bbW.set("bb width",350,0,2000));
	guiPSystem.add(PSystem::bbH.set("bb height",540,0,2000));
	guiPSystem.add(PSystem::bbD.set("bb depth",600,0,2000));
	guiPSystem.add(PSystem::bbZ.set("bb z",-PSystem::bbD*.5,-1000,1000));
	guiPSystem.add(PSystem::bbY.set("bb y",-768*.5,-1000,1000));
	guiPSystem.add(PSystem::drawBB.set("drawBB",false));
	guiPSystem.add(contoursClient->runAwaySensitivity.set("runAwaySensitivity",.90,.1,.99));
	guiPSystem.add(contoursClient->runAwayFramesThreshold.set("runAwayFramesThreshold",30,5,60));
	guiPSystem.add(ParticleFlocker::totalToFlock.set("total folllowing",5,0,20) );
	guiPSystem.add(runawayEvents.set("runawayEvents",false));

	guiField.setup("p. field","settings.xml",x+guiParticles.getWidth()+10,y + guiPSystem.getHeight()+10);
	guiField.add(EatableParticleField::r.set("r",16,0,255));
	guiField.add(EatableParticleField::g.set("g",0,0,255));
	guiField.add(EatableParticleField::b.set("b",230,0,255));
	guiField.add(EatableParticleField::sameQuadrantProbability.set("sameQuadrantProbability",.001,0,.1));
	guiField.add(EatableParticleField::eatDistance.set("eatDistance",300,0,500));


	guiRender.setup("render","settings.xml",x+guiParticles.getWidth()+guiPSystem.getWidth()+20,y);
	guiRender.add(viewportsInFbo.set("viewportsInFbo",true));
	guiRender.add(drawViewports.set("drawViewports",true));
	guiRender.add(drawOverlap.set("drawOverlap",true));
	guiRender.add(drawOverlapMarks.set("drawOverlapMarks",false));
	guiRender.add(drawAllViewports.set("drawAllViewports",true));
	guiRender.add(drawGrid.set("drawGrid",false));
	guiRender.add(showVideo.set("showVideo",true));
	guiRender.add(overlapVideo.set("overlapVideo",true));
	guiRender.add(mouseUser.set("mouseUser",true));
	guiRender.add(ratio.set("ratio",1600./(1024.*3.),.1,1));
	guiRender.add(glow->passes.set("glow passes",4,0,6));
	guiRender.add(viewportNum.set("viewport",0,0,2));
	guiRender.add(overlap.set("overlap",152,0,200));
	guiRender.add(recording.set("recording",false));
	guiRender.add(fps.set("fps",60,0,120));
	guiRender.add(virtualMouseX.set("virtualMouseX",0,-2000,2000));
	guiRender.add(virtualMouseY.set("virtualMouseY",60,-2000,2000));
	guiRender.add(fov.set("fov",60,1,130));
	guiRender.add(offsetLeft.set("offsetLeft",0,-1024,1024));
	guiRender.add(offsetRight.set("offsetRight",0,-1024,1024));

}

void Gui::load(string file){
	guiParticles.loadFromFile("settings.xml");
	guiPSystem.loadFromFile("settings.xml");
	guiField.loadFromFile("settings.xml");
	guiRender.loadFromFile("settings.xml");

}

void Gui::draw(){
	guiParticles.draw();
	guiPSystem.draw();
	guiField.draw();
	guiRender.draw();

}
