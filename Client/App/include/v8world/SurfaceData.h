#ifndef V8WORLD_SURFACEDATA_H
#define V8WORLD_SURFACEDATA_H

#include "decomp.h"
#include "util/SurfaceType.h"
#include "v8world/Controller.h"

namespace RBX {

class SurfaceData
{
public:
	SurfaceData() : inputType(Controller::NO_INPUT), paramA(-0.5f), paramB(0.5f) {}

	static const SurfaceData& empty();

	bool operator==(const SurfaceData& other) const;
	bool isEmpty() const;

	Controller::InputType inputType; // 0x00
	float paramA;                    // 0x04
	float paramB;                    // 0x08
};

DECOMP_SIZE_ASSERT(SurfaceData, 0x0c)

inline const SurfaceData& SurfaceData::empty()
{
	static SurfaceData s;
	return s;
}

inline bool SurfaceData::operator==(const SurfaceData& other) const
{
	return inputType == other.inputType && paramA == other.paramA && paramB == other.paramB;
}

// FUNCTION: WEBSERVICE 0x100a76d0
inline bool SurfaceData::isEmpty() const
{
	return *this == empty();
}

} // namespace RBX

#endif // V8WORLD_SURFACEDATA_H
