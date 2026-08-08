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
	enum ErrorReporting
	{
		DontReport = 0,
		Prompt = 1,
		Report = 2
	};

	bool getStackTracingEnabled() const { return stackTracingEnabled; }
	bool getioEnabled() const { return ioEnabled; }

	bool getShowAnchoredParts() const;
	void setShowAnchoredParts(bool value);
	bool getShowUnalignedParts() const;
	void setShowUnalignedParts(bool value);
	bool getShowPartCoordinateFrames() const;
	void setShowPartCoordinateFrames(bool value);
	bool getShowModelCoordinateFrames() const;
	void setShowModelCoordinateFrames(bool value);
	bool getShowWorldCoordinateFrames() const;
	void setShowWorldCoordinateFrames(bool value);
	bool getDisableEnvironmentalThrottle() const;
	void setDisableEnvironmentalThrottle(bool value);
	bool getShowSpanningTree() const;
	void setShowSpanningTree(bool value);
	bool getValidatingDebug() const;
	void setValidatingDebug(bool value);
	Debugable::AssertAction getAssertAction() const;
	void setAssertAction(Debugable::AssertAction value);
	ErrorReporting getErrorReporting() const;
	void setErrorReporting(ErrorReporting value);

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
// SYNTHETIC: WEBSERVICE 0x1021bac0
// RBX::`dynamic initializer for 'prop_AnchoredParts''
// SYNTHETIC: WEBSERVICE 0x1021bb00
// RBX::`dynamic initializer for 'prop_PartCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x1021bb80
// RBX::`dynamic initializer for 'prop_ModelCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x1021bbc0
// RBX::`dynamic initializer for 'prop_WorldCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x1021bc00
// RBX::`dynamic initializer for 'prop_DisableEnvironmentalThrottle''
// SYNTHETIC: WEBSERVICE 0x1021bc40
// RBX::`dynamic initializer for 'prop_ValidatingDebug''
// SYNTHETIC: WEBSERVICE 0x1021bc80
// RBX::`dynamic initializer for 'prop_assertAction''
// SYNTHETIC: WEBSERVICE 0x1021bcc0
// RBX::`dynamic initializer for 'prop_errorReporting''
// SYNTHETIC: WEBSERVICE 0x102236c0
// RBX::`dynamic atexit destructor for 'prop_ModelCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x102236e0
// RBX::`dynamic atexit destructor for 'prop_WorldCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x10223700
// RBX::`dynamic atexit destructor for 'prop_DisableEnvironmentalThrottle''
// SYNTHETIC: WEBSERVICE 0x10223720
// RBX::`dynamic atexit destructor for 'prop_ValidatingDebug''
// SYNTHETIC: WEBSERVICE 0x10223800
// RBX::`dynamic atexit destructor for 'prop_PartCoordinateFrames''
// SYNTHETIC: WEBSERVICE 0x10223820
// RBX::`dynamic atexit destructor for 'prop_AnchoredParts''
// SYNTHETIC: WEBSERVICE 0x10223840
// RBX::`dynamic atexit destructor for 'prop_errorReporting''
// SYNTHETIC: WEBSERVICE 0x10223860
// RBX::`dynamic atexit destructor for 'prop_assertAction''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_DEBUGSETTINGS_H
