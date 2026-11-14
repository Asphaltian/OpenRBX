#ifndef V8DATAMODEL_FORCEFIELD_H
#define V8DATAMODEL_FORCEFIELD_H

#include "placeholder.h"
#include "util/IRenderable.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sForceField[];

class PartInstance;

class ForceField : public DescribedCreatable<ForceField, Instance, sForceField>, public IRenderable
{
private:
	int cycle;

	virtual bool askSetParent(const Instance* instance) const;

	virtual bool shouldRender3dAdorn() const { return true; }

	virtual void render3dAdorn(Adorn* adorn);

public:
	ForceField();

	virtual ~ForceField() {}

	static bool partInForceField(PartInstance* part);
	static int cycles();
};

} // namespace RBX

#endif // V8DATAMODEL_FORCEFIELD_H
