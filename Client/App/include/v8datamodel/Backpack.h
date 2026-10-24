#ifndef V8DATAMODEL_BACKPACK_H
#define V8DATAMODEL_BACKPACK_H

#include "decomp.h"
#include "script/Script.h"
#include "v8datamodel/Hopper.h"

namespace RBX {

extern const char sBackpack[];

// SIZE 0x134
class Backpack : public DescribedCreatable<Backpack, Hopper, sBackpack>, public IScriptOwner
{
private:
	virtual IScriptOwner* scriptShouldRun(Script* script);

public:
	Backpack();

	// SYNTHETIC: WEBSERVICE 0x100da720
	// RBX::Backpack::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Backpack, 0x134)

} // namespace RBX

#endif // V8DATAMODEL_BACKPACK_H
