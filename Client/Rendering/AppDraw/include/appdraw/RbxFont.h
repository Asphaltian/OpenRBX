#ifndef APPDRAW_RBXFONT_H
#define APPDRAW_RBXFONT_H

#include "rbxgraphics/Adorn.h"
#include "placeholder.h"

#include <G3D/BinaryInput.h>
#include <G3D/Color4.h>
#include <OgreTexture.h>
#include <OgreVector2.h>
#include <string>

namespace Ogre {

class VisualEngine;

class RbxFont
{
public:
	RbxFont(const std::string& filename, G3D::BinaryInput& in);

	Vector2 FUN_0067de70(
		const std::string& s,
		double x,
		double y,
		double width,
		double height,
		RBX::Adorn::Spacing spacing,
		Vector2* quads
	);
	Vector2 FUN_0067e0f0(const std::string& s, double size, RBX::Adorn::Spacing spacing);
	Vector2 FUN_0067e1e0(
		VisualEngine* engine,
		const std::string& s,
		const Vector2& position,
		double size,
		const G3D::Color4& color,
		const G3D::Color4& outline,
		RBX::Adorn::XAlign xAlign,
		RBX::Adorn::YAlign yAlign,
		RBX::Adorn::Spacing spacing
	);

	static SharedPtr<RbxFont> FUN_0067e9d0(const std::string& filename);

private:
	int m_unk0x00[128];
	int m_unk0x200;
	int m_unk0x204;
	int m_unk0x208;
	TexturePtr m_unk0x20c;
};

extern int g_unk0x00961434;

} // namespace Ogre

#endif // APPDRAW_RBXFONT_H
