/*
 *  Delaunay.h
 *
 *  Gilles Dumoulin's C++ implementation of Paul Bourke's Delaunay triangulation algorithm.
 *  Source: http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/cpp.zip
 *  Paper:  http://local.wasp.uwa.edu.au/~pbourke/papers/triangulate/index.html
 *
 */

#ifndef Delaunay_H
#define Delaunay_H

#include <iostream>
#include <stdlib.h> // for C qsort
#include <cmath>
#include <time.h> // for random

const double EPSILON = 0.000001;

struct ITRIANGLE{
    int p1, p2, p3;
};

struct IEDGE{
    int p1, p2;
};

struct XY{
    double x, y;
};

int XYCompare(const void *v1, const void *v2);
int Triangulate(int nv, XY pxyz[], ITRIANGLE v[], int &ntri);
int CircumCircle(double, double, double, double, double, double, double,
double, double&, double&, double&);

#endif


