#pragma once

#include "Cmn.h"
#include "AAsset.h"
namespace ftgl
{
	struct texture_atlas_t;
	struct texture_font_t;
}

namespace talga
{
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

		ftgl::texture_atlas_t* getAtlas() const { return mTexAtlas; }
		ftgl::texture_font_t* getFTFont() const { return mTexFont; }

		I32 getSize() const { return mSize; }
		I32 getMaxGlyphHeight() const { return mMaxGlyphHeight; }
		//I32 getStringPixelWidth(const std::string& str);
	private:
		ftgl::texture_atlas_t* mTexAtlas;
		ftgl::texture_font_t* mTexFont;
		I32 mSize;
		size_t mMaxGlyphHeight;
	};
}