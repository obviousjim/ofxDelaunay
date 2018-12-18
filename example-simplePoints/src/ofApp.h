#pragma once

#include "ofMain.h"
#include "ofxDelaunay.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void mousePressed(int x, int y, int button);
		
        ofxDelaunay triangulation;

};
