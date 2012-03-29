/*
 * RibbonParticle.cpp
 *
 *  Created on: 28/03/2012
 *      Author: arturo
 */
#include "RibbonParticle.h"
#include "triangulate.h"
#include "ofxVoronoi.h"
#include "BoundingBox3D.h"

ofxParameter<int> RibbonParticle::r;
ofxParameter<int> RibbonParticle::g;
ofxParameter<int> RibbonParticle::b;
ofxParameter<int> RibbonParticle::rTex,RibbonParticle::gTex,RibbonParticle::bTex;
ofxParameter<int> RibbonParticle::rLines,RibbonParticle::gLines,RibbonParticle::bLines;
ofxParameter<float> RibbonParticle::speedFactor;
ofxParameter<float> RibbonParticle::friction;
ofxParameter<float> RibbonParticle::strengthRunnawayForce;
ofxParameter<float> RibbonParticle::maxDistanceRunAway;
ofxParameter<float> RibbonParticle::jitterFreq;
ofxParameter<float> RibbonParticle::jitterAmp;
ofxParameter<float> RibbonParticle::accelFactor;
ofxParameter<float> RibbonParticle::headSize;
ofxParameter<float> RibbonParticle::thicknessMin;
ofxParameter<float> RibbonParticle::thicknessMax;
ofxParameter<int> RibbonParticle::lengthMin;
ofxParameter<int> RibbonParticle::lengthMax;
ofFbo RibbonParticle::tex;
ofImage RibbonParticle::head;
int RibbonParticle::headRibbonCoordsWidth = 40;
ofPoint RibbonParticle::headRibbonCoordsZero(138,212);

static ofVec3f lerp(const ofVec3f & origin, const ofVec3f & dst, float t){
	return ofVec3f(ofLerp(origin.x,dst.x,t),ofLerp(origin.y,dst.y,t),ofLerp(origin.z,dst.z,t));
}

template<typename t>
static t filter(t prevValue, t value, float factor){
	return prevValue*(1.-factor) + value*factor;
}


ofVec3f calcNormal( const ofVec3f &a, const ofVec3f &b, const ofVec3f &c, const ofVec3f &d )
{
	ofVec3f n1,n2;
	ofVec3f v1=(c-a);
	ofVec3f v2=(d-a);
	ofVec3f v3=(b-a);
	n1=v1.cross(v2);
	n2=v2.cross(v3);
	return ((n1+n2)*.5).normalized();
}



bool compareXYZ(const XYZ & p1, const XYZ & p2){
	if (p1.x < p2.x)
	   return false;
	else if (p1.x > p2.x)
	   return true;
	else
	   return false;
	return p1.x<p2.x;
}

void RibbonParticle::generateTexture(TexMode mode){
	ofMesh textureMesh;
	textureMesh.setMode(OF_PRIMITIVE_LINES);

	int width = 200*4;
	int height = 100;
	if(!tex.isAllocated()){
		ofFbo::Settings settings;
		settings.width = width;
		settings.height = height;
		settings.numSamples = 8;
		settings.internalformat = GL_RGBA;
		tex.allocate(settings);
	}

	vector<XYZ> points;
	int numPoints = 20;
	points.resize(numPoints);

	for(int i=0;i<numPoints;i++){
		XYZ p(float(i)/float(numPoints)*float(width),ofRandom(height),0);
		points[i] = p;
	}
	points.push_back(XYZ(0,0,0));
	points.push_back(XYZ(width,0,0));
	points.push_back(XYZ(width,height,0));
	points.push_back(XYZ(0,height,0));
	sort(points.begin(),points.end(),compareXYZ);

	// KoreaParticle delaunay texture
	if(mode==Delaunay){
		vector<ITRIANGLE> triangles;
		vector<IEDGE> edges;
		points.resize(points.size()+3);
		triangles.resize(numPoints*3);
		int numTriangles;
		Triangulate(points.size()-3,&points[0],&triangles[0],&numTriangles,edges);
		triangles.resize(numTriangles);
		for(int i=0;i<(int)triangles.size();i++){
			XYZ p = points[triangles[i].p1];
			textureMesh.addVertex(ofVec3f(p.x,p.y,p.z));
			p = points[triangles[i].p2];
			textureMesh.addVertex(ofVec3f(p.x,p.y,p.z));
			p = points[triangles[i].p3];
			textureMesh.addVertex(ofVec3f(p.x,p.y,p.z));
		}
	}else if(mode==Voronoi){
		ofxVoronoi voronoiGen;

		for(int i=0;i<(int)points.size();i++){
			voronoiGen.addPoint(points[i].x,points[i].y);
		}
		voronoiGen.generateVoronoi();

		for(int i=0;i<(int)voronoiGen.edges.size();i++){
			textureMesh.addVertex(voronoiGen.edges[i].a);
			textureMesh.addVertex(voronoiGen.edges[i].b);
		}
	}

	ofSetLineWidth(1.5);
	ofEnableSmoothing();
	tex.begin();
	ofClear(255,0);
	ofSetColor(255,255,255,255);
	textureMesh.draw();
	tex.end();
	ofDisableSmoothing();
	ofSetLineWidth(1);

	head.loadImage("head.png");
}

void RibbonParticle::setupTrails(){
	length = ofRandom(lengthMin,lengthMax);

	trails.resize(length);
	trailStripLineL.getVertices().resize((length));
	trailStripLineR.getVertices().resize((length));
	trailStrip.getVertices().resize((length-1)*2);
	trailStripForGlow.getVertices().resize((length-1)*2);
	texturedStrip.getVertices().resize((length-1)*2);

	trailStripLineL.getColors().resize((length));
	trailStripLineR.getColors().resize((length));
	trailStrip.getColors().resize((length-1)*2);
	trailStripForGlow.getColors().resize((length-1)*2);
	texturedStrip.getColors().resize((length-1)*2);

	texturedStrip.getTexCoords().resize((length-1)*2);

	trails[0] = pos;
	for(int i=1; i<(int)trails.size();i++){
		trails[i] = trails[i-1];
		trails[i].x -= 4;
	}

	for(int i=0; i<((int)trails.size())-1;i++){
		float pct = float(trails.size()-i) / ((float)trails.size()*2) * .75 * 255;
		trailStrip.setColor(i*2,ofColor(r,g,b,pct));
		trailStrip.setColor(i*2+1,ofColor(r,g,b,pct));

		trailStripForGlow.setColor(i*2,ofColor(r,g,b,pct));
		trailStripForGlow.setColor(i*2+1,ofColor(r,g,b,pct));

		texturedStrip.setColor(i*2,ofColor(rTex,gTex,bTex,pct));
		texturedStrip.setColor(i*2+1,ofColor(rTex,gTex,bTex,pct));

		trailStripLineL.setColor(i,ofColor(rLines,gLines,bLines,pct*.35));
		trailStripLineR.setColor(i,ofColor(rLines,gLines,bLines,pct*.35));

		ofVec2f tc((float(i)*4),0);
		texturedStrip.setTexCoord(i*2,tc);
		tc.y = tex.getHeight();
		texturedStrip.setTexCoord(i*2+1,tc);
	}
}

void RibbonParticle::setup(){

	thisThickness = ofRandom(thicknessMin,thicknessMax);

	trailStrip.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	trailStripForGlow.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	texturedStrip.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	trailStripLineL.setMode(OF_PRIMITIVE_LINE_STRIP);
	trailStripLineR.setMode(OF_PRIMITIVE_LINE_STRIP);
	headMesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);

	headMesh.getVertices().resize(4);

	trailStrip.setUsage(GL_STREAM_DRAW);
	trailStripForGlow.setUsage(GL_STREAM_DRAW);
	texturedStrip.setUsage(GL_STREAM_DRAW);
	trailStripLineL.setUsage(GL_STREAM_DRAW);
	trailStripLineR.setUsage(GL_STREAM_DRAW);
	headMesh.setUsage(GL_STREAM_DRAW);

	setupTrails();

	headMesh.addTexCoord(ofVec2f(0,0));
	headMesh.addTexCoord(ofVec2f(head.getWidth(),0));
	headMesh.addTexCoord(ofVec2f(head.getWidth(),head.getHeight()));
	headMesh.addTexCoord(ofVec3f(0,head.getHeight()));

	noiseSeed = ofRandom(1);

	thisRGB.set(r,g,b);
	targetColor = thisRGB;
	state = ReachingTarget;
	speedState = Fast;

	lengthMin.addListener(this,&RibbonParticle::lengthChanged);
	lengthMax.addListener(this,&RibbonParticle::lengthChanged);

	thicknessMin.addListener(this,&RibbonParticle::thicknessChanged);
	thicknessMax.addListener(this,&RibbonParticle::thicknessChanged);
}

void RibbonParticle::lengthChanged(int & length){
	setupTrails();
}

void RibbonParticle::thicknessChanged(float & thickness){
	thisThickness = ofRandom(thicknessMin,thicknessMax);
}

void RibbonParticle::runAway(){
	state = RunningAway;
}

void RibbonParticle::goBack(){
	state = GoingBack;
}

void RibbonParticle::flock(){
	state = Flocking;
}

void RibbonParticle::update(float dt,const BoundingBox3D & bb){
	if(state==RunningAway && target.distance(pos)>maxDistanceRunAway){
		goBack();
	}

	if(state != RunningAway && state != Flocking){
		if(bb.inside(pos)){
			state = TargetReached;
		}else{
			state = ReachingTarget;
		}
	}


	thisFriction = friction;//ofMap(ofNoise(ofGetElapsedTimef(),noiseSeed),0,1,friction*.2,friction*1.8);
	interpTarget = lerp(prevTarget,target,ofClamp((ofGetElapsedTimef()-tTargetChanged)/3.f,0,1));//filter(interpTarget,target,.2f);
	ofVec3f accel = (interpTarget-pos).normalize() * dt * accelFactor;


	ofVec3f left(1, 0, 0);
	ofVec3f p0 = pos;
	ofVec3f p1 = trails[0];

	ofVec3f dir = (p1 - p0).normalize();			// normalized direction vector from p0 to p1
	ofVec3f up = dir.cross(left).normalize();	// right vector
	jitterPhase += dt*TWO_PI;
	if(jitterPhase>TWO_PI) jitterPhase -= TWO_PI;
	float currJitterFreq = (ofNoise(noiseSeed,ofGetElapsedTimef()*.3)+.3) * jitterFreq;
	ofVec3f jitter = up * sin(jitterPhase*currJitterFreq)*jitterAmp;

	switch(state){
	case TargetReached:{
		jitter *= .5;
		if(prevTarget!=target){
			if(ofRandom(1)>.8){
				speedState = Fast;
				targetColor.set(255,255,255);
			}else{
				speedState = Slow;
				targetColor.set(r,g,b);
			}
			tTargetChanged = ofGetElapsedTimef();
			prevTarget = target;
		}
		if(speedState==Fast){
			thisSpeedFactor = filter(thisSpeedFactor, speedFactor*2.f,.2f);//ofMap(ofNoise(ofGetElapsedTimef(),noiseSeed),0,1,speedFactor*.2,speedFactor*1.8);
			if(thisSpeedFactor-speedFactor*.2<.1 || ofGetElapsedTimef()-tTargetChanged>.5){
				targetColor.set(r,g,b);
			}
		}else{
			thisSpeedFactor = filter(thisSpeedFactor, speedFactor*1.f, .2f);
		}
	}break;
	case ReachingTarget:{
		thisSpeedFactor = filter(thisSpeedFactor, speedFactor * 1.5f, .2f);
		targetColor.set(r,g,b);
	}break;
	case RunningAway:{
		accel = (target-pos).normalize() *-1 * dt * strengthRunnawayForce;
		targetColor.set(255,255,255);
	}break;
	case Flocking:{
		accel = (target-pos).normalize() *-1 * dt * strengthRunnawayForce;
	}break;
	}

	thisRGB = filter(thisRGB,targetColor,.1);

	accel += jitter;

	//accel

	vel += accel + vel*-thisFriction ;  // accel + friction


	// upadte position
	pos += vel * thisSpeedFactor;   // vel * vel factor

	//cout << vel.length() << endl;

	ofVec3f diff,next;
	next = pos;
	diff = next-trails[0];
	ofQuaternion q;
	ofVec3f axis;
	float angle;
	for(int i=0;i<(int)trails.size()-1;i++){
		diff =  trails[i]-trails[i+1];
		q.makeRotate(diff,next-trails[i]);
		q.getRotate(angle,axis);
		q.makeRotate(angle,axis);
		q.normalize();

		diff = q * diff;
		trails[i] = next - diff;

		next = trails[i];
	}
	trails.back()=next-diff;

	// create some dpeth shading
	// not working with shader?
	float alphaPct = ofMap(pos.z,-600,100,.25,1,true);

	for(int i=0; i<((int)trails.size())-1;i++)
	{
		float pct = float(trails.size()-i) / ((float)trails.size() * 2.) * (.5+vel.length()) * 255.;
		trailStrip.setColor(i*2,ofColor(thisRGB,pct*alphaPct));
		trailStripForGlow.setColor(i*2,ofColor(thisRGB,pct*alphaPct));
		texturedStrip.setColor(i*2,ofColor(rTex,gTex,bTex,pct*alphaPct*1.2));

		trailStrip.setColor(i*2+1,ofColor(thisRGB,pct*alphaPct));
		trailStripForGlow.setColor(i*2+1,ofColor(thisRGB,pct*alphaPct));
		texturedStrip.setColor(i*2+1,ofColor(rTex,gTex,bTex,pct*alphaPct*1.2));

		trailStripLineL.setColor(i,ofColor(rLines,gLines,bLines,pct*alphaPct));
		trailStripLineR.setColor(i,ofColor(rLines,gLines,bLines,pct*alphaPct));

		//if(particleState == KPARTICLE_EATING)
		//	trailStrip.setColor(i,ofColor(255,0,0,pct*aplhaPct));

	}

	for(int i=0; i<((int)trails.size())-1;i++){


		ofVec3f up(0, 0, 1);
		ofVec3f &p0 = trails[i];
		ofVec3f &p1 = trails[i+1];

		ofVec3f dir = (p1 - p0).normalize();			// normalized direction vector from p0 to p1
		ofVec3f right = dir.cross(up).normalize();	// right vector
		right *= thisThickness;
		ofVec3f rightNotGlow = right * .5;


		trailStrip.getVertices()[i*2].set(trails[i] -rightNotGlow);
		trailStripForGlow.getVertices()[i*2].set(trails[i] -right);
		texturedStrip.getVertices()[i*2].set(trails[i] -rightNotGlow);
		trailStripLineL.getVertices()[i].set(trails[i] +rightNotGlow);

		trailStrip.getVertices()[i*2+1].set(trails[i] +rightNotGlow);
		trailStripForGlow.getVertices()[i*2+1].set(trails[i] +right);
		texturedStrip.getVertices()[i*2+1].set(trails[i] +rightNotGlow);
		trailStripLineR.getVertices()[i].set(trails[i] +rightNotGlow);
	}



	up.set(0, 0, 1);
	p0 = trails[0];
	p1 = trails[5];

	dir = (p1 - p0).normalize();			// normalized direction vector from p0 to p1
	ofVec3f right = dir.getCrossed(up).normalize();	// right vector
	right *= headSize * thisThickness;
	dir *= (headSize * thisThickness * head.getWidth()/head.getHeight());

	headMesh.getVertices()[0].set(trails[0] - right );
	headMesh.getVertices()[1].set(trails[0] + right );

	headMesh.getVertices()[2].set(trails[0] + right + dir);
	headMesh.getVertices()[3].set(trails[0] - right + dir);

}

void RibbonParticle::draw(){

	float aplhaPct = ofMap(pos.z,-200,100,0,1);
	ofSetColor(r,g,b,240*aplhaPct);

	trailStrip.draw();
	//ofSetColor(40);
	tex.getTextureReference().bind();
	texturedStrip.draw();
	tex.getTextureReference().unbind();

	ofSetColor(thisRGB);
	head.getTextureReference().bind();
	headMesh.draw();
	head.getTextureReference().unbind();
}

void RibbonParticle::drawForGlow(){

	float aplhaPct = ofMap(pos.z,-200,100,0,1);
	ofSetColor(r,g,b,240*aplhaPct);

	trailStripForGlow.draw();
	ofSetColor(20);
	//trailStripLineL.draw();
	//trailStripLineR.draw();
	tex.getTextureReference().bind();
	texturedStrip.draw();
	tex.getTextureReference().unbind();
	ofSetColor(255);
	head.getTextureReference().bind();
	headMesh.draw();
	head.getTextureReference().unbind();
}
