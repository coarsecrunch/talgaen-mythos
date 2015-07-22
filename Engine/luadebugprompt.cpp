#include <algorithm>

#include "luadebugprompt.h"
#include "Renderer.h"
#include "ext/freetype-gl/freetype-gl.h"
#include "font.h"


namespace talga
{
	const int YOFFSET = 10;

	LuaDebugPrompt::LuaDebugPrompt(cpTex tex, cpFont font)
		: Sprite(tex)
		, mFont(font)
		, mStartAt()
		, mMaxVisibleLines(0)
		, mMaxLinePixelWidth(0)
	{
		mMaxVisibleLines = (box().getH() - LUA_DEBUG_BORDER) / font->getMaxGlyphHeight();
		mMaxLinePixelWidth = box().getW() - LUA_DEBUG_BORDER * 2;
	}

	void LuaDebugPrompt::print(const std::string& line)
	{
		std::string proxyStr;
		std::string proxyWord;
		F32 wordWidth = 0.0f;
		F32 totalStrWidth = 0.0f;
		I32 previousWidth = 0;

		std::vector<std::string> tempVec;
		for (int i = 0; i < line.size(); ++i)
		{
			char c = line[i];

			auto glyph = ftgl::texture_font_get_glyph(mFont->getFTFont(), c);

			if (c == ' ')
			{
				I32 widthBeforeWord = totalStrWidth / mMaxLinePixelWidth;
				I32 widthAfterWord = (totalStrWidth + wordWidth + glyph->advance_x) / (F32)mMaxLinePixelWidth;
				
				
				if (widthAfterWord > widthAfterWord || widthBeforeWord > previousWidth)
				{
					totalStrWidth += wordWidth;
					proxyStr.push_back('\n');
					tempVec.push_back(proxyStr);
					proxyWord.clear();
					proxyStr.clear();
					proxyStr.append(proxyWord);

					wordWidth = 0.0f;
					totalStrWidth = 0.0f;
					previousWidth = 0;
				}
				else
				{
					proxyStr.append(proxyWord);
					totalStrWidth += wordWidth + glyph->advance_x;
					proxyWord.clear();
					proxyStr.push_back(' ');
					wordWidth = glyph->advance_x;
				}
				previousWidth = widthBeforeWord;
			}
			else
			{
				proxyWord.push_back(c);
				wordWidth += glyph->advance_x;
			}

		}

		tempVec.push_back(proxyStr + proxyWord + "\n");

		for (auto it = tempVec.rbegin(); it != tempVec.rend(); ++it)
			mTextLines.push_front(*it);

		

		while (mTextLines.size() > LUA_DEBUG_MAX_LINES)
			mTextLines.pop_back();

		proxyStr.push_back('\n');
	}

	void LuaDebugPrompt::render(Renderer* renderer, const Camera* camera) const
	{
		Sprite::render(renderer, camera);
		renderer->tStackPush(mImageBox.getTransformationMatrix());
		std::string rdrStr;
		I32 numNewLines = 0;
		for (auto it = mTextLines.begin(); it != mTextLines.end() && numNewLines <= mMaxVisibleLines; ++it)
		{
			numNewLines += std::count(it->begin(), it->end(), '\n');
			rdrStr += *it;
		}
		vec3 tempPos(mImageBox.getW() * -0.5f + LUA_DEBUG_BORDER, (mImageBox.getH() * -0.5f + mFont->getMaxGlyphHeight() + LUA_DEBUG_BORDER * 2) + YOFFSET);

		renderer->submit(rdrStr, mFont, tempPos,vec4(1.0f, 1.0f, 1.0f, 1.0f));
		renderer->tStackPop();
	}

	LuaDebugPrompt::~LuaDebugPrompt()
	{
	}
}