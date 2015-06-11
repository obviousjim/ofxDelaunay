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

struct XYI{
	double x, y;
	int i; // index
};

int XYICompare(const void *v1, const void *v2);

class ofxDelaunay {

public:

	void reset();

	int addPoint( const ofVec2f& point );
	int addPoint( float x, float y );
	int addPoints( vector<ofVec2f>& points );

	ofVec2f getPointNear(ofVec2f pos, float minDist, int & index); //returns actual point AND index to point
	ITRIANGLE getTriangleForPos(ofVec2f pos); //returns ITRIANGLE(0,0,0) if none found!
	void removePointAtIndex(int index); //invalidates triangles and mesh
	void setPointAtIndex(ofVec2f p, int index); //invalidates mesh
	ofVec2f getPointAtIndex(int index);
	vector<ofVec2f> getPointsForITriangle(ITRIANGLE t);
	vector<int> getPointsIndicesForITriangle(ITRIANGLE t);
	int getNumTriangles();
	int getNumPoints();
	ITRIANGLE getTriangleAtIndex(int index);

	int  triangulate();
	void draw();

    ofMesh triangleMesh; //output of triangulate();


private:

	    vector<XYI> vertices; //only input of triangulate();
		vector<ITRIANGLE> triangles; //output of triangulate();
		int ntri; //# tri


};


