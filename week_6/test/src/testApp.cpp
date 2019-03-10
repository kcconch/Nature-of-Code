#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	w = ofGetWidth();
	h = ofGetHeight();
	vid.initGrabber(w, h, true);
}

//--------------------------------------------------------------
void testApp::update() {
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	vid.update();
}

//--------------------------------------------------------------
void testApp::draw() {
	ofBackground(0, 0, 0);
	ofSetColor(255, 255, 255);

	for (int y = 0; y < h; y += 10) {
		for (int x = 0; x < w; x += 10) {

			//get pixel color
			int i = (y*w + x) * 3;
			int r = vid.getPixels()[i + 0];
			int g = vid.getPixels()[i + 1];
			int b = vid.getPixels()[i + 2];

			//calculate brightness
			float br = (r + b + g) / 765.0;

			//use HSB color
			ofSetColor(ofColor::fromHsb(br * 255, 255, 255));

			//calculate sine and cosine of brightness
			float co = cos(br*TWO_PI);
			float si = sin(br*TWO_PI);

			//draw rotated line based on brightness
			ofLine(x - 5 * co, y - 5 * si, x + 5 * co, y + 5 * si);
		}
	}
}


//////////////////////////////
/// You don't need the code below but it shows
/// you how to convert from RGB to HSB yourself.
//////////////////////////////

void ofxSetColorHSB(float h, float s, float b, float a) {
	if (b == 0) return ofSetColor(0, 0, 0, a); //black
	if (s == 0) return ofSetColor(b, b, b, a); //gray

	h *= 6.0 / 255.0;
	s *= 1.0 / 255.0;

	float x = b * (1 - s);
	float y = b * (1 - s * (h - int(h)));
	float z = b * (1 - s * (1 - (h - int(h))));

	switch (int(h)) {
	case 0: return ofSetColor(b, z, x, a); //red
	case 1: return ofSetColor(y, b, x, a); //green
	case 2: return ofSetColor(x, b, z, a);
	case 3: return ofSetColor(x, y, b, a); //blue
	case 4: return ofSetColor(z, x, b, a);
	case 5: return ofSetColor(b, x, y, a); //back to red
	}
}