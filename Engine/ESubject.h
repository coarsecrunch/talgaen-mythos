#pragma once

#include <vector>
#include <functional>


namespace talga
{
	class Observer;
	class Event;

	class ESubject
	{
	public:
		ESubject();
		~ESubject();

		void addObserver(Observer* observer);
		void removeObserver(Observer* observer);

	protected:
		void dispatchEvent(Event e);

		std::vector<Observer*> mObservers;
	};
}
