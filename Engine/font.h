#pragma once

#include "Cmn.h"
#include "AAsset.h"

#include <array>

namespace talga
{

	struct talga_glyph
	{
		unsigned short x0, y0, x1, y1; // coordinates of bbox in bitmap
		float xoff, yoff, xadvance;
		float xoff2, yoff2;
		float u0, v0, u1, v1;
		int w, h;
	};


	class Font : public AAsset
	{
	public:
		Font(I32 size);
		Font(const Font& cpy);
		const Font& operator=(const Font& cpy);
		~Font();

		virtual bool load(std::string path, AssetManager& manager) override;
		virtual bool save(std::string path, AssetManager& manager) override;
		virtual void destroy() override;

		const talga_glyph& getGlyph(char c) const { return mGlyphs[c - 32]; }

		I32 getSize() const { return mSize; }
		I32 getMaxGlyphHeight() const { return mMaxGlyphHeight; }
		I32 getTabWidth() const { return mTabWidth; }
		F32 getCharStep(char c);
		U32 id() const { return glTex; }
	private:
		I32 mSize;
		size_t mMaxGlyphHeight;
		I32 mTabWidth;
		U32 glTex;
		std::array<talga_glyph, 96> mGlyphs;
	};
}