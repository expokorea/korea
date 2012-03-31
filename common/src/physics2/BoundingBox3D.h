#pragma once




//----------------------------------------
static void ofBox(ofVec3f size){

	static ofMesh vertexData;
	ofPushMatrix();
	if(ofGetCoordHandedness() == OF_LEFT_HANDED){
		ofScale(1, 1, -1);
	}

	float w = size.x * .5;
	float h = size.y * .5;
	float d = size.z * .5;

	vertexData.clear();
	if(ofGetStyle().bFill){
		ofVec3f vertices[] = {
			ofVec3f(+w,-h,+d), ofVec3f(+w,-h,-d), ofVec3f(+w,+h,-d), ofVec3f(+w,+h,+d),
			ofVec3f(+w,+h,+d), ofVec3f(+w,+h,-d), ofVec3f(-w,+h,-d), ofVec3f(-w,+h,+d),
			ofVec3f(+w,+h,+d), ofVec3f(-w,+h,+d), ofVec3f(-w,-h,+d), ofVec3f(+w,-h,+d),
			ofVec3f(-w,-h,+d), ofVec3f(-w,+h,+d), ofVec3f(-w,+h,-d), ofVec3f(-w,-h,-d),
			ofVec3f(-w,-h,+d), ofVec3f(-w,-h,-d), ofVec3f(+w,-h,-d), ofVec3f(+w,-h,+d),
			ofVec3f(-w,-h,-d), ofVec3f(-w,+h,-d), ofVec3f(+w,+h,-d), ofVec3f(+w,-h,-d)
		};
		vertexData.addVertices(vertices,24);

		static ofVec3f normals[] = {
			ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0), ofVec3f(+1,0,0),
			ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0), ofVec3f(0,+1,0),
			ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1), ofVec3f(0,0,+1),
			ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0), ofVec3f(-1,0,0),
			ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0), ofVec3f(0,-1,0),
			ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1), ofVec3f(0,0,-1)
		};
		vertexData.addNormals(normals,24);

		static ofVec2f tex[] = {
			ofVec2f(1,0), ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1),
			ofVec2f(1,1), ofVec2f(1,0), ofVec2f(0,0), ofVec2f(0,1),
			ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0), ofVec2f(0,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0),
			ofVec2f(0,0), ofVec2f(0,1), ofVec2f(1,1), ofVec2f(1,0)
		};
		vertexData.addTexCoords(tex,24);

		static ofIndexType indices[] = {
			0,1,2, // right top left
			0,2,3, // right bottom right
			4,5,6, // bottom top right
			4,6,7, // bottom bottom left
			8,9,10, // back bottom right
			8,10,11, // back top left
			12,13,14, // left bottom right
			12,14,15, // left top left
			16,17,18, // ... etc
			16,18,19,
			20,21,22,
			20,22,23
		};
		vertexData.addIndices(indices,36);
		vertexData.setMode(OF_PRIMITIVE_TRIANGLES);
		ofGetCurrentRenderer()->draw(vertexData,vertexData.usingColors(),vertexData.usingTextures(),vertexData.usingNormals());
	} else {
		ofVec3f vertices[] = {
			ofVec3f(+w,+h,+d),
			ofVec3f(+w,+h,-d),
			ofVec3f(+w,-h,+d),
			ofVec3f(+w,-h,-d),
			ofVec3f(-w,+h,+d),
			ofVec3f(-w,+h,-d),
			ofVec3f(-w,-h,+d),
			ofVec3f(-w,-h,-d)
		};
		vertexData.addVertices(vertices,8);

		static float n = sqrtf(3);
		static ofVec3f normals[] = {
			ofVec3f(+n,+n,+n),
			ofVec3f(+n,+n,-n),
			ofVec3f(+n,-n,+n),
			ofVec3f(+n,-n,-n),
			ofVec3f(-n,+n,+n),
			ofVec3f(-n,+n,-n),
			ofVec3f(-n,-n,+n),
			ofVec3f(-n,-n,-n)
		};
		vertexData.addNormals(normals,8);

		static ofIndexType indices[] = {
			0,1, 1,3, 3,2, 2,0,
			4,5, 5,7, 7,6, 6,4,
			0,4, 5,1, 7,3, 6,2
		};
		vertexData.addIndices(indices,24);

		vertexData.setMode(OF_PRIMITIVE_LINES);
		ofGetCurrentRenderer()->draw(vertexData, vertexData.usingColors(),vertexData.usingTextures(),vertexData.usingNormals());
	}


	ofPopMatrix();
}

//----------------------------------------
static void ofBox(const ofPoint& position, float w, float h, float d){

	static ofMesh vertexData;
	ofPushMatrix();
	ofTranslate(position);
	ofBox(ofVec3f(w,h,d));
	ofPopMatrix();
}

class BoundingBox3D{
public:
	BoundingBox3D()
	:x(0)
	,y(0)
	,z(0)
	,width(0)
	,height(0)
	,depth(0){}

	BoundingBox3D(float x, float y, float z, float w, float h, float d)
	:x(x)
	,y(y)
	,z(z)
	,width(w)
	,height(h)
	,depth(d){}

	bool inside(const ofPoint & p) const{
		return p.x > x && p.x < x + width  &&
				p.y > y && p.y < y + height &&
				p.z > z && p.z < z + depth;
	}

	float x,y,z;
	float width, height, depth;

	void draw(){
		ofBox(ofVec3f(x+width*.5,y+height*.5,z+depth*.5),width,height,depth);
	}
};
