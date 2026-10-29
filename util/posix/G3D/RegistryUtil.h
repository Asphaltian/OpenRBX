#ifndef POSIX_G3D_REGISTRYUTIL_H
#define POSIX_G3D_REGISTRYUTIL_H

#include <G3D/g3dmath.h>
#include <G3D/platform.h>
#include <string>

namespace G3D {

class RegistryUtil
{
public:
	static bool keyExists(const std::string&) { return false; }
	static bool read32bitNumber(const std::string&, int32&) { return false; }
	static bool readBytes(const std::string&, uint8*, uint32&) { return false; }
	static bool readString(const std::string&, std::string&) { return false; }
	static bool write32bitNumber(const std::string&, int32) { return false; }
	static bool writeBytes(const std::string&, const uint8*, uint32) { return false; }
	static bool writeString(const std::string&, const std::string&) { return false; }
};

} // namespace G3D

#endif // POSIX_G3D_REGISTRYUTIL_H
