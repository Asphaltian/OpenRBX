#ifndef UTIL_FILESYSTEM_H
#define UTIL_FILESYSTEM_H

#include "placeholder.h"

#include <string>

namespace RBX {

class FileSystem
{
public:
	static std::string getUserAppDataDirectory(bool create, const char* subDirectory);
	static std::string getCacheDirectory(bool create);
};

} // namespace RBX

#endif // UTIL_FILESYSTEM_H
