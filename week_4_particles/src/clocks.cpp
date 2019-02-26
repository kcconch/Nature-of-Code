#include "clocks.h"

Clock::Clock() {
}

void Clock::setup(float _x, float _y) {

	x = _x;
	y = _y;

}

void Clock::update(float _mouseX, float _mouseY, int _i) {
	
	i = _i;
	mx = _mouseX;
	my = _mouseY;
}

void Clock::draw() {
	float time = ofGetElapsedTimef();

	ofVec2f pos;
	ofVec2f mpos;

	pos.set(x, y);
	mpos.set(mx, my);

	pos.normalize();
	mpos.normalize();

	//x *= sin(time);
	//y *= cos(time);

	
	float ang = (pos.angle(mpos)) * 2 ;
	float cAng = (pos.angle(mpos)/90) + 1;
	float dist = - + pos.distance(mpos) * 5;
	float cDist = pos.distance(mpos);

	ofSetColor(cDist*cAng*255*mpos.x, cDist*cAng*255*mpos.y*pos.x, cDist*cAng*255*pos.y);
	/*ofNoFill();*/
	//first and last set of values are curve control points
	ofDrawCurve(x + dist * (cos(time+i) * ang ), y + dist * (sin(time+i) * ang ), x, y, mx, my, mx + dist * (cos(time + i) * ang), my + dist * (sin(time + i) * ang));

}