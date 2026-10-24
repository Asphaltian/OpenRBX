#include "v8datamodel/Lighting.h"

#include "v8datamodel/Sky.h"

#include <boost/date_time/time_parsing.hpp>

namespace RBX {

char sLighting[] = "Lighting";

// FUNCTION: WEBSERVICE 0x100438e0
void Lighting::replaceSky(Sky* sky)
{
	while (Sky* oldSky = findFirstChildOfType<Sky>()) {
		oldSky->setParent(NULL);
	}

	sky->setParent(this);
}

// FUNCTION: WEBSERVICE 0x10046190
Lighting::Lighting()
	: ambientTop(209 / 255.0f, 208 / 255.0f, 217 / 255.0f), ambientBottom(122 / 255.0f, 134 / 255.0f, 120 / 255.0f),
	  hasSky(true), clearColor(G3D::Color3::white()),
	  timeOfDay(boost::date_time::parse_delimited_time_duration<boost::posix_time::time_duration>("14:00:00"))
{
	skyParameters.lightColor = G3D::Color3(152 / 255.0f, 137 / 255.0f, 102 / 255.0f);
	skyParameters.setTime(timeOfDay.total_seconds());
	setName("Lighting");
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting>;
template class RBX::Reflection::
	Described<RBX::Lighting, RBX::sLighting, RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting> >;
template class RBX::DescribedCreatable<RBX::Lighting, RBX::Instance, RBX::sLighting>;
