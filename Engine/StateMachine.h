#pragma once

#include "Cmn.h"

namespace talga
{
	class IState;

	class StateMachine
	{
	public:
		StateMachine();
		~StateMachine();

		void update(I32 dt);

		void changeState(IState* state);
		IState* getCurrentState() { return mCurrentState; }
	protected:
		IState* mCurrentState;
	};

}