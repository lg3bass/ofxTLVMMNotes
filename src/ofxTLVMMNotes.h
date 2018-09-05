/**
 * ofxTLNotes created by  Leo Zimmerman [http://www.leozimmerman.com.ar]
 *
 * ofxTimeline
 * openFrameworks graphical timeline addon
 *
 * Copyright (c) 2011-2012 James George
 * Development Supported by YCAM InterLab http://interlab.ycam.jp/en/
 * http://jamesgeorge.org + http://flightphase.com
 * http://github.com/obviousjim + http://github.com/flightphase
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxTLKeyframes.h"
#include "ofxTextInputField.h"
#include "ofxTween.h"

//for testing
#include "ofxOsc.h"

typedef struct {
    int id;
    ofRectangle bounds;
    string name;
    int duration;
    float length;
    int mode; //1-4 [ADSR]
    
    
} noteDuration;


class ofxTLVMMNote : public ofxTLKeyframe {

//  ofxTLKeyframe inherited variables.
//    ofVec2f screenPosition; // cached screen position
//    unsigned long long previousTime; //for preventing overlap conflicts
//    unsigned long long time; //in millis
//    float value; //normalized

public:
    ofxTLVMMNote(int p);
    int pitch;
    int duration;
    float length;
    string getPitchDisplay();
    
    //ADSR
    unsigned long long attackDuration;
    unsigned long long decayDuration;
    unsigned long long sustainDuration;
    unsigned long long releaseDuration;
    
    ofVec4f ADSR;
    
    bool notePlaying;
    
    int frame;
    int lastFrame;

};

struct GridRow {
    float yPos;
    int pitch;
};

class ofxTLVMMNotes : public ofxTLKeyframes {
public:
    ofxTLVMMNotes();
    virtual ~ofxTLVMMNotes();
    
    virtual void update();
    virtual void draw();
    
    virtual bool mousePressed(ofMouseEventArgs& args, long millis);
    virtual void mouseMoved(ofMouseEventArgs& args, long millis);
    virtual void mouseDragged(ofMouseEventArgs& args, long millis);
    virtual void mouseReleased(ofMouseEventArgs& args, long millis);

    virtual void keyPressed(ofKeyEventArgs& args);
    
    void setRange(ofRange range);
    void setOctavesNum(int oct);
    void didEnterText();
    
    #pragma mark note functions
    int getNote();
    int getNoteAtMillis(long millis);
    int getNoteAtPercent(float percent);
    float getNoteDuration(float BPM, float value, bool normalized);
    
    //events
    long lastTimelinePoint;
    float lastBangTime; //just for display
    virtual void bangFired(ofxTLKeyframe* key);
    
    ofxTextInputField textField;
    ofRectangle display;
    
    //time range contains MIN and MAX time in milliseconds
    //valueRange is 0 at the bottom of the track, and 1 at the top
    //if you have anything other than small dots keyframes you'll want to override
    //the default behavior
    virtual void regionSelected(ofLongRange timeRange, ofRange valueRange);

    virtual string getTrackType();

    // Note-Specific
    void drawBackgroundGrid();
    

    virtual void drawModalContent();
    
    
    //ofxTween test
    ofxEasingLinear easeLinear;
    
    float clamp;
    ofxTween::ofxEasingType easingType;
    string easestring;
    
    float outFrame = 0;
    int intFrame = 0;
    
    //osc
    ofxOscSender sender;
    void sendOSC(int buffer, int val);
    
protected:
    
    bool drawingEasingWindow;
    
    vector<GridRow> gridRows;
    //void quantizeAllNotesByPos();
    void quantizeNoteByPos(ofxTLVMMNote* note);
    void quantizeAllNotesByPitch();
    void quantizeNoteByPitch(ofxTLVMMNote* note);
    float rowNormHeight;
    
    //void drawNote(ofVec2f pos, float length, bool highlight);
    void drawNote(ofVec2f pos, ofxTLVMMNote* note, bool highlight);

    
    void sendNoteOnEvent();
    
    //note modal window
    void initializeNotes();
    
    vector<noteDuration*> noteDurations;
    ofVec2f durationWindowPosition;
    float durationBoxWidth;
    float durationBoxHeight;
    
    //always return the type for your track, in our case ofxTLEmptyKeyframe;
    //this will enusre that all keyframe objects passed to this class are of this type
    virtual ofxTLKeyframe* newKeyframe();
    //load this keyframe out of xml, which is alraedy pushed to the right level
    //only need to save custom properties that our subclass adds
    virtual void restoreKeyframe(ofxTLKeyframe* key, ofxXmlSettings& xmlStore);
    //save custom properties into the xml
    virtual void storeKeyframe(ofxTLKeyframe* key, ofxXmlSettings& xmlStore);
    
    //return keyframe at this mouse point if you have non circular keyframes
    virtual ofxTLKeyframe* keyframeAtScreenpoint(ofVec2f p);
    
    //responde to right clicks on keyframes
    virtual void selectedKeySecondaryClick(ofMouseEventArgs& args);
    
    //you can responde to a keyframe about to be killed with this function right before the parent class deletes it
    virtual void willDeleteKeyframe(ofxTLKeyframe* keyframe);
    
    //
};
