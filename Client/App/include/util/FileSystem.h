#ifndef UTIL_FILESYSTEM_H
#define UTIL_FILESYSTEM_H

#include "decomp.h"

#include <string>

namespace RBX {

// SIZE 0x1
class FileSystem
{
public:
	static std::string getUserAppDataDirectory(bool create, const char* subDirectory);
	static std::string getCacheDirectory(bool create);
};

} // namespace RBX

#endif // UTIL_FILESYSTEM_H
