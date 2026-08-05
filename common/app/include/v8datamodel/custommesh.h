#ifndef V8DATAMODEL_CUSTOMMESH_H
#define V8DATAMODEL_CUSTOMMESH_H

#include "decomp.h"
#include "v8tree/Instance.h"

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

	// FUNCTION: WEBSERVICE 0x10068b70 FOLDED
	MeshType getMeshType() const { return meshType; }

	void setMeshType(MeshType value);

private:
	MeshType meshType;                   // 0x0f8
	undefined m_unk0x0fc[0x154 - 0x0fc]; // 0x0fc
};

DECOMP_SIZE_ASSERT(SpecialShape, 0x154)

} // namespace RBX

#endif // V8DATAMODEL_CUSTOMMESH_H
