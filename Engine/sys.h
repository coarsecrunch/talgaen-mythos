#pragma once

#include <string>
#include <list>

namespace talga
{
	typedef std::list < std::string > StrList;
	std::string getWorkingDirectory();
	std::string getAbsFromRel(const std::string& relativeTo, const std::string& relativePath);
	std::list<std::string> seperatePath(const std::string& path);
	std::string getFileNameFromPath(const std::string& path);
	std::string getFileNameFromPathWithoutExtension(const std::string& path);
	std::string getPathFromFilePath(const std::string& path);
	std::string getRelFromAbs(const std::string& relativeToAbs, const std::string& path);
	std::string getFileExtension(const std::string& str);
	bool isAbs(const std::string& path);
	//std::string 
}
