#ifndef NETWORK_PLAYER_H
#define NETWORK_PLAYER_H

#include "decomp.h"
#include "reflection/property.h"
#include "reflection/signal.h"
#include "util/Events.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <string>

namespace RBX {

class Backpack;
class ModelInstance;

namespace Network {

extern char sPlayer[];

// SIZE 0x8
struct CharacterAdded
{
	const boost::shared_ptr<Instance> character; // 0x00

	CharacterAdded(Instance* character) : character(shared_from(character)) {}
};

DECOMP_SIZE_ASSERT(CharacterAdded, 0x8)

// SIZE 0x8
struct CharacterRemoving
{
	const boost::shared_ptr<Instance> character; // 0x00

	CharacterRemoving(Instance* character) : character(shared_from(character)) {}
};

DECOMP_SIZE_ASSERT(CharacterRemoving, 0x8)

// SIZE 0x178
class Player : public DescribedCreatable<Player, Instance, sPlayer>,
			   public Notifier<Player, CharacterAdded>,
			   public Notifier<Player, CharacterRemoving>
{
	boost::shared_ptr<ModelInstance> character;                // 0x128
	BrickColor teamColor;                                      // 0x130
	bool neutral;                                              // 0x134
	boost::signals::scoped_connection characterDiedConnection; // 0x138
	std::string characterAppearance;                           // 0x14c
	bool under13;                                              // 0x168
	bool superSafeChat;                                        // 0x169
	int userId;                                                // 0x16c

	virtual bool askAddChild(const Instance* instance) const { return true; }

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

	void onCharacterChangedFrontend();
	void registerLocalPlayerNotIdle();

public:
	static Reflection::BoundProp<int, 1> prop_userId;
	static Reflection::BoundProp<bool, 1> prop_Under13;
	static Reflection::BoundProp<bool, 1> prop_SuperSafeChat;

	Player();
	virtual ~Player();

	// SYNTHETIC: WEBSERVICE 0x10190170
	// RBX::Network::Player::`scalar deleting destructor'

	static Reflection::SignalDesc<Player, void(std::string, boost::shared_ptr<Instance>)> event_Chatted;

	virtual XmlElement* write() { return NULL; }

	virtual void setName(const std::string& name);

	// FUNCTION: WEBSERVICE 0x100ca220
	ModelInstance* getCharacter() const { return character.get(); }

	void setCharacter(ModelInstance* value);

	// FUNCTION: WEBSERVICE 0x100cd080
	BrickColor getTeamColor() const { return teamColor; }

	void setTeamColor(BrickColor value);
	// FUNCTION: WEBSERVICE 0x100cd090
	bool getNeutral() const { return neutral; }

	void setNeutral(bool value);

	std::string getCharacterAppearance() const;
	void setCharacterAppearance(const std::string& value);

	bool getUnder13() const { return under13; }
	bool getSuperSafeChat() const { return superSafeChat; }
	void setUnder13(bool value);
	void setSuperSafeChat(bool value);

	int getUserID() const { return userId; }

	void rebuildBackpack();
	Backpack* getPlayerBackpack() const;

	void loadCharacter();
	void removeCharacter();
	void removeCharacterAppearance();
	void loadCharacterAppearance();

private:
	void doPeriodicIdleCheck();

	double lastActivityTime; // 0x170
};

DECOMP_SIZE_ASSERT(Player, 0x178)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021f220
// RBX::Network::`dynamic initializer for 'loadCharacterFunction''
// SYNTHETIC: WEBSERVICE 0x1021f250
// RBX::Network::`dynamic initializer for 'removeCharacterFunction''
// SYNTHETIC: WEBSERVICE 0x1021f280
// RBX::Network::`dynamic initializer for 'func_SetUnder13''
// SYNTHETIC: WEBSERVICE 0x1021f2b0
// RBX::Network::`dynamic initializer for 'func_SetSuperSafeChat''
// SYNTHETIC: WEBSERVICE 0x1021f2e0
// RBX::Network::`dynamic initializer for 'prop_Character''
// SYNTHETIC: WEBSERVICE 0x1021f320
// RBX::Network::`dynamic initializer for 'prop_characterAppearance''
// SYNTHETIC: WEBSERVICE 0x1021f360
// `dynamic initializer for 'RBX::Network::Player::prop_userId''
// SYNTHETIC: WEBSERVICE 0x1021f390
// `dynamic initializer for 'RBX::Network::Player::prop_Under13''
// SYNTHETIC: WEBSERVICE 0x1021f3c0
// `dynamic initializer for 'RBX::Network::Player::prop_SuperSafeChat''
// SYNTHETIC: WEBSERVICE 0x1021f3f0
// `dynamic initializer for 'RBX::Network::Player::event_Chatted''
// SYNTHETIC: WEBSERVICE 0x1021f420
// RBX::Network::`dynamic initializer for 'prop_teamColor''
// SYNTHETIC: WEBSERVICE 0x1021f460
// RBX::Network::`dynamic initializer for 'prop_neutral''
// SYNTHETIC: WEBSERVICE 0x1021f4a0
// RBX::Network::`dynamic initializer for 'event_Idled''
// SYNTHETIC: WEBSERVICE 0x10225e50
// RBX::Network::`dynamic atexit destructor for 'prop_Character''
// SYNTHETIC: WEBSERVICE 0x10225e70
// RBX::Network::`dynamic atexit destructor for 'prop_neutral''
// SYNTHETIC: WEBSERVICE 0x10225e90
// `dynamic atexit destructor for 'RBX::Network::Player::prop_SuperSafeChat''
// SYNTHETIC: WEBSERVICE 0x10225eb0
// `dynamic atexit destructor for 'RBX::Network::Player::prop_Under13''
// SYNTHETIC: WEBSERVICE 0x10225ed0
// `dynamic atexit destructor for 'RBX::Network::Player::prop_userId''
// SYNTHETIC: WEBSERVICE 0x10225ef0
// RBX::Network::`dynamic atexit destructor for 'prop_characterAppearance''
// SYNTHETIC: WEBSERVICE 0x10225f10
// RBX::Network::`dynamic atexit destructor for 'prop_teamColor''
// SYNTHETIC: WEBSERVICE 0x10225f30
// RBX::Network::`dynamic atexit destructor for 'func_SetSuperSafeChat''
// SYNTHETIC: WEBSERVICE 0x10225f40
// RBX::Network::`dynamic atexit destructor for 'removeCharacterFunction''
// SYNTHETIC: WEBSERVICE 0x10225f50
// RBX::Network::`dynamic atexit destructor for 'func_SetUnder13''
// SYNTHETIC: WEBSERVICE 0x10225f60
// RBX::Network::`dynamic atexit destructor for 'event_Idled''
// SYNTHETIC: WEBSERVICE 0x10225f70
// RBX::Network::`dynamic atexit destructor for 'loadCharacterFunction''
// SYNTHETIC: WEBSERVICE 0x10225f80
// `dynamic atexit destructor for 'RBX::Network::Player::event_Chatted''
// clang-format on

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYER_H
