#ifndef RENDERLIB_PROFILER_H
#define RENDERLIB_PROFILER_H

#include "decomp.h"

#include <string>

namespace RBX {
namespace Render {

// SIZE 0x80
struct SpecData
{
	static bool glLoaded;

	const float shaderModel;      // 0x00
	const int videoMemory;        // 0x04
	const int CPUSpeed;           // 0x08
	const std::string glVendor;   // 0x0c
	const std::string gfxcard;    // 0x28
	const std::string cpu;        // 0x44
	const int ram;                // 0x60
	const std::string resolution; // 0x64

	SpecData();

	// SYNTHETIC: WEBSERVICE 0x100b2dc0
	// SYNTHETIC: ROBLOX 0x0057aba0
	// RBX::Render::SpecData::~SpecData
};

DECOMP_SIZE_ASSERT(SpecData, 0x80)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_PROFILER_H
