#ifndef RENDERLIB_EFFECTSETTINGS_H
#define RENDERLIB_EFFECTSETTINGS_H

#include "decomp.h"
#include "renderlib/DepthBlur.h"

#include <G3D/ReferenceCount.h>
#include <GLG3D/LightingParameters.h>
#include <GLG3D/ToneMap.h>

namespace RBX {
namespace Render {

// SIZE 0x14
class EffectSettings
{
private:
	bool _stencilShadows;               // 0x00
	bool _alphaBlendShadowLights;       // 0x01
	bool _useAllLightsInUnshadowedPass; // 0x02
	bool _skyBox;                       // 0x03
	bool _toneMap;                      // 0x04
	bool _hemisphereLighting;           // 0x05
	bool _depthBlur;                    // 0x06
	float _farCullZ;                    // 0x08
	float _groundLOD;                   // 0x0c
	float _LODShift;                    // 0x10

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

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x102264c0
	// `RBX::Render::EffectSettings::getToneMap'::`2'::`dynamic atexit destructor for 'toneMap''
	// SYNTHETIC: WEBSERVICE 0x102264e0
	// `RBX::Render::EffectSettings::getDepthBlur'::`2'::`dynamic atexit destructor for 'depthBlur''
	// clang-format on

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

DECOMP_SIZE_ASSERT(EffectSettings, 0x14)

} // namespace Render
} // namespace RBX

#endif // RENDERLIB_EFFECTSETTINGS_H
