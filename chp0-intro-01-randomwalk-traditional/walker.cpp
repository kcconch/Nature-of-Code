
#include "ofApp.h"
#include "walker.h"

walker::walker() {
    x = 0;
    y = 0;
    scale = 5;
}


void walker::update() {
	int coinflip = int(ofRandom(1));
    int choice = int(ofRandom(4));
    cout << choice << endl;
    
	if (coinflip == 0) {
		if (choice == 0) x++;
		else if (choice == 1) x--;
		else if (choice == 2) y++;
		else if (choice == 3) y--;
	}
	else if (coinflip == 1) {
		ofVec2f direction;
		oldPosition.set(x, y);
		newPosition.set(mouseX, mouseY);
		direction = newPosition - oldPosition;
		x += direction.x;
		y += direction.y;


	}

		
    
    int rangeWidth = (ofGetWidth()/2) /scale;
    int rangeHeight = (ofGetHeight()/2) /scale;
    
    x = constrain(x, -rangeWidth, rangeWidth);
    y = constrain(y, -rangeHeight, rangeHeight);
}


void walker::draw() {
    ofSetColor(0,255);
    ofDrawRectangle(x, y, 2, 2);
}

void walker::setNewPosition(ofVec2f _pos) {
	oldPosition = newPosition;
	newPosition = _pos;
	direction = newPosition - oldPosition;
}


int walker::constrain(int input, int min, int max) {
    return (input < min) ? min : ((input > max) ? max : input);
}