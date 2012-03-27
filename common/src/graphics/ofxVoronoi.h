// The algorithm is a modified version of Stephan Fortune's sweep line algorithm
// http://ect.bell-labs.com/who/sjf/

// Orignal Code from Shane O'Sullivan
// http://www.skynet.ie/~sos/mapviewer/voronoi.php

#pragma once

#include "ofMain.h"
#include "VoronoiDiagramGenerator.h"

class ofxVoronoiEdge {
  public:
    ofVec2f a, b;
    ofxVoronoiEdge(const ofVec2f &ptA, const ofVec2f &ptB) {
        a.set(ptA);
        b.set(ptB);
    }
    ofxVoronoiEdge(float x1, float y1, float x2, float y2) {
        a.set(x1, y1);
        b.set(x2, y2);
    }
    void draw() {
        ofLine(a, b);
    }
};

//--------------------------------------------------------------

class ofxVoronoi {
    
private:
    
    ofPolyline              pts;
    VoronoiDiagramGenerator vdg;
    float                   minDistance;
    
public:
	
    ofxVoronoi();
    ~ofxVoronoi();
    
    void clear();
    void setMinDistance(float minDis);
    bool generateVoronoi();
    void addPoint(const ofVec2f &pt);
    void addPoint(float x, float y);
    void draw();
    
    vector<ofPoint>& getPoints();
    ofRectangle getBounds();
    
    vector <ofxVoronoiEdge> edges;
};

