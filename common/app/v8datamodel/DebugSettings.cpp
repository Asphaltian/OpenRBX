#include "v8datamodel/DebugSettings.h"

#include "reflection/Property.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8world/World.h"

namespace RBX {

using namespace Reflection;

static PropDescriptor<DebugSettings, bool> prop_ValidatingDebug;
static EnumPropDescriptor<DebugSettings, Debugable::AssertAction> prop_assertAction;
static EnumPropDescriptor<DebugSettings, DebugSettings::ErrorReporting> prop_errorReporting;
static PropDescriptor<DebugSettings, bool> prop_AnchoredParts;
static PropDescriptor<DebugSettings, bool> prop_PartCoordinateFrames;
static PropDescriptor<DebugSettings, bool> prop_ModelCoordinateFrames;
static PropDescriptor<DebugSettings, bool> prop_WorldCoordinateFrames;
static PropDescriptor<DebugSettings, bool> prop_DisableEnvironmentalThrottle;

// FUNCTION: WEBSERVICE 0x100b2d30
bool DebugSettings::getValidatingDebug() const
{
	return Debugable::validatingDebug;
}

// FUNCTION: WEBSERVICE 0x100b2d40
Debugable::AssertAction DebugSettings::getAssertAction() const
{
	return Debugable::assertAction;
}

// FUNCTION: WEBSERVICE 0x100b2d50
DebugSettings::ErrorReporting DebugSettings::getErrorReporting() const
{
	return errorReporting;
}

// FUNCTION: WEBSERVICE 0x100b2d60
bool DebugSettings::getShowAnchoredParts() const
{
	return PartInstance::showAnchoredParts;
}

// FUNCTION: WEBSERVICE 0x100b2d70
bool DebugSettings::getShowUnalignedParts() const
{
	return PartInstance::showUnalignedParts;
}

// FUNCTION: WEBSERVICE 0x100b2d80
bool DebugSettings::getShowPartCoordinateFrames() const
{
	return PartInstance::showPartCoord;
}

// FUNCTION: WEBSERVICE 0x100b2d90
bool DebugSettings::getShowModelCoordinateFrames() const
{
	return ModelInstance::showModelCoord;
}

// FUNCTION: WEBSERVICE 0x100b2da0
bool DebugSettings::getShowWorldCoordinateFrames() const
{
	return Workspace::showWorldCoord;
}

// FUNCTION: WEBSERVICE 0x100b2db0
bool DebugSettings::getDisableEnvironmentalThrottle() const
{
	return World::disableEnvironmentalThrottle;
}

// FUNCTION: WEBSERVICE 0x100b51f0
void DebugSettings::setShowAnchoredParts(bool value)
{
	if (value != PartInstance::showAnchoredParts) {
		PartInstance::showAnchoredParts = value;
		raisePropertyChanged(prop_AnchoredParts);
	}
}

// FUNCTION: WEBSERVICE 0x100b5220
void DebugSettings::setShowUnalignedParts(bool value)
{
	if (value != PartInstance::showUnalignedParts) {
		PartInstance::showUnalignedParts = value;
		raisePropertyChanged(prop_PartCoordinateFrames);
	}
}

// FUNCTION: WEBSERVICE 0x100b5250
void DebugSettings::setShowPartCoordinateFrames(bool value)
{
	if (value != PartInstance::showPartCoord) {
		PartInstance::showPartCoord = value;
		raisePropertyChanged(prop_PartCoordinateFrames);
	}
}

// FUNCTION: WEBSERVICE 0x100b5280
void DebugSettings::setShowModelCoordinateFrames(bool value)
{
	if (value != ModelInstance::showModelCoord) {
		ModelInstance::showModelCoord = value;
		raisePropertyChanged(prop_ModelCoordinateFrames);
	}
}

// FUNCTION: WEBSERVICE 0x100b52b0
void DebugSettings::setShowWorldCoordinateFrames(bool value)
{
	if (value != Workspace::showWorldCoord) {
		Workspace::showWorldCoord = value;
		raisePropertyChanged(prop_WorldCoordinateFrames);
	}
}

// FUNCTION: WEBSERVICE 0x100b52e0
void DebugSettings::setDisableEnvironmentalThrottle(bool value)
{
	if (value != World::disableEnvironmentalThrottle) {
		World::disableEnvironmentalThrottle = value;
		raisePropertyChanged(prop_DisableEnvironmentalThrottle);
	}
}

// FUNCTION: WEBSERVICE 0x100b5310
void DebugSettings::setValidatingDebug(bool value)
{
	if (value != Debugable::validatingDebug) {
		Debugable::validatingDebug = value;
		raisePropertyChanged(prop_ValidatingDebug);
	}
}

// FUNCTION: WEBSERVICE 0x100b5340
void DebugSettings::setAssertAction(Debugable::AssertAction value)
{
	if (value != Debugable::assertAction) {
		Debugable::assertAction = value;
		raisePropertyChanged(prop_assertAction);
	}
}

// FUNCTION: WEBSERVICE 0x100b5370
void DebugSettings::setErrorReporting(ErrorReporting value)
{
	if (value != errorReporting) {
		errorReporting = value;
		raisePropertyChanged(prop_errorReporting);
	}
}

} // namespace RBX
