#ifndef APPDRAW_VISUALENGINE_H
#define APPDRAW_VISUALENGINE_H

#include "appdraw/TwoDManager.h"
#include "placeholder.h"

#include <OgreAxisAlignedBox.h>
#include <OgreEdgeListBuilder.h>
#include <OgreMesh.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreSubMesh.h>
#include <string>

class CRobloxWnd;

namespace G3D {
class Color4;
class Vector3;
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

class Unknown00671c70 : public SubMesh
{
public:
	Unknown00671c70();
	~Unknown00671c70();

	void FUN_0067c590();

	bool m_unk0x84;
	EdgeData* m_unk0x88;
	AxisAlignedBox m_unk0x8c;
	float m_unk0xac;
};

Vector3 FUN_005e0b40(const G3D::Vector3& v);
ColourValue FUN_00671cd0(const G3D::Color4& color);

class VisualEngine
{
public:
	class Unk0x18
	{
	public:
		struct Unk0x08
		{
			Unk0x08() : m_unk0x00(-1.0f), m_unk0x04(-1.0f), m_unk0x08(-1.0f), m_unk0x0c(-1.0f), m_unk0x10(-1.0f) {}

			float m_unk0x00;
			float m_unk0x04;
			float m_unk0x08;
			float m_unk0x0c;
			float m_unk0x10;
		};

		Unk0x18();

		int FUN_00671ea0() const;
		void FUN_00671eb0(const Unk0x08& entry);
		Unk0x08& FUN_00671ef0(int back);
		float FUN_00672980(int count);

	private:
		int m_unk0x00;
		undefined4 m_unk0x04;
		Unk0x08 m_unk0x08[100];
	};

	VisualEngine();
	virtual ~VisualEngine();

	SceneManager* getSceneManager() const { return m_unk0x04; }
	Viewport* getViewport() const { return m_unk0x10; }

	Unk0x18& getUnk0x18() { return m_unk0x18; }

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

private:
	friend class ::CRobloxWnd;
	friend class RBX::AdornRbxGfx;
	friend class RBX::RenderNew::Mesh;
	friend class RBX::RenderNew::RenderScene;
	friend class RBX::RenderNew::TextureProxy;
	friend class RBX::ViewNew::View;
	friend class RbxFont;

	SceneManager* m_unk0x04;
	Camera* m_unk0x08;
	RenderWindow* m_unk0x0c;
	Viewport* m_unk0x10;
	TwoDManager* m_unk0x14;
	Unk0x18 m_unk0x18;
	unsigned int m_unk0x7f0;
	bool m_unk0x7f4;
	bool m_unk0x7f5;
	int m_unk0x7f8;
	int m_unk0x7fc;
	int m_unk0x800;
	bool m_unk0x804;
	bool m_unk0x805;
	bool m_unk0x806;
};

extern bool g_unk0x00949dd0;
extern Root* g_unk0x0096134c;

unsigned int FUN_00671cf0();
void FUN_00672010();

inline void Unknown00671c70::FUN_0067c590()
{
	if (!m_unk0x84) {
		vertexData->prepareForShadowVolume();
		m_unk0x84 = true;
		EdgeListBuilder builder;
		builder.addVertexData(vertexData);
		builder.addIndexData(indexData, 0, operationType);
		m_unk0x88 = builder.build();
	}
}

inline Unknown00671c70::~Unknown00671c70()
{
	delete m_unk0x88;
}

} // namespace Ogre

#endif // APPDRAW_VISUALENGINE_H
