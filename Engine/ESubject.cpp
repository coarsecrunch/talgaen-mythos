#include "ESubject.h"
#include "Cmn.h"
#include "Event.h"
#include "Observer.h"
namespace talga
{
	ESubject::ESubject() 
		: mObservers{}
	{
	}

	void ESubject::addObserver(Observer* observer)
	{
		mObservers.push_back(observer);
	}
	
	void ESubject::removeObserver(Observer* observer)
	{
		for (auto iter = mObservers.begin(); iter != mObservers.end(); ++iter)
		{
			if (observer == *iter)
			{
				*iter = *(mObservers.end() - 1);
				mObservers.pop_back();
				return;
			}
		}

		TALGA_WARN(0, "attempted to remove non existent observer");
	}

	void ESubject::dispatchEvent(Event e)
	{
		for (auto iter = mObservers.begin(); iter != mObservers.end(); ++iter)
		{
//			(*iter)->onNotify(Event e);
		}
	}

	ESubject::~ESubject()
	{
	}
}
