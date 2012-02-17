/*
 *  ofxDelaunay.cpp
 *  openFrameworks
 *
 *  Created by Pat Long on 29/10/09.
 *  Copyright 2009 Tangible Interaction. All rights reserved.
 *
 *  Some parts based on demo code by Gilles Dumoulin. 
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *
 */
#include "ofxDelaunay.h"

ofxDelaunay :: ofxDelaunay()
{
	p = NULL;
	v = NULL;
	
	setMaxPoints();
}

ofxDelaunay :: ~ofxDelaunay()
{
	if( p != NULL )
	{
		delete[] p;
		p = NULL;
	}
	if( v != NULL )
	{
		delete[] v;
		v = NULL;
	}
}

void ofxDelaunay :: setMaxPoints ( int maxPoints )
{
	this->maxPoints = maxPoints;
	
	reset();
}

void ofxDelaunay :: reset()
{
	if( p != NULL )
	{
		delete[] p;
	}
	
	p = new XYZ[ maxPoints ];
	
	if( v != NULL )
	{
		delete[] v;
		v = NULL;
	}
	
	nv = 0;
	ntri = 0;
	
	triangles.clear();
}

int ofxDelaunay :: addPoint( const ofPoint& point )
{
	int i = addPoint( point.x, point.y, point.z );
	return i;
}

int ofxDelaunay :: addPoint( float x, float y, float z )
{
	if( nv < maxPoints )
	{
		p[ nv ].x = x;
		p[ nv ].y = y;
		p[ nv ].z = z;
		++nv;
	}
	
	return nv;
}

int ofxDelaunay :: triangulate()
{
	XYZ *p_Temp = new XYZ[ nv + 3 ];
	for( int i=0; i<nv; i++ )
	{
		p_Temp[ i ] = p[ i ];
	}
	delete[] p;
	p = p_Temp;
	
	v = new ITRIANGLE[ 3 * nv ];
	qsort( p, nv, sizeof( XYZ ), XYZCompare );
	Triangulate( nv, p, v, ntri );
	
	// copy triangle data to ofxDelaunayTriangle.
	
	triangles.clear();
	
	for( int i=0; i<ntri; i++ )
	{
		triangles.push_back( ofxDelaunayTriangle() );
		ofxDelaunayTriangle& triangle = triangles.back();
		
		int p1, p2, p3;
		p1 = v[ i ].p1;
		p2 = v[ i ].p2;
		p3 = v[ i ].p3;
		
		triangle.pointIndex[ 0 ] = p1;
		triangle.pointIndex[ 1 ] = p2;
		triangle.pointIndex[ 2 ] = p3;
		
		triangle.points[ 0 ].set( p[ p1 ].x, p[ p1 ].y, p[ p1 ].z );
		triangle.points[ 1 ].set( p[ p2 ].x, p[ p2 ].y, p[ p2 ].z );
		triangle.points[ 2 ].set( p[ p3 ].x, p[ p3 ].y, p[ p3 ].z );
	}
	
	return ntri;
}

void ofxDelaunay :: draw ()
{
	for( int i=0; i<triangles.size(); i++ )
	{
		ofxDelaunayTriangle& triangle = triangles[ i ];
		
		ofTriangle
		(
			triangle.points[ 0 ].x,
			triangle.points[ 0 ].y,
			triangle.points[ 1 ].x,
			triangle.points[ 1 ].y,
			triangle.points[ 2 ].x,
			triangle.points[ 2 ].y
		);
	}
}
