# pragma once

#ifndef INPUT_H
#define INPUT_H

#include "Globals.h"
#include "Window.h"

enum class Keys {
    Space = 32,
    Comma = 44,
    Minus,
    Period,
    Slash,
    Num0 = 48,
    Num1,
    Num2,
    Num3,
    Num4,
    Num5,
    Num6,
    Num7,
    Num8,
    Num9,
    SemiColon = 59,
    Equal = 61,
	A = 65,
	B,
	C,
	D,
	E,
	F,
	G,
	h,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
    LBracket = 91,
    BackSlash,
    RBracket,
    Escape = 256,
    Enter,
    Tab,
    BackSpace,
    Insert,
    Delete,
    Right,
    Left,
    Down,
    Up,
    PageUp,
    PageDown,
    Home,
    End,
    CapsLock = 280,
    ScrollLock,
    NumLock,
    PrintScreen,
    Pause,
    F1 = 290,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
};

enum class MouseButtons {
    Left,       
    Right,      
    Middle,
};

class GLN_API Input {
	static Input* instance;
public:
	bool isKeyPressed(Keys key);
	bool isMouseButtonPressed(MouseButtons button);
	glm::vec2 getMousePosition();
	static Input* getInstance();
};
#endif // !INPUT_H
