#ifndef V8DATAMODEL_SURFACES_H
#define V8DATAMODEL_SURFACES_H

#include "decomp.h"
#include "util/NormalId.h"
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

	const Reflection::PropertyDescriptor& getSurfaceType(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getSurfaceInput(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getParamA(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getParamB(NormalId normalId) const;

	const bool isStandardPart() const;
};

DECOMP_SIZE_ASSERT(Surfaces, 0x30)

} // namespace RBX

#endif // V8DATAMODEL_SURFACES_H
