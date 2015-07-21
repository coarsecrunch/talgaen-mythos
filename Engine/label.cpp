#include "label.h"
#include "Renderer.h"
namespace talga
{
	Label::Label(std::string text, vec3 pos, vec4 color, cpFont font)
		: mPosition(pos)
		, mText(text)
		, mColor(color)
		, mFont(font)
	{
		TALGA_ASSERT(font, "passed invalid font to label");
	}

	void Label::render(Renderer* renderer, const Camera* camera) const
	{
		renderer->submit(mText, mFont, mPosition, mColor);
	}

	Label::~Label()
	{
	}
}