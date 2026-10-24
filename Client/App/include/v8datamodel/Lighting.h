#ifndef V8DATAMODEL_LIGHTING_H
#define V8DATAMODEL_LIGHTING_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <G3D/Color3.h>
#include <G3D/Color4.h>
#include <GLG3D/LightingParameters.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/shared_ptr.hpp>

namespace RBX {

class Sky;

extern char sLighting[];
// SIZE 0x230
class Lighting : public DescribedCreatable<Lighting, Instance, sLighting>, public Service
{
private:
	G3D::LightingParameters skyParameters;      // 0x0f8
	G3D::Color3 ambientTop;                     // 0x1f0
	G3D::Color3 ambientBottom;                  // 0x1fc
	bool hasSky;                                // 0x208
	G3D::Color4 clearColor;                     // 0x20c
	boost::posix_time::time_duration timeOfDay; // 0x220

public:
	Lighting();

	void replaceSky(Sky* sky);

	boost::shared_ptr<Sky> sky; // 0x228
};

DECOMP_SIZE_ASSERT(Lighting, 0x230)

// SYNTHETIC: WEBSERVICE 0x100463f0
// RBX::Lighting::~Lighting

} // namespace RBX

#endif // V8DATAMODEL_LIGHTING_H
