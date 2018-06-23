updated: 2018-06-22

#ofxTLVMMNotes

ofxTLVMMNotes is an addon for ofxTimeline to add a piano roll style note track to drive my application [VMM](https://github.com/lg3bass/VMM). The goal is to have a notes track in the VMM timeline application to drive visual elements. 

A key feature of the ofxTLVMMNotes track is to create a visual representation of the concept of a 'anticipation' in animation.  Generally speaking, action is quantized to hit on a beat but sometimes the action begins and builds(anticipation) up the 'hit'.  We want the visual representation of the notes on the grid accurately match the 'hit' action to align to musical note information yet also display the anticipation. This is done using triangles before and after the notes.

<img src="https://raw.githubusercontent.com/lg3bass/ofxTLVMMNotes/master/ofxTLVMMNotes.png" width="600"/>

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
 
    

###TODO (20180622):
- Better representation of bars and beats
	- solid bar lines
	- alternating bg color for bars
- Hover over note display 
	- function to display notes.
	- buffer, note, duration, slice
- Create additional contextual menu to select slice.
- Create additional contextual menu to select segment.
- Make note durations dependent on tempo. 
	- Create function to convert note->duration to size.
	- Scale durations with zoomer.
- Visual representation of notes events debugging.
- Anticipation shift. 
	- offset the note display start time. 
	- snap where action hits
- Ease-in, ease-out
	- investigate drag out easing. 
	- contextual menu for easing.
	- convert easing curves to frames.
	- use of easing curves for note start and ends.
- Multisegment notes to indicate which sections of a object sequence to play.
- change the Yaxis number of notes (buffers)



###Notes(pun intended) and Issues:

- Notes will double trigger if the timeline units are set to frames.
- Notes do not scale. (yet)
- list

