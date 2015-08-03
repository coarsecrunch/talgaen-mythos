#include "sys.h"
#include "Cmn.h"
#include <vector>

#ifdef TALGA_WINDOWS_BUILD
#include <windows.h>
#include <cstring>
#endif

namespace talga
{
	std::string getWorkingDirectory()
	{
		char buffer[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, buffer);
		strcat(buffer, "\\");
		std::string::size_type pos = std::string(buffer).find_last_of("\\/");
		std::string path = std::string(buffer).substr(0, pos);
		StrList fldrs = seperatePath(path);

		path.clear();

		for (auto it = fldrs.begin(); it != fldrs.end(); ++it)
		{
			path.append(*it);
			path.append("/");
		}
		return path;
	}
	
	std::string getAbsFromRel(const std::string& relativeTo, const std::string& relativePath)
	{
		StrList absPath = seperatePath(relativeTo);
		StrList relPath = seperatePath(relativePath);
		std::string newPath;

		auto beginRelPath = relPath.begin();
		auto endAbsPath = absPath.end();
		for (auto it = relPath.begin(); it != relPath.end(); ++it)
		{
			if (*it == "..")
			{
				--endAbsPath;
			}	
			else
			{
				beginRelPath = it;
				break;
			}
		}

		for (auto it = absPath.begin(); it != endAbsPath; ++it)
		{
			newPath.append(*it);
			newPath.append("/");
		}

		for (auto it = beginRelPath; it != relPath.end(); ++it)
		{
			newPath.append(*it);
			newPath.append("/");
		}

		return newPath;
	}

	std::string getFileNameFromPath(const std::string& path)
	{
		return seperatePath(path).back();
	}

	std::string getFileNameFromPathWithoutExtension(const std::string& path)
	{
		std::string name = getFileNameFromPath(path);

		I32 i = 0;
		I32 indexOfDot = 0;
		while (i < name.size())
		{
			if (name[i] == '.')
				indexOfDot = i;

			++i;
		}

		if (indexOfDot)
			for (I32 c = name.size() - 1; c >= indexOfDot; --c)
				name.pop_back();
	
		return name;
	}


	std::string getPathFromFilePath(const std::string& path)
	{
		bool isAbsolute = false;
		if (path.back() == '/')
			isAbsolute = true;
		StrList temp = seperatePath(path);
		temp.pop_back();
		
		std::string newPath;

		for (auto it = temp.begin(); it != temp.end(); ++it)
		{
			newPath.append(*it);
			newPath.append("/");
		}
		return newPath;
	}

	std::string getRelFromAbs(const std::string& relativeToAbs, const std::string& path)
	{
		StrList relativeToAbsFolders = seperatePath(relativeToAbs);
		StrList pathFolders = seperatePath(path);
		std::string newPath;
		auto relativeIter = relativeToAbsFolders.begin(), pathIter = pathFolders.begin();
		for (; relativeIter != relativeToAbsFolders.end() && pathIter != pathFolders.end(); ++relativeIter, ++pathIter)
		{
			if (*relativeIter != *pathIter)
			{
				break;
			}
		}

		I32 count = 0;

		while (relativeIter != relativeToAbsFolders.end())
		{
			++count;
			++relativeIter;
		}

		for (I32 i = 0; i < count; ++i)
		{
			newPath.append("../");
		}

		while (pathIter != pathFolders.end())
		{
			newPath.append(*pathIter);
			newPath.append("/");
			++pathIter;
		}

		return newPath;
	}

	//also removes weird relative/absolute problems with visual studio
	std::list<std::string> seperatePath(const std::string& path)
	{
		std::list<std::string> folders;
		if (path == "" || path == " ") return folders;
		folders.push_back(std::string());

		for (I32 i = 0; i < path.size(); ++i)
		{
			if (path[i] == '\\' || path[i] == '/')
			{
				folders.push_back(std::string());
				continue;
			}

			folders.back().push_back(path[i]);
		}

		if (path.back() == '/' || path.back() == '\\')
			folders.pop_back();
		
		bool relative = true;

		for (auto it = folders.begin(); it != folders.end(); ++it)
		{
			if (*it != "..")
				relative = false;

			if (*it == ".." && !relative)
			{
				auto startRelativeWeirdness = it;
				auto endRelativeWeirdness = it;
				endRelativeWeirdness++;
				++it;

				while (it != folders.end() && *it == "..")
				{

					//once for the actual ".." and one for the path it's going into
					++endRelativeWeirdness;
					++endRelativeWeirdness;
					++it;
				}

				folders.erase(startRelativeWeirdness, endRelativeWeirdness);
			}
		}

    return folders;
  }
std::string getFileExtension(const std::string& str)
{
	int countback = str.size();
	for (auto it = str.rbegin(); it != str.rend(); ++it)
	{
		--countback;
		if (*it == '.')
			return str.substr(countback, str.size() - countback);
	}
}
  bool isAbs(const std::string &path)
  {
#ifdef TALGA_WINDOWS_BUILD
    if (seperatePath(path).front().find(":") != std::string::npos)
      return true;

    return false;
#endif
  }

}
