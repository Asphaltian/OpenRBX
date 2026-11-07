#include "appdraw/AdornRbxGfx.h"

#include "appdraw/VisualEngine.h"

#include <OgreViewport.h>

namespace RBX {

// FUNCTION: ROBLOX 0x0066a0d0
int AdornRbxGfx::getWidth() const
{
	return m_unk0x04->getViewport()->getActualWidth();
}

// FUNCTION: ROBLOX 0x0066a0e0
int AdornRbxGfx::getHeight() const
{
	return m_unk0x04->getViewport()->getActualHeight();
}

// FUNCTION: ROBLOX 0x0066a660
void AdornRbxGfx::setObjectToWorldMatrix(const G3D::CoordinateFrame& coord)
{
	m_unk0x94 = coord;
}

} // namespace RBX
