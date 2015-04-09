#include "Texture.h"
#include "SDL2\SDL_image.h"
#include "GL\glew.h"
#include <cstring>
#include <iostream>
#include <cassert>

namespace talga
{
	Texture::Texture() :
		mWidth(0), mHeight(0), mTexture(0)
	{
	}

	int Texture::Init(std::string path)
	{
		SDL_Surface* tempSurf = IMG_Load(path.c_str());
		if (!tempSurf)
		{
			std::cout << "Failed to load texture at " << path << std::endl;
			return -1;
		}
		glGenTextures(1, &mTexture);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tempSurf->w, tempSurf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tempSurf->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);

		mWidth = tempSurf->w;
		mHeight = tempSurf->h;

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