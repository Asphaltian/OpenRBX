#ifndef V8DATAMODEL_LOCALBACKPACK_H
#define V8DATAMODEL_LOCALBACKPACK_H

#include "gui/GUI.h"
#include "gui/Widget.h"
#include "network/Player.h"
#include "placeholder.h"
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

class LocalBackpackItem : public DescribedCreatable<LocalBackpackItem, Widget, sLocalBackpackItem>
{
private:
	boost::shared_ptr<BackpackItem> myItem;
	GuiDrawImage guiImageDraw;

	virtual bool askSetParent(const Instance* instance) const { return true; }
	virtual G3D::Vector2 getSize() const;
	virtual void render2d(Adorn* adorn);
	virtual void onClick(const GuiEvent& event);
	int getBinId() const { return getParent()->findChildIndex(this) + 1; }

public:
	LocalBackpackItem();

	void setItem(BackpackItem* item) { myItem = shared_from(item); }

	BackpackItem* getItem() { return myItem.get(); }
};

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
	boost::shared_ptr<Network::Players> players;
	boost::shared_ptr<Network::Player> localPlayer;
	boost::shared_ptr<Backpack> localBackpack;
	boost::shared_ptr<Instance> localCharacter;
	boost::shared_ptr<BackpackItem> pendingClick;
	int lastRemovedIndex;

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

	void onHeartbeat();
	void onClick(LocalBackpackItem* clickedItem);
	bool onKey(int key);
	void cycleUp();
	void cycleDown();

	virtual XmlElement* write() { return NULL; }
};

} // namespace RBX

#endif // V8DATAMODEL_LOCALBACKPACK_H
