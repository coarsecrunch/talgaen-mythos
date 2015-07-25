#include "StateMachine.h"
#include "IState.h"
namespace talga
{
	StateMachine::StateMachine()
		: mCurrentState{nullptr}
	{
	}

	void StateMachine::update(I32 dt)
	{
		if (mCurrentState)
			mCurrentState->update(dt);
	}

	void StateMachine::changeState(IState* state)
	{
		if (mCurrentState)
		{
			mCurrentState->exit();
			delete mCurrentState;
		}
		mCurrentState = state;

		if (mCurrentState)
			mCurrentState->begin();
	}

	StateMachine::~StateMachine()
	{
		if (mCurrentState)
			delete mCurrentState;
	}
}