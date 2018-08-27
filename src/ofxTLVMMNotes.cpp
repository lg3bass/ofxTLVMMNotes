//
//  ofxTLSequencer.cpp
//  example-emptyTrackTemplate
//
//  Created by Leo on 12/1/17.
//

#include "ofxTLVMMNotes.h"
#include "ofxTimeline.h"
#include "ofxHotKeys.h"



ofxTLVMMNote::ofxTLVMMNote(int p){
    pitch = p;
    duration = 0;
    //default values for testing purposes
    attackDuration = 0.0625;
    decayDuration = 0.0625;
    sustainDuration = 0.125;
    releaseDuration = 0.125;
    
    //1.0 == quarter note
    ADSR = ofVec4f(0.5, 0.5, 2.0, 1.0); //default values
    //ADSR = ofVec4f(0.0625, 0.0625, 0.125, 0.125); //TBD - testing
    notePlaying = false;
    
    frame = 0;
}

string ofxTLVMMNote::getPitchDisplay() {
    
    string noteStr = "";
    int mod = pitch % 12;
    
    switch (mod) {
        case 0:
            noteStr = "C";
            break;
        case 1:
            noteStr = "C#";
            break;
        case 2:
            noteStr = "D";
            break;
        case 3:
            noteStr = "Eb";
            break;
        case 4:
            noteStr = "E";
            break;
        case 5:
            noteStr = "F";
            break;
        case 6:
            noteStr = "F#";
            break;
        case 7:
            noteStr = "G";
            break;
        case 8:
            noteStr = "Ab";
            break;
        case 9:
            noteStr = "A";
            break;
        case 10:
            noteStr = "Bb";
            break;
        case 11:
            noteStr = "B";
            break;
        default:
            break;
    }
    
    string display = ofToString(pitch) + "-" + noteStr;
    
    return ofToString(display);
}

//----------------------------

ofxTLVMMNotes::ofxTLVMMNotes(){
    
    initializeNotes();
    
    setRange(ofRange(60,72));
    drawingEasingWindow = false;
    
    //ofxTween test
    clamp = false;
    easingType = ofxTween::easeInOut;
    easestring = "ease in/out";
    
    //osc test
    sender.setup("localhost", 7005);
    
}

ofxTLVMMNotes::~ofxTLVMMNotes(){
    
}

void ofxTLVMMNotes::update(){
    
    //this needs to be done from update otherwise we get double triggers
    sendNoteOnEvent();

}


void ofxTLVMMNotes::draw(){
    ofPopStyle();//need this for some reason.
    
    if(bounds.width == 0 || bounds.height < 2){
        return;
    }

    if(shouldRecomputePreviews || viewIsDirty){
        recomputePreviews();
    }
    
    drawBackgroundGrid();
  
    ofPushStyle();
    
    ofFill();
    for(int i = 0; i < keyframes.size(); i++){
        //make sure it's on screen
        if(isKeyframeIsInBounds(keyframes[i])){
            //we know the type because we created it in newKeyframe()
            //so we can safely cast
            ofxTLVMMNote* note = (ofxTLVMMNote*)keyframes[i];
            ofVec2f screenPoint = screenPositionForKeyframe(note);
            
            
            if(hoverKeyframe == note){
                //hovering over a note.
                ofSetColor(timeline->getColors().highlightColor, 100);
                drawNote(screenPoint, note, true);
                
                //draw note name
                ofSetColor(timeline->getColors().textColor);
                timeline->getFont().drawString(note->getPitchDisplay(), screenPoint.x, screenPoint.y - 5);
            }
            else if(isKeyframeSelected(note)){
                //selected note
                ofSetColor(timeline->getColors().textColor);
                drawNote(screenPoint, note, true);
                
                //draw note name
                ofSetColor(timeline->getColors().textColor);
                timeline->getFont().drawString(note->getPitchDisplay(), screenPoint.x, screenPoint.y - 5);
            } else {
            
                ofSetColor(timeline->getColors().keyColor);
                drawNote(screenPoint,note, false);
                
            }
            
            //draw red dot where the key is
            ofSetColor(ofColor::red);
            ofDrawCircle(screenPoint, 1);

            
            //draw my note info
            long thisTimelinePoint = currentTrackTime();
            float t = keyframes[i]->time;
            float oneBeatInMS = getNoteDuration(timeline->getBPM(), 1.0, false);
            
            //calculate how long each ADSR component is.
            float a = (note->ADSR[0]*oneBeatInMS);
            float d = (note->ADSR[1]*oneBeatInMS);
            float s = (note->ADSR[2]*oneBeatInMS);
            float r = (note->ADSR[3]*oneBeatInMS);
            
            //ADSR components
            //attack
            if(thisTimelinePoint >= t && thisTimelinePoint <= t+a){
                ofSetColor(ofColor::red);
                ofDrawBitmapString(note[i].getPitchDisplay(), t, screenPoint.y + 20);
                //intFrame = ofxTween::map(thisTimelinePoint, t, t+a, 0, 10, clamp, easeLinear, easingType);
                note->frame = ofxTween::map(thisTimelinePoint, t, t+a, 0, 10, clamp, easeLinear, easingType);
                cout << "outFrame(a): " << i << " - " << note->frame << endl;
                sendOSC(note->pitch-60, note->frame);
            }

            //decay
            if(thisTimelinePoint >= t+a && thisTimelinePoint <= t+a+d) {
                ofSetColor(ofColor::green);
                ofDrawBitmapString(note[i].getPitchDisplay(), t+a, screenPoint.y + 20);
                //intFrame = ofxTween::map(thisTimelinePoint, t+a, t+a+d, 11, 15, clamp, easeLinear, easingType);
                note->frame = ofxTween::map(thisTimelinePoint, t+a, t+a+d, 11, 15, clamp, easeLinear, easingType);
                cout << "outFrame(d): " << i << " - " << note->frame << endl;
                sendOSC(note->pitch-60, note->frame);
            }

            //sustain
            if(thisTimelinePoint >= t+a+d && thisTimelinePoint <= t+a+d+s) {
                ofSetColor(ofColor::blue);
                ofDrawBitmapString(note[i].getPitchDisplay(), t+a+d, screenPoint.y + 20);
                //intFrame = 16;
                note->frame = 16;
                cout << "outFrame(s): " << i << " - " << note->frame << endl;
                sendOSC(note->pitch-60, note->frame);
            }
            
            //release
            if(thisTimelinePoint >= t+a+d+s && thisTimelinePoint <= t+a+d+s+r) {
                ofSetColor(ofColor::lightBlue);
                ofDrawBitmapString(note[i].getPitchDisplay(), t+a+d+s, screenPoint.y + 20);
                //intFrame = ofxTween::map(thisTimelinePoint, t+a+d+s, t+a+d+s+r, 17, 30, clamp, easeLinear, easingType);
                note->frame = ofxTween::map(thisTimelinePoint, t+a+d+s, t+a+d+s+r, 17, 30, clamp, easeLinear, easingType);

                cout << "outFrame(r): " << i << " - " << note->frame << endl;
                sendOSC(note->pitch-60, note->frame);
            }
            
            //send my osc out port 7005
            //sendOSC(intFrame);
            
            
            
        }// end if
        
    }// end for
    
        //Text field on the top left showing the note range.
        display = ofRectangle(bounds.x , bounds.y , 50, 20);
    
        textField.bounds.x = display.x;
        textField.bounds.y = display.y;
    
        ofSetColor(0);
        ofDrawRectangle(display);
    
        ofSetColor(255);
        textField.draw();
    
    ofPopStyle();
    
}

void ofxTLVMMNotes::drawBackgroundGrid() {
  
    ofPushStyle();

    int white = 200;
    int black = 200;
    
    for (int i = 0; i <= gridRows.size(); i++) {
        
        int mod = i%12;
        
        switch (mod) {
            case 0:
                ofSetColor(255, 255, 255, white);
                break;
            case 1:
                
                ofSetColor(255, 255, 255, black);
                break;
            case 2:
                ofSetColor(255, 255, 255, white);
                break;
            case 3:
                ofSetColor(255, 255, 255, black);
                break;
            case 4:
                ofSetColor(255, 255, 255, white);
                break;
            case 5:
                ofSetColor(255, 255, 255, white);
                break;
            case 6:
                ofSetColor(255, 255, 255, black);
                break;
            case 7:
                ofSetColor(255, 255, 255, white);
                break;
            case 8:
                ofSetColor(255, 255, 255, black);
                break;
            case 9:
                ofSetColor(255, 255, 255, white);
                break;
            case 10:
                ofSetColor(255, 255, 255, black);
                break;
            case 11:
                ofSetColor(255, 255, 255, white);
                break;
            default:
                break;
        }

        float y = (bounds.y + bounds.height) - (gridRows[i].yPos * bounds.height);
        ofDrawLine(bounds.x, y, bounds.x + bounds.width, y);
    }
    ofPopStyle();
    
}

//This draw function executes only when timeline->presentedModalContent(this) called
//any click outside of note will close the modal.
void ofxTLVMMNotes::drawModalContent(){
    
    //****** DRAW EASING CONTROLS
    if(!drawingEasingWindow){
        return;
    }
    
    cout << ".";
    

    for(int i = 0; i < keyframes.size(); i++){
        //make sure it's on screen
        if(isKeyframeIsInBounds(keyframes[i])){
            ofxTLVMMNote* note = (ofxTLVMMNote*)keyframes[i];
            ofVec2f screenPoint = screenPositionForKeyframe(note);
            
            ofNoFill();
            
            //draw circle (testing)
            if(hoverKeyframe == note){
                //cout << "draw modal at: " << ofToString(floor(screenPoint.x)) << ":" << ofToString(floor(screenPoint.y)) << endl;
                
                ofSetColor(timeline->getColors().textColor);
                ofDrawCircle(screenPoint, 20);

            }
            else if(isKeyframeSelected(note)){
                
                ofSetColor(timeline->getColors().textColor);
                ofDrawCircle(screenPoint, 30);
                


            }
            
            if(isKeyframeSelected(note)){
                for(int i = 0; i < noteDurations.size(); i++){
                    
                    ofFill();
                    ofDrawRectangle(screenPoint.x + noteDurations[i]->bounds.x,
                                    screenPoint.y + noteDurations[i]->bounds.y,
                                    noteDurations[i]->bounds.width, noteDurations[i]->bounds.height);
                    ofSetColor(200, 200, 200);
                    timeline->getFont().drawString(noteDurations[i]->name,screenPoint.x + noteDurations[i]->bounds.x+11,screenPoint.y + noteDurations[i]->bounds.y+10);
                    ofNoFill();
                    ofSetColor(40, 40, 40);
                    ofDrawRectangle(screenPoint.x + noteDurations[i]->bounds.x,
                                    screenPoint.y + noteDurations[i]->bounds.y,
                                    noteDurations[i]->bounds.width, noteDurations[i]->bounds.height);
                    
                }
                /*
                //test drawing a second modal
                // -------------------------------------------
                for(int i = 0; i < noteDurations.size(); i++){
                    
                    ofFill();
                    //draw black box around all the note durations.
                    ofDrawRectangle(screenPoint.x + noteDurations[i]->bounds.x + noteDurations[i]->bounds.width,
                                    screenPoint.y + noteDurations[i]->bounds.y,
                                    noteDurations[i]->bounds.width, noteDurations[i]->bounds.height);
                    ofSetColor(200, 200, 200);
                    timeline->getFont().drawString(noteDurations[i]->name,screenPoint.x + noteDurations[i]->bounds.width + noteDurations[i]->bounds.x+11,screenPoint.y + noteDurations[i]->bounds.y+10);
                    ofNoFill();
                    ofSetColor(40, 40, 40);
                    ofDrawRectangle(screenPoint.x + noteDurations[i]->bounds.x + noteDurations[i]->bounds.width,
                                    screenPoint.y + noteDurations[i]->bounds.y,
                                    noteDurations[i]->bounds.width, noteDurations[i]->bounds.height);
                    
                }
                // -------------------------------------------
                */
            }
            
            
            
        }
    }
    
    

    
    
}

void ofxTLVMMNotes::sendOSC(int buffer, int val){
    ofxOscMessage m;
    m.setAddress("/vmmNotes");
    m.addIntArg(1);
    m.addIntArg(buffer);
    m.addIntArg(val);
    sender.sendMessage(m, false);
    
}

int ofxTLVMMNotes::getNote() {
    return getNoteAtMillis(currentTrackTime());
    
}

int ofxTLVMMNotes::getNoteAtPercent(float percent){
    unsigned long long millis = percent*timeline->getDurationInMilliseconds();
    return getNoteAtMillis(millis);
    
}

float ofxTLVMMNotes::getNoteDuration(float BPM, float value, bool normalized = false){
    float duration;
    
    if(normalized) {
        duration = (value/(BPM/60.0));
    } else {
        duration = (value/(BPM/60.0))*1000;
    }
    
    return duration;
    
}



void ofxTLVMMNotes::bangFired(ofxTLKeyframe* key){

    ofxTLBangEventArgs args;
    args.sender = timeline;
    args.track = this;
    //play solo change
    //args.currentMillis = timeline->getCurrentTimeMillis();
    args.currentMillis = currentTrackTime();
    args.currentPercent = timeline->getPercentComplete();
    args.currentFrame = timeline->getCurrentFrame();
    args.currentTime = timeline->getCurrentTime();
    //args.flag = ((ofxTLFlag*)key)->textField.text;
    
    ofxTLVMMNote* note = (ofxTLVMMNote*)key;
    cout << "[pitch " << ofToString(note->pitch) << "]," <<
            "[frame " << ofToString(timeline->getCurrentFrame()) << "]," <<
            "[duration " << ofToString(note->duration) << "]" << endl;
    
    //Notify event up to
    ///addons/ofxTimeline/src/ofxTLEvents.h:109:	ofEvent<ofxTLBangEventArgs> bangFired;
    ofNotifyEvent(events().bangFired, args);
}


int ofxTLVMMNotes::getNoteAtMillis(long millis){
    
    if(keyframes.size() == 0){
        return 0;
    }
    for(int i = 1; i < keyframes.size(); i++){
        
        if(keyframes[i]->time == millis) {
            ofxTLVMMNote* note  = (ofxTLVMMNote*)keyframes[i];
            return note->pitch;
        }
    }
    
    //vector<int> notes;
    /*
    //just one, or sampling before the first we can just return the first
    if(keyframes.size() == 1 || keyframes[0]->time >= millis){
        return ((ofxTLVMMNote*)keyframes[0])->pitch;
        
    }
    //sampling after the last we return the last
    if(keyframes[keyframes.size()-1]->time <= millis){
        return ((ofxTLVMMNote*)keyframes[keyframes.size()-1])->pitch;
    }
    
    //now we are somewhere in between, search
    //keyframes will always be sorted
    for(int i = 1; i < keyframes.size(); i++){
        ///FIXME: Siempre van a coincidir??
        if(keyframes[i]->time >= millis) {
            ofxTLVMMNote* note  = (ofxTLVMMNote*)keyframes[i];
            return note->pitch;
            //ofxTLVMMNote* prevKey  = (ofxTLVMMNote*)keyframes[i-1];
            //interpolate
//            float alpha = ofMap(sampleTime, prevKey->time, nextKey->time, 0, 1.0);
//            return prevKey->pitch.getLerped(nextKey->pitch, alpha);
        }
    }
    */
    return 0;
}

string ofxTLVMMNotes::getTrackType() {
    return "Notes";
}

void ofxTLVMMNotes::setOctavesNum(int oct) {
    
    int min = 0;
    int max = 12 * oct;
    
    setRange(ofRange(min, max));
}

void ofxTLVMMNotes::setRange(ofRange range){
    
    valueRange = range;
    textField.text = ofToString(valueRange.min) + "-" + ofToString(valueRange.max);
    gridRows.clear();
    
    for (int i = 0; i <= valueRange.span(); i++) {
        rowNormHeight = 1 / (valueRange.span()+1);
        float rowNormOffset = rowNormHeight * 0.5;
        float y = i * rowNormHeight + rowNormOffset;
        int p = valueRange.min + i;
        
        GridRow row = GridRow();
        row.yPos = y;
        row.pitch = p;
        gridRows.push_back(row);
    }
    
    quantizeAllNotesByPitch();
    
}

void ofxTLVMMNotes::didEnterText(){
    
    string s = textField.text;
    string delimiter = "-";
    
//    string token = s.substr(0, s.find(delimiter));
//
//    cout<<"token "<<token<<endl;
//    cout<<"string "<<s<<endl;

    int min;
    int max;
    
    size_t pos = 0;
    string token;
    vector<string> v;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        //std::cout << token << std::endl;
        v.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    //std::cout<< "final: " << s << std::endl;
    v.push_back(s);
    
    min = std::stoi(v[0]);
    max = std::stoi(v[1]);
    
    
    if (min < 0) {min = 0;}
    if (max < 0) {max = 0;}
    if (min > 96) {min = 96;}
    if (max > 96) {max = 96;}
    if (max <= min) {max = min+1;}
    
    setRange(ofRange(min, max));
}

bool enteringText = false;

#pragma mark mouse interactions

bool ofxTLVMMNotes::mousePressed(ofMouseEventArgs& args, long millis){
    
    if(drawingEasingWindow){
        return true;
    } else {
        //for the general behavior call the super class
        //or you can do your own thing. Return true if the click caused an item to
        //become selectd
        
        if (display.inside(args.x, args.y)) {
            if (textField.isEditing()) {
                textField.endEditing();
                enteringText = false;
                timeline->dismissedModalContent();
            } else {
                textField.beginEditing();
                enteringText = true;
                timeline->presentedModalContent(this);
            }
            return false;
        } else {
            return ofxTLKeyframes::mousePressed(args, millis);
        }
        
        //if we get all the way here we didn't click on a text field and we aren't
        //currently entering text so proceed as normal
        
        
    }
    
}

void ofxTLVMMNotes::mouseMoved(ofMouseEventArgs& args, long millis){
    ofxTLKeyframes::mouseMoved(args, millis);
}

void ofxTLVMMNotes::mouseDragged(ofMouseEventArgs& args, long millis){
	if(!drawingEasingWindow){
        ofxTLKeyframes::mouseDragged(args, millis);
    }
}

void ofxTLVMMNotes::mouseReleased(ofMouseEventArgs& args, long millis){
    
    if(drawingEasingWindow && (args.button == 0 && !ofGetModifierControlPressed()) ){
        drawingEasingWindow = false;
        timeline->dismissedModalContent();
        ofVec2f screenpoint(args.x,args.y);
        
        for(int i = 0; i < noteDurations.size(); i++){
            if(noteDurations[i]->bounds.inside(screenpoint-durationWindowPosition)){
                for(int k = 0; k < selectedKeyframes.size(); k++){
                    //((ofxTLTweenKeyframe*)selectedKeyframes[k])->easeType = easingTypes[i];
                    //cout << "duration: " << noteDurations[i]->name << ":" << noteDurations[i]->duration << endl;
                    
                    ofxTLVMMNote* note = (ofxTLVMMNote*)selectedKeyframes[k];
                    
                    note->duration = noteDurations[i]->duration;//TODO: do I need ->duration anymore?
                    note->length = noteDurations[i]->length;//TODO: do I need length

                    //TODO: case statement
                    if(noteDurations[i]->mode == 0){
                        note->ADSR[0] = noteDurations[i]->length;
                    }
                    if(noteDurations[i]->mode == 1){
                        note->ADSR[1] = noteDurations[i]->length;
                    }
                    if(noteDurations[i]->mode == 2){
                        note->ADSR[2] = noteDurations[i]->length;
                    }
                    if(noteDurations[i]->mode == 3){
                        note->ADSR[3] = noteDurations[i]->length;
                    }
                    cout << "note: " << note->pitch << ":" << note->duration << ":" << note->length << endl;
                    
                }
                timeline->flagTrackModified(this);
                shouldRecomputePreviews = true;
                return;
            } else {
                
                //cout << " you didn't hit it " << endl;
            }
        }
        
        cout << "ofxTLVMMNotes::mouseReleased - END MODAL" << endl;
    } else {
        
        //standard mouseReleased
        ofxTLKeyframes::mouseReleased(args, millis);
        
        //snap the notes on the grid.
        for(int k = 0; k < selectedKeyframes.size(); k++) {
            ofxTLVMMNote* note = (ofxTLVMMNote*)selectedKeyframes[k];
            cout << "ofxTLVMMNotes::mouseReleased - CREATE NOTE" << endl;
            quantizeNoteByPos(note);
        }
        
        
    }
}

#pragma mark note functions

void ofxTLVMMNotes::quantizeNoteByPos(ofxTLVMMNote* note){
    for (int i = 0; i <= gridRows.size(); i++) {
        float diff = abs(note->value - gridRows[i].yPos);
        if ( diff <= (rowNormHeight * 0.5) ) {
            note->value = gridRows[i].yPos;
            note->pitch = gridRows[i].pitch;
            return;
        }
    }

}

void ofxTLVMMNotes::quantizeAllNotesByPitch(){
    for(int k = 0; k < keyframes.size(); k++) {
        ofxTLVMMNote* note = (ofxTLVMMNote*)keyframes[k];
        quantizeNoteByPitch(note);
    }

}

void ofxTLVMMNotes::quantizeNoteByPitch(ofxTLVMMNote* note){    
    if (note->pitch < valueRange.min) {note->pitch = valueRange.min;}
    else if (note->pitch > valueRange.max) {note->pitch = valueRange.max;}
    
    for (int i = 0; i <= gridRows.size(); i++) {
        if (note->pitch == gridRows[i].pitch) {
            note->value = gridRows[i].yPos;
            return;
        }
    }
    
}



//keys pressed events, and nuding from arrow keys with normalized nudge amount 0 - 1.0
void ofxTLVMMNotes::keyPressed(ofKeyEventArgs& args){
    
    if (enteringText) {
        if(args.key == OF_KEY_RETURN){
            textField.endEditing();
            enteringText = false;
            timeline->dismissedModalContent();
            //timeline->flagTrackModified(this);
            didEnterText();
            
            
        } else {
            textField.keyPressed(args);
        }
        
    } else {
        ofxTLKeyframes::keyPressed(args);
    }
}

void ofxTLVMMNotes::regionSelected(ofLongRange timeRange, ofRange valueRange){
    //you can override the default to select other things than just dots
    ofxTLKeyframes::regionSelected(timeRange, valueRange);
}



void ofxTLVMMNotes::drawNote(ofVec2f pos, ofxTLVMMNote* note, bool highlight){
    
    //double oneBeat = 1.0/(timeline->getBPM()/60.0);
    double oneBeat = getNoteDuration(timeline->getBPM(), 1.0, true);//60 is fps
    double beatLengthOnTimeline = screenXForTime(oneBeat) - screenXForTime(0);

    int h = 10;
    if(highlight){
        h = 14;
    }
    
    int p = h/2;
    
    float a = beatLengthOnTimeline*note->ADSR[0];
    float d = beatLengthOnTimeline*note->ADSR[1];
    float s = beatLengthOnTimeline*note->ADSR[2];
    float r = beatLengthOnTimeline*note->ADSR[3];
    
    ofPushStyle();
        if(highlight){
            ofSetColor(255,255,255);
            ofDrawCircle(pos.x, pos.y, 4);
        } else {
            ofSetColor(255,255,255);
            ofNoFill();
            ofDrawCircle(pos.x, pos.y, 10);
        }
    ofPopStyle();
    
    
    //draw attack ramp
    ofPushStyle();
    ofSetColor(255, 0, 0); 
        ofBeginShape();
            ofVertex(pos.x, pos.y);
            ofVertex(pos.x+a-1, pos.y+p+2);
            ofVertex(pos.x+a-1, pos.y-p-2);
        ofEndShape();
    ofPopStyle();
    
    //draw decay
    ofPushStyle();
        ofSetColor(0, 255, 0);
        ofBeginShape();
            ofVertex(pos.x+a, pos.y+p+2);
            ofVertex(pos.x+a+d, pos.y+p);
            ofVertex(pos.x+a+d, pos.y-p);
            ofVertex(pos.x+a, pos.y-p-2);
        ofEndShape();
    ofPopStyle();
    
    //draw sustain
    ofPushStyle();
        ofSetColor(0, 0, 255);
        ofDrawRectangle(pos.x+a+d,pos.y-p, s, h);
    ofPopStyle();
    
    //draw triangle
    ofPushStyle();
        ofSetColor(0, 255, 255);
        ofBeginShape();
            ofVertex(pos.x+a+d+s+1, pos.y+p);
            ofVertex(pos.x+a+d+s+r, pos.y);
            ofVertex(pos.x+a+d+s+1, pos.y-p);
        ofEndShape();
    ofPopStyle();

}

void ofxTLVMMNotes::sendNoteOnEvent(){
    long thisTimelinePoint = currentTrackTime();
    for(int i = 0; i < keyframes.size(); i++){
        if(timeline->getInOutRangeMillis().contains(keyframes[i]->time) &&
           lastTimelinePoint <= keyframes[i]->time &&
           thisTimelinePoint >= keyframes[i]->time &&
           thisTimelinePoint != lastTimelinePoint)
        {
            //ofLogNotice()   << "Accuracy of keyframes[" << i << "]->time(miliseconds) [" << keyframes[i]->time << "|" << thisTimelinePoint << "]";
            
            //FIRE BANG
            bangFired(keyframes[i]);
            lastBangTime = ofGetElapsedTimef();
            
        }
    }
    lastTimelinePoint = thisTimelinePoint;

}

//-------------------------------------------------------
void ofxTLVMMNotes::initializeNotes(){
    
    //4x for ADSR
    for(int j=0;j<4;j++){

        noteDuration* nd;
        nd = new noteDuration();
        nd->name = "0";
        nd->duration = 0;
        nd->length = 0.0;
        nd->mode = j;
        noteDurations.push_back(nd);
        
        nd = new noteDuration();
        nd->name = "1/32";
        nd->duration = 31;
        nd->length = 0.125;
        nd->mode = j;
        noteDurations.push_back(nd);
        
        nd = new noteDuration();
        nd->name = "1/16";
        nd->duration = 62;
        nd->length = 0.25;
        nd->mode = j;
        noteDurations.push_back(nd);
        
        nd = new noteDuration();
        nd->name = "1/8";
        nd->duration = 125;
        nd->length = 0.5;
        nd->mode = j;
        noteDurations.push_back(nd);
        
        nd = new noteDuration();
        nd->name = "1/4";
        nd->duration = 250;
        nd->length = 1.0;
        nd->mode = j;
        noteDurations.push_back(nd);
        
        nd = new noteDuration();
        nd->name = "1/2";
        nd->duration = 500;
        nd->length = 2.0;
        nd->mode = j;
        noteDurations.push_back(nd);
        
        nd = new noteDuration();
        nd->name = "1";
        nd->duration = 1000;
        nd->length = 4.0;
        nd->mode = j;
        noteDurations.push_back(nd);
        
        nd = new noteDuration();
        nd->name = "2";
        nd->duration = 2000;
        nd->length = 8.0;
        nd->mode = j;
        noteDurations.push_back(nd);
        
    }
    ///NOTE DURATIONS -------
    
    durationBoxWidth  = 50;
    durationBoxHeight = 15;
    
    int column = 0;
    int y = 0;
    
    //	easingWindowSeperatorHeight = 4;
    
    //set the size of each box
    for(int i = 0; i < noteDurations.size(); i++){
        float x = durationBoxWidth * noteDurations[i]->mode;

        if(i % 8 == 0){
            column++;
            cout << "column " << column << endl;
            y = 0;
        }
        y += durationBoxHeight;
        
        cout << "bounds(" << x << "," << y << ")" << endl;
        noteDurations[i]->bounds = ofRectangle(x, y, durationBoxWidth, durationBoxHeight);
        
        noteDurations[i]->id = i;
    }
    
}

ofxTLKeyframe* ofxTLVMMNotes::newKeyframe(){

    ofxTLVMMNote* newNote = new ofxTLVMMNote(valueRange.min);
    quantizeNoteByPos(newNote);
    return newNote;
}


void ofxTLVMMNotes::restoreKeyframe(ofxTLKeyframe* key, ofxXmlSettings& xmlStore){
    ofxTLVMMNote* note = (ofxTLVMMNote*)key;
    note->pitch = xmlStore.getValue("pitch", valueRange.min);
    note->value = xmlStore.getValue("value", 0.5);
    note->duration = xmlStore.getValue("duration", 0);
    note->length = xmlStore.getValue("length", 0.0);
    note->ADSR[0] = xmlStore.getValue("attack", 0.0);
    note->ADSR[1] = xmlStore.getValue("decay", 0.0);
    note->ADSR[2] = xmlStore.getValue("sustain", 0.0);
    note->ADSR[3] = xmlStore.getValue("release", 0.0);
    note->notePlaying = false;
    
}

void ofxTLVMMNotes::storeKeyframe(ofxTLKeyframe* key, ofxXmlSettings& xmlStore){
    ofxTLVMMNote* note = (ofxTLVMMNote*)key;
    xmlStore.addValue("pitch", note->pitch);
    xmlStore.addValue("value", note->value);
    xmlStore.addValue("duration", note->duration);
    xmlStore.addValue("length", note->length);
    xmlStore.addValue("attack", note->ADSR[0]);
    xmlStore.addValue("decay", note->ADSR[1]);
    xmlStore.addValue("sustain", note->ADSR[2]);
    xmlStore.addValue("release", note->ADSR[3]);
    
}

ofxTLKeyframe* ofxTLVMMNotes::keyframeAtScreenpoint(ofVec2f p){
    return ofxTLKeyframes::keyframeAtScreenpoint(p);
}

void ofxTLVMMNotes::selectedKeySecondaryClick(ofMouseEventArgs& args){
    cout << "right mouse clicked!" << endl;
    
    durationWindowPosition = ofVec2f(MIN(args.x, bounds.width - durationBoxWidth*2),
                                     MIN(args.y, timeline->getBottomLeft().y - durationBoxHeight));
    
    drawingEasingWindow = true;
    timeline->presentedModalContent(this);
    
    cout << "ofxTLVMMNotes::drawModalContent()";
    
    //you can make a popup window start here
    //    timeline->presentedModalContent(this);
    //and then when you want to get rid of it call somewhere else
    //    timeline->dismissedModalContent();
    //this will lock all other timeline interaction and feed all things into your track
    //so feel free to draw out of bounds, but don't go off the screen or out of the timeline
}


void ofxTLVMMNotes::willDeleteKeyframe(ofxTLKeyframe* keyframe){
    //do any cleanup before this keyframe gets hosed
}



