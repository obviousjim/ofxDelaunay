# ofxDelaunay

Paul Bourke's Delaunay Triangulation implementation.

This is a big refactor from Julapy (Lukasz Karluk) and Pat Long's version (http://code.google.com/p/julapy/source/browse/trunk/openframeworks/ofxDelaunay/?r=708) 

Changes:
* Now uses ofMesh rather than custom data structure
* uses std::vector for internal storage, which removes need for reallocating memory or constructor/destructors
* Removed need for setMaxPoints(). No max points enforced
