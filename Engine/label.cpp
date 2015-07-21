#include "label.h"

namespace talga
{
	Label::Label(std::string text, vec3 pos, vec4 color)
		: mPosition(pos)
		, mText(text)
		, mColor(color)
	{
	}

	void Label::render(Renderer* renderer, const Camera* camera) const
	{

	}

	Label::~Label()
	{
	}
}