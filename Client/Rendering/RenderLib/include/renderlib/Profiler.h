#ifndef RENDERLIB_PROFILER_H
#define RENDERLIB_PROFILER_H

#include "placeholder.h"

#include <string>

namespace RBX {
namespace Render {

struct SpecData
{
	static bool glLoaded;

	const float shaderModel;
	const int videoMemory;
	const int CPUSpeed;
	const std::string glVendor;
	const std::string gfxcard;
	const std::string cpu;
	const int ram;
	const std::string resolution;

	SpecData();
};

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_PROFILER_H
