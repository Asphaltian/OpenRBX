#ifndef V8DATAMODEL_SURFACE_H
#define V8DATAMODEL_SURFACE_H

#include "placeholder.h"
#include "util/NormalId.h"
#include "v8world/SurfaceData.h"

class XmlElement;

namespace RBX {

class IReferenceBinder;
class PartInstance;

class Surface
{
private:
	PartInstance* partInstance;
	NormalId surfId;

public:
	void flat();

	Surface(PartInstance* partInstance, NormalId surfId);

	PartInstance* getPartInstance() const { return partInstance; }
	NormalId getNormalId() const { return surfId; }

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

} // namespace RBX

#endif // V8DATAMODEL_SURFACE_H
