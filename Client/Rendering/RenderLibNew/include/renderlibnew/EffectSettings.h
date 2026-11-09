#ifndef RENDERLIBNEW_EFFECTSETTINGS_H
#define RENDERLIBNEW_EFFECTSETTINGS_H

#include "decomp.h"

#include <G3D/ReferenceCount.h>
#include <GLG3D/LightingParameters.h>
#include <GLG3D/ToneMap.h>

namespace RBX {
namespace RenderNew {

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
	// SYNTHETIC: ROBLOX 0x007f1b10
	// `RBX::RenderNew::EffectSettings::getToneMap'::`2'::`dynamic atexit destructor for 'toneMap''
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
};

DECOMP_SIZE_ASSERT(EffectSettings, 0x14)

} // namespace RenderNew
} // namespace RBX

#endif // RENDERLIBNEW_EFFECTSETTINGS_H
