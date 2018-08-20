updated: 2018-09-219 - WIP

#ofxTLVMMNotes

ofxTLVMMNotes is an addon for ofxTimeline to add a piano roll style note track to drive my application [VMM](https://github.com/lg3bass/VMM). The goal is to have a notes track in the VMM timeline application to drive visual elements. 

A key feature of the ofxTLVMMNotes track is to create a visual representation of the concept of a 'anticipation' in animation.  Generally speaking, action is quantized to hit on a beat but sometimes the action begins and builds(anticipation) up the 'hit'.  We want the visual representation of the notes on the grid accurately match the 'hit' action to align to musical note information yet also display the anticipation. This is done using triangles before and after the notes.

##ADSR
The different colors represent (A)ttack - red, (D)ecay - green, (S)ustain - blue, and (R)elease - aqua.  You will be able to set each component of the keyframe to a note value (1/16,1/8,1/4,1/2,etc) by selecting with a contextual menu.

<img src="https://raw.githubusercontent.com/lg3bass/ofxTLVMMNotes/master/ofxTLVMMNotes_20180819.png" width="600"/>

###Credits
This addon is a derivitive/customization of the ofxTLNote addon by Leo Zimmerman.  This code appears in his branch of [ofxTimeline](https://github.com/leozimmerman/ofxTimeline/commit/10847277b4535697b1e6ac2084669f13c90f51fe) for his application Sonoscopio.

###Usage
To use it with ofxTimeline, the quickstart option is to setup your project using my fork of [ofxTimeline](https://github/lg3bass/ofxTimeline")

See the "example-basic" project in the addon folder. Right click on each note to see a menu with note durations.

###New Project using the project generator to add ofxTLVMMNotes

In your ofApp.h file add (usually next to #include "ofxTimeline.h"):

	
	#include "ofxTLVMMNotes.h"
	

And the in your ofApp.cpp file you can add buttons track with something like this:

	
	notesTrack = new ofxTLVMMNotes();
    notesTrack->setRange(ofRange(60,80));
    timeline.addTrack("My TL Notes Track", notesTrack);
 
    

###TODO (20180819):
- Function Make note durations dependent on tempo. 
	- Create function to convert note->duration to size.
	- Scale durations with zoomer.
- Segments
	- Create additional contextual menu to select segment.
- Slices are determined by note value (1-12)
- Better representation of bars and beats
	- solid bar lines
	- alternating bg color for bars
- Hover over note display 
	- function to display notes.
	- buffer, note, duration, slice
- Visual representation of notes events debugging.
- Anticipation shift.
	- select each segment of the note and have that snap to beat. 
	- offset the note display start time. 
	- snap where action hits
- Ease-in, ease-out
	- investigate drag out easing. 
	- contextual menu for easing.
	- convert easing curves to frames.
	- use of easing curves for note start and ends.
- Multisegment notes to indicate which sections of a object sequence to play.
- change the Yaxis number of notes (buffers)
- Wrap around notes when looping
- Create 2 modes (scrub and trigger)



###Notes(pun intended) and Issues:

- Notes will double trigger if the timeline units are set to frames.


