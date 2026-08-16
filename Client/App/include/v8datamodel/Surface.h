#ifndef V8DATAMODEL_SURFACE_H
#define V8DATAMODEL_SURFACE_H

#include "decomp.h"
#include "util/NormalId.h"
#include "v8world/SurfaceData.h"

namespace RBX {

class PartInstance;

// SIZE 0x8
class Surface
{
private:
	PartInstance* partInstance; // 0x00
	NormalId surfId;            // 0x04

public:
	Surface(PartInstance* partInstance, NormalId surfId);

	SurfaceType getSurfaceType() const;
	void setSurfaceType(SurfaceType type);

	Controller::InputType getInput() const;
	void setSurfaceInput(Controller::InputType value);

	float getParamA() const;
	void setParamA(float value);

	float getParamB() const;
	void setParamB(float value);

	bool isControllable() const;
};

DECOMP_SIZE_ASSERT(Surface, 0x8)

} // namespace RBX

#endif // V8DATAMODEL_SURFACE_H
