#ifndef V8DATAMODEL_LOCALBACKPACK_H
#define V8DATAMODEL_LOCALBACKPACK_H

#include "decomp.h"
#include "gui/GUI.h"
#include "gui/Widget.h"
#include "network/Player.h"
#include "v8datamodel/Hopper.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>

class XmlElement;

namespace RBX {

class Backpack;
class LocalBackpack;

namespace Network {
class Players;
} // namespace Network

extern const char sLocalBackpack[];

extern const char sLocalBackpackItem[];

// SIZE 0x150
class LocalBackpackItem : public DescribedCreatable<LocalBackpackItem, Widget, sLocalBackpackItem>
{
private:
	boost::shared_ptr<BackpackItem> myItem; // 0x110
	GuiDrawImage guiImageDraw;              // 0x118

	virtual bool askSetParent(const Instance* instance) const { return true; }
	virtual G3D::Vector2 getSize() const;
	virtual void render2d(Adorn* adorn);
	virtual void onClick(const GuiEvent& event);
	int getBinId() const;

public:
	LocalBackpackItem();

	// FUNCTION: WEBSERVICE 0x100d63b0
	void setItem(BackpackItem* item) { myItem = shared_from(item); }

	BackpackItem* getItem() { return myItem.get(); }
};

DECOMP_SIZE_ASSERT(LocalBackpackItem, 0x150)

// SIZE 0x174
class LocalBackpack : public DescribedCreatable<LocalBackpack, RelativePanel, sLocalBackpack>,
					  public Service,
					  public Listener<Instance, ChildAdded>,
					  public Listener<Instance, ChildRemoved>,
					  public Listener<Instance, DescendentAdded>,
					  public Listener<Instance, DescendentRemoving>,
					  public Listener<Network::Player, Network::CharacterAdded>,
					  public Listener<Network::Player, Network::CharacterRemoving>
{
private:
	boost::shared_ptr<Network::Players> players;    // 0x148
	boost::shared_ptr<Network::Player> localPlayer; // 0x150
	boost::shared_ptr<Backpack> localBackpack;      // 0x158
	boost::shared_ptr<Instance> localCharacter;     // 0x160
	boost::shared_ptr<BackpackItem> pendingClick;   // 0x168
	int lastRemovedIndex;                           // 0x170

	virtual bool askSetParent(const Instance* instance) const { return true; }
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);
	virtual void onEvent(const Network::Player* source, Network::CharacterRemoving event);
	virtual void onEvent(const Network::Player* source, Network::CharacterAdded event);
	virtual void onEvent(const Instance* source, DescendentRemoving event);
	virtual void onEvent(const Instance* source, DescendentAdded event);
	virtual void onEvent(const Instance* source, ChildRemoved event);
	virtual void onEvent(const Instance* source, ChildAdded event);

	void onLocalCharacterAdded(Instance* added);
	void onLocalPlayerAdded(Network::Player* added);
	void onLocalBackpackAdded(Backpack* added);
	void clearAll();
	void clearLocalCharacter();
	void clearLocalPlayer();
	void clearLocalBackpack();
	void insertBackpackItem(BackpackItem* item);
	void removeBackpackItem(BackpackItem* item);

public:
	LocalBackpack();
	virtual ~LocalBackpack();

	// SYNTHETIC: WEBSERVICE 0x100d6cc0
	// RBX::LocalBackpack::`scalar deleting destructor'

	void onHeartbeat();
	void onClick(LocalBackpackItem* clickedItem);
	bool onKey(int key);
	void cycleUp();
	void cycleDown();

	virtual XmlElement* write() { return NULL; }
};

DECOMP_SIZE_ASSERT(LocalBackpack, 0x174)

} // namespace RBX

#endif // V8DATAMODEL_LOCALBACKPACK_H
