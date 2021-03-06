#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(255*.15);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    timeline.setup();
    timeline.setLoopType(OF_LOOP_NORMAL);
    timeline.setBPM(120.0);
    timeline.toggleSnapToBPM();
    timeline.setShowBPMGrid(true);
    
    //when i switch to frames it bangs twice.
    timeline.setFrameRate(60);
    timeline.setFrameBased(true);
    timeline.setDurationInFrames(480);//4 measures @ 120bpm
    timeline.setOffset(ofVec2f(50,0));
    timeline.setWidth(1000);
    
    
    notesTrack = new ofxTLVMMNotes();
    notesTrack->setRange(ofRange(60,72));
    timeline.addTrack("My TL Notes Track", notesTrack);
    
    //curves track for comparison
    //timeline.addCurves("My Curves Track", "myCurvesTrack.xml",ofRange(-10.0,10.0),0.0);

    //timeline.getTicker()->bpmUnit;
    
    
}

//--------------------------------------------------------------
void ofApp::update(){

    curtime = timeline.getCurrentTime();
    
    //cout << timeline.getTicker()->bpmUnit << endl;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    string title = ofToString(curtime) + " : " + ofToString(timeline.getCurrentTimeMillis());
    
    ofSetWindowTitle(title);
    timeline.draw(false,false);
    
    ofDrawBitmapString("TEST", 10, 40);
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
