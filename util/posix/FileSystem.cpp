#include "util/FileSystem.h"

#include <errno.h>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>

namespace RBX {

static std::string homeDirectory()
{
	const char* home = getenv("HOME");
	return home && *home ? home : "/tmp";
}

static std::string xdgDirectory(const char* variable, const char* fallback)
{
	const char* value = getenv(variable);
	if (value && *value) {
		return value;
	}
	return homeDirectory() + fallback;
}

static bool makeDirectories(const std::string& path)
{
	for (size_t i = 1; i <= path.size(); ++i) {
		if (i == path.size() || path[i] == '/') {
			std::string partial = path.substr(0, i);
			if (mkdir(partial.c_str(), 0755) != 0 && errno != EEXIST) {
				return false;
			}
		}
	}
	return true;
}

static std::string withSubDirectory(std::string base, const char* subDirectory)
{
	if (subDirectory && *subDirectory) {
		base += "/";
		for (const char* p = subDirectory; *p; ++p) {
			base += *p == '\\' ? '/' : *p;
		}
	}
	while (base.size() > 1 && base[base.size() - 1] == '/') {
		base.erase(base.size() - 1);
	}
	return base;
}

std::string FileSystem::getUserAppDataDirectory(bool create, const char* subDirectory)
{
	std::string path = withSubDirectory(xdgDirectory("XDG_DATA_HOME", "/.local/share") + "/Roblox", subDirectory);
	if (create && !makeDirectories(path)) {
		return "";
	}
	return path + "/";
}

std::string FileSystem::getCacheDirectory(bool create)
{
	std::string path = xdgDirectory("XDG_CACHE_HOME", "/.cache") + "/Roblox";
	if (create && !makeDirectories(path)) {
		return "";
	}
	return path + "/";
}

} // namespace RBX
