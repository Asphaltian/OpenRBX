#ifndef V8DATAMODEL_IPRIMARYPART_H
#define V8DATAMODEL_IPRIMARYPART_H

#include "decomp.h"

namespace RBX {

class PartInstance;

// SIZE 0x4
class __declspec(novtable) IPrimaryPart
{
public:
	virtual PartInstance* getPrimaryPart() = 0;                  // vtable+0x00
	virtual const PartInstance* getPrimaryPartConst() const = 0; // vtable+0x04
};

DECOMP_SIZE_ASSERT(IPrimaryPart, 0x4)

} // namespace RBX

#endif // V8DATAMODEL_IPRIMARYPART_H
