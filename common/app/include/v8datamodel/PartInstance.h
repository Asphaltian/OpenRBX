#ifndef V8DATAMODEL_PARTINSTANCE_H
#define V8DATAMODEL_PARTINSTANCE_H

#include "decomp.h"

namespace RBX {

// SIZE 0x2ac
class PartInstance
{
public:
	static bool highlightSleepParts;
	static bool highlightAwakeParts;
	static bool showAnchoredParts;
	static bool showPartCoord;
	static bool showUnalignedParts;
	static bool showSpanningTree;

private:
	undefined m_unk0x000[0x2ac - 0x000]; // 0x000
};

DECOMP_SIZE_ASSERT(PartInstance, 0x2ac)

} // namespace RBX

#endif // V8DATAMODEL_PARTINSTANCE_H
