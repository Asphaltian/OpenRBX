#ifndef V8DATAMODEL_IEQUIPABLE_H
#define V8DATAMODEL_IEQUIPABLE_H

#include "decomp.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

class Weld;
class Workspace;

// SIZE 0x10
class __declspec(novtable) IEquipable
{
protected:
	IEquipable();

	virtual ~IEquipable(); // vtable+0x00

	boost::shared_ptr<Weld> weld; // 0x04
	Workspace* workspace;         // 0x0c
};

DECOMP_SIZE_ASSERT(IEquipable, 0x10)

} // namespace RBX

#endif // V8DATAMODEL_IEQUIPABLE_H
