#include "font.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb/stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#include "GL/glew.h"
#include "sys.h"
#include <stdio.h>
#include "Math/Operations.h"

namespace talga
{
	Font::Font(I32 size)
		: AAsset()
		, mSize{ size }
		, mMaxGlyphHeight{0}
		, glTex(0)
		, mGlyphs{}
	{
	}

	Font::Font(const Font& cpy)
		: AAsset(cpy)
		, mSize{ cpy.mSize }
		, glTex{ cpy.glTex }
		, mGlyphs( cpy.mGlyphs)
		, mMaxGlyphHeight{cpy.mMaxGlyphHeight}
	{
	}

	const Font& Font::operator=(const Font& cpy)
	{
		AAsset::operator=(cpy);
		mSize = cpy.mSize;
		glTex = cpy.glTex;
		mGlyphs = cpy.mGlyphs;
		mMaxGlyphHeight = cpy.mMaxGlyphHeight;
		
		return *this;
	}


	bool Font::load(std::string path, AssetManager& manager)
	{
		mName = getFileNameFromPath(path);
		mPath = getPathFromFilePath(path);

		FILE* file = fopen(path.c_str(), "rb");
		size_t fSize = 0;
		U8* buffer = nullptr;
		
		if (!file)
			return false;

		fseek(file, 0, SEEK_END);
		fSize = ftell(file);
		rewind(file);

		buffer = new U8[fSize];
		
		const U32 TATLAS_SIZE = 512;
		size_t result = fread(buffer, sizeof(U8), fSize, file);
		if (result != fSize)
			return false;

		stbtt_fontinfo fontInfo;
		stbtt_InitFont(&fontInfo, buffer, stbtt_GetFontOffsetForIndex(buffer, 0));
		
		U8* img = new U8[TATLAS_SIZE * TATLAS_SIZE];
		U8* rgbaImg = new U8[TATLAS_SIZE * TATLAS_SIZE * 4];
		
		stbtt_pack_context pack;
		stbtt_packedchar pcdata[96];
		int packSuccess = stbtt_PackBegin(&pack, img, TATLAS_SIZE, TATLAS_SIZE, 0, 1, NULL);


		int rangeSuc = stbtt_PackFontRange(&pack, buffer, 0, mSize, 32, 95, pcdata);
		stbtt_PackEnd(&pack);
		F32 largestY = 0.0f;

		for (int i = 0; i < 96; ++i)
		{
			mGlyphs[i].x0 = pcdata[i].x0;
			mGlyphs[i].y0 = pcdata[i].y0;
			mGlyphs[i].x1 = pcdata[i].x1;
			mGlyphs[i].y1 = pcdata[i].y1;
			mGlyphs[i].xoff = pcdata[i].xoff;
			mGlyphs[i].yoff = pcdata[i].yoff;
			mGlyphs[i].xoff2 = pcdata[i].xoff2;
			mGlyphs[i].yoff2 = pcdata[i].yoff2;
			mGlyphs[i].xadvance = pcdata[i].xadvance;
			vec2 tl = GET_UV(mGlyphs[i].x0, TATLAS_SIZE, mGlyphs[i].y0, TATLAS_SIZE);
			vec2 br = GET_UV(mGlyphs[i].x1, TATLAS_SIZE, mGlyphs[i].y1, TATLAS_SIZE);
			mGlyphs[i].u0 = tl.x();
			mGlyphs[i].v0 = tl.y();
			mGlyphs[i].u1 = br.x();
			mGlyphs[i].v1 = br.y();
			mGlyphs[i].w = mGlyphs[i].x1 - mGlyphs[i].x0;
			mGlyphs[i].h = abs(mGlyphs[i].y1 - mGlyphs[i].y0);

			if (mGlyphs[i].y1 - mGlyphs[i].y0 > largestY)
				largestY = (mGlyphs[i].y1 - mGlyphs[i].y0);
		}

		mMaxGlyphHeight = largestY;

		for (int i = 0; i < TATLAS_SIZE * TATLAS_SIZE; ++i)
		{
			rgbaImg[(i * 4) + 0] = 255; //r
			rgbaImg[(i * 4) + 1] = 255; //g
			rgbaImg[(i * 4) + 2] = 255; //b
			rgbaImg[(i * 4) + 3] = img[i]; //a
		}

		glGenTextures(1, &glTex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TATLAS_SIZE, TATLAS_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgbaImg);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
		
		fclose(file);
		delete[] buffer;
		delete[] img;
		delete[] rgbaImg;
		return true;
	}

	F32 Font::getCharStep(char c)
	{
		return 0.0f;
	}

	bool Font::save(std::string path, AssetManager& manager)
	{
		return true;
	}

	void Font::destroy()
	{
		glDeleteTextures(1, &glTex);
	}

	Font::~Font()
	{
	}
}