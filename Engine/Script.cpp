#include "Script.h"
#include "sys.h"
#include "LuaEngine.h"
#include <fstream>

namespace talga
{
	Script::Script()
	{
	}

	void Script::execute() const
	{
		LuaEngine::instance()->ExecuteFile(mPath + mName);
	}

	bool Script::load(std::string path, AssetManager& manager)
	{
		mName = getFileNameFromPath(path);
		mPath = getPathFromFilePath(path);

		std::fstream stream;

		stream.open(path, std::fstream::in);

		if (!stream.is_open())
			return false;

		stream.close();
		return true;
	}

	bool Script::save(std::string path, AssetManager& manager)
	{

		return true;
	}

	void Script::destroy()
	{
	}

	Script::~Script()
	{
	}
}