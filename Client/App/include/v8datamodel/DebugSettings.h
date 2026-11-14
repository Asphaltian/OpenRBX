#ifndef V8DATAMODEL_DEBUGSETTINGS_H
#define V8DATAMODEL_DEBUGSETTINGS_H

#include "placeholder.h"
#include "util/Utilities.h"
#include "v8datamodel/GlobalSettings.h"

namespace RBX {

extern const char sDebugSettings[];

class DebugSettings : public GlobalSettingsItem<DebugSettings, sDebugSettings>
{
public:
	DebugSettings();

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
	ErrorReporting getErrorReporting() const { return errorReporting; }

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
	bool stackTracingEnabled;
	bool ioEnabled;
	bool validatingDebug;

public:
	bool soundWarnings;
	ErrorReporting errorReporting;
};

} // namespace RBX

#endif // V8DATAMODEL_DEBUGSETTINGS_H
