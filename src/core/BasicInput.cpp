//
//  BasicInput.cpp
//  PixEngine
//
//  Created by rodo on 11/02/2020.
//  Copyright © 2020 rodo. All rights reserved.
//

#include "BasicInput.hpp"

using namespace rgl;


Keyboard::Keyboard(int numkeys):NUMKEYS(numkeys) {
	pNextState = new bool[NUMKEYS];
	pThisState = new bool[NUMKEYS];
	pStateHeld = new bool[NUMKEYS];
	pStatePressed = new bool[NUMKEYS];
	pStateReleased = new bool[NUMKEYS];
	for (int i = 0; i<NUMKEYS; i++) pThisState[i] = pNextState[i]=pStatePressed[i]=pStateHeld[i]=false;
};

bool Keyboard::isPressed(KeyNames key) { return pStatePressed[key];}
bool Keyboard::isReleased(KeyNames key) { return pStateReleased[key];}
bool Keyboard::isHeld(KeyNames key) { return pStateHeld[key];}

void Keyboard::sync() {
	
	// Handle User Input - Keyboard
	
	for (int i = 0; i < NUMKEYS; i++) {
		
		pStateHeld[i]=pStatePressed[i]=false;
		
		if (pNextState[i] != pThisState[i]) {
			if (pNextState[i]) {
				pStatePressed[i] = !pStateHeld[i];
				pStateHeld[i] = true;
			} else {
				pStateReleased[i] = true;
				pStateHeld[i] = false;
			}
		}
		
		pThisState[i] = pNextState[i];
	}
	
}

void Mouse::input(int px, int py) { nx=px; ny = py; }
void Mouse::inputWheel(int px, int py) { nwx=px; nwy = py; }
void Mouse::inputButton(int b, bool stat) { pNextButtonState[b] = stat; }
Mouse::Mouse(int buttons):BUTTONS(buttons) {
	pNextButtonState= new bool[BUTTONS];
	pButtonState= new bool[BUTTONS];
};
void Mouse::sync() {
	x=nx; y=ny; wx=nwx; wy=nwy;
	for (int i=0; i<BUTTONS; i++) pButtonState[i]=pNextButtonState[i];
}
