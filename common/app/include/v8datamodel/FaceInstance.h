#ifndef V8DATAMODEL_FACEINSTANCE_H
#define V8DATAMODEL_FACEINSTANCE_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8tree/Instance.h"

namespace RBX {

// SIZE 0xfc
class FaceInstance : public Instance
{
public:
	// FUNCTION: WEBSERVICE 0x10068b70 FOLDED
	NormalId getFace() const { return face; }

	void setFace(NormalId value);

private:
	NormalId face; // 0x0f8
};

DECOMP_SIZE_ASSERT(FaceInstance, 0xfc)

} // namespace RBX

#endif // V8DATAMODEL_FACEINSTANCE_H
