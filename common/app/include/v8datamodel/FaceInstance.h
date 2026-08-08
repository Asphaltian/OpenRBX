#ifndef V8DATAMODEL_FACEINSTANCE_H
#define V8DATAMODEL_FACEINSTANCE_H

#include "decomp.h"
#include "reflection/property.h"
#include "util/NormalId.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sFaceInstance[];

// SIZE 0xfc
class FaceInstance : public Reflection::Described<FaceInstance, sFaceInstance, Instance>
{
public:
	FaceInstance();

	static const Reflection::EnumPropDescriptor<FaceInstance, NormalId> prop_Face;

	// FUNCTION: WEBSERVICE 0x10068b70 FOLDED
	NormalId getFace() const { return face; }

	void setFace(NormalId value);

private:
	NormalId face; // 0x0f8
};

DECOMP_SIZE_ASSERT(FaceInstance, 0xfc)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10225590
// `dynamic atexit destructor for 'RBX::FaceInstance::prop_Face''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_FACEINSTANCE_H
