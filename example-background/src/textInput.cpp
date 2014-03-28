//
//  textInput.cpp
//  textInput
//
//  Created by Elliot Woods on 12/09/2011.
//  Copyright 2011 Kimchi and Chips.
//	MIT license
//	http://www.opensource.org/licenses/mit-license.php
//

#include "textInput.h"

textInput::textInput() {
	text="";
	position=0;
	cursorx=0;
	cursory=0;
}

void textInput::init() {
	ofAddListener(ofEvents().keyPressed , this, &textInput::keyPressedEvent);
    
    fontTextbox.loadFont("Comicv3c.ttf", 30);
}

void textInput::draw()  {
	//draw text
	
	
	//draw cursor line
	ofPushStyle();
	float timeFrac = 0.5 * sin(3.0f * ofGetElapsedTimef()) + 0.5;
	
	    
    ofFill();
    ofSetColor(0);
    ofRect(-30, -30, 500, 100);
    ofFill();
    ofSetColor(255);
    ofRect(-25, -25,490, 90);
    ofSetColor(0);
    fontTextbox.drawString(text, 5,0);
    ofSetColor(255);
    
   /*  different text/balloon boxes depending on mode...  I want to let user position them on frame... arrow keys or joystick..
    
    if (mode == 0){
    ofEllipse(160, 200, 50, 30);
    ofEllipse(160, 250, 40, 20);
    ofEllipse(160, 300, 30, 10);
    ofSetColor(255);
    ofEllipse(170, 210, 50, 30);
    ofEllipse(170, 252, 40, 20);
    ofEllipse(170, 303, 30, 10);
    
    ofSetColor(0);
    
    //ofTriangle(160, 20, 180, 30, 160, 20);
    // ofSetColor(0);
	//ofSetLineWidth(3.0f);
        
    }
    else if (mode==1){
        ofFill();
        ofSetColor(0);
        ofRect(800, 50, 400, 100);
        ofFill();
        ofSetColor(255);
        ofRect(805, 55,395, 90);
        
        ofSetColor(0);
        
        font.drawString("and you will contribute a verse",830,100); //dummy for now
        
        ofSetColor(255);
        
        ofTriangle(860, 200, 870, 250, 860, 200);
        
    }
*/
	ofLine(cursorx*20 + 10, 13.7*cursory, cursorx*20 + 10, 10+13.7*cursory);
	ofPopStyle();
}

void textInput::keyPressed(int key) {	
	//add charachter (non unicode sorry!)
	if (key >=32 && key <=126) {
		text.insert(text.begin()+position, key);
		position++;
	}
	
	if (key==OF_KEY_RETURN) {
        text.insert(text.begin()+position, '\n');
        position++;
	
	}
	
	if (key==OF_KEY_BACKSPACE) {
		if (position>0) {
			text.erase(text.begin()+position-1);
			--position;
		}
	}
	
	if (key==OF_KEY_DEL) {
		if (text.size() > position) {
			text.erase(text.begin()+position);
		}
	}
	
	if (key==OF_KEY_LEFT)
		if (position>0)
			--position;
	
	if (key==OF_KEY_RIGHT)
		if (position<text.size()+1)
			++position;	
	
	//for multiline:
	cursorx = cursory = 0;
	for (int i=0; i<position; ++i) {
		if (*(text.begin()+i) == '\n') {
			++cursory;
			cursorx = 0;
		} else {
			cursorx++;
		}
	}
}

void textInput::clear() {
	text.clear();
	position=0;
}

void textInput::keyPressedEvent(ofKeyEventArgs &a) {
	keyPressed(a.key);
}