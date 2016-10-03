#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

}

//--------------------------------------------------------------
void ofApp::update() {
	_wave.step();
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(0);
	
	_camera.begin();
	ofPushMatrix();
	ofRotateZ(90.0f);
	ofSetColor(255);
	ofDrawGridPlane(10.0f);
	ofPopMatrix();

	ofPushMatrix();
	// float s = 0.5f;
	float s = 1.0f;
	ofScale(s, s, s);
	ofDrawAxis(50);

	_mesh.clear();

	float stepX = _wave.width() / _wave.widthN();
	float stepY = _wave.height() / _wave.heightN();

	for (int x = 0; x < _wave.widthN(); ++x) {
		for (int y = 0; y < _wave.heightN(); ++y) {
			float value = _wave.value(x, y);
			_mesh.addVertex(ofVec3f(x * stepX, value, y * stepY));
		}
	}
	glPointSize(2);
	_mesh.drawVertices();
	ofPopMatrix();
	_camera.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == ' ') {
		_wave.reset();
	}
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
