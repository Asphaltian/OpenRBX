#ifndef UTIL_INDEXBOX_H
#define UTIL_INDEXBOX_H

#include <G3D/Plane.h>
#include <G3D/Vector3.h>

namespace RBX {

using G3D::Vector3;

// VTABLE: WEBSERVICE 0x1022cf1c
// VTABLE: ROBLOX 0x0083cf14
class IndexBox
{
public:
	IndexBox(const Vector3& min, const Vector3& max);

	// FUNCTION: WEBSERVICE 0x100314e0
	virtual ~IndexBox() // vtable+0x00
	{
	}

	// SYNTHETIC: WEBSERVICE 0x100314f0
	// SYNTHETIC: ROBLOX 0x0066a0b0
	// RBX::IndexBox::`scalar deleting destructor'

	Vector3 getFaceNormal(int f) const
	{
		return Vector3(INDEXBOX_FACE_TO_NORMAL[f][0], INDEXBOX_FACE_TO_NORMAL[f][1], INDEXBOX_FACE_TO_NORMAL[f][2]);
	}
	G3D::Vector3 getEdgeNormal(int, int) const;

	void getFaceCorners(int f, Vector3& v0, Vector3& v1, Vector3& v2, Vector3& v3) const;
	void getEdge(int, G3D::Vector3&, G3D::Vector3&, G3D::Vector3&, G3D::Vector3&) const;
	void getEdge(int, G3D::Vector4&, G3D::Vector4&, G3D::Vector3&, G3D::Vector3&) const;
	static void getTextureCornersCentered(
		int, const G3D::Vector3&, G3D::Vector2&, G3D::Vector2&, G3D::Vector2&, G3D::Vector2&
	);
	static void getTextureCornersGrid(
		int, const G3D::Vector3&, G3D::Vector2&, G3D::Vector2&, G3D::Vector2&, G3D::Vector2&
	);
	bool culledBy(const G3D::Plane*, int) const;
	bool contains(const G3D::Vector3&) const;

	static const int INDEXBOX_FACE_TO_VERTEX[6][4];
	static const float INDEXBOX_FACE_TO_NORMAL[6][3];
	G3D::Vector3 getCenter() const;
	G3D::Vector3 getCorner(int) const;

private:
	Vector3 corner[8]; // 0x04
};

} // namespace RBX

#endif // UTIL_INDEXBOX_H
