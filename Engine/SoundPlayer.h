#pragma once

#include "Cmn.h"

struct ALCContext;
struct ALCDevice;

namespace talga
{
	class SoundPlayer
	{
	public:
		SoundPlayer();
		~SoundPlayer();

		void playSound(U8* soundData, int samples);

	private:
	};

}