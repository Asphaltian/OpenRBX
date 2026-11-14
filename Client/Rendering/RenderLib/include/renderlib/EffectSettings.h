#ifndef RENDERLIB_EFFECTSETTINGS_H
#define RENDERLIB_EFFECTSETTINGS_H

#include "placeholder.h"
#include "renderlib/DepthBlur.h"

#include <G3D/ReferenceCount.h>
#include <GLG3D/LightingParameters.h>
#include <GLG3D/ToneMap.h>

namespace RBX {
namespace Render {

class EffectSettings
{
private:
	bool _stencilShadows;
	bool _alphaBlendShadowLights;
	bool _useAllLightsInUnshadowedPass;
	bool _skyBox;
	bool _toneMap;
	bool _hemisphereLighting;
	bool _depthBlur;
	float _farCullZ;
	float _groundLOD;
	float _LODShift;

public:
	static G3D::ToneMap* getToneMap();

	static DepthBlur* getDepthBlur();

	G3D::ReferenceCountedPointer<G3D::Lighting> update(
		float shadingDetail,
		float meshDetail,
		bool shadows,
		float cameraDistanceFromCharacters,
		const G3D::ReferenceCountedPointer<G3D::Lighting>& desiredLighting,
		const G3D::LightingParameters& desiredSkyParameters,
		G3D::LightingParameters& outSkyParameters
	);

	bool stencilShadows() const { return _stencilShadows; }

	bool hemisphereLighting() const { return _hemisphereLighting; }

	bool skyBox() const { return _skyBox; }

	float farCullZ() const { return _farCullZ; }

	bool alphaBlendShadowLights() const { return _alphaBlendShadowLights; }

	bool useAllLightsInUnshadowedPass() const { return _useAllLightsInUnshadowedPass; }

	float groundLOD() const { return _groundLOD; }

	float LODShift() const { return _LODShift; }

	bool applyToneMap() const { return _toneMap; }

	bool applyDepthBlur() const { return _depthBlur; }
};

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_EFFECTSETTINGS_H
