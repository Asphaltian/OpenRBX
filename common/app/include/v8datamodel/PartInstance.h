#ifndef V8DATAMODEL_PARTINSTANCE_H
#define V8DATAMODEL_PARTINSTANCE_H

#include "decomp.h"
#include "util/Extents.h"
#include "v8datamodel/Surfaces.h"
#include "v8world/Primitive.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace RBX {

using boost::shared_ptr;

class World;

// SIZE 0x2ac
class PartInstance
{
public:
	enum FormFactor
	{
		SYMETRIC = 0,
		BRICK = 1,
		PLATE = 2
	};

	static bool highlightSleepParts;
	static bool highlightAwakeParts;
	static bool showAnchoredParts;
	static bool showPartCoord;
	static bool showUnalignedParts;
	static bool showSpanningTree;

	static bool nonNullInWorkspace(shared_ptr<PartInstance> part);

	FormFactor getFormFactor() const { return formFactor; }

	float getRenderImportance() const { return renderImportance; }

	float getTransparencyXml() const { return transparency; }

	float getReflectance() const { return reflectance; }

	bool getPartLocked() const { return locked; }

	Surfaces& getSurfaces() { return surfaces; }

	Primitive* getBiggestPrimitive() const { return primitive.get(); }

	bool getIsTransparent() const { return 1.0f - (1.0f - transparency) * alphaModifier > 0.1f; }

	const CoordinateFrame& getCoordinateFrame() const;

	float getMass() const;

	const Vector3& getPartSizeXml() const;
	Vector3 getPartSizeUi() const;

	bool getDragging() const;
	bool getCanCollide() const;
	bool getAnchored() const;

	float getFriction() const;
	float getElasticity() const;

	Extents getExtentsLocal() const;

private:
	undefined m_unk0x000[0x19c - 0x000];    // 0x000
	undefined m_unk0x19c[0x1a0 - 0x19c];    // 0x19c
	FormFactor formFactor;                  // 0x1a0
	undefined m_unk0x1a4[0x1a8 - 0x1a4];    // 0x1a4
	float transparency;                     // 0x1a8
	float reflectance;                      // 0x1ac
	bool locked;                            // 0x1b0
	undefined m_unk0x1b1[0x1b4 - 0x1b1];    // 0x1b1
	Surfaces surfaces;                      // 0x1b4
	float renderImportance;                 // 0x1e4
	boost::scoped_ptr<Primitive> primitive; // 0x1e8
	World* myWorld;                         // 0x1ec
	float alphaModifier;                    // 0x1f0
	undefined m_unk0x1f4[0x2ac - 0x1f4];    // 0x1f4
};

DECOMP_SIZE_ASSERT(PartInstance, 0x2ac)

} // namespace RBX

#endif // V8DATAMODEL_PARTINSTANCE_H
