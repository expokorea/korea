/*
 *  KoreaParticle.cpp
 *  mainServer
 *
 *  Created by Chris on 2/15/12.
 *  Copyright 2012 csugrue. All rights reserved.
 *
 */

#include "KoreaParticle.h"
#include "ofxTimeUtils.h"

ofxIntSlider KoreaParticle::r;
ofxIntSlider KoreaParticle::g;
ofxIntSlider KoreaParticle::b;
ofxToggle KoreaParticle::debug;
ofxToggle KoreaParticle::useModel;
ofxFloatSlider KoreaParticle::thickness;
ofxIntSlider KoreaParticle::length;
ofxAssimpModelLoader KoreaParticle::model;
float KoreaParticle::speedFactor;

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

KoreaParticle::KoreaParticle(){
groupFlag = 0;
};

void KoreaParticle::setup(ofVec3f pos, ofVec3f vel, float damping)
{
	
	this->vel = vel;
	this->pos = pos;
	this->damping = damping;
	
	target.set(0,0,0);
	targetForce = .01;
	bUseTarget  = false;
	bFlocking   = false;
	bDrawTrails = true;
	bEating = false;
	particleState = KPARTICLE_FLOCKING;
	
	rt = ofRandom(0,100);
	
	sepDist = 40;
	alnDist = 70;
	cohDist = 60;
	
	separation =  .025;
    cohesion   =  .005;
    alignment  =  .005;
	
	targetSpeed = 8;
	speedFactor = ofMap(targetSpeed,1,80,0,1)*ofMap(targetSpeed,1,80,0,1);
	
	node.setScale(1);

	length = 50;

	trailStrip.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
	trailStripForGlow.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

	trails.resize(length);
	angles.resize(length,0);
	zangles.resize(length,0);
	trailStrip.getVertices().resize((length-1)*2);
	trailStripForGlow.getVertices().resize((length-1)*2);

	trailStrip.setUsage(GL_STREAM_DRAW);
	trailStripForGlow.setUsage(GL_STREAM_DRAW);

	trails[0] = pos;
	for(int i=0; i<trails.size();i++){
		if(i>1){
			trails[i] = trails[i-1];
			trails[i].x -= 4;
		}

		float pct = float(trails.size()-i) / (float)trails.size() * .75 * 255;
		trailStrip.addColor(ofColor(r,g,b,pct));
		trailStripForGlow.addColor(ofColor(r,g,b,pct));
		trailStrip.addColor(ofColor(r,g,b,pct));
		trailStripForGlow.addColor(ofColor(r,g,b,pct));
	}
}

void KoreaParticle::update(float dt)
{
	
	vel += ofVec3f(	ofNoise(rt*t,0)*.001-.0005,
					ofNoise(0,rt*t)*.001-.0005,
					0);
	
	vel.x -= damping*vel.x;
	vel.y -= damping*vel.y;
	vel.z -= damping*vel.z;
	
	// apply attractions
	if(bUseTarget) applyTargetAttraction();
	
	// note: check is this working? or doing anything anymore...?
	if(particleState == KPARTICLE_TARGET) repelFrom(target,targetForce*2,90);
	
	// upadte position
	pos += vel*dt;
	
	// for targets and trails
	t = ofxTimeUtils::getElapsedTimef()*speedFactor;

	/*trails[0] = pos;
	for(int i=1;i<trails.size();i++){
		float dx = trails[i-1].x - trails[i].x;
		float dy = trails[i-1].y - trails[i].y;
		float dz = trails[i-1].z - trails[i].z;
		float a  = atan2(dy, dx);
		float az  = atan2(dz, dy);
		float dis = 7;

		if(i == 1) {
			trails[i].x = trails[i-1].x - cos(a) * dis;
			trails[i].y = trails[i-1].y - sin(a) * dis;
			trails[i].z = trails[i-1].z - sin(az) * dis;
		}
		else {
		    float range = ofDegToRad(25);
		    float pa = angles[i-1];

		    float d = a - pa;
		    float absA = fabs(d);
		    float s = d>=0?1:-1;
		    if (absA > PI) {
		        a = s * (2 * PI - fabs(a)) * -1;
		    }

		    float c  = ofClamp(a, pa-range, pa+range);

		    trails[i].x = trails[i-1].x - cos(c) * dis;
		    trails[i].y = trails[i-1].y - sin(c) * dis;



		    float paz = zangles[i-1];
		    float dz = az - paz;
		    float absAz = fabs(dz);
		    float sz = dz>=0?1:-1;
		    if (absAz > PI) {
		        az = sz * (2 * PI - fabs(az)) * -1;
		    }

		    float cz  = ofClamp(az, paz-range, paz+range);

			trails[i].z = trails[i-1].z - sin(cz) * dis;
		}
		//trails[i].z = trails[i-1].z;

		angles[i] = ofWrapRadians(a);
		zangles[i] = ofWrapRadians(az);
		//trail[i].z=trail[i-1].z;ofVec3f up(0, 0, 1);
		//float pct = (float)i / (float)trails.size();
	}*/

	ofVec3f diff,next;
	next = pos;
	diff = next-trails[0];
	ofQuaternion q;
	float angle;
	ofVec3f axis;
	for(int i=0;i<(int)trails.size()-1;i++){
		diff =  trails[i]-trails[i+1];
		q.makeRotate(diff,next-trails[i]);

		/*q.getRotate(angle,axis);
		if(angle>90) angle = 90;
		q.makeRotate(angle,axis);*/

		diff = q * diff;
		trails[i] = next - diff;

		next = trails[i];
	}
	trails[trails.size()-1]=next-diff;
	
	node.setPosition(pos);
	if(trails.size()>10)node.lookAt(trails[0]-trails[1]);

	// create some dpeth shading
	// not working with shader?
	float aplhaPct = ofMap(pos.z,-600,100,.25,1,true);
	
	for(int i=0; i<(int)trailStrip.getNumColors();i++)
	{
		float pct = float(trails.size()-i) / (float)trails.size() * .75 * 255;
		trailStrip.setColor(i,ofColor(r,g,b,pct*aplhaPct));
		//if(particleState == KPARTICLE_EATING)
		//	trailStrip.setColor(i,ofColor(255,0,0,pct*aplhaPct));
	
	}
	
	for(int i=0; i<(int)trails.size()-1;i++){
		
		
		ofVec3f up(0, 0, 1);
		//float pct = (float)i / (float)trails.size();
		ofVec3f &p0 = trails[i];
		ofVec3f &p1 = trails[i+1];

		ofVec3f dir = (p1 - p0).normalize();			// normalized direction vector from p0 to p1
		ofVec3f right = dir.cross(up).normalize();	// right vector
		right *= thickness; //ofClamp(thickness*ofNoise(float(i)/float(trails.size()))*ofNoise(float(i)/float(trails.size()))*ofNoise(float(i)/float(trails.size()))+2,0,thickness); //*sin(float(i)/float(trails.size())*PI)
		ofVec3f rightNotGlow = right * .5;


		trailStrip.getVertices()[i*2].set(trails[i] -rightNotGlow);
		trailStripForGlow.getVertices()[i*2].set(trails[i] -right);

		trailStrip.getVertices()[i*2+1].set(trails[i] +rightNotGlow);
		trailStripForGlow.getVertices()[i*2+1].set(trails[i] +right);

		//ofVec3f normal = calcNormal(trails[i] -rightNotGlow,trails[i] +rightNotGlow,trails[i+1] -rightNotGlow,trails[i+1] +rightNotGlow);
		//for(int i=0;i<4;i++){
			/*trailStrip.addNormal(normal);
			trailStrip.addNormal(normal);
			trailStrip.addNormal(normal);
			trailStrip.addNormal(normal);*/
		//}
		//normal = calcNormal(trails[i] +rightNotGlow,trails[i+1] -rightNotGlow,trails[i+1] +rightNotGlow);
		//for(int i=0;i<4;i++){
		//trailStrip.addNormal(normal);
		//trailStrip.addNormal(normal);
	}
}

void KoreaParticle::draw()
{
	if(debug){
		drawDebug();
	}
	if(KoreaParticle::useModel){
		
		float aplhaPct = ofMap(pos.z,-300,300,.45,1);
		if(particleState == KPARTICLE_FLOCKING) ofSetColor(r,g,b,200*aplhaPct);
		else ofSetColor(r,g,b,200*aplhaPct);
		
		ofVec3f axis;
		float angle;
		node.getOrientationQuat().getRotate(angle, axis);

		ofPushMatrix();
			glTranslatef(pos.x,pos.y,pos.z);
			glRotatef(90,1,0,0);
			glRotatef(90,0,0,1);
			ofRotate(angle, axis.x, axis.y, axis.z);
			glScalef(.075,.075,.075);
			model.draw(OF_MESH_FILL);
		ofPopMatrix();
	}
	
	float aplhaPct = ofMap(pos.z,-200,100,0,1);
	if(particleState == KPARTICLE_FLOCKING) ofSetColor(r,g,b,200*aplhaPct);
	else ofSetColor(r,g,b,240*aplhaPct);
	
	if(bDrawTrails)
	{
		trailStrip.draw();
	}
	
	/*if(particleState == KPARTICLE_EATING){
	glBegin(GL_LINES);
	glVertex3f(pos.x,pos.y,pos.z);
	glVertex3f(target.x,target.y,target.z);
	glEnd();
	}*/
}


void KoreaParticle::drawForGlow() {

	if(debug){
		drawDebug();
	}
	if(KoreaParticle::useModel && !KoreaParticle::debug){
		if(particleState == KPARTICLE_FLOCKING) ofSetColor(r,g,b,255);
		else ofSetColor(r,g,b,200);
		ofVec3f axis;
		float angle;
		node.getOrientationQuat().getRotate(angle, axis);

		ofPushMatrix();
			glTranslatef(pos.x,pos.y,pos.z);
			glRotatef(90,1,0,0);
			glRotatef(90,0,0,1);
			ofRotate(angle, axis.x, axis.y, axis.z);
			glScalef(.075,.075,.075);
			model.draw(OF_MESH_FILL);
		ofPopMatrix();
	}

	if(bDrawTrails)
	{
		trailStripForGlow.draw();
	}

}

void KoreaParticle::drawDebug(){
	if(particleState == KPARTICLE_FLOCKING) ofSetColor(100,100,100);
		else ofSetColor(255,255,255);

		//ofFill();
		glEnable(GL_DEPTH_TEST);
		ofSphere(pos,5);
		node.draw();
		glDisable(GL_DEPTH_TEST);
		
		/*glBegin(GL_LINES);
			glVertex3f(pos.x,pos.y,pos.z);
			glVertex3f(target.x,target.y,target.z);
		glEnd();*/
		
}

void KoreaParticle::setTarget(ofVec3f targ, float targetForce)
{
	target = targ;
	bUseTarget = true;
	this->targetForce = targetForce;
}
	
void KoreaParticle::applyTargetAttraction()
{
	
	
	ofVec3f diff	= target-pos;
	float length	= diff.lengthSquared();
	
	// don't pull if too close
	// radius changed based on particle mode so no "bouncing" when chasing prey...
	
	float targetRadius = 30;
	if(particleState == KPARTICLE_EATING) targetRadius = 90;
	
	if(length > targetRadius)
	{
	ofVec3f frc = ofVec3f(0,0,0);
	
	diff.normalize();
		
	frc.x = diff.x * targetForce;
	frc.y = diff.y * targetForce;
	frc.z = diff.z * targetForce;

	vel+=frc;
	
	
	}
	else{
		// if in prey/eating mode, and target is reached, go back to flock
		if(bEating){
		 setState(KPARTICLE_FLOCKING);
		}
	}
}

void KoreaParticle::repelFrom(ofVec3f pt,float force,float dist)
{
	
	
	ofVec3f diff	= pos-pt;
	float length	= diff.lengthSquared();
	//float radius	= 10;
	
	/*bool bAmCloseEnough = false;
	 if (radius > 0){
	 if (length > 200){
	 bAmCloseEnough = false;
	 }
	 }*/
	//if(force < 0 ) force *= -1;
	
	if( length < dist)
	{
	ofVec3f frc = ofVec3f(0,0,0);
	
	diff.normalize();
	
	frc.x = diff.x * force * ((dist-length)*.01);
	frc.y = diff.y * force * ((dist-length)*.01);
	frc.z = diff.z * force * ((dist-length)*.01);
	
	vel+=frc;
	}
	
}


void KoreaParticle::addForFlocking(KoreaParticle * sister)
{
	
	
	ofVec3f diff, direction;
	float d, attention_factor;
	
	
	// add for seperation
	diff = pos-sister->pos;// - pos;
	d 	 = diff.length();
	diff.normalize();
	
	
	// attention angle factor
	direction = vel;
	direction.normalize();
	/*attention_factor = diff.dot( direction );
	const float CUTOFF = .651;//PI;
	attention_factor -= CUTOFF;
	attention_factor /= (1.0f-CUTOFF);
	*/
	attention_factor = 1;
	
	// only groups have chesion and alignment with own group
	// but separation with everyone
	
	if(d > 0)
	{
			if( d < sepDist )
			{
				sumSep += (diff) * attention_factor;
				countSep++;
			}
		
			if( sister->groupFlag != groupFlag){
				
				attention_factor = .1;
				
				if( d < alnDist )
				{
					sumAlign += sister->vel  * attention_factor;
					countAlign++;
				}
				
				//add for cohesion
				if( d < cohDist )
				{
					sumCoh += sister->pos * attention_factor;
					countCoh++;
				}
			}
			else{
		
				sumAlign += sister->vel  * attention_factor;
				countAlign++;
				
				sumCoh += sister->pos * attention_factor;
				countCoh++;			
			}
		}
}


void KoreaParticle::applyForces()
{
	float sepFrc 	= separation;
	float cohFrc 	= cohesion;
	float alignFrc 	= alignment;
	
	// forces changed based on state
	// note: check how this is affected or not by code in flock class
	
	if(particleState == KPARTICLE_TARGET)
	{
		   cohFrc	= 0;
		   alignFrc = 0;
		   sepFrc *= .125;
	}else if(  particleState == KPARTICLE_EATING)
	{
		cohFrc	= 0;
		alignFrc = 0;
		sepFrc *= .25;
	}
	
	// seperation
	if(countSep > 0)
	{
		sumSep /= (float)countSep;
		vel += (sumSep.normalized() 	* sepFrc);
	}
	
	
	// alignment
	if(countAlign > 0)
	{
		sumAlign /= (float)countAlign;
		vel += (sumAlign.normalized() * alignFrc);
	}
	
	// cohesion
	if(countCoh > 0)
	{
		
		sumCoh /= (float)countCoh;
		sumCoh -= pos;
		vel += (sumCoh.normalized() 	* cohFrc);
	}
	

}

void KoreaParticle::resetFlocking()
{
    countCoh 	= 0;
	countSep	= 0;
	countAlign	= 0;
	
	sumCoh.set(0,0,0);
	sumSep.set(0,0,0);
	sumAlign.set(0,0,0);
}

void KoreaParticle::setState(koreaParticleState state)
{
	particleState = state;
	
	// more damping on prey chase state
	// note: this is now overwriting the damping set on init
	
	if( particleState == KPARTICLE_EATING){
		damping = .05;
		bEating = true;
	}else{
		 bEating = false;
		 damping = .02;
	}
}


void KoreaParticle::setFlockingParams()
{
		
}

