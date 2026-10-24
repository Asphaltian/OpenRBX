#ifndef V8DATAMODEL_FORCEFIELD_H
#define V8DATAMODEL_FORCEFIELD_H

#include "decomp.h"
#include "util/IRenderable.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sForceField[];

class PartInstance;

// SIZE 0x10c
class ForceField : public DescribedCreatable<ForceField, Instance, sForceField>, public IRenderable
{
private:
	int cycle; // 0x108

	virtual bool askSetParent(const Instance* instance) const;

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	virtual bool shouldRender3dAdorn() const { return true; }

	virtual void render3dAdorn(Adorn* adorn);

public:
	ForceField();

	// FUNCTION: WEBSERVICE 0x100e9950
	virtual ~ForceField() {}

	// SYNTHETIC: WEBSERVICE 0x100e99f0
	// RBX::ForceField::`scalar deleting destructor'

	static bool partInForceField(PartInstance* part);
};

DECOMP_SIZE_ASSERT(ForceField, 0x10c)

} // namespace RBX

#endif // V8DATAMODEL_FORCEFIELD_H
