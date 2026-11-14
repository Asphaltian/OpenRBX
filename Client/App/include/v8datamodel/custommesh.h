#ifndef V8DATAMODEL_CUSTOMMESH_H
#define V8DATAMODEL_CUSTOMMESH_H

#include "placeholder.h"
#include "util/MeshId.h"
#include "util/TextureId.h"
#include "v8tree/Instance.h"

#include <G3D/Vector3.h>

namespace RBX {

extern char sSpecialShape[];

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
	MeshType meshType;
	G3D::Vector3 scale;
	TextureId textureId;
	MeshId meshId;
	G3D::Vector3 vertColor;

public:
	SpecialShape();

	const MeshType getMeshType() const { return meshType; }

	void setMeshType(MeshType value);

	const G3D::Vector3& getScale() const { return scale; }

	void setScale(const G3D::Vector3& value);

	const G3D::Vector3& getVertColor() const { return vertColor; }

	void setVertColor(const G3D::Vector3& value);

	const float getAlpha() const;

	const MeshId getMeshId() const { return meshId; }

	void setMeshId(const MeshId& value);

	const TextureId getTextureId() const { return textureId; }

	void setTextureId(const TextureId& value);

protected:
	virtual bool askSetParent(const Instance* instance) const;

public:
};

} // namespace RBX

#endif // V8DATAMODEL_CUSTOMMESH_H
