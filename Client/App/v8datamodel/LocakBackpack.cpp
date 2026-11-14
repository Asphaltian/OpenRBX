#include "compat.h"
#include "network/Player.h"
#include "network/Players.h"
#include "util/Events.h"
#include "v8datamodel/Backpack.h"
#include "v8datamodel/LocalBackpack.h"

#include <algorithm>

namespace RBX {

const char sLocalBackpack[] = "LocalBackpack";

const char sLocalBackpackItem[] = "LocalBackpackItem";

G3D::Vector2 LocalBackpackItem::getSize() const
{
	float size = GuiRoot::toPixelSize(G3D::Vector2(10, 10)).x;
	return G3D::Vector2(size, size);
}

void LocalBackpackItem::render2d(Adorn* adorn)
{
	int fontSize = GuiRoot::normalizedFontSize(12);

	Rect draw = getMyRect();
	float numberSize = draw.size().x * 0.2f;
	Rect numberRect = Rect::fromLowSize(
		G3D::Vector2(draw.low.x + 2, draw.high.y - 2 - numberSize), G3D::Vector2(numberSize, numberSize)
	);

	adorn->rect2d(draw.toRect2D(), GuiItem::translucentBackdrop());

	adorn->rect2d(numberRect.toRect2D(), G3D::Color4(G3D::Color3::gray(), 1));

	adorn->drawFont2D(
		StringConverter<int>::convertToString(getBinId()),
		numberRect.center(),
		fontSize,
		G3D::Color3::white(),
		G3D::Color4::clear(),
		Adorn::XALIGN_CENTER,
		Adorn::YALIGN_CENTER,
		Adorn::PROPORTIONAL_SPACING
	);

	if (myItem) {
		if (myItem->drawSelected()) {
			adorn->outlineRect2d(draw.toRect2D(), 3.0f, G3D::Color4(G3D::Color3::green(), 1));
		}

		if (guiImageDraw.setImage(adorn, myItem->getTextureId())) {
#ifdef COMPAT_MODE
			Rect imageRect = draw.inset(fontSize);
			guiImageDraw.render2d(adorn, myItem->drawEnabled(), imageRect, widgetState);
#else
			guiImageDraw.render2d(adorn, myItem->drawEnabled(), draw.inset(fontSize), widgetState);
#endif
			return;
		}

		setName(myItem->getName());
	}

	Widget::render2d(adorn);
}

void LocalBackpack::removeBackpackItem(BackpackItem* item)
{
	for (unsigned int i = 0; i < numChildren(); ++i) {
		LocalBackpackItem* localItem = static_cast<LocalBackpackItem*>(getChild(i));
		if (localItem->getItem() == item) {
			lastRemovedIndex = i;
			localItem->setParent(NULL);
			if (item == pendingClick.get()) {
				pendingClick.reset();
			}
			return;
		}
	}

	debugAssert(false);
}

void LocalBackpack::onHeartbeat()
{
	if (pendingClick.get()) {
		boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > c = getChildren().read();
		if (c) {
			for (std::vector<boost::shared_ptr<Instance> >::const_iterator iter = c->begin(); iter != c->end();
				 ++iter) {
				LocalBackpackItem* item = static_cast<LocalBackpackItem*>(iter->get());
				if (item->getItem() != pendingClick.get()) {
					item->getItem()->onLocalOtherClicked();
				}
			}
		}

		pendingClick->onLocalClicked();
		pendingClick.reset();
	}
}

void LocalBackpack::onEvent(const Instance* source, DescendentRemoving event)
{
	if (BackpackItem* item = dynamic_cast<BackpackItem*>(event.instance.get())) {
		removeBackpackItem(item);
	}
}

void LocalBackpack::onClick(LocalBackpackItem* clickedItem)
{
	pendingClick = shared_from(clickedItem->getItem());
}

void LocalBackpackItem::onClick(const GuiEvent& event)
{
	static_cast<LocalBackpack*>(getParent())->onClick(this);
}

void LocalBackpack::clearLocalCharacter()
{
	if (localCharacter) {
		stopListening(localCharacter, static_cast<Listener<Instance, DescendentAdded>*>(this));
		stopListening(localCharacter, static_cast<Listener<Instance, DescendentRemoving>*>(this));
		localCharacter.reset();
	}
}

void LocalBackpack::clearLocalPlayer()
{
	removeAllChildren();

	if (localPlayer) {
		stopListening(localPlayer, static_cast<Listener<Instance, ChildAdded>*>(this));
		stopListening(localPlayer, static_cast<Listener<Instance, ChildRemoved>*>(this));
		stopListening(localPlayer, static_cast<Listener<Network::Player, Network::CharacterAdded>*>(this));
		stopListening(localPlayer, static_cast<Listener<Network::Player, Network::CharacterRemoving>*>(this));
		localPlayer.reset();
	}
}

void LocalBackpack::clearLocalBackpack()
{
	removeAllChildren();

	if (localBackpack) {
		stopListening(localBackpack, static_cast<Listener<Instance, ChildAdded>*>(this));
		stopListening(localBackpack, static_cast<Listener<Instance, ChildRemoved>*>(this));
		localBackpack.reset();
	}

	lastRemovedIndex = -1;
}

bool LocalBackpack::onKey(int key)
{
	if (key <= numChildren()) {
		onClick(static_cast<LocalBackpackItem*>(getChild(key - 1)));
		return true;
	}

	return false;
}

void LocalBackpack::cycleUp()
{
}

void LocalBackpack::cycleDown()
{
}

void LocalBackpack::onEvent(const Instance* source, ChildRemoved event)
{
	if (Network::Player* player = dynamic_cast<Network::Player*>(event.child.get())) {
		if (player == localPlayer.get()) {
			clearLocalPlayer();
		}
	}
	else if (dynamic_cast<Backpack*>(event.child.get())) {
		clearLocalBackpack();
	}
	else if (BackpackItem* item = dynamic_cast<BackpackItem*>(event.child.get())) {
		removeBackpackItem(item);
	}
}

void LocalBackpack::onEvent(const Network::Player* source, Network::CharacterRemoving event)
{
	if (localCharacter) {
		clearLocalCharacter();
	}
}

void LocalBackpack::clearAll()
{
	clearLocalCharacter();
	clearLocalPlayer();
	clearLocalBackpack();
	if (players) {
		stopListening(players.get(), static_cast<Listener<Instance, ChildAdded>*>(this));
		stopListening(players, static_cast<Listener<Instance, ChildRemoved>*>(this));
		players.reset();
	}
}

LocalBackpack::~LocalBackpack()
{
	clearAll();
}

void LocalBackpack::onLocalCharacterAdded(Instance* added)
{
	localCharacter = shared_from(added);

	startListening(localCharacter, static_cast<Listener<Instance, DescendentAdded>*>(this));
	startListening(localCharacter, static_cast<Listener<Instance, DescendentRemoving>*>(this));
}

void LocalBackpack::onLocalPlayerAdded(Network::Player* added)
{
	localPlayer = shared_from(added);

	startListening(localPlayer, static_cast<Listener<Instance, ChildAdded>*>(this));
	startListening(localPlayer, static_cast<Listener<Instance, ChildRemoved>*>(this));
	startListening(localPlayer, static_cast<Listener<Network::Player, Network::CharacterAdded>*>(this));
	startListening(localPlayer, static_cast<Listener<Network::Player, Network::CharacterRemoving>*>(this));
}

void LocalBackpack::onLocalBackpackAdded(Backpack* added)
{
	localBackpack = shared_from(added);

	startListening(localBackpack, static_cast<Listener<Instance, ChildAdded>*>(this));
	startListening(localBackpack, static_cast<Listener<Instance, ChildRemoved>*>(this));
}

void LocalBackpack::onEvent(const Network::Player* source, Network::CharacterAdded event)
{
	onLocalCharacterAdded(event.character.get());
}

LocalBackpackItem::LocalBackpackItem()
{
}

LocalBackpack::LocalBackpack() : lastRemovedIndex(-1)
{
	yLocation = Rect::BOTTOM;
}

void LocalBackpack::onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider)
{
	if (oldProvider) {
		clearAll();
	}

	Instance::onServiceProvider(oldProvider, newProvider);

	if (newProvider) {
		players = shared_from(ServiceProvider::create<Network::Players>(this));
		startListening(players, static_cast<Listener<Instance, ChildAdded>*>(this));
		startListening(players, static_cast<Listener<Instance, ChildRemoved>*>(this));
	}
}

void LocalBackpack::insertBackpackItem(BackpackItem* item)
{
	boost::shared_ptr<LocalBackpackItem> localItem = Creatable<Instance>::create<LocalBackpackItem>();

	localItem->setParent(this);

	int maxId = numChildren() - 1;

	int insertIndex = lastRemovedIndex >= 0 ? std::min(lastRemovedIndex, maxId) : maxId;

	for (int i = maxId; i > insertIndex; --i) {
		LocalBackpackItem* prev = static_cast<LocalBackpackItem*>(getChild(i - 1));
		LocalBackpackItem* next = static_cast<LocalBackpackItem*>(getChild(i));
		next->setItem(prev->getItem());
	}

	static_cast<LocalBackpackItem*>(getChild(insertIndex))->setItem(item);

	lastRemovedIndex = -1;
}

void LocalBackpack::onEvent(const Instance* source, ChildAdded event)
{
	if (Network::Player* player = dynamic_cast<Network::Player*>(event.child.get())) {
		if (player == Network::Players::findLocalPlayer(this)) {
			onLocalPlayerAdded(player);
		}
	}
	else if (Backpack* backpack = dynamic_cast<Backpack*>(event.child.get())) {
		onLocalBackpackAdded(backpack);
	}
	else if (BackpackItem* item = dynamic_cast<BackpackItem*>(event.child.get())) {
		insertBackpackItem(item);
	}
}

void LocalBackpack::onEvent(const Instance* source, DescendentAdded event)
{
	if (BackpackItem* item = dynamic_cast<BackpackItem*>(event.instance.get())) {
		insertBackpackItem(item);
	}
}

} // namespace RBX

template boost::shared_ptr<RBX::Network::Player> RBX::shared_from(RBX::Network::Player* r);
template void RBX::Notifier<RBX::Network::Player, RBX::Network::CharacterAdded>::addListener(
	RBX::Listener<RBX::Network::Player, RBX::Network::CharacterAdded>* listener
) const;
template void RBX::Notifier<RBX::Network::Player, RBX::Network::CharacterAdded>::removeListener(
	RBX::Listener<RBX::Network::Player, RBX::Network::CharacterAdded>* listener
) const;
template void RBX::Notifier<RBX::Network::Player, RBX::Network::CharacterRemoving>::addListener(
	RBX::Listener<RBX::Network::Player, RBX::Network::CharacterRemoving>* listener
) const;
template void RBX::Notifier<RBX::Network::Player, RBX::Network::CharacterRemoving>::removeListener(
	RBX::Listener<RBX::Network::Player, RBX::Network::CharacterRemoving>* listener
) const;
