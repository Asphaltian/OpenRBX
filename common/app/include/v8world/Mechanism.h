#ifndef V8WORLD_MECHANISM_H
#define V8WORLD_MECHANISM_H

#include "decomp.h"

#include <boost/noncopyable.hpp>

namespace RBX {

class Assembly;
class Mechanism;
class Primitive;

// SIZE 0x04
class MechanismTracker
{
public:
	bool tracking() const;

private:
	Mechanism* mechanism; // 0x00
};

DECOMP_SIZE_ASSERT(MechanismTracker, 0x04)

// SIZE 0x24
class Mechanism : public boost::noncopyable
{
public:
	static Mechanism* getMechanismFromPrimitive(Primitive* primitive);

private:
	undefined m_unk0x00[0x24 - 0x00]; // 0x00
};

DECOMP_SIZE_ASSERT(Mechanism, 0x24)

} // namespace RBX

#endif // V8WORLD_MECHANISM_H
