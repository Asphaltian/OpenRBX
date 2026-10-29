#ifndef V8DATAMODEL_CUSTOMMESH_H
#define V8DATAMODEL_CUSTOMMESH_H

#include "decomp.h"
#include "util/MeshId.h"
#include "util/TextureId.h"
#include "v8tree/Instance.h"

#include <G3D/Vector3.h>

namespace RBX {

extern char sSpecialShape[];

// SIZE 0x154
class SpecialShape : public DescribedCreatable<SpecialShape, Instance, sSpecialShape>
{
public:
	enum MeshType
	{
		HEAD_MESH = 0,
		TORSO_MESH = 1,
		WEDGE_MESH = 2,
		SPHERE_MESH = 3,
		CYLINDER_MESH = 4,
		FILE_MESH = 5,
		BRICK_MESH = 6
	};

private:
	MeshType meshType;      // 0x0f8
	G3D::Vector3 scale;     // 0x0fc
	TextureId textureId;    // 0x108
	MeshId meshId;          // 0x128
	G3D::Vector3 vertColor; // 0x148

public:
	SpecialShape();

	// FUNCTION: WEBSERVICE 0x10068b70 FOLDED
	const MeshType getMeshType() const { return meshType; }

	void setMeshType(MeshType value);

	// FUNCTION: WEBSERVICE 0x1012c910
	const G3D::Vector3& getScale() const { return scale; }

	void setScale(const G3D::Vector3& value);

	// FUNCTION: WEBSERVICE 0x1012c920
	const G3D::Vector3& getVertColor() const { return vertColor; }

	void setVertColor(const G3D::Vector3& value);

	const float getAlpha() const;

	// FUNCTION: WEBSERVICE 0x1012cae0
	// FUNCTION: ROBLOX 0x00477c60
	const MeshId getMeshId() const { return meshId; }

	void setMeshId(const MeshId& value);

	// FUNCTION: WEBSERVICE 0x1007cdb0 FOLDED
	// FUNCTION: ROBLOX 0x005c0ea0 FOLDED
	const TextureId getTextureId() const { return textureId; }

	void setTextureId(const TextureId& value);

protected:
	virtual bool askSetParent(const Instance* instance) const;

public:
	// SYNTHETIC: WEBSERVICE 0x1012db20
	// SYNTHETIC: ROBLOX 0x005b4420
	// RBX::SpecialShape::~SpecialShape

	// SYNTHETIC: WEBSERVICE 0x1012db90
	// RBX::SpecialShape::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(SpecialShape, 0x154)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021e810
// RBX::`dynamic initializer for 'desc_meshType''
// SYNTHETIC: WEBSERVICE 0x1021e850
// RBX::`dynamic initializer for 'desc_scale''
// SYNTHETIC: WEBSERVICE 0x1021e890
// RBX::`dynamic initializer for 'desc_meshId''
// SYNTHETIC: WEBSERVICE 0x1021e8d0
// RBX::`dynamic initializer for 'desc_textureId''
// SYNTHETIC: WEBSERVICE 0x1021e910
// RBX::`dynamic initializer for 'desc_vertColor''
// SYNTHETIC: WEBSERVICE 0x10225650
// RBX::`dynamic atexit destructor for 'desc_vertColor''
// SYNTHETIC: WEBSERVICE 0x10225670
// RBX::`dynamic atexit destructor for 'desc_textureId''
// SYNTHETIC: WEBSERVICE 0x10225690
// RBX::`dynamic atexit destructor for 'desc_meshId''
// SYNTHETIC: WEBSERVICE 0x102256b0
// RBX::`dynamic atexit destructor for 'desc_scale''
// FUNCTION: WEBSERVICE 0x10225630
// RBX::`dynamic atexit destructor for 'desc_meshType''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_CUSTOMMESH_H
