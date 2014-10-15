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
    XYZI v;
    v.x = x;
    v.y = y;
    v.z = z;
	v.i = vertices.size();
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
    
	int ntri;
	Triangulate( nv, &verticesXYZ[0], &triangles[0], ntri );
	
	//copy triangle data to ofxDelaunayTriangle.
	triangleMesh.clear();
	triangleMesh.setMode(OF_PRIMITIVE_TRIANGLES);
	
    //copy vertices
	for (int i = 0; i < nv; i++){
        triangleMesh.addVertex(ofVec3f(vertices[i].x,vertices[i].y,vertices[i].z));
    }
	
	//copy triangles
	for(int i = 0; i < ntri; i++){
		triangleMesh.addIndex(verticesTemp.at(triangles[ i ].p1).i);
		triangleMesh.addIndex(verticesTemp.at(triangles[ i ].p2).i);
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
}

int XYZICompare(const void *v1, const void *v2){
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
