#ifndef NETWORK_PLAYER_H
#define NETWORK_PLAYER_H

#include "decomp.h"
#include "util/Events.h"
#include "v8datamodel/BrickColor.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>
#include <string>

namespace RBX {

class ModelInstance;

namespace Network {

extern char sPlayer[];

// SIZE 0x8
struct CharacterAdded
{
	const boost::shared_ptr<Instance> character; // 0x00

	CharacterAdded(boost::shared_ptr<Instance> character) : character(character) {}
};

DECOMP_SIZE_ASSERT(CharacterAdded, 0x8)

// SIZE 0x8
struct CharacterRemoving
{
	const boost::shared_ptr<Instance> character; // 0x00

	CharacterRemoving(boost::shared_ptr<Instance> character) : character(character) {}
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
	double lastActivityTime;                                   // 0x170

	virtual bool askAddChild(const Instance* instance) const { return true; }

	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

public:
	Player();
	virtual ~Player();

	// SYNTHETIC: WEBSERVICE 0x10190170
	// RBX::Network::Player::`scalar deleting destructor'

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
};

DECOMP_SIZE_ASSERT(Player, 0x178)

} // namespace Network
} // namespace RBX

#endif // NETWORK_PLAYER_H
