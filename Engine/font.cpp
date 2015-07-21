#include "font.h"
#include "ext/freetype-gl/freetype-gl.h"
#include "sys.h"

namespace talga
{
	Font::Font(I32 size)
		: mSize{ size }
	{
	}

	bool Font::load(std::string path, AssetManager& manager)
	{
		mTexAtlas = ftgl::texture_atlas_new(512, 512, 2);
		mTexFont = ftgl::texture_font_new_from_file(mTexAtlas, 30, path.c_str());

		if (!mTexAtlas || !mTexFont)
			return false;

		mName = getFileNameFromPath(path);
		mPath = getPathFromFilePath(path);

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