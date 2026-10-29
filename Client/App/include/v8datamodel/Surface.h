#ifndef V8DATAMODEL_SURFACE_H
#define V8DATAMODEL_SURFACE_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/SurfaceData.h"

class XmlElement;

namespace RBX {

class IReferenceBinder;
class PartInstance;

// SIZE 0x8
class Surface
{
private:
	PartInstance* partInstance; // 0x00
	NormalId surfId;            // 0x04

public:
	void flat();

	Surface(PartInstance* partInstance, NormalId surfId);

	PartInstance* getPartInstance() const;
	NormalId getNormalId() const;

	bool isControllable() const;

	SurfaceType getSurfaceType() const;
	void setSurfaceType(SurfaceType type);

	Controller::InputType getInput() const;
	void setSurfaceInput(Controller::InputType value);

	float getParamA() const;
	float getParamB() const;
	void setParamA(float value);
	void setParamB(float value);

	void toggleSurfaceType();

	void writeValue(XmlElement*) const;
	void readValue(const XmlElement* element, IReferenceBinder& binder);
};

DECOMP_SIZE_ASSERT(Surface, 0x8)

} // namespace RBX

#endif // V8DATAMODEL_SURFACE_H
