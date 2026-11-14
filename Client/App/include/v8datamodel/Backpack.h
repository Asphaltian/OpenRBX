#ifndef V8DATAMODEL_BACKPACK_H
#define V8DATAMODEL_BACKPACK_H

#include "script/Script.h"
#include "placeholder.h"
#include "v8datamodel/Hopper.h"

namespace RBX {

extern const char sBackpack[];

class Backpack : public DescribedCreatable<Backpack, Hopper, sBackpack>, public IScriptOwner
{
private:
	virtual IScriptOwner* scriptShouldRun(Script* script);

public:
	Backpack();
};

} // namespace RBX

#endif // V8DATAMODEL_BACKPACK_H
