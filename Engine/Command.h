#pragma once

#include "Character.h"
namespace talga
{
	class ACommand
	{
	public:
		ACommand();
		~ACommand();

		virtual void execute(Character& c) = 0;
	};

	class JumpCommand : public ACommand
	{
		virtual void execute(Character& c)
		{
			c.jump();
		}
	};

	class StandR : public ACommand
	{
		virtual void execute(Character& c)
		{
			c.standR();
		}
	};

	class StandL : public ACommand
	{
		virtual void execute(Character& c)
		{
			c.standL();
		}
	};

	class WalkR : public ACommand
	{
		virtual void execute(Character& c)
		{
			c.walkR();
		}
	};

	class WalkL : public ACommand
	{
		virtual void execute(Character& c)
		{
			c.walkL();
		}
	};

	class RunR : public ACommand
	{
		virtual void execute(Character& c)
		{
			c.runR();
		}
	};

	class RunL : public ACommand
	{
		virtual void execute(Character& c)
		{
			c.runL();
		}
	};

}