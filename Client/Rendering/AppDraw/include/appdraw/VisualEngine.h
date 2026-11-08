#ifndef APPDRAW_VISUALENGINE_H
#define APPDRAW_VISUALENGINE_H

#include "appdraw/TwoDManager.h"
#include "decomp.h"

#include <OgreAxisAlignedBox.h>
#include <OgreMesh.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSubMesh.h>
#include <string>

class CRobloxWnd;

namespace G3D {
class Color4;
} // namespace G3D

namespace RBX {
class AdornRbxGfx;

namespace RenderNew {
class Mesh;
class RenderScene;
class TextureProxy;
} // namespace RenderNew

namespace ViewNew {
class View;
} // namespace ViewNew
} // namespace RBX

namespace Ogre {

// SIZE 0xb0
class Unknown00671c70 : public SubMesh
{
public:
	Unknown00671c70();

	bool m_unk0x84;           // 0x84
	undefined4 m_unk0x88;     // 0x88
	AxisAlignedBox m_unk0x8c; // 0x8c
	float m_unk0xac;          // 0xac
};

DECOMP_SIZE_ASSERT(Unknown00671c70, 0xb0)

ColourValue FUN_00671cd0(const G3D::Color4& color);

// SIZE 0x808
class VisualEngine
{
public:
	// SIZE 0x7d8
	class Unk0x18
	{
	public:
		// SIZE 0x14
		struct Unk0x08
		{
			Unk0x08() : m_unk0x00(-1.0f), m_unk0x04(-1.0f), m_unk0x08(-1.0f), m_unk0x0c(-1.0f), m_unk0x10(-1.0f) {}

			float m_unk0x00; // 0x00
			float m_unk0x04; // 0x04
			float m_unk0x08; // 0x08
			float m_unk0x0c; // 0x0c
			float m_unk0x10; // 0x10
		};

		Unk0x18();

		int FUN_00671ea0() const;
		void FUN_00671eb0(const Unk0x08& entry);
		Unk0x08& FUN_00671ef0(int back);
		float FUN_00672980(int count);

	private:
		int m_unk0x00;          // 0x00
		undefined4 m_unk0x04;   // 0x04
		Unk0x08 m_unk0x08[100]; // 0x08
	};

	VisualEngine();
	virtual ~VisualEngine(); // vtable+0x00

	Viewport* getViewport() const { return m_unk0x10; }

	SubMesh* FUN_00671d90(
		MeshPtr& mesh, const unsigned short* indices16, const unsigned int* indices32, size_t triangleCount
	);

	VertexData* FUN_00672ab0(
		const float* positions,
		size_t vertexCount,
		const float* normals,
		const ColourValue* colours,
		const float* textureCoords
	);

	void FUN_00672e60(
		MeshPtr& mesh,
		const float* positions,
		size_t vertexCount,
		const float* normals,
		const ColourValue* colours,
		const float* textureCoords,
		const Vector3* minimum,
		const Vector3* maximum
	);

	MeshPtr FUN_006731e0(
		const char* name,
		const char* group,
		const float* positions,
		size_t vertexCount,
		const float* normals,
		const ColourValue* colours,
		const float* textureCoords,
		const Vector3* minimum,
		const Vector3* maximum
	);

	Unknown00671c70* FUN_006735b0(
		const float* positions,
		size_t vertexCount,
		const float* normals,
		const ColourValue* colours,
		const float* textureCoords,
		const unsigned short* indices16,
		const unsigned int* indices32,
		size_t triangleCount,
		Vector3* minimumOut,
		Vector3* maximumOut
	);

	void FUN_00673860(
		int renderSystem, unsigned long window, int width, int height, int antialiasing, std::string logFileName
	);

	// SYNTHETIC: ROBLOX 0x00671fb0
	// Ogre::VisualEngine::`scalar deleting destructor'

private:
	friend class ::CRobloxWnd;
	friend class RBX::AdornRbxGfx;
	friend class RBX::RenderNew::Mesh;
	friend class RBX::RenderNew::RenderScene;
	friend class RBX::RenderNew::TextureProxy;
	friend class RBX::ViewNew::View;
	friend class RbxFont;

	SceneManager* m_unk0x04; // 0x04
	Camera* m_unk0x08;       // 0x08
	RenderWindow* m_unk0x0c; // 0x0c
	Viewport* m_unk0x10;     // 0x10
	TwoDManager* m_unk0x14;  // 0x14
	Unk0x18 m_unk0x18;       // 0x18
	unsigned int m_unk0x7f0; // 0x7f0
	bool m_unk0x7f4;         // 0x7f4
	bool m_unk0x7f5;         // 0x7f5
	int m_unk0x7f8;          // 0x7f8
	int m_unk0x7fc;          // 0x7fc
	int m_unk0x800;          // 0x800
	bool m_unk0x804;         // 0x804
	bool m_unk0x805;         // 0x805
	bool m_unk0x806;         // 0x806
};

DECOMP_SIZE_ASSERT(VisualEngine::Unk0x18::Unk0x08, 0x14)
DECOMP_SIZE_ASSERT(VisualEngine::Unk0x18, 0x7d8)
DECOMP_SIZE_ASSERT(VisualEngine, 0x808)

extern bool g_unk0x00949dd0;
extern Root* g_unk0x0096134c;

unsigned int FUN_00671cf0();
void FUN_00672010();

} // namespace Ogre

#endif // APPDRAW_VISUALENGINE_H
