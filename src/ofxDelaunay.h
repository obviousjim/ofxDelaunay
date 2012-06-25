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

#pragma once

#include "ofMain.h"
#include "Delaunay.h"

class ofxDelaunay {
    
public:
    
	void reset();
	
	int addPoint( const ofPoint& point );
	int addPoint( float x, float y, float z);
	int addPoints( vector<ofPoint>& points );
	
	int  triangulate();
	void draw();
	
    ofMesh triangleMesh;
	
private:
    vector<ITRIANGLE> triangles;
    vector<XYZ> vertices;
	
};


