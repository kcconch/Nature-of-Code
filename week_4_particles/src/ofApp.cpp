#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetFrameRate(144);
	
	ofBackground(0, 0, 0);

	img.load("texture.png");
	
	float size = 180;

	for (int x = size; x < ofGetWidth(); x += size) {
		for (int y = size; y < ofGetHeight(); y += size) {
			//Clock tempClock; // create ball object
			//tempClock.setup(x,y); // set initial state
			//clocks.push_back(tempClock); // push to array

			tmpPs = new particleSystem(ofPoint(x, y), img);
			//tmpPs(ofPoint(x, y), img);
			ps.push_back(tmpPs);
		}
	}
	
}

//--------------------------------------------------------------
void ofApp::update(){
	float dx = ofMap(mouseX, 0, ofGetWidth(), -0.2, 0.2);

	for (int i = 0; i < ps.size(); i++) {
		//ps[i].update(mouseX, mouseY, i); // calling the object's update method
		ps[i]->addParticle();
		wind = *new ofPoint(dx, 0);
		ps[i]->applyForce(wind);
		ps[i]->update();
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	for (int i = 0; i < ps.size(); i++) {
		ps[i]->display();
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
