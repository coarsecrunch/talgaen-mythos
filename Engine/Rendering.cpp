#include "Cmn.h"
#include "Rendering.h"
#include "Texture.h"
#include "AssetManager.h"
#include <fstream>
#include <streambuf>
#include <vector>
#include <iostream>

namespace talga
{

	GLuint LoadCompileShaders(const char* vSrc, const char* fSrc)
	{
		GLuint vShaderObj = glCreateShader(GL_VERTEX_SHADER);
		GLuint fShaderObj = glCreateShader(GL_FRAGMENT_SHADER);

		const char* const* vSrcPtr = &vSrc;
		const char* const* fSrcPtr = &fSrc;

		glShaderSource(vShaderObj, 1, vSrcPtr, 0);
		glShaderSource(fShaderObj, 1, fSrcPtr, 0);

		GLint isCompiled = 0;

		glCompileShader(vShaderObj);
		glCompileShader(fShaderObj);

		glGetShaderiv(vShaderObj, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vShaderObj, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(vShaderObj, maxLength, &maxLength, &errorLog[0]);
			glDeleteShader(vShaderObj);
			int it = 0;
			while (it < errorLog.size())
			{
				std::cout << errorLog[it];
				it++;
			}
		}

		glGetShaderiv(fShaderObj, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fShaderObj, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(fShaderObj, maxLength, &maxLength, &errorLog[0]);
			glDeleteShader(fShaderObj);

			int it = 0;
			while (it < errorLog.size())
			{
				std::cout << errorLog[it];
				it++;
			}
		}

		GLuint program = glCreateProgram();
		glAttachShader(program, vShaderObj);
		glAttachShader(program, fShaderObj);

		glLinkProgram(program);

		glDeleteShader(vShaderObj);
		glDeleteShader(fShaderObj);

		return program;
	}

	void LoadShaderSrc(const char* path, std::string& src)
	{
		std::ifstream t(path);

        TALGA_ASSERT(t.is_open(), std::string("failed to find shader at ") + path)

		t.seekg(0, std::ios::end);
		src.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		src.assign((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

	}

	vec3 CartToGL(vec3 point, int screenWidth, int screenHeight)
	{
		return vec3((((point(0) / screenWidth) * 2) - 1), (((point(1) / screenHeight) * -2)) + 1);
	}

	struct

		Map LoadMap(const char* path, AssetManager* manager)
	{
		return Map();
	}
}
