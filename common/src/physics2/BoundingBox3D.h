#pragma once

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
};
