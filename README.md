updated: 2018-05-26

ofxTLVMMNotes is an addon for ofxTimeline to add a note track to drive my application [VMM](https://github.com/lg3bass/VMM). This is very much in progress. 

![ofxVMMTLNotes Track](https://raw.githubusercontent.com/lg3bass/ofxTLVMMNotes/master/ofxTLVMMNotes.png =600x)

This addon is a derivitive/customization of the ofxTLNote addon by Leo Zimmerman.  This code appears in his branch of [ofxTimeline](https://github.com/leozimmerman/ofxTimeline/commit/10847277b4535697b1e6ac2084669f13c90f51fe) for his application Sonoscopio.

To use it with ofxTimeline basically you have to first - setup your project to use [ofxTimeline](https://github/lg3bass/ofxTimeline")

####Use the Project Generator to add ofxTLVMMNotes

In your ofApp.h file add (usually next to #include "ofxTimeline.h"):

	
	#include "ofxTLVMMNotes.h"
	

And the in your ofApp.cpp file you can add buttons track with something like this:

	
	notesTrack = new ofxTLVMMNotes();
    notesTrack->setRange(ofRange(60,80));
    timeline.addTrack("My TL Notes Track", notesTrack);
    

See the ""example-basic"" project in the addon folder. Right click on each note to see a menu with note durations.


####TODO:
- Make note durations dependent on tempo.
- Scale durations with zoomer.
- use easing curves for note start and ends.
- additional contextual menu to 
- anticipation shift. (move start time left but snap where action hits)
- segment and note numbers
- multisegment notes to indicate which sections of a object sequence to play.





