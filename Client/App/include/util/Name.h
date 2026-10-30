#ifndef UTIL_NAME_H
#define UTIL_NAME_H

#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/once.hpp>
#include <map>
#include <string>

namespace RBX {

// SIZE 0x20
class Name : public boost::noncopyable
{
	static boost::mutex& mutex();

public:
	static const Name& declare(const char* name, int dictionaryIndex);
	static const Name& lookup(const std::string& name);
	static const Name& lookup(const char* name);
	static const Name& lookup(int);
	static const Name& getNullName();

private:
	class NamMap;

	static NamMap& namMap();
	static std::map<int, Name*>& dictionary();

	Name(const char* name, int dictionaryIndex);
	Name& operator=(const Name& other);

public:
	template <const char* name>
	static const Name& declare()
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;

		boost::call_once(&callDoDeclare<name>, flag);

		return doDeclare<name>();
	}

	bool empty() const;

	const std::string& toString() const { return name; }

	const char* c_str() const { return name.c_str(); }

	int compare(const Name& other) const
	{
		if (this == &other) {
			return 0;
		}
		return name.compare(other.name);
	}

	static int compare(const Name* a, const Name* b);

	bool operator<(const Name& other) const;
	bool operator>(const Name& other) const;

	bool operator==(const Name& other) const { return this == &other; }
	bool operator==(const std::string& other) const { return name.compare(other) == 0; }
	bool operator==(const char* other) const { return name.compare(other) == 0; }

	// FUNCTION: ROBLOX 0x005a9380
	bool operator!=(const Name& other) const { return this != &other; }
	bool operator!=(const std::string& other) const;
	bool operator!=(const char* other) const { return name.compare(other) != 0; }
	int getDictionaryIndex() const;

private:
	// TEMPLATE: WEBSERVICE 0x1000b0b0
	// RBX::Name::doDeclare<&sThumbnailGenerator>
	// FUNCTION: WEBSERVICE 0x10071c60
	// STUB: ROBLOX 0x00448410
	// RBX::Name::doDeclare<0>
	// TEMPLATE: WEBSERVICE 0x1000b140
	// STUB: ROBLOX 0x004747c0
	// RBX::Name::doDeclare<&RBX::sLighting>
	// TEMPLATE: WEBSERVICE 0x1000b1d0
	// STUB: ROBLOX 0x004023e0
	// RBX::Name::doDeclare<&RBX::sRunService>
	// TEMPLATE: WEBSERVICE 0x1000b260
	// STUB: ROBLOX 0x004022e0
	// RBX::Name::doDeclare<&RBX::sModel>
	// TEMPLATE: WEBSERVICE 0x1000b2f0
	// STUB: ROBLOX 0x00402360
	// RBX::Name::doDeclare<&RBX::sSelection>
	// TEMPLATE: WEBSERVICE 0x1000b380
	// STUB: ROBLOX 0x00402460
	// RBX::Name::doDeclare<&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x1000b410
	// STUB: ROBLOX 0x004024e0
	// RBX::Name::doDeclare<&RBX::sScript>
	// TEMPLATE: WEBSERVICE 0x1000b4a0
	// STUB: ROBLOX 0x00402560
	// RBX::Name::doDeclare<&RBX::sLocalScript>
	// TEMPLATE: WEBSERVICE 0x1000b530
	// STUB: ROBLOX 0x004a5090
	// RBX::Name::doDeclare<&RBX::sWorkspace>
	// TEMPLATE: WEBSERVICE 0x1001c980
	// STUB: ROBLOX 0x0043c360
	// RBX::Name::doDeclare<&RBX::Soundscape::sSoundService>
	// TEMPLATE: WEBSERVICE 0x1001ca10
	// STUB: ROBLOX 0x0043c3e0
	// RBX::Name::doDeclare<&RBX::Soundscape::sSoundChannel>
	// TEMPLATE: WEBSERVICE 0x100428c0
	// TEMPLATE: ROBLOX 0x00474830
	// RBX::Name::doDeclare<&RBX::sSky>
	// TEMPLATE: WEBSERVICE 0x10059e20
	// STUB: ROBLOX 0x0041e800
	// RBX::Name::doDeclare<&RBX::sPart>
	// TEMPLATE: WEBSERVICE 0x10059e90
	// STUB: ROBLOX 0x004a5710
	// RBX::Name::doDeclare<&RBX::sHumanoid>
	// TEMPLATE: WEBSERVICE 0x1005f300
	// STUB: ROBLOX 0x0045f810
	// RBX::Name::doDeclare<&RBX::Stats::sStatsItem>
	// TEMPLATE: WEBSERVICE 0x1005f370
	// STUB: ROBLOX 0x0045ac30
	// RBX::Name::doDeclare<&RBX::Stats::sStats>
	// TEMPLATE: WEBSERVICE 0x1005f3e0
	// STUB: ROBLOX 0x004077a0
	// RBX::Name::doDeclare<&RBX::sDebugSettings>
	// TEMPLATE: WEBSERVICE 0x100689c0
	// STUB: ROBLOX 0x0040d870
	// RBX::Name::doDeclare<&RBX::sTeam>
	// TEMPLATE: WEBSERVICE 0x10068a30
	// STUB: ROBLOX 0x0040d8f0
	// RBX::Name::doDeclare<&RBX::Network::sPlayer>
	// TEMPLATE: WEBSERVICE 0x1006a1e0
	// STUB: ROBLOX 0x0045f790
	// RBX::Name::doDeclare<&RBX::sControllerService>
	// TEMPLATE: WEBSERVICE 0x1006a250
	// STUB: ROBLOX 0x0045f890
	// RBX::Name::doDeclare<&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x1006a2c0
	// STUB: ROBLOX 0x004a5110
	// RBX::Name::doDeclare<&RBX::sHopperBin>
	// TEMPLATE: WEBSERVICE 0x1006a330
	// STUB: ROBLOX 0x004a5190
	// RBX::Name::doDeclare<&RBX::sStarterPackService>
	// TEMPLATE: WEBSERVICE 0x1006a3a0
	// STUB: ROBLOX 0x004a5210
	// RBX::Name::doDeclare<&RBX::sLegacyHopperService>
	// TEMPLATE: WEBSERVICE 0x1006a410
	// TEMPLATE: ROBLOX 0x0058ef40
	// RBX::Name::doDeclare<&RBX::sTool>
	// TEMPLATE: WEBSERVICE 0x1006a480
	// TEMPLATE: ROBLOX 0x005b4830
	// RBX::Name::doDeclare<&RBX::sFlag>
	// TEMPLATE: WEBSERVICE 0x1006a4f0
	// TEMPLATE: ROBLOX 0x005b48a0
	// RBX::Name::doDeclare<&RBX::sArrowTool>
	// TEMPLATE: WEBSERVICE 0x10071bf0
	// TEMPLATE: ROBLOX 0x00581d80
	// RBX::Name::doDeclare<&RBX::sServiceProvider>
	// TEMPLATE: WEBSERVICE 0x10071cd0
	// TEMPLATE: ROBLOX 0x0058ee60
	// RBX::Name::doDeclare<&RBX::sLocalBackpackItem>
	// TEMPLATE: WEBSERVICE 0x10071d40
	// TEMPLATE: ROBLOX 0x0058eed0
	// RBX::Name::doDeclare<&RBX::sLocalBackpack>
	// TEMPLATE: WEBSERVICE 0x10071db0
	// STUB: ROBLOX 0x004c2a60
	// RBX::Name::doDeclare<&RBX::sSnap>
	// TEMPLATE: WEBSERVICE 0x10071e20
	// STUB: ROBLOX 0x004c2ae0
	// RBX::Name::doDeclare<&RBX::sWeld>
	// TEMPLATE: WEBSERVICE 0x10071e90
	// STUB: ROBLOX 0x004c2b60
	// RBX::Name::doDeclare<&RBX::sGlue>
	// TEMPLATE: WEBSERVICE 0x10071f00
	// STUB: ROBLOX 0x004c2be0
	// RBX::Name::doDeclare<&RBX::sRotate>
	// TEMPLATE: WEBSERVICE 0x10071f70
	// STUB: ROBLOX 0x004c2c60
	// RBX::Name::doDeclare<&RBX::sRotateP>
	// TEMPLATE: WEBSERVICE 0x10071fe0
	// STUB: ROBLOX 0x004c2ce0
	// RBX::Name::doDeclare<&RBX::sRotateV>
	// TEMPLATE: WEBSERVICE 0x10072050
	// STUB: ROBLOX 0x004c2d60
	// RBX::Name::doDeclare<&RBX::sMotor>
	// TEMPLATE: WEBSERVICE 0x100720c0
	// STUB: ROBLOX 0x004a5290
	// RBX::Name::doDeclare<&RBX::sBackpack>
	// TEMPLATE: WEBSERVICE 0x10072130
	// STUB: ROBLOX 0x004a5610
	// RBX::Name::doDeclare<&RBX::sTeams>
	// TEMPLATE: WEBSERVICE 0x100721a0
	// STUB: ROBLOX 0x004330d0
	// RBX::Name::doDeclare<&RBX::sAccoutrement>
	// TEMPLATE: WEBSERVICE 0x10072210
	// STUB: ROBLOX 0x00433150
	// RBX::Name::doDeclare<&RBX::sHat>
	// TEMPLATE: WEBSERVICE 0x10072280
	// STUB: ROBLOX 0x0040d970
	// RBX::Name::doDeclare<&RBX::Network::sPlayers>
	// TEMPLATE: WEBSERVICE 0x1007af90
	// TEMPLATE: ROBLOX 0x00594150
	// RBX::Name::doDeclare<&RBX::sDataModel>
	// TEMPLATE: WEBSERVICE 0x1007d1c0
	// STUB: ROBLOX 0x0044dfe0
	// RBX::Name::doDeclare<&RBX::sGameSettings>
	// TEMPLATE: WEBSERVICE 0x1007d230
	// TEMPLATE: ROBLOX 0x005c10e0
	// RBX::Name::doDeclare<&sStockSound>
	// TEMPLATE: WEBSERVICE 0x10087f80
	// STUB: ROBLOX 0x004a5410
	// RBX::Name::doDeclare<&RBX::sShirtGraphic>
	// TEMPLATE: WEBSERVICE 0x10087ff0
	// STUB: ROBLOX 0x004a5490
	// RBX::Name::doDeclare<&RBX::sShirt>
	// TEMPLATE: WEBSERVICE 0x10088060
	// STUB: ROBLOX 0x004a5510
	// RBX::Name::doDeclare<&RBX::sBodyColors>
	// TEMPLATE: WEBSERVICE 0x100880d0
	// STUB: ROBLOX 0x004a5590
	// RBX::Name::doDeclare<&RBX::sSkin>
	// TEMPLATE: WEBSERVICE 0x10088140
	// TEMPLATE: ROBLOX 0x005c9a90
	// RBX::Name::doDeclare<&RBX::sExplosion>
	// TEMPLATE: WEBSERVICE 0x100881b0
	// TEMPLATE: ROBLOX 0x00596a30
	// RBX::Name::doDeclare<&RBX::sMotorFeature>
	// TEMPLATE: WEBSERVICE 0x10088220
	// TEMPLATE: ROBLOX 0x00596aa0
	// RBX::Name::doDeclare<&RBX::sHole>
	// TEMPLATE: WEBSERVICE 0x10088290
	// TEMPLATE: ROBLOX 0x00596b10
	// RBX::Name::doDeclare<&RBX::sVelocityMotor>
	// TEMPLATE: WEBSERVICE 0x10088300
	// TEMPLATE: ROBLOX 0x005c9b00
	// RBX::Name::doDeclare<&RBX::sFlagStand>
	// TEMPLATE: WEBSERVICE 0x10088370
	// TEMPLATE: ROBLOX 0x005c9b70
	// RBX::Name::doDeclare<&RBX::sFlagStandService>
	// TEMPLATE: WEBSERVICE 0x100883e0
	// TEMPLATE: ROBLOX 0x005c9be0
	// RBX::Name::doDeclare<&RBX::sForceField>
	// TEMPLATE: WEBSERVICE 0x10088450
	// TEMPLATE: ROBLOX 0x005c9c50
	// RBX::Name::doDeclare<&RBX::sGeometryService>
	// TEMPLATE: WEBSERVICE 0x100884c0
	// TEMPLATE: ROBLOX 0x005c9cc0
	// RBX::Name::doDeclare<&RBX::sBodyGyro>
	// TEMPLATE: WEBSERVICE 0x10088530
	// TEMPLATE: ROBLOX 0x005c9d30
	// RBX::Name::doDeclare<&RBX::sBodyForce>
	// TEMPLATE: WEBSERVICE 0x100885a0
	// TEMPLATE: ROBLOX 0x005c9da0
	// RBX::Name::doDeclare<&RBX::sBodyThrust>
	// TEMPLATE: WEBSERVICE 0x10088610
	// TEMPLATE: ROBLOX 0x005c9e10
	// RBX::Name::doDeclare<&RBX::sBodyPosition>
	// TEMPLATE: WEBSERVICE 0x10088680
	// TEMPLATE: ROBLOX 0x005c9e80
	// RBX::Name::doDeclare<&RBX::sBodyVelocity>
	// TEMPLATE: WEBSERVICE 0x100886f0
	// TEMPLATE: ROBLOX 0x005c9ef0
	// RBX::Name::doDeclare<&RBX::sRocket>
	// TEMPLATE: WEBSERVICE 0x10088760
	// TEMPLATE: ROBLOX 0x005c9f60
	// RBX::Name::doDeclare<&RBX::sMessage>
	// TEMPLATE: WEBSERVICE 0x100887d0
	// TEMPLATE: ROBLOX 0x005c9fd0
	// RBX::Name::doDeclare<&RBX::sHint>
	// TEMPLATE: WEBSERVICE 0x10088840
	// TEMPLATE: ROBLOX 0x005ca040
	// RBX::Name::doDeclare<&RBX::sObjectValue>
	// TEMPLATE: WEBSERVICE 0x100888b0
	// STUB: ROBLOX 0x00453fa0
	// RBX::Name::doDeclare<&RBX::sVisit>
	// TEMPLATE: WEBSERVICE 0x10088920
	// TEMPLATE: ROBLOX 0x005ca0b0
	// RBX::Name::doDeclare<&RBX::sDebrisService>
	// TEMPLATE: WEBSERVICE 0x10088990
	// STUB: ROBLOX 0x004a5690
	// RBX::Name::doDeclare<&RBX::sTimerService>
	// TEMPLATE: WEBSERVICE 0x10088a00
	// TEMPLATE: ROBLOX 0x005ca120
	// RBX::Name::doDeclare<&RBX::sSeat>
	// TEMPLATE: WEBSERVICE 0x10088a70
	// STUB: ROBLOX 0x004a5310
	// RBX::Name::doDeclare<&RBX::sSpawnLocation>
	// TEMPLATE: WEBSERVICE 0x10088ae0
	// STUB: ROBLOX 0x004a5390
	// RBX::Name::doDeclare<&RBX::sSpawnerService>
	// TEMPLATE: WEBSERVICE 0x100aafc0
	// STUB: ROBLOX 0x0041e700
	// RBX::Name::doDeclare<&RBX::sDecal>
	// TEMPLATE: WEBSERVICE 0x100ab030
	// STUB: ROBLOX 0x0041e780
	// RBX::Name::doDeclare<&RBX::sTexture>
	// TEMPLATE: WEBSERVICE 0x100b71d0
	// TEMPLATE: ROBLOX 0x00582690
	// RBX::Name::doDeclare<&RBX::sGlobalSettings>
	// TEMPLATE: WEBSERVICE 0x100bf160
	// TEMPLATE: ROBLOX 0x00620dc0
	// RBX::Name::doDeclare<&RBX::sBoxSelectCommand>
	// TEMPLATE: WEBSERVICE 0x100c51d0
	// TEMPLATE: ROBLOX 0x00622960
	// RBX::Name::doDeclare<&RBX::sNewNullTool>
	// TEMPLATE: WEBSERVICE 0x100c7540
	// TEMPLATE: ROBLOX 0x005e1730
	// RBX::Name::doDeclare<&RBX::sBackpackItem>
	// TEMPLATE: WEBSERVICE 0x100ca0b0
	// TEMPLATE: ROBLOX 0x0060ed60
	// RBX::Name::doDeclare<&RBX::sToolMouseCommand>
	// TEMPLATE: WEBSERVICE 0x100ca120
	// TEMPLATE: ROBLOX 0x0060edd0
	// RBX::Name::doDeclare<&RBX::sMouse>
	// TEMPLATE: WEBSERVICE 0x100d7b10
	// TEMPLATE: ROBLOX 0x005f1a20
	// RBX::Name::doDeclare<&RBX::sJointInstance>
	// TEMPLATE: WEBSERVICE 0x100d81e0
	// TEMPLATE: ROBLOX 0x005f2110
	// RBX::Name::doDeclare<&RBX::sAutoJoint>
	// TEMPLATE: WEBSERVICE 0x100e3d00
	// TEMPLATE: ROBLOX 0x00617350
	// RBX::Name::doDeclare<&RBX::sFeature>
	// TEMPLATE: WEBSERVICE 0x100eebf0
	// TEMPLATE: ROBLOX 0x0062dd00
	// RBX::Name::doDeclare<&RBX::sIntValue>
	// TEMPLATE: WEBSERVICE 0x100eec60
	// TEMPLATE: ROBLOX 0x0062dd70
	// RBX::Name::doDeclare<&RBX::sBoolValue>
	// TEMPLATE: WEBSERVICE 0x100eecd0
	// TEMPLATE: ROBLOX 0x0062dde0
	// RBX::Name::doDeclare<&RBX::sDoubleValue>
	// TEMPLATE: WEBSERVICE 0x100eed40
	// TEMPLATE: ROBLOX 0x0062de50
	// RBX::Name::doDeclare<&RBX::sStringValue>
	// TEMPLATE: WEBSERVICE 0x100eedb0
	// TEMPLATE: ROBLOX 0x0062dec0
	// RBX::Name::doDeclare<&RBX::sVector3Value>
	// TEMPLATE: WEBSERVICE 0x100eee20
	// TEMPLATE: ROBLOX 0x0062df30
	// RBX::Name::doDeclare<&RBX::sCFrameValue>
	// TEMPLATE: WEBSERVICE 0x100eee90
	// TEMPLATE: ROBLOX 0x0062dfa0
	// RBX::Name::doDeclare<&RBX::sColor3Value>
	// TEMPLATE: WEBSERVICE 0x100eef00
	// TEMPLATE: ROBLOX 0x0062e010
	// RBX::Name::doDeclare<&RBX::sBrickColorValue>
	// TEMPLATE: WEBSERVICE 0x101066f0
	// TEMPLATE: ROBLOX 0x00640d60
	// RBX::Name::doDeclare<&RBX::sRunning>
	// TEMPLATE: WEBSERVICE 0x10107e20
	// TEMPLATE: ROBLOX 0x00641a70
	// RBX::Name::doDeclare<&RBX::sFallingDown>
	// TEMPLATE: WEBSERVICE 0x10122a40
	// TEMPLATE: ROBLOX 0x00663bc0
	// RBX::Name::doDeclare<&RBX::sSeated>
	// TEMPLATE: WEBSERVICE 0x10122e20
	// TEMPLATE: ROBLOX 0x00663fa0
	// RBX::Name::doDeclare<&RBX::sJumping>
	// TEMPLATE: WEBSERVICE 0x10123150
	// TEMPLATE: ROBLOX 0x00664220
	// RBX::Name::doDeclare<&RBX::sGettingUp>
	// TEMPLATE: WEBSERVICE 0x10125e80
	// TEMPLATE: ROBLOX 0x006674e0
	// RBX::Name::doDeclare<&RBX::sGroupDragTool>
	// TEMPLATE: WEBSERVICE 0x101263a0
	// TEMPLATE: ROBLOX 0x00662860
	// RBX::Name::doDeclare<&RBX::sPartDragTool>
	// TEMPLATE: WEBSERVICE 0x10129ca0
	// STUB: ROBLOX 0x0041e880
	// RBX::Name::doDeclare<&RBX::sSpecialShape>
	// TEMPLATE: WEBSERVICE 0x1012a450
	// TEMPLATE: ROBLOX 0x00669e60
	// RBX::Name::doDeclare<&RBX::sFlying>
	// TEMPLATE: WEBSERVICE 0x1012a5d0
	// TEMPLATE: ROBLOX 0x00669fe0
	// RBX::Name::doDeclare<&RBX::sFreefall>
	// TEMPLATE: WEBSERVICE 0x1018b5f0
	// TEMPLATE: ROBLOX 0x004afca0
	// RBX::Name::doDeclare<&RBX::Network::sClient>
	// TEMPLATE: WEBSERVICE 0x1018b670
	// TEMPLATE: ROBLOX 0x004afd20
	// RBX::Name::doDeclare<&RBX::Network::sServer>
	// TEMPLATE: WEBSERVICE 0x1018b6f0
	// TEMPLATE: ROBLOX 0x004b80d0
	// RBX::Name::doDeclare<&RBX::sNetworkSettings>
	// TEMPLATE: WEBSERVICE 0x101a9ff0
	// TEMPLATE: ROBLOX 0x004d3130
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
	// TEMPLATE: ROBLOX 0x00474970
	// RBX::Name::callDoDeclare<&RBX::sLighting>
	// TEMPLATE: WEBSERVICE 0x1000b1c0
	// TEMPLATE: ROBLOX 0x00403100
	// RBX::Name::callDoDeclare<&RBX::sRunService>
	// TEMPLATE: WEBSERVICE 0x1000b250
	// TEMPLATE: ROBLOX 0x004030e0
	// RBX::Name::callDoDeclare<&RBX::sModel>
	// TEMPLATE: WEBSERVICE 0x1000b2e0
	// TEMPLATE: ROBLOX 0x004030f0
	// RBX::Name::callDoDeclare<&RBX::sSelection>
	// TEMPLATE: WEBSERVICE 0x1000b370
	// TEMPLATE: ROBLOX 0x00403110
	// RBX::Name::callDoDeclare<&RBX::sScriptContext>
	// TEMPLATE: WEBSERVICE 0x1000b400
	// TEMPLATE: ROBLOX 0x00403120
	// RBX::Name::callDoDeclare<&RBX::sScript>
	// TEMPLATE: WEBSERVICE 0x1000b490
	// TEMPLATE: ROBLOX 0x00403130
	// RBX::Name::callDoDeclare<&RBX::sLocalScript>
	// TEMPLATE: WEBSERVICE 0x1000b520
	// TEMPLATE: ROBLOX 0x004a5de0
	// RBX::Name::callDoDeclare<&RBX::sWorkspace>
	// TEMPLATE: WEBSERVICE 0x1001c970
	// TEMPLATE: ROBLOX 0x0043cd40
	// RBX::Name::callDoDeclare<&RBX::Soundscape::sSoundService>
	// TEMPLATE: WEBSERVICE 0x1001ca00
	// TEMPLATE: ROBLOX 0x0043cd50
	// RBX::Name::callDoDeclare<&RBX::Soundscape::sSoundChannel>
	// TEMPLATE: WEBSERVICE 0x10042df0
	// TEMPLATE: ROBLOX 0x00474980
	// RBX::Name::callDoDeclare<&RBX::sSky>
	// TEMPLATE: WEBSERVICE 0x1005a690
	// TEMPLATE: ROBLOX 0x0041e9d0
	// RBX::Name::callDoDeclare<&RBX::sPart>
	// TEMPLATE: WEBSERVICE 0x1005a6a0
	// TEMPLATE: ROBLOX 0x004a5eb0
	// RBX::Name::callDoDeclare<&RBX::sHumanoid>
	// TEMPLATE: WEBSERVICE 0x100606d0
	// TEMPLATE: ROBLOX 0x0045fd80
	// RBX::Name::callDoDeclare<&RBX::Stats::sStatsItem>
	// TEMPLATE: WEBSERVICE 0x100606e0
	// TEMPLATE: ROBLOX 0x0045b000
	// RBX::Name::callDoDeclare<&RBX::Stats::sStats>
	// TEMPLATE: WEBSERVICE 0x100606f0
	// TEMPLATE: ROBLOX 0x00407e70
	// RBX::Name::callDoDeclare<&RBX::sDebugSettings>
	// TEMPLATE: WEBSERVICE 0x10068b80
	// TEMPLATE: ROBLOX 0x0040de70
	// RBX::Name::callDoDeclare<&RBX::sTeam>
	// TEMPLATE: WEBSERVICE 0x10068b90
	// TEMPLATE: ROBLOX 0x0040de80
	// RBX::Name::callDoDeclare<&RBX::Network::sPlayer>
	// TEMPLATE: WEBSERVICE 0x1006aeb0
	// TEMPLATE: ROBLOX 0x0045fd70
	// RBX::Name::callDoDeclare<&RBX::sControllerService>
	// TEMPLATE: WEBSERVICE 0x1006aec0
	// TEMPLATE: ROBLOX 0x0045fd90
	// RBX::Name::callDoDeclare<&RBX::sCamera>
	// TEMPLATE: WEBSERVICE 0x1006aed0
	// TEMPLATE: ROBLOX 0x004a5df0
	// RBX::Name::callDoDeclare<&RBX::sHopperBin>
	// TEMPLATE: WEBSERVICE 0x1006aee0
	// TEMPLATE: ROBLOX 0x004a5e00
	// RBX::Name::callDoDeclare<&RBX::sStarterPackService>
	// TEMPLATE: WEBSERVICE 0x1006aef0
	// TEMPLATE: ROBLOX 0x004a5e10
	// RBX::Name::callDoDeclare<&RBX::sLegacyHopperService>
	// TEMPLATE: WEBSERVICE 0x1006af00
	// TEMPLATE: ROBLOX 0x0058ffa0
	// RBX::Name::callDoDeclare<&RBX::sTool>
	// TEMPLATE: WEBSERVICE 0x1006af10
	// TEMPLATE: ROBLOX 0x005b4e00
	// RBX::Name::callDoDeclare<&RBX::sFlag>
	// TEMPLATE: WEBSERVICE 0x1006af20
	// TEMPLATE: ROBLOX 0x005b4e10
	// RBX::Name::callDoDeclare<&RBX::sArrowTool>
	// TEMPLATE: WEBSERVICE 0x10072f00
	// TEMPLATE: ROBLOX 0x00581e00
	// RBX::Name::callDoDeclare<&RBX::sServiceProvider>
	// TEMPLATE: WEBSERVICE 0x10072f10
	// TEMPLATE: ROBLOX 0x00448590
	// RBX::Name::callDoDeclare<0>
	// TEMPLATE: WEBSERVICE 0x10072f20
	// TEMPLATE: ROBLOX 0x0058ff80
	// RBX::Name::callDoDeclare<&RBX::sLocalBackpackItem>
	// TEMPLATE: WEBSERVICE 0x10072f30
	// TEMPLATE: ROBLOX 0x0058ff90
	// RBX::Name::callDoDeclare<&RBX::sLocalBackpack>
	// TEMPLATE: WEBSERVICE 0x10072f40
	// TEMPLATE: ROBLOX 0x004c3fb0
	// RBX::Name::callDoDeclare<&RBX::sSnap>
	// TEMPLATE: WEBSERVICE 0x10072f50
	// TEMPLATE: ROBLOX 0x004c3fc0
	// RBX::Name::callDoDeclare<&RBX::sWeld>
	// TEMPLATE: WEBSERVICE 0x10072f60
	// TEMPLATE: ROBLOX 0x004c3fd0
	// RBX::Name::callDoDeclare<&RBX::sGlue>
	// TEMPLATE: WEBSERVICE 0x10072f70
	// TEMPLATE: ROBLOX 0x004c3fe0
	// RBX::Name::callDoDeclare<&RBX::sRotate>
	// TEMPLATE: WEBSERVICE 0x10072f80
	// TEMPLATE: ROBLOX 0x004c3ff0
	// RBX::Name::callDoDeclare<&RBX::sRotateP>
	// TEMPLATE: WEBSERVICE 0x10072f90
	// TEMPLATE: ROBLOX 0x004c4000
	// RBX::Name::callDoDeclare<&RBX::sRotateV>
	// TEMPLATE: WEBSERVICE 0x10072fa0
	// TEMPLATE: ROBLOX 0x004c4010
	// RBX::Name::callDoDeclare<&RBX::sMotor>
	// TEMPLATE: WEBSERVICE 0x10072fb0
	// TEMPLATE: ROBLOX 0x004a5e20
	// RBX::Name::callDoDeclare<&RBX::sBackpack>
	// TEMPLATE: WEBSERVICE 0x10072fc0
	// TEMPLATE: ROBLOX 0x004a5e90
	// RBX::Name::callDoDeclare<&RBX::sTeams>
	// TEMPLATE: WEBSERVICE 0x10072fd0
	// TEMPLATE: ROBLOX 0x00434820
	// RBX::Name::callDoDeclare<&RBX::sAccoutrement>
	// TEMPLATE: WEBSERVICE 0x10072fe0
	// TEMPLATE: ROBLOX 0x00434830
	// RBX::Name::callDoDeclare<&RBX::sHat>
	// TEMPLATE: WEBSERVICE 0x10072ff0
	// TEMPLATE: ROBLOX 0x0040de90
	// RBX::Name::callDoDeclare<&RBX::Network::sPlayers>
	// TEMPLATE: WEBSERVICE 0x1007b020
	// TEMPLATE: ROBLOX 0x005941d0
	// RBX::Name::callDoDeclare<&RBX::sDataModel>
	// TEMPLATE: WEBSERVICE 0x1007d7b0
	// TEMPLATE: ROBLOX 0x0044e900
	// RBX::Name::callDoDeclare<&RBX::sGameSettings>
	// TEMPLATE: WEBSERVICE 0x1007d7c0
	// TEMPLATE: ROBLOX 0x005c16e0
	// RBX::Name::callDoDeclare<&sStockSound>
	// TEMPLATE: WEBSERVICE 0x10088b50
	// TEMPLATE: ROBLOX 0x004a5e50
	// RBX::Name::callDoDeclare<&RBX::sShirtGraphic>
	// TEMPLATE: WEBSERVICE 0x10088b60
	// TEMPLATE: ROBLOX 0x004a5e60
	// RBX::Name::callDoDeclare<&RBX::sShirt>
	// TEMPLATE: WEBSERVICE 0x10088b70
	// TEMPLATE: ROBLOX 0x004a5e70
	// RBX::Name::callDoDeclare<&RBX::sBodyColors>
	// TEMPLATE: WEBSERVICE 0x10088b80
	// TEMPLATE: ROBLOX 0x004a5e80
	// RBX::Name::callDoDeclare<&RBX::sSkin>
	// TEMPLATE: WEBSERVICE 0x10088b90
	// TEMPLATE: ROBLOX 0x005ca190
	// RBX::Name::callDoDeclare<&RBX::sExplosion>
	// TEMPLATE: WEBSERVICE 0x10088ba0
	// TEMPLATE: ROBLOX 0x00597150
	// RBX::Name::callDoDeclare<&RBX::sMotorFeature>
	// TEMPLATE: WEBSERVICE 0x10088bb0
	// TEMPLATE: ROBLOX 0x00597160
	// RBX::Name::callDoDeclare<&RBX::sHole>
	// TEMPLATE: WEBSERVICE 0x10088bc0
	// TEMPLATE: ROBLOX 0x00597170
	// RBX::Name::callDoDeclare<&RBX::sVelocityMotor>
	// TEMPLATE: WEBSERVICE 0x10088bd0
	// TEMPLATE: ROBLOX 0x005ca1a0
	// RBX::Name::callDoDeclare<&RBX::sFlagStand>
	// TEMPLATE: WEBSERVICE 0x10088be0
	// TEMPLATE: ROBLOX 0x005ca1b0
	// RBX::Name::callDoDeclare<&RBX::sFlagStandService>
	// TEMPLATE: WEBSERVICE 0x10088bf0
	// TEMPLATE: ROBLOX 0x005ca1c0
	// RBX::Name::callDoDeclare<&RBX::sForceField>
	// TEMPLATE: WEBSERVICE 0x10088c00
	// TEMPLATE: ROBLOX 0x005ca1d0
	// RBX::Name::callDoDeclare<&RBX::sGeometryService>
	// TEMPLATE: WEBSERVICE 0x10088c10
	// TEMPLATE: ROBLOX 0x005ca1e0
	// RBX::Name::callDoDeclare<&RBX::sBodyGyro>
	// TEMPLATE: WEBSERVICE 0x10088c20
	// TEMPLATE: ROBLOX 0x005ca1f0
	// RBX::Name::callDoDeclare<&RBX::sBodyForce>
	// TEMPLATE: WEBSERVICE 0x10088c30
	// TEMPLATE: ROBLOX 0x005ca200
	// RBX::Name::callDoDeclare<&RBX::sBodyThrust>
	// TEMPLATE: WEBSERVICE 0x10088c40
	// TEMPLATE: ROBLOX 0x005ca210
	// RBX::Name::callDoDeclare<&RBX::sBodyPosition>
	// TEMPLATE: WEBSERVICE 0x10088c50
	// TEMPLATE: ROBLOX 0x005ca220
	// RBX::Name::callDoDeclare<&RBX::sBodyVelocity>
	// TEMPLATE: WEBSERVICE 0x10088c60
	// TEMPLATE: ROBLOX 0x005ca230
	// RBX::Name::callDoDeclare<&RBX::sRocket>
	// TEMPLATE: WEBSERVICE 0x10088c70
	// TEMPLATE: ROBLOX 0x005ca240
	// RBX::Name::callDoDeclare<&RBX::sMessage>
	// TEMPLATE: WEBSERVICE 0x10088c80
	// TEMPLATE: ROBLOX 0x005ca250
	// RBX::Name::callDoDeclare<&RBX::sHint>
	// TEMPLATE: WEBSERVICE 0x10088c90
	// TEMPLATE: ROBLOX 0x005ca260
	// RBX::Name::callDoDeclare<&RBX::sObjectValue>
	// TEMPLATE: WEBSERVICE 0x10088ca0
	// TEMPLATE: ROBLOX 0x00455860
	// RBX::Name::callDoDeclare<&RBX::sVisit>
	// TEMPLATE: WEBSERVICE 0x10088cb0
	// TEMPLATE: ROBLOX 0x005ca270
	// RBX::Name::callDoDeclare<&RBX::sDebrisService>
	// TEMPLATE: WEBSERVICE 0x10088cc0
	// TEMPLATE: ROBLOX 0x004a5ea0
	// RBX::Name::callDoDeclare<&RBX::sTimerService>
	// TEMPLATE: WEBSERVICE 0x10088cd0
	// TEMPLATE: ROBLOX 0x005ca280
	// RBX::Name::callDoDeclare<&RBX::sSeat>
	// TEMPLATE: WEBSERVICE 0x10088ce0
	// TEMPLATE: ROBLOX 0x004a5e30
	// RBX::Name::callDoDeclare<&RBX::sSpawnLocation>
	// TEMPLATE: WEBSERVICE 0x10088cf0
	// TEMPLATE: ROBLOX 0x004a5e40
	// RBX::Name::callDoDeclare<&RBX::sSpawnerService>
	// TEMPLATE: WEBSERVICE 0x100ab2b0
	// TEMPLATE: ROBLOX 0x0041e9b0
	// RBX::Name::callDoDeclare<&RBX::sDecal>
	// TEMPLATE: WEBSERVICE 0x100ab2c0
	// TEMPLATE: ROBLOX 0x0041e9c0
	// RBX::Name::callDoDeclare<&RBX::sTexture>
	// TEMPLATE: WEBSERVICE 0x100b7270
	// TEMPLATE: ROBLOX 0x00582720
	// RBX::Name::callDoDeclare<&RBX::sGlobalSettings>
	// TEMPLATE: WEBSERVICE 0x100bf230
	// TEMPLATE: ROBLOX 0x00620e90
	// RBX::Name::callDoDeclare<&RBX::sBoxSelectCommand>
	// TEMPLATE: WEBSERVICE 0x100c52d0
	// TEMPLATE: ROBLOX 0x00622ab0
	// RBX::Name::callDoDeclare<&RBX::sNewNullTool>
	// TEMPLATE: WEBSERVICE 0x100c75d0
	// TEMPLATE: ROBLOX 0x005e17a0
	// RBX::Name::callDoDeclare<&RBX::sBackpackItem>
	// TEMPLATE: WEBSERVICE 0x100ca2d0
	// TEMPLATE: ROBLOX 0x0060ef40
	// RBX::Name::callDoDeclare<&RBX::sToolMouseCommand>
	// TEMPLATE: WEBSERVICE 0x100ca2e0
	// TEMPLATE: ROBLOX 0x0060ef50
	// RBX::Name::callDoDeclare<&RBX::sMouse>
	// TEMPLATE: WEBSERVICE 0x100d7d90
	// TEMPLATE: ROBLOX 0x005f1cb0
	// RBX::Name::callDoDeclare<&RBX::sJointInstance>
	// TEMPLATE: WEBSERVICE 0x100d82d0
	// TEMPLATE: ROBLOX 0x005f2200
	// RBX::Name::callDoDeclare<&RBX::sAutoJoint>
	// TEMPLATE: WEBSERVICE 0x100e43b0
	// TEMPLATE: ROBLOX 0x006179d0
	// RBX::Name::callDoDeclare<&RBX::sFeature>
	// TEMPLATE: WEBSERVICE 0x100ef040
	// TEMPLATE: ROBLOX 0x0062e140
	// RBX::Name::callDoDeclare<&RBX::sIntValue>
	// TEMPLATE: WEBSERVICE 0x100ef050
	// TEMPLATE: ROBLOX 0x0062e150
	// RBX::Name::callDoDeclare<&RBX::sBoolValue>
	// TEMPLATE: WEBSERVICE 0x100ef060
	// TEMPLATE: ROBLOX 0x0062e160
	// RBX::Name::callDoDeclare<&RBX::sDoubleValue>
	// TEMPLATE: WEBSERVICE 0x100ef070
	// TEMPLATE: ROBLOX 0x0062e170
	// RBX::Name::callDoDeclare<&RBX::sStringValue>
	// TEMPLATE: WEBSERVICE 0x100ef080
	// TEMPLATE: ROBLOX 0x0062e180
	// RBX::Name::callDoDeclare<&RBX::sVector3Value>
	// TEMPLATE: WEBSERVICE 0x100ef090
	// TEMPLATE: ROBLOX 0x0062e190
	// RBX::Name::callDoDeclare<&RBX::sCFrameValue>
	// TEMPLATE: WEBSERVICE 0x100ef0a0
	// TEMPLATE: ROBLOX 0x0062e1a0
	// RBX::Name::callDoDeclare<&RBX::sColor3Value>
	// TEMPLATE: WEBSERVICE 0x100ef0b0
	// TEMPLATE: ROBLOX 0x0062e1b0
	// RBX::Name::callDoDeclare<&RBX::sBrickColorValue>
	// TEMPLATE: WEBSERVICE 0x10106760
	// TEMPLATE: ROBLOX 0x00640dd0
	// RBX::Name::callDoDeclare<&RBX::sRunning>
	// TEMPLATE: WEBSERVICE 0x10107e90
	// TEMPLATE: ROBLOX 0x00641ae0
	// RBX::Name::callDoDeclare<&RBX::sFallingDown>
	// TEMPLATE: WEBSERVICE 0x10122ab0
	// TEMPLATE: ROBLOX 0x00663c30
	// RBX::Name::callDoDeclare<&RBX::sSeated>
	// TEMPLATE: WEBSERVICE 0x10122e90
	// TEMPLATE: ROBLOX 0x00664010
	// RBX::Name::callDoDeclare<&RBX::sJumping>
	// TEMPLATE: WEBSERVICE 0x101231c0
	// TEMPLATE: ROBLOX 0x00664290
	// RBX::Name::callDoDeclare<&RBX::sGettingUp>
	// TEMPLATE: WEBSERVICE 0x10125f10
	// TEMPLATE: ROBLOX 0x00667550
	// RBX::Name::callDoDeclare<&RBX::sGroupDragTool>
	// TEMPLATE: WEBSERVICE 0x10126460
	// TEMPLATE: ROBLOX 0x00662920
	// RBX::Name::callDoDeclare<&RBX::sPartDragTool>
	// TEMPLATE: WEBSERVICE 0x10129d10
	// TEMPLATE: ROBLOX 0x0041e9e0
	// RBX::Name::callDoDeclare<&RBX::sSpecialShape>
	// TEMPLATE: WEBSERVICE 0x1012a4c0
	// TEMPLATE: ROBLOX 0x00669ed0
	// RBX::Name::callDoDeclare<&RBX::sFlying>
	// TEMPLATE: WEBSERVICE 0x1012a640
	// TEMPLATE: ROBLOX 0x0066a050
	// RBX::Name::callDoDeclare<&RBX::sFreefall>
	// TEMPLATE: WEBSERVICE 0x1018b770
	// TEMPLATE: ROBLOX 0x004b04f0
	// RBX::Name::callDoDeclare<&RBX::Network::sClient>
	// TEMPLATE: WEBSERVICE 0x1018b780
	// TEMPLATE: ROBLOX 0x004b0500
	// RBX::Name::callDoDeclare<&RBX::Network::sServer>
	// TEMPLATE: WEBSERVICE 0x1018b790
	// TEMPLATE: ROBLOX 0x004b8150
	// RBX::Name::callDoDeclare<&RBX::sNetworkSettings>
	// TEMPLATE: WEBSERVICE 0x101aa0c0
	// TEMPLATE: ROBLOX 0x004d31f0
	// RBX::Name::callDoDeclare<&RBX::Network::sReplicator>
	template <const char* name>
	static void callDoDeclare()
	{
		doDeclare<name>();
	}

	// clang-format off
	// SYNTHETIC: WEBSERVICE 0x10221870
	// SYNTHETIC: ROBLOX 0x007f1ba0
	// `RBX::moo2'::`2'::`dynamic atexit destructor for 'mutex2''
	// SYNTHETIC: WEBSERVICE 0x10221880
	// SYNTHETIC: ROBLOX 0x007f1bb0
	// `RBX::Name::dictionary'::`2'::`dynamic atexit destructor for 'd''
	// SYNTHETIC: WEBSERVICE 0x102218c0
	// SYNTHETIC: ROBLOX 0x007f1bf0
	// `RBX::Name::namMap'::`2'::`dynamic atexit destructor for 'n''
	// clang-format on

	int dictionaryIndex; // 0x00

public:
	std::string name; // 0x04
};

// SIZE 0x04
class INamed
{
public:
	virtual const Name& getName() const = 0; // vtable+0x00
};

template <class Base, const char* nameValue>
class __declspec(novtable) Named : public Base
{
public:
	template <class A>
	Named(A a) : Base(a)
	{
	}

	template <class A, class B, class C>
	Named(A a, B b, C c) : Base(a, b, c)
	{
	}

	static const Name& name() { return Name::declare<nameValue>(); }

	virtual const Name& getName() const { return name(); }

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x1006c110
	// TEMPLATE: ROBLOX 0x005b5840
	// RBX::Named<RBX::ArrowToolBase,&RBX::sArrowTool>::getName
	// TEMPLATE: WEBSERVICE 0x100bfa10
	// TEMPLATE: ROBLOX 0x00621450
	// RBX::Named<RBX::MouseCommand,&RBX::sBoxSelectCommand>::getName
	// TEMPLATE: WEBSERVICE 0x100c5340
	// TEMPLATE: ROBLOX 0x00622ba0
	// RBX::Named<RBX::MouseCommand,&RBX::sNewNullTool>::getName
	// TEMPLATE: WEBSERVICE 0x100cabf0
	// TEMPLATE: ROBLOX 0x0060f950
	// RBX::Named<RBX::ScriptMouseCommand,&RBX::sToolMouseCommand>::getName
	// TEMPLATE: WEBSERVICE 0x10106770
	// TEMPLATE: ROBLOX 0x00640df0
	// RBX::Named<RBX::Balancing,&RBX::sRunning>::getName
	// TEMPLATE: WEBSERVICE 0x10107ea0
	// TEMPLATE: ROBLOX 0x00641af0
	// RBX::Named<RBX::Humanoid::State,&RBX::sFallingDown>::getName
	// TEMPLATE: WEBSERVICE 0x10122ac0
	// TEMPLATE: ROBLOX 0x00663c40
	// RBX::Named<RBX::Humanoid::State,&RBX::sSeated>::getName
	// TEMPLATE: WEBSERVICE 0x10122ea0
	// TEMPLATE: ROBLOX 0x00664020
	// RBX::Named<RBX::Flying,&RBX::sJumping>::getName
	// TEMPLATE: WEBSERVICE 0x101231e0
	// TEMPLATE: ROBLOX 0x006642a0
	// RBX::Named<RBX::Balancing,&RBX::sGettingUp>::getName
	// TEMPLATE: WEBSERVICE 0x10126330
	// TEMPLATE: ROBLOX 0x00667970
	// RBX::Named<RBX::MouseCommand,&RBX::sGroupDragTool>::getName
	// TEMPLATE: WEBSERVICE 0x10126a20
	// TEMPLATE: ROBLOX 0x00662ee0
	// RBX::Named<RBX::MouseCommand,&RBX::sPartDragTool>::getName
	// TEMPLATE: WEBSERVICE 0x1012a510
	// TEMPLATE: ROBLOX 0x00669f20
	// RBX::Named<RBX::Balancing,&RBX::sFlying>::getName
	// TEMPLATE: WEBSERVICE 0x1012a650
	// TEMPLATE: ROBLOX 0x0066a060
	// RBX::Named<RBX::Flying,&RBX::sFreefall>::getName
	// clang-format on
};

} // namespace RBX

#endif // UTIL_NAME_H
