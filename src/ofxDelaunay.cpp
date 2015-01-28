/*
 *  ofxDelaunay.h
 *
 *  Created by Pat Long on 29/10/09.
 *  Copyright 2009 Tangible Interaction. All rights reserved.
 *
 *  Some parts based on demo code by Gilles Dumoulin. 
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *
 *	Cleaned up by Lukasz Karluk, 19/05/2010
 *
 *  Refactored for 0071 + ofMesh by James George on 21/06/2012
 */

#include "ofxDelaunay.h"

bool ptInTriangle(const ofVec2f & p, const XYZ & p0, const XYZ & p1, const XYZ& p2) {
    float A = 0.5f * (-p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y);
    float sign = A < 0.0f ? -1.0f : 1.0f;
    float s = (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y) * sign;
    float t = (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y) * sign;

    return s > 0.0f && t > 0.0f && (s + t) < 2.0f * A * sign;
}

void ofxDelaunay::reset(){
    vertices.clear();
    triangles.clear();
	triangleMesh.clear();
	ntri = 0;
}

int ofxDelaunay::addPoint( const ofPoint& point ){
	return addPoint( point.x, point.y, point.z );
}

int ofxDelaunay::addPoint( float x, float y, float z ){
    XYZ v;
    v.x = x;
    v.y = y;
    v.z = z;
    vertices.push_back(v);
	return vertices.size();
}

int ofxDelaunay::addPoints( vector<ofPoint>& points ){
	for(int i = 0; i < points.size(); i++){
        addPoint( points[i] );
    }
    return vertices.size();
}

int ofxDelaunay::triangulate(){
    
    if(vertices.size() < 3){
        return;
    }
    
    int nv = vertices.size();
    //add 3 empty slots, required by the Triangulate call
    vertices.push_back(XYZ());
    vertices.push_back(XYZ());
    vertices.push_back(XYZ());
    
    //allocate space for triangle indices
    triangles.resize(3*nv);
    
	ntri;
	qsort( &vertices[0], vertices.size()-3, sizeof( XYZ ), XYZCompare );
	Triangulate( nv, &vertices[0], &triangles[0], ntri );
	
	// copy triangle data to ofxDelaunayTriangle.
	triangleMesh.clear();
    
    //copy vertices
	for (int i = 0; i < nv; i++){
        triangleMesh.addVertex(ofVec3f(vertices[i].x,vertices[i].y,vertices[i].z));
    }
    
    //copy triagles
	for(int i = 0; i < ntri; i++){
		triangleMesh.addIndex(triangles[ i ].p1);
		triangleMesh.addIndex(triangles[ i ].p2);
		triangleMesh.addIndex(triangles[ i ].p3);
	}
	
    //erase the last three vertices
    vertices.erase(vertices.end()-1);
    vertices.erase(vertices.end()-1);
    vertices.erase(vertices.end()-1);
	return ntri;
}

void ofxDelaunay::draw(){
	if(ofGetStyle().bFill){
	    triangleMesh.draw();
    }
    else{
    	triangleMesh.drawWireframe();
    }

//	for(int i = 0; i < vertices.size(); i++ ){
//		ofDrawBitmapStringHighlight(ofToString(i) , vertices[i].x, vertices[i].y);
//	}
	ofDrawBitmapStringHighlight("tri: " + ofToString(ntri) + "\nver:" + ofToString(vertices.size()), 30, 300);
}


int ofxDelaunay::getNumTriangles(){
	return ntri;
}


int ofxDelaunay::getNumPoints(){
	return vertices.size();
}

ofPoint ofxDelaunay::getPointNear(ofPoint pos, float minimumDist, int & index){

	XYZ ret;
	XYZ p; p.x = pos.x; p.y = pos.y; p.z = pos.z;
	float minDist = FLT_MAX;
	for(int i = 0; i < vertices.size() ; i++){
		float d = ofDist(vertices[i].x, vertices[i].y, p.x, p.y);
		if(d < minDist ) {
			minDist = d;
			index = i;
			ret = vertices[i];
		}
	}
	if (minDist > minimumDist){
		ret.x = ret.y = ret.z = 0.0f;
		index = -1;
	}
	return ofVec3f(ret.x, ret.y, ret.z);
}

ITRIANGLE ofxDelaunay::getTriangleAtIndex(int index){
	if (index >= 0 && index < ntri){
		return triangles[index];
	}
	return ITRIANGLE();
}

void ofxDelaunay::removePointAtIndex(int index){
	if (index >= 0 && index < vertices.size()){
		vertices.erase(vertices.begin()+index);
	}
	triangulate();
}


void ofxDelaunay::setPointAtIndex(ofPoint p, int index){
	if (index >= 0 && index < vertices.size()){
		XYZ pp; pp.x = p.x; pp.y = p.y; pp.z = p.z;
		vertices[index] = pp;
	}
	triangulate();
}

vector<ofPoint> ofxDelaunay::getPointsForITriangle(ITRIANGLE t){
	vector<ofPoint> pts;
	pts.push_back( ofPoint(vertices[t.p1].x, vertices[t.p1].y, vertices[t.p1].z));
	pts.push_back( ofPoint(vertices[t.p2].x, vertices[t.p2].y, vertices[t.p2].z));
	pts.push_back( ofPoint(vertices[t.p3].x, vertices[t.p3].y, vertices[t.p3].z));
	return pts;
}

ITRIANGLE ofxDelaunay::getTriangleForPos(ofPoint pos){
	
	ITRIANGLE ti;

	for(int i = 0; i < ntri ; i++){
		XYZ p0 = vertices[triangles[i].p1];
		XYZ p1 = vertices[triangles[i].p2];
		XYZ p2 = vertices[triangles[i].p3];
		bool inside = ptInTriangle(pos, p0, p1, p2);
		if(inside) {
			ti = triangles[i];
			break;
		}
	}
	return ti;
}