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

void ofxDelaunay::reset(){
    vertices.clear();
    triangles.clear();
	triangleMesh.clear();
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
    
    int nv = vertices.size();
    //add 3 emptly slots, required by the Triangulate call
    vertices.push_back(XYZ());
    vertices.push_back(XYZ());
    vertices.push_back(XYZ());
    
    //allocate space for triangle indices
    triangles.resize(3*nv);
    
	int ntri;
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
	
	return ntri;
}

void ofxDelaunay::draw(){
	if(ofGetStyle().bFill){
	    triangleMesh.draw();
    }
    else{
    	triangleMesh.drawWireframe();    
    }
}
