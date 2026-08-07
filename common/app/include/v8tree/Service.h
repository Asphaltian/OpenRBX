#ifndef V8TREE_SERVICE_H
#define V8TREE_SERVICE_H

#include "decomp.h"

namespace RBX {

extern const char sServiceProvider[];

// SIZE 0x1
class Service
{
};

DECOMP_SIZE_ASSERT(Service, 0x1)

} // namespace RBX

#endif // V8TREE_SERVICE_H
