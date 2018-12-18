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
 *  Ported to GLM (legacy support) by Rafael Redondo 17/12/2018
 */

#pragma once

#include "ofMain.h"
#include "Delaunay.h"

struct XYZI{
	double x, y, z;     //< Triangle Coordinates
	int i;              //< Triangle index
};

int XYZICompare(const void *v1, const void *v2);

class ofxDelaunay {
    
public:
    
    /// \brief reset() removes all vertices, triangles and meshes
	void reset();
	
    /// \brief add points for triangulation
    int addPoint( const ofDefaultVec3& point );
    int addPoint( float x, float y, float z);
	int addPoints( vector<ofDefaultVec3>& points );
    
    /// \brief setPointAtIndex invalidates mesh
    void setPointAtIndex(ofDefaultVec3 p, int index);

    /// \brief removePointAtIndex invalidates triangles and mesh
    void removePointAtIndex(int index);

    /// \return actual point AND index to point
    ofDefaultVec3 getPointNear(ofDefaultVec3 pos, float minDist, int & index);
    
    /// \return returns ITRIANGLE(0,0,0) if none found!
    ITRIANGLE getTriangleForPos(ofDefaultVec3 pos);

    /// \brief getPointsForITriangle retrieves vertices for a triangle
    /// \param t stands for the triangle
    /// \return vertices in vector format
    vector<ofDefaultVec3> getPointsForITriangle(ITRIANGLE triangle);
    
    /// \brief getNumTriangles gets the total number of triangles
    /// \return number of triangles
	int getNumTriangles();
    
    /// \brief getNumPoints returns the total the number of inserted points
    /// \return number of points
	int getNumPoints();
    
    /// \brief getTriangleAtIndex returns the vertices of a given triangle
    /// \param index triangle index
    /// \return the triangle vertices
	ITRIANGLE getTriangleAtIndex(int index);

    /// \brief triangulate is where all the magic happens. Call this function after points have been inserted.
	int  triangulate();
    
    /// \brief draw() displays the resulting Delaunay triangulation
	void draw();
	
    ofMesh triangleMesh;                //< Output of triangulate();


private:

    vector<XYZI> vertices;                          //< Only input of triangulate();
    vector<ITRIANGLE> triangles;                    //< Output of triangulate();
    int nTriangles;                                 //< Number of triangles
    
    /// \brief Auxiliary method to check if a point is inside a triangle
    bool pointIsInsideTriangle(const ofDefaultVec3 & p, const XYZ & p0, const XYZ & p1, const XYZ& p2);
    
    /// \brief Compare two vertices
    static int XYZICompare(const void *v1, const void *v2);
};


