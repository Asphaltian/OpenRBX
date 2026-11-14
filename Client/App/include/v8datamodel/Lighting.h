#ifndef V8DATAMODEL_LIGHTING_H
#define V8DATAMODEL_LIGHTING_H

#include "reflection/signal.h"
#include "placeholder.h"
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
class Lighting : public DescribedCreatable<Lighting, Instance, sLighting>, public Service
{
private:
	G3D::LightingParameters skyParameters;
	G3D::Color3 ambientTop;
	G3D::Color3 ambientBottom;
	bool hasSky;
	G3D::Color4 clearColor;
	boost::posix_time::time_duration timeOfDay;

public:
	Lighting();

	void replaceSky(Sky* sky);

	static Reflection::SignalDesc<Lighting, void(bool)> event_LightingChanged;

	bool isSkySuppressed() const { return !hasSky; }
	void suppressSky(const bool value) { hasSky = !value; }

	boost::shared_ptr<Sky> sky;

	const G3D::LightingParameters& getSkyParameters() const { return skyParameters; }
	G3D::Color4 getClearColor() const { return clearColor; }
	void setClearColor(G3D::Color4 value);

	G3D::Color3 getLightColor() const { return skyParameters.lightColor; }
	void setLightColor(G3D::Color3 value);

	G3D::Color3 getAmbientTop() const { return ambientTop; }
	void setAmbientTop(G3D::Color3 value);

	G3D::Color3 getAmbientBottom() const { return ambientBottom; }
	void setAmbientBottom(G3D::Color3 value);

	std::string getTimeStr() const;
	void setTimeStr(const std::string& value);
	void setTime(const boost::posix_time::time_duration& value);
	double getGameTime() const;
	double getMinutesAfterMidnight();
	void setMinutesAfterMidnight(double value);

	float getMoonPhase() { return skyParameters.moonPhase; }

	G3D::Vector3 getMoonPosition();
	G3D::Vector3 getSunPosition();

	float getGeographicLatitude() const { return skyParameters.geoLatitude; }
	void setGeographicLatitude(float value);

	G3D::Color3 getClearColor3() const { return G3D::Color3(clearColor.r, clearColor.g, clearColor.b); }

	void setClearColor3(G3D::Color3 value) { setClearColor(G3D::Color4(value, 1.0f)); }

protected:
	virtual void onChildAdded(Instance* child);
	virtual void onChildRemoving(Instance* child);
	virtual void onChildChanged(Instance* instance, const PropertyChanged& event);
	virtual bool askAddChild(const Instance* instance) const;

private:
	void fireLightingChanged(bool skyChanged);
};

} // namespace RBX

#endif // V8DATAMODEL_LIGHTING_H
