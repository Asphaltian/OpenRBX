#ifndef V8DATAMODEL_MODELINSTANCE_H
#define V8DATAMODEL_MODELINSTANCE_H

#include "decomp.h"

namespace RBX {

// SIZE 0x254
class ModelInstance
{
public:
	static bool showModelCoord;

private:
	undefined m_unk0x000[0x254 - 0x000]; // 0x000
};

DECOMP_SIZE_ASSERT(ModelInstance, 0x254)

} // namespace RBX

#endif // V8DATAMODEL_MODELINSTANCE_H
