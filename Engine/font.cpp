#include "font.h"
#include "ext/freetype-gl/freetype-gl.h"
#include "sys.h"

namespace talga
{
	Font::Font(I32 size)
		: mSize{ size }
		, mMaxGlyphHeight{0}
		, mTexAtlas{ nullptr }
		, mTexFont{nullptr}
	{
	}

	Font::Font(const Font& cpy)
		: AAsset(cpy)
		, mSize{ cpy.mSize }
		, mMaxGlyphHeight{ cpy.mMaxGlyphHeight }
		, mTexAtlas{ cpy.mTexAtlas }
		, mTexFont{ cpy.mTexFont }
	{
	}
	const Font& Font::operator=(const Font& cpy)
	{
		AAsset::operator=(cpy);
		mSize = cpy.mSize;
		mMaxGlyphHeight = cpy.mMaxGlyphHeight;
		mTexAtlas = cpy.mTexAtlas;
		mTexFont = cpy.mTexFont;
		
		return *this;
	}

	bool Font::load(std::string path, AssetManager& manager)
	{
		mTexAtlas = ftgl::texture_atlas_new(512, 512, 2);
		mTexFont = ftgl::texture_font_new_from_file(mTexAtlas, mSize, path.c_str());

		if (!mTexAtlas || !mTexFont)
			return false;

		mName = getFileNameFromPath(path);
		mPath = getPathFromFilePath(path);

		for (unsigned char c = 33; c < 128; ++c)
		{
			ftgl::texture_glyph_t* glyph = ftgl::texture_font_get_glyph(mTexFont, c);

			if (glyph)
			{
				
				if (glyph->height > mMaxGlyphHeight)
				{
					mMaxGlyphHeight = glyph->height;
				}
			}
			
		}

		return true;
	}

	bool Font::save(std::string path, AssetManager& manager)
	{
		return true;
	}

	void Font::destroy()
	{
		ftgl::texture_atlas_clear(mTexAtlas);
		ftgl::texture_font_delete(mTexFont);
	}

	Font::~Font()
	{
	}
}