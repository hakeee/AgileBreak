//
// Created by hakeee on 2015-09-06.
//

#include "input.h"

using namespace discobreak;

std::array<bool, 255> Input::previousKeyboardState_;
std::array<bool, 255> Input::keyboardState_;

Input::Input() {
    keyboardState_.fill(false);
    previousKeyboardState_.fill(false);
}


Input::~Input() {
}

void Input::SetKeyState(unsigned char key, bool value) {
        keyboardState_[key] = value;
}

bool Input::IsKeyDown(unsigned char key) {
	if(key >= 65  && key <= 90)
		return keyboardState_[key] || keyboardState_[key+32];
	return keyboardState_[key];
}

bool Input::IsKeyPressed(unsigned char key) {
	if (key >= 65 && key <= 90)
		return ((keyboardState_[key] && !previousKeyboardState_[key]) || (keyboardState_[key + 32] && !previousKeyboardState_[key + 32]));
    return (keyboardState_[key] && !previousKeyboardState_[key]);
}

bool Input::IsKeyReleased(unsigned char key) {
	if (key >= 65 && key <= 90)
		return ((!keyboardState_[key] && previousKeyboardState_[key]) || (!keyboardState_[key + 32] && previousKeyboardState_[key + 32]));
    return (!keyboardState_[key] && previousKeyboardState_[key]);
}

void Input::PostUpdate() {
	previousKeyboardState_ = keyboardState_;
}

