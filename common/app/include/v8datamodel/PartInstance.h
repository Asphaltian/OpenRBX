#ifndef V8DATAMODEL_PARTINSTANCE_H
#define V8DATAMODEL_PARTINSTANCE_H

#include "decomp.h"
#include "util/Extents.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/Surfaces.h"
#include "v8tree/Instance.h"
#include "v8world/Primitive.h"

#include <G3D/Color3.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace RBX {

using boost::shared_ptr;

class World;

// SIZE 0x2ac
class PartInstance : public Instance
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

	static const Reflection::PropDescriptor<PartInstance, float> prop_RenderImportance;
	static const Reflection::PropDescriptor<PartInstance, float> prop_Transparency;
	static const Reflection::PropDescriptor<PartInstance, float> prop_Reflectance;
	static const Reflection::PropDescriptor<PartInstance, bool> prop_Locked;
	static const Reflection::PropDescriptor<PartInstance, G3D::Color3> prop_Color;
	static const Reflection::PropDescriptor<PartInstance, BrickColor> prop_BrickColor;
	static const Reflection::PropDescriptor<PartInstance, bool> prop_CanCollide;
	static const Reflection::PropDescriptor<PartInstance, bool> prop_Anchored;

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

	void setRenderImportance(float value);
	void setPartLocked(bool value);
	void setTransparency(float value);
	void setAlphaModifier(float value);
	void setReflectance(float value);
	void setFormFactorXml(FormFactor value);
	void setDragging(bool value);
	void setCanCollide(bool value);
	void setAnchored(bool value);
	void setColor(BrickColor value);

private:
	undefined m_unk0x0f8[0x1a0 - 0x0f8];    // 0x0f8
	FormFactor formFactor;                  // 0x1a0
	BrickColor color;                       // 0x1a4
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
