#include "Texture.h"
#include <GL/glew.h>
#include <iostream>
#include <cstdio>
#include "sys.h"
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

namespace talga
{
	Texture::Texture() 
		: AAsset()
		, mWidth(-1)
		, mHeight(-1)
		, mTexture(-1)
	{
	}

	bool Texture::load(std::string path, AssetManager& manager)
	{
		FILE* imgFile = nullptr;

		imgFile = fopen(path.c_str(), "rb");
		
		I32 imgW = -1, imgH = -1, channels = -1;
		const U8* data = nullptr;

		if (!imgFile)
		{
			TALGA_WARN(0, "couldn't open texture " + path);
			stbi_image_free((void*)data);
			fclose(imgFile);
			return false;
			
		}
		
		data = stbi_load_from_file(imgFile, &imgW, &imgH, &channels, 0);

		if (!data)
		{
			TALGA_WARN(0, "stbi could no load data on texture " + path);
			stbi_image_free((void*)data);
			fclose(imgFile);
			return false;
		}

		glGenTextures(1, &mTexture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		

		mWidth = imgW;
		mHeight = imgH;

		int start = -1;
		
		std::string tempName = path;
		std::string tempPath = path;

		for (int i = path.size() - 1; i >= 0; i--)
		{
			if (path[i] == '\\' || path[i] == '/') break;
			tempName = path.substr(i, path.size() - i);
			tempPath = path.substr(0, i);
		}

		mName = tempName;
		mPath = tempPath;

		stbi_image_free((void*)data);
		fclose(imgFile);
		imgFile = nullptr;

		return true;
	}

	bool Texture::save(std::string path, AssetManager& manager)
	{
		return true;
	}

	void Texture::bind() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture);
	}

	void Texture::destroy()
	{
		glDeleteTextures(1, &mTexture);
	}

	Texture::~Texture()
	{
		
	}
}
