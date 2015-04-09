#pragma once

#include <vector>
#include <cassert>
#include <string>
#include "Cmn.h"
#include "Rect.h"
#include "Texture.h"

namespace talga
{
	class Animation
	{
	public:
		std::vector<Rect> frames;
		I32 numFrames;
		U32 bufferLoc;
		std::string name;
		cpTex tex;

		U32 getOffset(I32 frame) const
		{
			assert(frame < numFrames && frame >= 0);
			return bufferLoc + (frame * sizeof(U16) * 6); // returns a byte offset inside the index buffer
		}
	};

}