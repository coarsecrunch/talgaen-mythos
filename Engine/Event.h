#pragma once

#include "Cmn.h"

namespace talga
{
	class Event
	{
	public:
		enum EVENT_TYPE {EVENT};
	
		Event(I32 eventType) : type{ eventType } {};
		~Event() {}

		I32 type;
	};

	class MouseEvent : Event
	{
	public:
		enum MOUSE_EVENT_TYPE { MOUSE_MOVE, MOUSE_CLICK, MOUSE_DOUBLECLICK, MOUSE_RIGHCLICK };

		MouseEvent(I32 eventType, I32 mx, I32 my) : Event{ eventType }, x{ mx }, y{ my } {}

		F32 x;
		F32 y;
	};

	class KeyboardEvent : Event
	{
	public:
		enum KEYBOARD_EVENT_TYPE { KEY_DOWN, KEY_UP };
		KeyboardEvent(I32 eventType, char keyPressed) : Event{ eventType }, key{ keyPressed }{}

		char key;
	};
}