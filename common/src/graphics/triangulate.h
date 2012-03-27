/*
 * triangulate.h
 *
 *  Created on: 21/03/2012
 *      Author: arturo
 */

#pragma once

#include "ofConstants.h"

typedef struct {
   int p1,p2,p3;
} ITRIANGLE;
typedef struct {
   int p1,p2;
} IEDGE;
struct XYZ{
	XYZ(double x, double y, double z)
	:x(x)
	,y(y)
	,z(z){}
	XYZ()
	:x(0)
	,y(0)
	,z(0){}
   double x,y,z;
};




int Triangulate(int nv,XYZ *pxyz,ITRIANGLE *v,int *ntri, vector<IEDGE> & edges);
int CircumCircle(double xp,double yp,
   double x1,double y1,double x2,double y2,double x3,double y3,
   double *xc,double *yc,double *rsqr);
