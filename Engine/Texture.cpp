#include "Texture.h"
#include "GL\glew.h"
#include <cstring>
#include <cstdio>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"

/*TODO: get rid of cstring code replace with std::string version
*/

namespace talga
{
	Texture::Texture() :
		mWidth(0), mHeight(0), mTexture(0)
	{
	}

	int Texture::Init(std::string path)
	{
		FILE* imgFile = fopen(path.c_str(), "rb");
		
		I32 imgW = -1, imgH = -1, channels = -1;
		const U8* data = nullptr;

		if (!imgFile)
		{
			TALGA_ASSERT(0, "invalid textue was attempted to  be loaded");
			return -1;
		}
		
		data = stbi_load_from_file(imgFile, &imgW, &imgH, &channels, 0);

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
		for (int i = strlen(path.c_str()) - 1; i > 0; --i)
		{
			if (path[i] == '\\' || path[i] == '/')
			{
				start = i;
				break;
			}
		}

		name = path.substr(start + 1);

		stbi_image_free((void*)data);

		return 0;
	}

	void Texture::Bind() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture);
	}

	void Texture::Destroy()
	{
		mWidth = 0;
		mHeight = 0;
	}


	Texture::~Texture()
	{

	}
}