#pragma once

enum class KEY : unsigned char
{
	EMPTY = 0x40,
	A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
	K0 = 0x30, K1, K2, K3, K4, K5, K6, K7, K8, K9,
	F1 = 0x70, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	LEFT = 0x25, UP, RIGHT, DOWN,
	SPACE = 0x20, PAGE_UP, PAGE_DOWN, END, HOME, TAB = 0x09, LSHIFT = 0xA0, RSHIFT, LCTRL, RCTRL, INS = 0x2D, DEL,
	BACKSPACE = 0x08, ESC = 0x1B, ENTER = 0x0D, CTRL, ALT, PAUSE,
	NP0 = 0x60, NP1, NP2, NP3, NP4, NP5, NP6, NP7, NP8, NP9,
	NP_MUL, NP_ADD, NP_SEPARATOR, NP_SUB, NP_DECIMAL
};

class KeyboardEvent
{
public:
	enum EventType
	{
		Press,
		Repeat,
		Release,
		Invalid
	};

	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	bool IsPressed();
	bool IsRepeated();
	bool IsReleased();
	bool IsValid();
	unsigned char Key();

private:
	EventType type;
	unsigned char key;
};