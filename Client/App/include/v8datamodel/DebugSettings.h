#ifndef V8DATAMODEL_DEBUGSETTINGS_H
#define V8DATAMODEL_DEBUGSETTINGS_H

#include "decomp.h"
#include "util/Utilities.h"
#include "v8datamodel/GlobalSettings.h"

namespace RBX {

extern const char sDebugSettings[];

// SIZE 0x100
class DebugSettings : public GlobalSettingsItem<DebugSettings, sDebugSettings>
{
public:
	DebugSettings();

	// SYNTHETIC: WEBSERVICE 0x100b51d0
	// RBX::DebugSettings::`scalar deleting destructor'

	enum ErrorReporting
	{
		DontReport = 0,
		Prompt = 1,
		Report = 2
	};

	static Reflection::BoundProp<bool, 1> prop_stackTracingEnabled;
	static Reflection::BoundProp<bool, 1> prop_ioEnabled;

	bool getStackTracingEnabled() const { return stackTracingEnabled; }
	bool getioEnabled() const { return ioEnabled; }

	bool getShowAnchoredParts() const;
	void setShowAnchoredParts(bool value);
	bool getShowAggregation() const;
	void setShowAggregation(bool);
	bool getShowUnalignedParts() const;
	void setShowUnalignedParts(bool value);
	bool getHighlightSleepParts() const;
	void setHighlightSleepParts(bool);
	bool getHighlightAwakeParts() const;
	void setHighlightAwakeParts(bool);
	bool getShowPartCoordinateFrames() const;
	void setShowPartCoordinateFrames(bool value);
	bool getShowModelCoordinateFrames() const;
	void setShowModelCoordinateFrames(bool value);
	bool getShowWorldCoordinateFrames() const;
	void setShowWorldCoordinateFrames(bool value);
	bool getDisableSleep() const;
	void setDisableSleep(bool);
	bool getDisableEnvironmentalThrottle() const;
	void setDisableEnvironmentalThrottle(bool value);
	bool getUseNewGraphics() const;
	void setUseNewGraphics(bool);
	bool getShowSpanningTree() const;
	void setShowSpanningTree(bool value);
	bool getValidatingDebug() const;
	void setValidatingDebug(bool value);
	Debugable::AssertAction getAssertAction() const;
	void setAssertAction(Debugable::AssertAction value);
	ErrorReporting getErrorReporting() const;
	void setErrorReporting(ErrorReporting value);

	float shaderModel() const;
	int videoMemory() const;
	int cpuSpeed() const;
	std::string osVer() const;
	int osPlatformId() const;
	std::string glVendor() const;
	std::string gfxcard() const;
	std::string cpu() const;
	int ram() const;
	std::string resolution() const;

private:
	bool stackTracingEnabled; // 0x0f8
	bool ioEnabled;           // 0x0f9
	bool validatingDebug;     // 0x0fa

public:
	bool soundWarnings;            // 0x0fb
	ErrorReporting errorReporting; // 0x0fc
};

DECOMP_SIZE_ASSERT(DebugSettings, 0x100)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021b850
// `dynamic initializer for 'prop_shaderModel''
// SYNTHETIC: WEBSERVICE 0x1021b880
// `dynamic initializer for 'prop_videoMemory''
// SYNTHETIC: WEBSERVICE 0x1021b8b0
// `dynamic initializer for 'prop_cpuSpeed''
// SYNTHETIC: WEBSERVICE 0x1021b8e0
// `dynamic initializer for 'prop_osPlatformId''
// SYNTHETIC: WEBSERVICE 0x1021b910
// `dynamic initializer for 'prop_osVer''
// SYNTHETIC: WEBSERVICE 0x1021b940
// `dynamic initializer for 'prop_glVendor''
// SYNTHETIC: WEBSERVICE 0x1021b970
// `dynamic initializer for 'prop_gfxcard''
// SYNTHETIC: WEBSERVICE 0x1021b9a0
// `dynamic initializer for 'prop_cpu''
// SYNTHETIC: WEBSERVICE 0x1021b9d0
// `dynamic initializer for 'prop_ram''
// SYNTHETIC: WEBSERVICE 0x1021ba00
// `dynamic initializer for 'prop_resolution''
// SYNTHETIC: WEBSERVICE 0x1021ba30
// `dynamic initializer for 'prop_SoundWarnings''
// SYNTHETIC: WEBSERVICE 0x1021ba60
// `dynamic initializer for 'RBX::DebugSettings::prop_stackTracingEnabled''
// SYNTHETIC: WEBSERVICE 0x1021ba90
// `dynamic initializer for 'RBX::DebugSettings::prop_ioEnabled''
// SYNTHETIC: WEBSERVICE 0x1021bac0
// `dynamic initializer for 'prop_AnchoredParts''
// SYNTHETIC: WEBSERVICE 0x1021bb00
// `dynamic initializer for 'prop_PartCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x1021bb40
// `dynamic initializer for 'prop_UnalignedParts''
// SYNTHETIC: WEBSERVICE 0x1021bb80
// `dynamic initializer for 'prop_ModelCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x1021bbc0
// `dynamic initializer for 'prop_WorldCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x1021bc00
// `dynamic initializer for 'prop_DisableEnvironmentalThrottle''
// SYNTHETIC: WEBSERVICE 0x1021bc40
// `dynamic initializer for 'prop_ValidatingDebug''
// SYNTHETIC: WEBSERVICE 0x1021bc80
// `dynamic initializer for 'prop_assertAction''
// SYNTHETIC: WEBSERVICE 0x1021bcc0
// `dynamic initializer for 'prop_errorReporting''
// SYNTHETIC: WEBSERVICE 0x102236a0
// `dynamic atexit destructor for 'prop_UnalignedParts''
// SYNTHETIC: WEBSERVICE 0x102236c0
// `dynamic atexit destructor for 'prop_ModelCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x102236e0
// `dynamic atexit destructor for 'prop_WorldCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x10223700
// `dynamic atexit destructor for 'prop_DisableEnvironmentalThrottle''
// SYNTHETIC: WEBSERVICE 0x10223720
// `dynamic atexit destructor for 'prop_ValidatingDebug''
// SYNTHETIC: WEBSERVICE 0x10223740
// `dynamic atexit destructor for 'prop_osPlatformId''
// SYNTHETIC: WEBSERVICE 0x10223760
// `dynamic atexit destructor for 'prop_gfxcard''
// SYNTHETIC: WEBSERVICE 0x10223780
// `dynamic atexit destructor for 'prop_cpu''
// SYNTHETIC: WEBSERVICE 0x102237a0
// `dynamic atexit destructor for 'prop_ram''
// SYNTHETIC: WEBSERVICE 0x102237c0
// `dynamic atexit destructor for 'prop_resolution''
// SYNTHETIC: WEBSERVICE 0x102237e0
// `dynamic atexit destructor for 'RBX::DebugSettings::prop_ioEnabled''
// SYNTHETIC: WEBSERVICE 0x10223800
// `dynamic atexit destructor for 'prop_PartCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x10223820
// `dynamic atexit destructor for 'prop_AnchoredParts''
// SYNTHETIC: WEBSERVICE 0x10223840
// `dynamic atexit destructor for 'prop_errorReporting''
// SYNTHETIC: WEBSERVICE 0x10223860
// `dynamic atexit destructor for 'prop_assertAction''
// SYNTHETIC: WEBSERVICE 0x10223880
// `dynamic atexit destructor for 'prop_cpuSpeed''
// SYNTHETIC: WEBSERVICE 0x102238a0
// `dynamic atexit destructor for 'prop_videoMemory''
// SYNTHETIC: WEBSERVICE 0x102238c0
// `dynamic atexit destructor for 'prop_shaderModel''
// SYNTHETIC: WEBSERVICE 0x102238e0
// `dynamic atexit destructor for 'prop_glVendor''
// SYNTHETIC: WEBSERVICE 0x10223900
// `dynamic atexit destructor for 'prop_osVer''
// SYNTHETIC: WEBSERVICE 0x10223920
// `dynamic atexit destructor for 'RBX::DebugSettings::prop_stackTracingEnabled''
// SYNTHETIC: WEBSERVICE 0x10223940
// `dynamic atexit destructor for 'prop_SoundWarnings''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_DEBUGSETTINGS_H
