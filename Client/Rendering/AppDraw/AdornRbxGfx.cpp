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

// FUNCTION: ROBLOX 0x0066a0f0
void AdornRbxGfx::rect2d(const G3D::Rect2D& rect, const G3D::Color4& color) const
{
	m_unk0x04->m_unk0x14->FUN_0067d9e0(
		m_unk0x08,
		color,
		rect.x0(),
		getHeight() - 1 - rect.y0(),
		rect.x1(),
		getHeight() - 1 - rect.y1(),
		0.0f,
		0.0f,
		1.0f,
		1.0f
	);
}

// FUNCTION: ROBLOX 0x0066a1b0
void AdornRbxGfx::outlineRect2d(const G3D::Rect2D& rect, float thickness, const G3D::Color4& color) const
{
	float y1 = rect.y1();
	float x1 = rect.x1();
	float y0 = rect.y0();
	float x0 = rect.x0();
	Ogre::TwoDManager* twoDManager = m_unk0x04->m_unk0x14;
	twoDManager->FUN_0067dcc0(2, x0, getHeight() - 1 - y0, x1, getHeight() - 1 - y1, color);
}

// FUNCTION: ROBLOX 0x0066a250
void AdornRbxGfx::line2d(const G3D::Vector2& from, const G3D::Vector2& to, const G3D::Color4& color) const
{
	Ogre::TwoDManager* twoDManager = m_unk0x04->m_unk0x14;
	twoDManager->FUN_0067dd70(from.x, getHeight() - 1 - from.y, to.x, getHeight() - 1 - to.y, color);
}

// FUNCTION: ROBLOX 0x0066a660
void AdornRbxGfx::setObjectToWorldMatrix(const G3D::CoordinateFrame& coord)
{
	m_unk0x94 = coord;
}

// STUB: ROBLOX 0x0066a760
G3D::Rect2D AdornRbxGfx::getViewport() const
{
	Ogre::Viewport* viewport = m_unk0x04->getViewport();
	float left = viewport->getActualLeft();
	float top = viewport->getActualTop();
	float width = viewport->getActualWidth();
	float height = viewport->getActualHeight();
	return G3D::Rect2D::xywh(left, top, width, height);
}

} // namespace RBX
