#include "v8datamodel/DebugSettings.h"

#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8datamodel/Workspace.h"
#include "v8world/World.h"

namespace RBX {

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

} // namespace RBX
