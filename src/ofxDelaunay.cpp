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
#include <cassert>

bool ptInTriangle(const ofVec2f & p, const XY & p0, const XY & p1, const XY& p2) {
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

int ofxDelaunay::addPoint( const ofVec2f& point ){
	return addPoint( point.x, point.y );
}

int ofxDelaunay::addPoint( float x, float y ){
    XYI v;
    v.x = x;
    v.y = y;
	v.i = vertices.size();
    vertices.push_back(v);
	return vertices.size();
}

int ofxDelaunay::addPoints( vector<ofVec2f>& points ){
	for(size_t i = 0; i < points.size(); i++){
        addPoint( points[i] );
    }
    return vertices.size();
}

int ofxDelaunay::triangulate(){
    if(vertices.size() < 3){
        return 0;
    }

    int nv = vertices.size();

	// make clone not to destroy vertices
	vector<XYI> verticesTemp = vertices;
	qsort( &verticesTemp[0], verticesTemp.size(), sizeof( XYI ), XYICompare );

	//vertices required for Triangulate
    vector<XY> verticesXY;

	//copy XYZIs to XYZ
	for (int i = 0; i < nv; i++) {
		XY v;
		v.x = verticesTemp.at(i).x;
		v.y = verticesTemp.at(i).y;
		verticesXY.push_back(v);
	}

    //add 3 emptly slots, required by the Triangulate call
    verticesXY.push_back(XY());
    verticesXY.push_back(XY());
    verticesXY.push_back(XY());
    //allocate space for triangle indices
    triangles.resize(3*nv);

	Triangulate( nv, &verticesXY[0], &triangles[0], ntri );

	//copy triangle data to ofxDelaunayTriangle.
	triangleMesh.clear();
	triangleMesh.setMode(OF_PRIMITIVE_TRIANGLES);

    //copy vertices
	for (int i = 0; i < nv; i++){
        triangleMesh.addVertex(ofVec3f(vertices[i].x,vertices[i].y,0.0));
    }

	//copy triangles
	for(int i = 0; i < ntri; i++) {
        if(triangles[i].p1 > nv)
            printf("Tri %d %d | %d %d %d", i, ntri, triangles[ i ].p1, triangles[ i ].p2, triangles[ i ].p3);
        assert(triangles[i].p1 <= nv);
		triangleMesh.addIndex(verticesTemp.at(triangles[ i ].p1).i);
		assert(triangles[i].p2 <= nv);
		triangleMesh.addIndex(verticesTemp.at(triangles[ i ].p2).i);
		assert(triangles[i].p3 <= nv);
		triangleMesh.addIndex(verticesTemp.at(triangles[ i ].p3).i);
	}

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

ofVec2f ofxDelaunay::getPointNear(ofVec2f pos, float minimumDist, int & index) {
	XYI ret;
	ret.x = ret.y = 0.0f;
    index = -1;

	XY p; p.x = pos.x; p.y = pos.y;
	float minDist = FLT_MAX;
	for(size_t i = 0; i < vertices.size() ; i++){
		float d = ofDist(vertices[i].x, vertices[i].y, p.x, p.y);
		if(d < minDist ) {
			minDist = d;
			index = i;
			ret = vertices[i];
		}
	}
	if (minDist > minimumDist){
		ret.x = ret.y = 0.0f;
		index = -1;
	}
	return ofVec3f(ret.x, ret.y);
}

ITRIANGLE ofxDelaunay::getTriangleAtIndex(int index){
	if (index >= 0 && index < ntri){
		return triangles[index];
	}
	return ITRIANGLE();
}

void ofxDelaunay::removePointAtIndex(int index){
	if (index >= 0 && index < (int)vertices.size()){
		vertices.erase(vertices.begin()+index);
	}
	triangulate();
}


void ofxDelaunay::setPointAtIndex(ofVec2f p, int index){
	if (index >= 0 && index < (int)vertices.size()){
        XYI pp; pp.x = p.x; pp.y = p.y; pp.i = index;
		vertices[index] = pp;
	}
	triangulate();
}

ofVec2f ofxDelaunay::getPointAtIndex(int index) {
    return ofVec2f(vertices[index].x, vertices[index].y);
}

vector<ofVec2f> ofxDelaunay::getPointsForITriangle(ITRIANGLE t){
	vector<ofVec2f> pts;
	pts.push_back( ofVec2f(vertices[t.p1].x, vertices[t.p1].y));
	pts.push_back( ofVec2f(vertices[t.p2].x, vertices[t.p2].y));
	pts.push_back( ofVec2f(vertices[t.p3].x, vertices[t.p3].y));
	return pts;
}

ITRIANGLE ofxDelaunay::getTriangleForPos(ofVec2f pos){

	ITRIANGLE ti;

	for(int i = 0; i < ntri ; i++){
        XY p0; p0.x = vertices[triangles[i].p1].x; p0.y = vertices[triangles[i].p1].y;
        XY p1; p1.x = vertices[triangles[i].p2].x; p1.y = vertices[triangles[i].p2].y;
        XY p2; p2.x = vertices[triangles[i].p3].x; p2.y = vertices[triangles[i].p3].y;
		bool inside = ptInTriangle(pos, p0, p1, p2);
		if(inside) {
			ti = triangles[i];
			break;
		}
	}
	return ti;
}

int XYICompare(const void *v1, const void *v2){
	XYI *p1, *p2;

	p1 = (XYI*)v1;
	p2 = (XYI*)v2;
	if(p1->x < p2->x)
		return(-1);
	else if(p1->x > p2->x)
		return(1);
	else
		return(0);
}
