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
 *
 *  Ported to GLM by Rafael Redondo 17/12/2018
 */

#include "ofxDelaunay.h"

void ofxDelaunay::reset(){
    vertices.clear();
    triangles.clear();
	triangleMesh.clear();
	nTriangles = 0;
}

// ------------------------------------------------------------------------------------------------------
int ofxDelaunay::addPoint( float x, float y, float z ){
    XYZI v;
    v.x = x;
    v.y = y;
    v.z = z;
    v.i = vertices.size();
    vertices.push_back(v);
    return vertices.size();
}

int ofxDelaunay::addPoint( const ofDefaultVec3& point ){
	return addPoint( point.x, point.y, point.z );
}

int ofxDelaunay::addPoints( vector<ofDefaultVec3>& points ){
	for(int i = 0; i < points.size(); i++){
        addPoint( points[i] );
    }
    return vertices.size();
}

void ofxDelaunay::setPointAtIndex(ofDefaultVec3 p, int index){
    if (index >= 0 && index < vertices.size()){
        XYZI pp; pp.x = p.x; pp.y = p.y; pp.z = p.z; pp.i = index;
        vertices[index] = pp;
    }
    triangulate();
}

void ofxDelaunay::removePointAtIndex(int index){
    if (index >= 0 && index < vertices.size()){
        vertices.erase(vertices.begin()+index);
    }
    triangulate();
}

// ------------------------------------------------------------------------------------------------------
ofDefaultVec3 ofxDelaunay::getPointNear(ofDefaultVec3 pos, float minimumDist, int & index){
    XYZI ret;
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
    return ofDefaultVec3(ret.x, ret.y, ret.z);
}

ITRIANGLE ofxDelaunay::getTriangleAtIndex(int index){
    if (index >= 0 && index < nTriangles){
        return triangles[index];
    }
    return ITRIANGLE();
}


vector<ofDefaultVec3> ofxDelaunay::getPointsForITriangle(ITRIANGLE triangle){
    vector<ofDefaultVec3> pts;
    pts.push_back( ofDefaultVec3(vertices[triangle.p1].x, vertices[triangle.p1].y, vertices[triangle.p1].z));
    pts.push_back( ofDefaultVec3(vertices[triangle.p2].x, vertices[triangle.p2].y, vertices[triangle.p2].z));
    pts.push_back( ofDefaultVec3(vertices[triangle.p3].x, vertices[triangle.p3].y, vertices[triangle.p3].z));
    return pts;
}

int ofxDelaunay::getNumTriangles(){
	return nTriangles;
}

int ofxDelaunay::getNumPoints(){
	return vertices.size();
}

// ------------------------------------------------------------------------------------------------------
bool ofxDelaunay::pointIsInsideTriangle(const ofDefaultVec3 & p, const XYZ & p0, const XYZ & p1, const XYZ& p2) {
    float A = 0.5f * (-p1.y * p2.x + p0.y * (-p1.x + p2.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y);
    float sign = A < 0.0f ? -1.0f : 1.0f;
    float s = (p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y) * sign;
    float t = (p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y) * sign;
    
    return s > 0.0f && t > 0.0f && (s + t) < 2.0f * A * sign;
}

int ofxDelaunay::XYZICompare(const void *v1, const void *v2){
    XYZI *p1, *p2;
    p1 = (XYZI*)v1;
    p2 = (XYZI*)v2;
    if(p1->x < p2->x)
        return(-1);
    else if(p1->x > p2->x)
        return(1);
    else
        return(0);
}

// ------------------------------------------------------------------------------------------------------
ITRIANGLE ofxDelaunay::getTriangleForPos(ofDefaultVec3 pos){
	
	ITRIANGLE triangle;

	for(int i = 0; i < nTriangles ; i++){
        XYZ p0; p0.x = vertices[triangles[i].p1].x; p0.y = vertices[triangles[i].p1].y; p0.z = vertices[triangles[i].p1].z;
        XYZ p1; p1.x = vertices[triangles[i].p2].x; p1.y = vertices[triangles[i].p2].y; p1.z = vertices[triangles[i].p2].z;
        XYZ p2; p2.x = vertices[triangles[i].p3].x; p2.y = vertices[triangles[i].p3].y; p2.z = vertices[triangles[i].p3].z;

		if(pointIsInsideTriangle(pos, p0, p1, p2)) {
			triangle = triangles[i];
			break;
		}
	}
	return triangle;
}

// ------------------------------------------------------------------------------------------------------
int ofxDelaunay::triangulate(){
    
    if(vertices.size() < 3){
        return NULL;
    }
    
    int nv = vertices.size();
    
    // make clone not to destroy vertices
    vector<XYZI> verticesTemp = vertices;
    qsort( &verticesTemp[0], verticesTemp.size(), sizeof( XYZI ), XYZICompare );
    
    //vertices required for Triangulate
    vector<XYZ> verticesXYZ;
    
    //copy XYZIs to XYZ
    for (int i = 0; i < nv; i++) {
        XYZ v;
        v.x = verticesTemp.at(i).x;
        v.y = verticesTemp.at(i).y;
        v.z = verticesTemp.at(i).z;
        verticesXYZ.push_back(v);
    }
    
    //add 3 emptly slots, required by the Triangulate call
    verticesXYZ.push_back(XYZ());
    verticesXYZ.push_back(XYZ());
    verticesXYZ.push_back(XYZ());
    
    //allocate space for triangle indices
    triangles.resize(3*nv);
    
    Triangulate( nv, &verticesXYZ[0], &triangles[0], nTriangles );
    
    //copy triangle data to ofxDelaunayTriangle.
    triangleMesh.clear();
    triangleMesh.setMode(OF_PRIMITIVE_TRIANGLES);
    
    //copy vertices
    for (int i = 0; i < nv; i++){
        triangleMesh.addVertex(ofDefaultVec3(vertices[i].x,vertices[i].y,vertices[i].z));
    }
    
    //copy triangles
    for(int i = 0; i < nTriangles; i++){
        triangleMesh.addIndex(verticesTemp.at(triangles[ i ].p1).i);
        triangleMesh.addIndex(verticesTemp.at(triangles[ i ].p2).i);
        triangleMesh.addIndex(verticesTemp.at(triangles[ i ].p3).i);
    }
    
    return nTriangles;
}

// ------------------------------------------------------------------------------------------------------
void ofxDelaunay::draw(){
    if(ofGetStyle().bFill)
        triangleMesh.draw();
    else triangleMesh.drawWireframe();
}
