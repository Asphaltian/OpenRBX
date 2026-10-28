#ifndef V8DATAMODEL_LIGHTING_H
#define V8DATAMODEL_LIGHTING_H

#include "decomp.h"
#include "reflection/signal.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <G3D/Color3.h>
#include <G3D/Color4.h>
#include <GLG3D/LightingParameters.h>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

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

	static Reflection::SignalDesc<Lighting, void(bool)> event_LightingChanged;

	bool isSkySuppressed() const { return !hasSky; }
	void suppressSky(const bool value) { hasSky = !value; }

	boost::shared_ptr<Sky> sky; // 0x228

	const G3D::LightingParameters& getSkyParameters() const { return skyParameters; }
	G3D::Color4 getClearColor() const { return clearColor; }
	void setClearColor(G3D::Color4 value);

	// FUNCTION: WEBSERVICE 0x10042590
	G3D::Color3 getLightColor() const { return skyParameters.lightColor; }
	void setLightColor(G3D::Color3 value);

	// FUNCTION: WEBSERVICE 0x100425c0
	G3D::Color3 getAmbientTop() const { return ambientTop; }
	void setAmbientTop(G3D::Color3 value);

	// FUNCTION: WEBSERVICE 0x100425f0
	G3D::Color3 getAmbientBottom() const { return ambientBottom; }
	void setAmbientBottom(G3D::Color3 value);

	std::string getTimeStr() const;
	void setTimeStr(const std::string& value);
	void setTime(const boost::posix_time::time_duration& value);
	double getGameTime() const;
	double getMinutesAfterMidnight();
	void setMinutesAfterMidnight(double value);

	// FUNCTION: WEBSERVICE 0x10042620
	float getMoonPhase() { return skyParameters.moonPhase; }

	G3D::Vector3 getMoonPosition();
	G3D::Vector3 getSunPosition();

	// FUNCTION: WEBSERVICE 0x10042630
	float getGeographicLatitude() const { return skyParameters.geoLatitude; }
	void setGeographicLatitude(float value);

	// FUNCTION: WEBSERVICE 0x10042640
	G3D::Color3 getClearColor3() const { return G3D::Color3(clearColor.r, clearColor.g, clearColor.b); }

	// FUNCTION: WEBSERVICE 0x10005d40
	void setClearColor3(G3D::Color3 value) { setClearColor(G3D::Color4(value, 1.0f)); }

protected:
	virtual void onChildAdded(Instance* child);
	virtual void onChildRemoving(Instance* child);
	virtual void onChildChanged(Instance* instance, const PropertyChanged& event);
	virtual bool askAddChild(const Instance* instance) const;

private:
	void fireLightingChanged(bool skyChanged);
};

DECOMP_SIZE_ASSERT(Lighting, 0x230)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10218180
// `dynamic initializer for 'RBX::Lighting::event_LightingChanged''
// SYNTHETIC: WEBSERVICE 0x102181a0
// `dynamic initializer for 'desc_AmbientTop''
// SYNTHETIC: WEBSERVICE 0x102181e0
// `dynamic initializer for 'desc_AmbientBottom''
// SYNTHETIC: WEBSERVICE 0x10218220
// `dynamic initializer for 'desc_LightColor''
// SYNTHETIC: WEBSERVICE 0x10218260
// `dynamic initializer for 'desc_ClearColor''
// SYNTHETIC: WEBSERVICE 0x102182a0
// `dynamic initializer for 'prop_Time''
// SYNTHETIC: WEBSERVICE 0x102182e0
// `dynamic initializer for 'prop_GeographicLatitude''
// SYNTHETIC: WEBSERVICE 0x10218320
// `dynamic initializer for 'prop_GetMoonPhase''
// SYNTHETIC: WEBSERVICE 0x10218350
// `dynamic initializer for 'prop_GetMoonPosition''
// SYNTHETIC: WEBSERVICE 0x10218380
// `dynamic initializer for 'prop_GetSunPosition''
// SYNTHETIC: WEBSERVICE 0x102183b0
// `dynamic initializer for 'func_GetMinutesAfterMidnight''
// SYNTHETIC: WEBSERVICE 0x102183e0
// `dynamic initializer for 'func_SetMinutesAfterMidnight''
// SYNTHETIC: WEBSERVICE 0x10221510
// `dynamic atexit destructor for 'desc_LightColor''
// SYNTHETIC: WEBSERVICE 0x10221530
// `dynamic atexit destructor for 'desc_ClearColor''
// SYNTHETIC: WEBSERVICE 0x10221550
// `dynamic atexit destructor for 'desc_AmbientBottom''
// SYNTHETIC: WEBSERVICE 0x10221570
// `dynamic atexit destructor for 'desc_AmbientTop''
// SYNTHETIC: WEBSERVICE 0x10221590
// `dynamic atexit destructor for 'prop_GeographicLatitude''
// SYNTHETIC: WEBSERVICE 0x102215b0
// `dynamic atexit destructor for 'prop_Time''
// SYNTHETIC: WEBSERVICE 0x102215e0
// `dynamic atexit destructor for 'prop_GetSunPosition''
// SYNTHETIC: WEBSERVICE 0x102215f0
// `dynamic atexit destructor for 'prop_GetMoonPosition''
// SYNTHETIC: WEBSERVICE 0x10221600
// `dynamic atexit destructor for 'func_GetMinutesAfterMidnight''
// SYNTHETIC: WEBSERVICE 0x10221610
// `dynamic atexit destructor for 'func_SetMinutesAfterMidnight''
// SYNTHETIC: WEBSERVICE 0x10221620
// `dynamic atexit destructor for 'prop_GetMoonPhase''
// SYNTHETIC: WEBSERVICE 0x10221670
// `dynamic atexit destructor for 'RBX::Lighting::event_LightingChanged''
// clang-format on

// SYNTHETIC: WEBSERVICE 0x100463f0
// RBX::Lighting::~Lighting

} // namespace RBX

#endif // V8DATAMODEL_LIGHTING_H
