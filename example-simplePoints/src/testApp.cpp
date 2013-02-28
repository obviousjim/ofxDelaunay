#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
    ofBackground(0);
    
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofNoFill();
    triangulation.draw();
    ofDrawBitmapString("'r' to reset", ofPoint(10,20));
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'r'){
        triangulation.reset();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

    triangulation.addPoint(ofPoint(x,y));
    triangulation.triangulate();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}