#pragma once

namespace talga
{
	class Clock
	{
	public:
		void Init();

		unsigned TimePassed();
		Clock();
		~Clock();
	private:
		unsigned long mZeroPoint;
	};

}
