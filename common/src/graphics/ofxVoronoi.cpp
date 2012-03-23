#include "ofxVoronoi.h"


//--------------------------------------------------------------
ofxVoronoi::ofxVoronoi() {
    minDistance = 3;    
}

//--------------------------------------------------------------
ofxVoronoi::~ofxVoronoi() {
    
}

//--------------------------------------------------------------
void ofxVoronoi::clear() {
    edges.clear();
    vdg.resetIterator();
    pts.clear();
}

//--------------------------------------------------------------
void ofxVoronoi::setMinDistance(float minDis) {
    minDistance = minDis;
}

//--------------------------------------------------------------
void ofxVoronoi::addPoint(float x, float y) {
    pts.addVertex(x, y);
}

//--------------------------------------------------------------
void ofxVoronoi::addPoint(const ofVec2f &pt) {
    addPoint(pt.x, pt.y);    
}

//--------------------------------------------------------------
ofRectangle ofxVoronoi::getBounds() {
    return pts.getBoundingBox();
}

//--------------------------------------------------------------
vector<ofPoint>& ofxVoronoi::getPoints() {
    return pts.getVertices();    
}

//--------------------------------------------------------------
bool ofxVoronoi::generateVoronoi() {
   
    
    bool bMade = false;
    if(pts.size() > 0) {
                
        edges.clear();
        
        int nPts = (int)pts.size();
        float xValues[nPts];
        float yValues[nPts];
        
        for (int i=0; i<nPts; i++) {
            xValues[i] = pts[i].x;
            yValues[i] = pts[i].y;
        }
        
        ofRectangle bounds = pts.getBoundingBox();
        float minX = bounds.x;
        float maxX = bounds.x + bounds.width;
        float minY = bounds.y;
        float maxY = bounds.y + bounds.height;
        
        bMade = vdg.generateVoronoi(xValues, yValues, nPts, minX, maxX, minY, maxY, minDistance);
        vdg.resetIterator();
        
        float x1,y1,x2,y2;
        while(vdg.getNext(x1,y1,x2,y2)) {
            edges.push_back(ofxVoronoiEdge(x1, y1, x2, y2));
        }
        
    } 
    else {
        ofLog(OF_LOG_WARNING, "need points to generate voronoi\n");
    }
    
    return bMade;
}

//--------------------------------------------------------------
void ofxVoronoi::draw() {
    
}
