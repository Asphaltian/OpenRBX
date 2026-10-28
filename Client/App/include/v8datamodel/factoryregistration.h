#ifndef V8DATAMODEL_FACTORYREGISTRATION_H
#define V8DATAMODEL_FACTORYREGISTRATION_H

#include "decomp.h"

namespace RBX {

// SIZE 0x1
class FactoryRegistrator
{
public:
	FactoryRegistrator();
};

DECOMP_SIZE_ASSERT(FactoryRegistrator, 0x1)

} // namespace RBX

#endif // V8DATAMODEL_FACTORYREGISTRATION_H
