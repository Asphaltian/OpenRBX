#ifndef UTIL_NAME_H
#define UTIL_NAME_H

#include <boost/noncopyable.hpp>
#include <boost/thread/once.hpp>
#include <map>
#include <string>

namespace RBX {

// SIZE 0x20
class Name : public boost::noncopyable
{
public:
	static const Name& declare(const char* name, int dictionaryIndex);
	static const Name& lookup(const char* name);
	static const Name& lookup(const std::string& name);
	static const Name& getNullName();

private:
	class NamMap;

	static NamMap& namMap();
	static std::map<int, Name*>& dictionary();

	Name(const char* name, int dictionaryIndex);

public:
	template <const char* name>
	static const Name& declare()
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;

		boost::call_once(&callDoDeclare<name>, flag);

		return doDeclare<name>();
	}

	bool empty() const;

private:
	// TEMPLATE: WEBSERVICE 0x1000b0b0
	// RBX::Name::doDeclare<&sThumbnailGenerator>
	// TEMPLATE: WEBSERVICE 0x1000b140
	// RBX::Name::doDeclare<&RBX::sLighting>
	// TEMPLATE: WEBSERVICE 0x1000b1d0
	// RBX::Name::doDeclare<&RBX::sRunService>
	// TEMPLATE: WEBSERVICE 0x1000b260
	// RBX::Name::doDeclare<&RBX::sModel>
	// TEMPLATE: WEBSERVICE 0x1000b2f0
	// RBX::Name::doDeclare<&RBX::sSelection>
	// TEMPLATE: WEBSERVICE 0x1000b380
	// RBX::Name::doDeclare<&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x1000b410
	// RBX::Name::doDeclare<&RBX::sScript>
	// TEMPLATE: WEBSERVICE 0x1000b4a0
	// RBX::Name::doDeclare<&RBX::sLocalScript>
	// TEMPLATE: WEBSERVICE 0x1000b530
	// RBX::Name::doDeclare<&RBX::sWorkspace>
	// TEMPLATE: WEBSERVICE 0x1001c980
	// RBX::Name::doDeclare<&RBX::Soundscape::sSoundService>
	// TEMPLATE: WEBSERVICE 0x1001ca10
	// RBX::Name::doDeclare<&RBX::Soundscape::sSoundChannel>
	// TEMPLATE: WEBSERVICE 0x10059e20
	// RBX::Name::doDeclare<&RBX::sPart>
	// TEMPLATE: WEBSERVICE 0x10059e90
	// RBX::Name::doDeclare<&RBX::sHumanoid>
	// TEMPLATE: WEBSERVICE 0x1005f300
	// RBX::Name::doDeclare<&RBX::Stats::sStatsItem>
	// TEMPLATE: WEBSERVICE 0x1005f370
	// RBX::Name::doDeclare<&RBX::Stats::sStats>
	// TEMPLATE: WEBSERVICE 0x1005f3e0
	// RBX::Name::doDeclare<&RBX::sDebugSettings>
	// TEMPLATE: WEBSERVICE 0x100689c0
	// RBX::Name::doDeclare<&RBX::sTeam>
	// TEMPLATE: WEBSERVICE 0x10068a30
	// RBX::Name::doDeclare<&RBX::Network::sPlayer>
	// TEMPLATE: WEBSERVICE 0x1006a1e0
	// RBX::Name::doDeclare<&RBX::sControllerService>
	// TEMPLATE: WEBSERVICE 0x1006a250
	// RBX::Name::doDeclare<&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x1006a2c0
	// RBX::Name::doDeclare<&RBX::sHopperBin>
	// TEMPLATE: WEBSERVICE 0x1006a330
	// RBX::Name::doDeclare<&RBX::sStarterPackService>
	// TEMPLATE: WEBSERVICE 0x1006a3a0
	// RBX::Name::doDeclare<&RBX::sLegacyHopperService>
	// TEMPLATE: WEBSERVICE 0x1006a410
	// RBX::Name::doDeclare<&RBX::sTool>
	// TEMPLATE: WEBSERVICE 0x1006a480
	// RBX::Name::doDeclare<&RBX::sFlag>
	// TEMPLATE: WEBSERVICE 0x1006a4f0
	// RBX::Name::doDeclare<&RBX::sArrowTool>
	// TEMPLATE: WEBSERVICE 0x10071bf0
	// RBX::Name::doDeclare<&RBX::sServiceProvider>
	// TEMPLATE: WEBSERVICE 0x10071cd0
	// RBX::Name::doDeclare<&RBX::sLocalBackpackItem>
	// TEMPLATE: WEBSERVICE 0x10071d40
	// RBX::Name::doDeclare<&RBX::sLocalBackpack>
	// TEMPLATE: WEBSERVICE 0x10071db0
	// RBX::Name::doDeclare<&RBX::sSnap>
	// TEMPLATE: WEBSERVICE 0x10071e20
	// RBX::Name::doDeclare<&RBX::sWeld>
	// TEMPLATE: WEBSERVICE 0x10071e90
	// RBX::Name::doDeclare<&RBX::sGlue>
	// TEMPLATE: WEBSERVICE 0x10071f00
	// RBX::Name::doDeclare<&RBX::sRotate>
	// TEMPLATE: WEBSERVICE 0x10071f70
	// RBX::Name::doDeclare<&RBX::sRotateP>
	// TEMPLATE: WEBSERVICE 0x10071fe0
	// RBX::Name::doDeclare<&RBX::sRotateV>
	// TEMPLATE: WEBSERVICE 0x10072050
	// RBX::Name::doDeclare<&RBX::sMotor>
	// TEMPLATE: WEBSERVICE 0x100720c0
	// RBX::Name::doDeclare<&RBX::sBackpack>
	// TEMPLATE: WEBSERVICE 0x10072130
	// RBX::Name::doDeclare<&RBX::sTeams>
	// TEMPLATE: WEBSERVICE 0x100721a0
	// RBX::Name::doDeclare<&RBX::sAccoutrement>
	// TEMPLATE: WEBSERVICE 0x10072210
	// RBX::Name::doDeclare<&RBX::sHat>
	// TEMPLATE: WEBSERVICE 0x10072280
	// RBX::Name::doDeclare<&RBX::Network::sPlayers>
	// TEMPLATE: WEBSERVICE 0x1007af90
	// RBX::Name::doDeclare<&RBX::sDataModel>
	// TEMPLATE: WEBSERVICE 0x1007d1c0
	// RBX::Name::doDeclare<&RBX::sGameSettings>
	// TEMPLATE: WEBSERVICE 0x1007d230
	// RBX::Name::doDeclare<&sStockSound>
	// TEMPLATE: WEBSERVICE 0x10087f80
	// RBX::Name::doDeclare<&RBX::sShirtGraphic>
	// TEMPLATE: WEBSERVICE 0x10087ff0
	// RBX::Name::doDeclare<&RBX::sShirt>
	// TEMPLATE: WEBSERVICE 0x10088060
	// RBX::Name::doDeclare<&RBX::sBodyColors>
	// TEMPLATE: WEBSERVICE 0x100880d0
	// RBX::Name::doDeclare<&RBX::sSkin>
	// TEMPLATE: WEBSERVICE 0x10088140
	// RBX::Name::doDeclare<&RBX::sExplosion>
	// TEMPLATE: WEBSERVICE 0x100881b0
	// RBX::Name::doDeclare<&RBX::sMotorFeature>
	// TEMPLATE: WEBSERVICE 0x10088220
	// RBX::Name::doDeclare<&RBX::sHole>
	// TEMPLATE: WEBSERVICE 0x10088290
	// RBX::Name::doDeclare<&RBX::sVelocityMotor>
	// TEMPLATE: WEBSERVICE 0x10088300
	// RBX::Name::doDeclare<&RBX::sFlagStand>
	// TEMPLATE: WEBSERVICE 0x10088370
	// RBX::Name::doDeclare<&RBX::sFlagStandService>
	// TEMPLATE: WEBSERVICE 0x100883e0
	// RBX::Name::doDeclare<&RBX::sForceField>
	// TEMPLATE: WEBSERVICE 0x10088450
	// RBX::Name::doDeclare<&RBX::sGeometryService>
	// TEMPLATE: WEBSERVICE 0x100884c0
	// RBX::Name::doDeclare<&RBX::sBodyGyro>
	// TEMPLATE: WEBSERVICE 0x10088530
	// RBX::Name::doDeclare<&RBX::sBodyForce>
	// TEMPLATE: WEBSERVICE 0x100885a0
	// RBX::Name::doDeclare<&RBX::sBodyThrust>
	// TEMPLATE: WEBSERVICE 0x10088610
	// RBX::Name::doDeclare<&RBX::sBodyPosition>
	// TEMPLATE: WEBSERVICE 0x10088680
	// RBX::Name::doDeclare<&RBX::sBodyVelocity>
	// TEMPLATE: WEBSERVICE 0x100886f0
	// RBX::Name::doDeclare<&RBX::sRocket>
	// TEMPLATE: WEBSERVICE 0x10088760
	// RBX::Name::doDeclare<&RBX::sMessage>
	// TEMPLATE: WEBSERVICE 0x100887d0
	// RBX::Name::doDeclare<&RBX::sHint>
	// TEMPLATE: WEBSERVICE 0x10088840
	// RBX::Name::doDeclare<&RBX::sObjectValue>
	// TEMPLATE: WEBSERVICE 0x100888b0
	// RBX::Name::doDeclare<&RBX::sVisit>
	// TEMPLATE: WEBSERVICE 0x10088920
	// RBX::Name::doDeclare<&RBX::sDebrisService>
	// TEMPLATE: WEBSERVICE 0x10088990
	// RBX::Name::doDeclare<&RBX::sTimerService>
	// TEMPLATE: WEBSERVICE 0x10088a00
	// RBX::Name::doDeclare<&RBX::sSeat>
	// TEMPLATE: WEBSERVICE 0x10088a70
	// RBX::Name::doDeclare<&RBX::sSpawnLocation>
	// TEMPLATE: WEBSERVICE 0x10088ae0
	// RBX::Name::doDeclare<&RBX::sSpawnerService>
	// TEMPLATE: WEBSERVICE 0x100aafc0
	// RBX::Name::doDeclare<&RBX::sDecal>
	// TEMPLATE: WEBSERVICE 0x100ab030
	// RBX::Name::doDeclare<&RBX::sTexture>
	// TEMPLATE: WEBSERVICE 0x100b71d0
	// RBX::Name::doDeclare<&RBX::sGlobalSettings>
	// TEMPLATE: WEBSERVICE 0x100bf160
	// RBX::Name::doDeclare<&RBX::sBoxSelectCommand>
	// TEMPLATE: WEBSERVICE 0x100c51d0
	// RBX::Name::doDeclare<&RBX::sNewNullTool>
	// TEMPLATE: WEBSERVICE 0x100c7540
	// RBX::Name::doDeclare<&RBX::sBackpackItem>
	// TEMPLATE: WEBSERVICE 0x100ca0b0
	// RBX::Name::doDeclare<&RBX::sToolMouseCommand>
	// TEMPLATE: WEBSERVICE 0x100ca120
	// RBX::Name::doDeclare<&RBX::sMouse>
	// TEMPLATE: WEBSERVICE 0x100d7b10
	// RBX::Name::doDeclare<&RBX::sJointInstance>
	// TEMPLATE: WEBSERVICE 0x100d81e0
	// RBX::Name::doDeclare<&RBX::sAutoJoint>
	// TEMPLATE: WEBSERVICE 0x100e3d00
	// RBX::Name::doDeclare<&RBX::sFeature>
	// TEMPLATE: WEBSERVICE 0x100eebf0
	// RBX::Name::doDeclare<&RBX::sIntValue>
	// TEMPLATE: WEBSERVICE 0x100eec60
	// RBX::Name::doDeclare<&RBX::sBoolValue>
	// TEMPLATE: WEBSERVICE 0x100eecd0
	// RBX::Name::doDeclare<&RBX::sDoubleValue>
	// TEMPLATE: WEBSERVICE 0x100eed40
	// RBX::Name::doDeclare<&RBX::sStringValue>
	// TEMPLATE: WEBSERVICE 0x100eedb0
	// RBX::Name::doDeclare<&RBX::sVector3Value>
	// TEMPLATE: WEBSERVICE 0x100eee20
	// RBX::Name::doDeclare<&RBX::sCFrameValue>
	// TEMPLATE: WEBSERVICE 0x100eee90
	// RBX::Name::doDeclare<&RBX::sColor3Value>
	// TEMPLATE: WEBSERVICE 0x100eef00
	// RBX::Name::doDeclare<&RBX::sBrickColorValue>
	// TEMPLATE: WEBSERVICE 0x101066f0
	// RBX::Name::doDeclare<&RBX::sRunning>
	// TEMPLATE: WEBSERVICE 0x10107e20
	// RBX::Name::doDeclare<&RBX::sFallingDown>
	// TEMPLATE: WEBSERVICE 0x10122a40
	// RBX::Name::doDeclare<&RBX::sSeated>
	// TEMPLATE: WEBSERVICE 0x10122e20
	// RBX::Name::doDeclare<&RBX::sJumping>
	// TEMPLATE: WEBSERVICE 0x10123150
	// RBX::Name::doDeclare<&RBX::sGettingUp>
	// TEMPLATE: WEBSERVICE 0x10125e80
	// RBX::Name::doDeclare<&RBX::sGroupDragTool>
	// TEMPLATE: WEBSERVICE 0x101263a0
	// RBX::Name::doDeclare<&RBX::sPartDragTool>
	// TEMPLATE: WEBSERVICE 0x10129ca0
	// RBX::Name::doDeclare<&RBX::sSpecialShape>
	// TEMPLATE: WEBSERVICE 0x1012a450
	// RBX::Name::doDeclare<&RBX::sFlying>
	// TEMPLATE: WEBSERVICE 0x1012a5d0
	// RBX::Name::doDeclare<&RBX::sFreefall>
	// TEMPLATE: WEBSERVICE 0x1018b5f0
	// RBX::Name::doDeclare<&RBX::Network::sClient>
	// TEMPLATE: WEBSERVICE 0x1018b670
	// RBX::Name::doDeclare<&RBX::Network::sServer>
	// TEMPLATE: WEBSERVICE 0x1018b6f0
	// RBX::Name::doDeclare<&RBX::sNetworkSettings>
	// TEMPLATE: WEBSERVICE 0x101a9ff0
	// RBX::Name::doDeclare<&RBX::Network::sReplicator>
	template <const char* name>
	static const Name& doDeclare()
	{
		static const Name& n = declare(name, -1);

		return n;
	}

	// TEMPLATE: WEBSERVICE 0x1000b0a0
	// RBX::Name::callDoDeclare<&sThumbnailGenerator>
	// TEMPLATE: WEBSERVICE 0x1000b130
	// RBX::Name::callDoDeclare<&RBX::sLighting>
	// TEMPLATE: WEBSERVICE 0x1000b1c0
	// RBX::Name::callDoDeclare<&RBX::sRunService>
	// TEMPLATE: WEBSERVICE 0x1000b250
	// RBX::Name::callDoDeclare<&RBX::sModel>
	// TEMPLATE: WEBSERVICE 0x1000b2e0
	// RBX::Name::callDoDeclare<&RBX::sSelection>
	// TEMPLATE: WEBSERVICE 0x1000b370
	// RBX::Name::callDoDeclare<&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x1000b400
	// RBX::Name::callDoDeclare<&RBX::sScript>
	// TEMPLATE: WEBSERVICE 0x1000b490
	// RBX::Name::callDoDeclare<&RBX::sLocalScript>
	// TEMPLATE: WEBSERVICE 0x1000b520
	// RBX::Name::callDoDeclare<&RBX::sWorkspace>
	// TEMPLATE: WEBSERVICE 0x1001c970
	// RBX::Name::callDoDeclare<&RBX::Soundscape::sSoundService>
	// TEMPLATE: WEBSERVICE 0x1001ca00
	// RBX::Name::callDoDeclare<&RBX::Soundscape::sSoundChannel>
	// TEMPLATE: WEBSERVICE 0x1005a690
	// RBX::Name::callDoDeclare<&RBX::sPart>
	// TEMPLATE: WEBSERVICE 0x1005a6a0
	// RBX::Name::callDoDeclare<&RBX::sHumanoid>
	// TEMPLATE: WEBSERVICE 0x100606d0
	// RBX::Name::callDoDeclare<&RBX::Stats::sStatsItem>
	// TEMPLATE: WEBSERVICE 0x100606e0
	// RBX::Name::callDoDeclare<&RBX::Stats::sStats>
	// TEMPLATE: WEBSERVICE 0x100606f0
	// RBX::Name::callDoDeclare<&RBX::sDebugSettings>
	// TEMPLATE: WEBSERVICE 0x10068b80
	// RBX::Name::callDoDeclare<&RBX::sTeam>
	// TEMPLATE: WEBSERVICE 0x10068b90
	// RBX::Name::callDoDeclare<&RBX::Network::sPlayer>
	// TEMPLATE: WEBSERVICE 0x1006aeb0
	// RBX::Name::callDoDeclare<&RBX::sControllerService>
	// TEMPLATE: WEBSERVICE 0x1006aec0
	// RBX::Name::callDoDeclare<&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x1006aed0
	// RBX::Name::callDoDeclare<&RBX::sHopperBin>
	// TEMPLATE: WEBSERVICE 0x1006aee0
	// RBX::Name::callDoDeclare<&RBX::sStarterPackService>
	// TEMPLATE: WEBSERVICE 0x1006aef0
	// RBX::Name::callDoDeclare<&RBX::sLegacyHopperService>
	// TEMPLATE: WEBSERVICE 0x1006af00
	// RBX::Name::callDoDeclare<&RBX::sTool>
	// TEMPLATE: WEBSERVICE 0x1006af10
	// RBX::Name::callDoDeclare<&RBX::sFlag>
	// TEMPLATE: WEBSERVICE 0x1006af20
	// RBX::Name::callDoDeclare<&RBX::sArrowTool>
	// TEMPLATE: WEBSERVICE 0x10072f00
	// RBX::Name::callDoDeclare<&RBX::sServiceProvider>
	// TEMPLATE: WEBSERVICE 0x10072f20
	// RBX::Name::callDoDeclare<&RBX::sLocalBackpackItem>
	// TEMPLATE: WEBSERVICE 0x10072f30
	// RBX::Name::callDoDeclare<&RBX::sLocalBackpack>
	// TEMPLATE: WEBSERVICE 0x10072f40
	// RBX::Name::callDoDeclare<&RBX::sSnap>
	// TEMPLATE: WEBSERVICE 0x10072f50
	// RBX::Name::callDoDeclare<&RBX::sWeld>
	// TEMPLATE: WEBSERVICE 0x10072f60
	// RBX::Name::callDoDeclare<&RBX::sGlue>
	// TEMPLATE: WEBSERVICE 0x10072f70
	// RBX::Name::callDoDeclare<&RBX::sRotate>
	// TEMPLATE: WEBSERVICE 0x10072f80
	// RBX::Name::callDoDeclare<&RBX::sRotateP>
	// TEMPLATE: WEBSERVICE 0x10072f90
	// RBX::Name::callDoDeclare<&RBX::sRotateV>
	// TEMPLATE: WEBSERVICE 0x10072fa0
	// RBX::Name::callDoDeclare<&RBX::sMotor>
	// TEMPLATE: WEBSERVICE 0x10072fb0
	// RBX::Name::callDoDeclare<&RBX::sBackpack>
	// TEMPLATE: WEBSERVICE 0x10072fc0
	// RBX::Name::callDoDeclare<&RBX::sTeams>
	// TEMPLATE: WEBSERVICE 0x10072fd0
	// RBX::Name::callDoDeclare<&RBX::sAccoutrement>
	// TEMPLATE: WEBSERVICE 0x10072fe0
	// RBX::Name::callDoDeclare<&RBX::sHat>
	// TEMPLATE: WEBSERVICE 0x10072ff0
	// RBX::Name::callDoDeclare<&RBX::Network::sPlayers>
	// TEMPLATE: WEBSERVICE 0x1007b020
	// RBX::Name::callDoDeclare<&RBX::sDataModel>
	// TEMPLATE: WEBSERVICE 0x1007d7b0
	// RBX::Name::callDoDeclare<&RBX::sGameSettings>
	// TEMPLATE: WEBSERVICE 0x1007d7c0
	// RBX::Name::callDoDeclare<&sStockSound>
	// TEMPLATE: WEBSERVICE 0x10088b50
	// RBX::Name::callDoDeclare<&RBX::sShirtGraphic>
	// TEMPLATE: WEBSERVICE 0x10088b60
	// RBX::Name::callDoDeclare<&RBX::sShirt>
	// TEMPLATE: WEBSERVICE 0x10088b70
	// RBX::Name::callDoDeclare<&RBX::sBodyColors>
	// TEMPLATE: WEBSERVICE 0x10088b80
	// RBX::Name::callDoDeclare<&RBX::sSkin>
	// TEMPLATE: WEBSERVICE 0x10088b90
	// RBX::Name::callDoDeclare<&RBX::sExplosion>
	// TEMPLATE: WEBSERVICE 0x10088ba0
	// RBX::Name::callDoDeclare<&RBX::sMotorFeature>
	// TEMPLATE: WEBSERVICE 0x10088bb0
	// RBX::Name::callDoDeclare<&RBX::sHole>
	// TEMPLATE: WEBSERVICE 0x10088bc0
	// RBX::Name::callDoDeclare<&RBX::sVelocityMotor>
	// TEMPLATE: WEBSERVICE 0x10088bd0
	// RBX::Name::callDoDeclare<&RBX::sFlagStand>
	// TEMPLATE: WEBSERVICE 0x10088be0
	// RBX::Name::callDoDeclare<&RBX::sFlagStandService>
	// TEMPLATE: WEBSERVICE 0x10088bf0
	// RBX::Name::callDoDeclare<&RBX::sForceField>
	// TEMPLATE: WEBSERVICE 0x10088c00
	// RBX::Name::callDoDeclare<&RBX::sGeometryService>
	// TEMPLATE: WEBSERVICE 0x10088c10
	// RBX::Name::callDoDeclare<&RBX::sBodyGyro>
	// TEMPLATE: WEBSERVICE 0x10088c20
	// RBX::Name::callDoDeclare<&RBX::sBodyForce>
	// TEMPLATE: WEBSERVICE 0x10088c30
	// RBX::Name::callDoDeclare<&RBX::sBodyThrust>
	// TEMPLATE: WEBSERVICE 0x10088c40
	// RBX::Name::callDoDeclare<&RBX::sBodyPosition>
	// TEMPLATE: WEBSERVICE 0x10088c50
	// RBX::Name::callDoDeclare<&RBX::sBodyVelocity>
	// TEMPLATE: WEBSERVICE 0x10088c60
	// RBX::Name::callDoDeclare<&RBX::sRocket>
	// TEMPLATE: WEBSERVICE 0x10088c70
	// RBX::Name::callDoDeclare<&RBX::sMessage>
	// TEMPLATE: WEBSERVICE 0x10088c80
	// RBX::Name::callDoDeclare<&RBX::sHint>
	// TEMPLATE: WEBSERVICE 0x10088c90
	// RBX::Name::callDoDeclare<&RBX::sObjectValue>
	// TEMPLATE: WEBSERVICE 0x10088ca0
	// RBX::Name::callDoDeclare<&RBX::sVisit>
	// TEMPLATE: WEBSERVICE 0x10088cb0
	// RBX::Name::callDoDeclare<&RBX::sDebrisService>
	// TEMPLATE: WEBSERVICE 0x10088cc0
	// RBX::Name::callDoDeclare<&RBX::sTimerService>
	// TEMPLATE: WEBSERVICE 0x10088cd0
	// RBX::Name::callDoDeclare<&RBX::sSeat>
	// TEMPLATE: WEBSERVICE 0x10088ce0
	// RBX::Name::callDoDeclare<&RBX::sSpawnLocation>
	// TEMPLATE: WEBSERVICE 0x10088cf0
	// RBX::Name::callDoDeclare<&RBX::sSpawnerService>
	// TEMPLATE: WEBSERVICE 0x100ab2b0
	// RBX::Name::callDoDeclare<&RBX::sDecal>
	// TEMPLATE: WEBSERVICE 0x100ab2c0
	// RBX::Name::callDoDeclare<&RBX::sTexture>
	// TEMPLATE: WEBSERVICE 0x100b7270
	// RBX::Name::callDoDeclare<&RBX::sGlobalSettings>
	// TEMPLATE: WEBSERVICE 0x100bf230
	// RBX::Name::callDoDeclare<&RBX::sBoxSelectCommand>
	// TEMPLATE: WEBSERVICE 0x100c52d0
	// RBX::Name::callDoDeclare<&RBX::sNewNullTool>
	// TEMPLATE: WEBSERVICE 0x100c75d0
	// RBX::Name::callDoDeclare<&RBX::sBackpackItem>
	// TEMPLATE: WEBSERVICE 0x100ca2d0
	// RBX::Name::callDoDeclare<&RBX::sToolMouseCommand>
	// TEMPLATE: WEBSERVICE 0x100ca2e0
	// RBX::Name::callDoDeclare<&RBX::sMouse>
	// TEMPLATE: WEBSERVICE 0x100d7d90
	// RBX::Name::callDoDeclare<&RBX::sJointInstance>
	// TEMPLATE: WEBSERVICE 0x100d82d0
	// RBX::Name::callDoDeclare<&RBX::sAutoJoint>
	// TEMPLATE: WEBSERVICE 0x100e43b0
	// RBX::Name::callDoDeclare<&RBX::sFeature>
	// TEMPLATE: WEBSERVICE 0x100ef040
	// RBX::Name::callDoDeclare<&RBX::sIntValue>
	// TEMPLATE: WEBSERVICE 0x100ef050
	// RBX::Name::callDoDeclare<&RBX::sBoolValue>
	// TEMPLATE: WEBSERVICE 0x100ef060
	// RBX::Name::callDoDeclare<&RBX::sDoubleValue>
	// TEMPLATE: WEBSERVICE 0x100ef070
	// RBX::Name::callDoDeclare<&RBX::sStringValue>
	// TEMPLATE: WEBSERVICE 0x100ef080
	// RBX::Name::callDoDeclare<&RBX::sVector3Value>
	// TEMPLATE: WEBSERVICE 0x100ef090
	// RBX::Name::callDoDeclare<&RBX::sCFrameValue>
	// TEMPLATE: WEBSERVICE 0x100ef0a0
	// RBX::Name::callDoDeclare<&RBX::sColor3Value>
	// TEMPLATE: WEBSERVICE 0x100ef0b0
	// RBX::Name::callDoDeclare<&RBX::sBrickColorValue>
	// TEMPLATE: WEBSERVICE 0x10106760
	// RBX::Name::callDoDeclare<&RBX::sRunning>
	// TEMPLATE: WEBSERVICE 0x10107e90
	// RBX::Name::callDoDeclare<&RBX::sFallingDown>
	// TEMPLATE: WEBSERVICE 0x10122ab0
	// RBX::Name::callDoDeclare<&RBX::sSeated>
	// TEMPLATE: WEBSERVICE 0x10122e90
	// RBX::Name::callDoDeclare<&RBX::sJumping>
	// TEMPLATE: WEBSERVICE 0x101231c0
	// RBX::Name::callDoDeclare<&RBX::sGettingUp>
	// TEMPLATE: WEBSERVICE 0x10125f10
	// RBX::Name::callDoDeclare<&RBX::sGroupDragTool>
	// TEMPLATE: WEBSERVICE 0x10126460
	// RBX::Name::callDoDeclare<&RBX::sPartDragTool>
	// TEMPLATE: WEBSERVICE 0x10129d10
	// RBX::Name::callDoDeclare<&RBX::sSpecialShape>
	// TEMPLATE: WEBSERVICE 0x1012a4c0
	// RBX::Name::callDoDeclare<&RBX::sFlying>
	// TEMPLATE: WEBSERVICE 0x1012a640
	// RBX::Name::callDoDeclare<&RBX::sFreefall>
	// TEMPLATE: WEBSERVICE 0x1018b770
	// RBX::Name::callDoDeclare<&RBX::Network::sClient>
	// TEMPLATE: WEBSERVICE 0x1018b780
	// RBX::Name::callDoDeclare<&RBX::Network::sServer>
	// TEMPLATE: WEBSERVICE 0x1018b790
	// RBX::Name::callDoDeclare<&RBX::sNetworkSettings>
	// TEMPLATE: WEBSERVICE 0x101aa0c0
	// RBX::Name::callDoDeclare<&RBX::Network::sReplicator>
	template <const char* name>
	static void callDoDeclare()
	{
		doDeclare<name>();
	}

	// clang-format off
	// FUNCTION: WEBSERVICE 0x10221870
	// `RBX::moo2'::`2'::`dynamic atexit destructor for 'mutex2''
	// FUNCTION: WEBSERVICE 0x10221880
	// `RBX::Name::dictionary'::`2'::`dynamic atexit destructor for 'd''
	// STUB: WEBSERVICE 0x102218c0
	// `RBX::Name::namMap'::`2'::`dynamic atexit destructor for 'n''
	// clang-format on

	int dictionaryIndex; // 0x00

public:
	std::string name; // 0x04
};

} // namespace RBX

#endif // UTIL_NAME_H
