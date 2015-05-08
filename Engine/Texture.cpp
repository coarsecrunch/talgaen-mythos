#include "Texture.h"
#include "GL\glew.h"
#include <iostream>
#include <cstdio>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"

namespace talga
{
	Texture::Texture() 
		: AAsset()
		, mWidth(-1)
		, mHeight(-1)
		, mTexture(-1)
	{
	}

	void Texture::load(std::string path, AssetManager& manager)
	{
		FILE* imgFile = nullptr;
		
		imgFile = fopen(path.c_str(), "rb");
		
		I32 imgW = -1, imgH = -1, channels = -1;
		const U8* data = nullptr;

		if (!imgFile)
		{
			TALGA_WARN(0, path + ", invalid textue was attempted to  be loaded");
			return;
		}
		
		data = stbi_load_from_file(imgFile, &imgW, &imgH, &channels, 0);

		glGenTextures(1, &mTexture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgW, imgH, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		static int cc = 0; 

		if (cc == 0)
		{
			for (int y = 0; y < imgH; ++y)
			{
				for (int x = 0; x < imgW; ++x)
				{
					//std::cout << std::hex << *((int*)(data + (y * imgW + x))) << std::endl;
				}
			}

			std::cout << "CHANNELS: " << channels << std::endl;
		}
		

		++cc;

		mWidth = imgW;
		mHeight = imgH;

		int start = -1;
		
		std::string tempName = path;

		for (int i = path.size() - 1; i >= 0; i--)
		{
			if (path[i] == '\\' || path[i] == '/') break;
			tempName = path.substr(i, path.size() - i);
		}

		mName = tempName;

		stbi_image_free((void*)data);
		fclose(imgFile);
		imgFile = nullptr;
	}

	void Texture::Bind() const
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