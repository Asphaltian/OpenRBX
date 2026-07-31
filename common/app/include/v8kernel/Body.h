#ifndef V8KERNEL_BODY_H
#define V8KERNEL_BODY_H

#include "decomp.h"
#include "util/PV.h"
#include "util/Velocity.h"

#include <G3D/CoordinateFrame.h>

namespace RBX {

using G3D::CoordinateFrame;

// SIZE 0xd0
class Body
{
public:
	~Body();

	void updatePV() const;

	const CoordinateFrame& getCoordinateFrame() const
	{
		updatePV();
		return pv.position;
	}

	void setVelocity(const Velocity& velocity);

private:
	undefined m_unk0x00[0x88 - 0x00]; // 0x00
	PV pv;                            // 0x88
};

DECOMP_SIZE_ASSERT(Body, 0xd0)

} // namespace RBX

#endif // V8KERNEL_BODY_H
