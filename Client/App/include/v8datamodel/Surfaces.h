#ifndef V8DATAMODEL_SURFACES_H
#define V8DATAMODEL_SURFACES_H

#include "decomp.h"
#include "util/NormalId.h"
#include "util/Vector6.h"
#include "v8datamodel/Surface.h"

#include <boost/noncopyable.hpp>

namespace RBX {

namespace Reflection {
class PropertyDescriptor;
}

// SIZE 0x30
class Surfaces : public boost::noncopyable
{
private:
	Surface Top;    // 0x00
	Surface Bottom; // 0x08
	Surface Left;   // 0x10
	Surface Right;  // 0x18
	Surface Front;  // 0x20
	Surface Back;   // 0x28

public:
	static const bool isSurfaceDescriptor(const Reflection::PropertyDescriptor& desc);

	Surfaces(PartInstance* partInstance);

	Surface& operator[](NormalId normalId);
	const Surface& operator[](NormalId normalId) const;

	// FUNCTION: WEBSERVICE 0x1009ace0
	Vector6<SurfaceType> surf6() const
	{
		Vector6<SurfaceType> answer;

		for (int i = 0; i < 6; i++) {
			answer[i] = (*this)[static_cast<NormalId>(i)].getSurfaceType();
		}

		return answer;
	}

	const Reflection::PropertyDescriptor& getSurfaceType(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getSurfaceInput(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getParamA(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getParamB(NormalId normalId) const;

	const bool isStandardPart() const;
};

DECOMP_SIZE_ASSERT(Surfaces, 0x30)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10224e10
// RBX::`dynamic atexit destructor for 'desc_TopSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10224e30
// RBX::`dynamic atexit destructor for 'desc_TopType''
// SYNTHETIC: WEBSERVICE 0x10224e50
// RBX::`dynamic atexit destructor for 'desc_BottomParamB''
// SYNTHETIC: WEBSERVICE 0x10224e70
// RBX::`dynamic atexit destructor for 'desc_BottomParamA''
// SYNTHETIC: WEBSERVICE 0x10224e90
// RBX::`dynamic atexit destructor for 'desc_BottomSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10224eb0
// RBX::`dynamic atexit destructor for 'desc_BottomType''
// SYNTHETIC: WEBSERVICE 0x10224ed0
// RBX::`dynamic atexit destructor for 'desc_LeftParamB''
// SYNTHETIC: WEBSERVICE 0x10224ef0
// RBX::`dynamic atexit destructor for 'desc_LeftParamA''
// SYNTHETIC: WEBSERVICE 0x10224f10
// RBX::`dynamic atexit destructor for 'desc_LeftSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10224f30
// RBX::`dynamic atexit destructor for 'desc_LeftType''
// SYNTHETIC: WEBSERVICE 0x10224f50
// RBX::`dynamic atexit destructor for 'desc_RightParamB''
// SYNTHETIC: WEBSERVICE 0x10224f70
// RBX::`dynamic atexit destructor for 'desc_RightParamA''
// SYNTHETIC: WEBSERVICE 0x10224f90
// RBX::`dynamic atexit destructor for 'desc_RightSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10224fb0
// RBX::`dynamic atexit destructor for 'desc_RightType''
// SYNTHETIC: WEBSERVICE 0x10224fd0
// RBX::`dynamic atexit destructor for 'desc_FrontParamB''
// SYNTHETIC: WEBSERVICE 0x10224ff0
// RBX::`dynamic atexit destructor for 'desc_FrontParamA''
// SYNTHETIC: WEBSERVICE 0x10225010
// RBX::`dynamic atexit destructor for 'desc_FrontSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10225030
// RBX::`dynamic atexit destructor for 'desc_FrontType''
// SYNTHETIC: WEBSERVICE 0x10225050
// RBX::`dynamic atexit destructor for 'desc_BackParamB''
// SYNTHETIC: WEBSERVICE 0x10225070
// RBX::`dynamic atexit destructor for 'desc_BackParamA''
// SYNTHETIC: WEBSERVICE 0x10225090
// RBX::`dynamic atexit destructor for 'desc_BackSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x102250b0
// RBX::`dynamic atexit destructor for 'desc_BackType''
// SYNTHETIC: WEBSERVICE 0x102250d0
// RBX::`dynamic atexit destructor for 'desc_TopParamB''
// SYNTHETIC: WEBSERVICE 0x102250f0
// RBX::`dynamic atexit destructor for 'desc_TopParamA''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_SURFACES_H
