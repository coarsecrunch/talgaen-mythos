#pragma once

#include "GL\glew.h"
#include <string>
#include "Cmn.h"
#include "Math/Vector3.h"
#include "Map.h"

namespace talga
{
	class AssetManager;

	GLuint LoadCompileShaders(const char* vSrc, const char* fSrc);
	void LoadShaderSrc(const char* path, std::string& src);
	vec3 CartToGL(vec3 point, int screenWidth, int screenHeight);
}