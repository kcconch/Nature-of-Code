#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	
	ofBackground(0, 0, 0);
	
	float size = 50;

	for (int x = size; x < ofGetWidth(); x += size) {
		for (int y = size; y < ofGetHeight(); y += size) {
			Clock tempClock; // create ball object
			tempClock.setup(x,y); // set initial state
			clocks.push_back(tempClock); // push to array
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::update(){

	for (int i = 0; i < clocks.size(); i++) {
		clocks[i].update(mouseX, mouseY, i); // calling the object's update method
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	for (int i = 0; i < clocks.size(); i++) {
		clocks[i].draw(); // calling the object's draw method
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
