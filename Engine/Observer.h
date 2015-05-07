#pragma once

namespace talga
{
	class Event;

	class Observer
	{
	public:
		Observer();
		virtual ~Observer();

		virtual void onNotify(Event e) = 0;
	};
}
