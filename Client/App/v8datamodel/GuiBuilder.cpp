#include "v8datamodel/GuiBuilder.h"

#include "gui/ArrowPanel.h"
#include "gui/Button.h"
#include "gui/ChatOutput.h"
#include "gui/ChatWidget.h"
#include "gui/EquationDisplay.h"
#include "gui/GUI.h"
#include "gui/HealthHud.h"
#include "gui/Layout.h"
#include "gui/MenuItem.h"
#include "gui/ScoreHud.h"
#include "util/object.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/SafeChat.h"

#include <boost/lexical_cast.hpp>

namespace RBX {

// FUNCTION: ROBLOX 0x00613ef0
boost::shared_ptr<TopMenuBar> GuiBuilder::buildMenu()
{
	Layout layout;
	layout.xLocation = Rect::LEFT;
	layout.yLocation = Rect::TOP;
	layout.layoutStyle = Layout::HORIZONTAL;
	layout.backdropColor = GuiItem::translucentBackdrop();

	boost::shared_ptr<RelativePanel> menu = Creatable<Instance>::create<RelativePanel>(layout);
	menu->setName("MainMenu");

	menu->addGuiItem(Creatable<Instance>::create<MenuItem>(getVerb("TogglePlayMode"), "Tools"));
	menu->addGuiItem(Creatable<Instance>::create<MenuItem>(getVerb("ToggleToolbox"), "Insert"));
	menu->addGuiItem(Creatable<Instance>::create<MenuItem>(getVerb("ToggleFullScreen"), "Fullscreen"));
	menu->addGuiItem(Creatable<Instance>::create<MenuItem>(getVerb("Help"), "Help..."));
	menu->addGuiItem(Creatable<Instance>::create<MenuItem>(getVerb("Exit"), "   Exit"));

	for (unsigned int i = 0; i < menu->numChildren(); ++i) {
		GuiItem* item = menu->getGuiItem(i);
		if (item != NULL) {
			item->setGuiSize(G3D::Vector2(100, 20));
		}
	}

	return menu;
}

// FUNCTION: ROBLOX 0x00614430
void GuiBuilder::buildRightPaletteItems(Adorn* adorn, boost::shared_ptr<TopMenuBar> palette)
{
	palette->addGuiItem(
		Creatable<Instance>::create<ImageButton>(adorn, getVerb("FirstPersonIndicator"), "FirstPersonIndicator")
	);
	palette->addGuiItem(Creatable<Instance>::create<ImageButton>(adorn, getVerb("ReportAbuse"), "ReportAbuse"));

	boost::shared_ptr<TopMenuBar> cameraLeft = Creatable<Instance>::create<TopMenuBar>("CameraLeft", Layout::VERTICAL);
	cameraLeft->addGuiItem(Creatable<Instance>::create<ImageButton>(adorn, getVerb("CameraTiltUp"), "Tilt Up"));
	cameraLeft->addGuiItem(Creatable<Instance>::create<ImageButton>(adorn, getVerb("CameraTiltDown"), "Tilt Down"));

	boost::shared_ptr<TopMenuBar> cameraRight =
		Creatable<Instance>::create<TopMenuBar>("CameraRight", Layout::VERTICAL);
	cameraRight->addGuiItem(Creatable<Instance>::create<ImageButton>(adorn, getVerb("CameraZoomIn"), "Zoom In"));
	cameraRight->addGuiItem(Creatable<Instance>::create<ImageButton>(adorn, getVerb("CameraZoomOut"), "Zoom Out"));

	boost::shared_ptr<TopMenuBar> cameraPanel =
		Creatable<Instance>::create<TopMenuBar>("CameraPanel", Layout::HORIZONTAL);
	cameraPanel->addGuiItem(cameraLeft);
	cameraPanel->addGuiItem(cameraRight);

	palette->addGuiItem(cameraPanel);
}

// FUNCTION: ROBLOX 0x00614ab0
boost::shared_ptr<TopMenuBar> GuiBuilder::buildChatHud()
{
	Layout layout;
	layout.xLocation = Rect::LEFT;
	layout.yLocation = Rect::TOP;
	layout.offset.x = 25;
	layout.offset.y = 100;
	layout.layoutStyle = Layout::VERTICAL;

	boost::shared_ptr<RelativePanel> hud = Creatable<Instance>::create<RelativePanel>(layout);
	hud->setName("ChatHud");
	hud->addGuiItem(Creatable<Instance>::create<ChatOutput>());

	return hud;
}

// FUNCTION: ROBLOX 0x00614c50
void GuiBuilder::buildChatMenu(ChatOption* option, std::string prefix, boost::shared_ptr<UnifiedWidget> parent)
{
	for (int i = 0; i < option->children.size(); ++i) {
		ChatOption* child = option->children[i];
		std::string code = prefix + " " + boost::lexical_cast<std::string>(i);

		boost::shared_ptr<ChatWidget> widget = Creatable<Instance>::create<ChatWidget>(child->text, code);
		parent->addGuiItem(widget);

		buildChatMenu(child, code, widget);
	}
}

// FUNCTION: ROBLOX 0x00614ec0
boost::shared_ptr<TopMenuBar> GuiBuilder::buildChatMenu(Adorn* adorn)
{
	boost::shared_ptr<PercentPanel> panel =
		Creatable<Instance>::create<PercentPanel>(Rect::LEFT, Rect::BOTTOM, G3D::Vector2(2.0f, 26.0f));
	panel->setName("ChatMenuPanel");

	boost::shared_ptr<ChatButton> chatButton = Creatable<Instance>::create<ChatButton>(adorn, "Chat");
	panel->addGuiItem(chatButton);

	ChatOption* root = SafeChat::singleton().getChatRoot();
	std::string prefix = "";
	buildChatMenu(root, prefix, chatButton);

	return panel;
}

// FUNCTION: ROBLOX 0x006150b0
boost::shared_ptr<TopMenuBar> GuiBuilder::buildScoreHud()
{
	Layout layout;
	layout.xLocation = Rect::RIGHT;
	layout.yLocation = Rect::TOP;
	layout.offset.x = 10;
	layout.offset.y = 10;
	layout.layoutStyle = Layout::VERTICAL;

	boost::shared_ptr<RelativePanel> hud = Creatable<Instance>::create<RelativePanel>(layout);
	hud->setName("ScoreHud");
	hud->addGuiItem(Creatable<Instance>::create<ScoreHud>(dataModel));

	return hud;
}

// FUNCTION: ROBLOX 0x00615250
boost::shared_ptr<TopMenuBar> GuiBuilder::buildHealthHud()
{
	Layout layout;
	layout.xLocation = Rect::RIGHT;
	layout.yLocation = Rect::TOP;
	layout.offset.x = 60;
	layout.offset.y = 200;
	layout.layoutStyle = Layout::VERTICAL;

	boost::shared_ptr<RelativePanel> hud = Creatable<Instance>::create<RelativePanel>(layout);
	hud->setName("HealthHud");
	hud->addGuiItem(Creatable<Instance>::create<HealthHud>());

	return hud;
}

// FUNCTION: ROBLOX 0x006153f0
boost::shared_ptr<TopMenuBar> GuiBuilder::buildControlHud(Adorn* adorn, int offset)
{
	Layout layout;
	layout.xLocation = Rect::RIGHT;
	layout.yLocation = Rect::TOP;
	layout.offset.y = offset;
	layout.layoutStyle = Layout::VERTICAL;

	boost::shared_ptr<RelativePanel> hud = Creatable<Instance>::create<RelativePanel>(layout);
	hud->setName("ControlHud");
	hud->addGuiItem(Creatable<Instance>::create<ArrowPanel>(adorn, dataModel, Controller::PRIMARY_CONTROLLER));
	hud->addGuiItem(Creatable<Instance>::create<MenuSpacer>(5.0f, 5.0f));
	hud->addGuiItem(Creatable<Instance>::create<ArrowPanel>(adorn, dataModel, Controller::SECONDARY_CONTROLLER));

	return hud;
}

// FUNCTION: ROBLOX 0x00615800
boost::shared_ptr<TopMenuBar> GuiBuilder::buildRightPalette(Adorn* adorn)
{
	Layout layout;
	layout.xLocation = Rect::RIGHT;
	layout.yLocation = Rect::BOTTOM;
	layout.layoutStyle = Layout::VERTICAL;

	boost::shared_ptr<RelativePanel> palette = Creatable<Instance>::create<RelativePanel>(layout);
	palette->setName("RightPalette");
	buildRightPaletteItems(adorn, palette);

	return palette;
}

// FUNCTION: ROBLOX 0x00615960
boost::shared_ptr<TopMenuBar> GuiBuilder::buildStatsHud1()
{
	Layout layout;
	layout.xLocation = Rect::LEFT;
	layout.yLocation = Rect::BOTTOM;
	layout.offset.x = 50;
	layout.offset.y = 40;
	layout.layoutStyle = Layout::VERTICAL;
	layout.backdropColor = GuiItem::translucentBackdrop();

	boost::shared_ptr<RelativePanel> hud = Creatable<Instance>::create<RelativePanel>(layout);
	hud->setName("StatsHud1");

	hud->addGuiItem(Creatable<Instance>::create<TextDisplay>("", "-- Performance --"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("FPS           : ", "FPS"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Sim           : ", "simDC"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Gfx           : ", "GfxTime"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Mode          : ", "graphicsMode"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Video Memory  : ", "videoMemory"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("FRM  : ", "FRM"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Shadows:  : ", "Shadows"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Anti-Aliasing : ", "antiAliasing"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Bevels        : ", "bevels"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("FrameId       : ", "drawId"));

	for (unsigned int i = 0; i < hud->numChildren(); ++i) {
		boost::shared_ptr<TextDisplay> display = shared_from_dynamic_cast<TextDisplay>(hud->getChild(i));
		display->setFontSize(14);
		display->setFontColor(G3D::Color4(G3D::Color3::purple()));
		display->setBorderColor(G3D::Color4::clear());
		display->setGuiSize(G3D::Vector2(310, 22));
	}

	hud->setVisible(false);

	return hud;
}

// FUNCTION: ROBLOX 0x00616120
boost::shared_ptr<TopMenuBar> GuiBuilder::buildStatsHud2()
{
	Layout layout;
	layout.xLocation = Rect::RIGHT;
	layout.yLocation = Rect::BOTTOM;
	layout.offset.x = 50;
	layout.offset.y = 40;
	layout.layoutStyle = Layout::VERTICAL;
	layout.backdropColor = GuiItem::translucentBackdrop();

	boost::shared_ptr<RelativePanel> hud = Creatable<Instance>::create<RelativePanel>(layout);
	hud->setName("StatsHud2");

	hud->addGuiItem(Creatable<Instance>::create<TextDisplay>("", "----- World ----"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Primitives    : ", "numPrimitives"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Joints        : ", "numJoints"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Contacts      : ", "numContacts"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Tree          : ", "maxTreeDepth"));
	hud->addGuiItem(Creatable<Instance>::create<TextDisplay>("", "----- Contacts ----"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("CtctStageCtcts: ", "numContactsInCollisionStage"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("SteppingCtcts : ", "numSteppingContacts"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("TouchingCtcts : ", "numTouchingContacts"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Pair Hit Ratio: ", "contactPairHitRatio"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("# link(p)     : ", "numLinkCalls"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("H Nodes Out   : ", "numHashNodes"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Max Bucket    : ", "maxBucketSize"));
	hud->addGuiItem(Creatable<Instance>::create<TextDisplay>("", "----- Kernel ----"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Bodies        : ", "numBodies"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Constraints   : ", "numConstraints"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Points        : ", "numPoints"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Energy B      : ", "energyBody"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Energy C      : ", "energyConnector"));
	hud->addGuiItem(Creatable<Instance>::create<EquationDisplay>("Energy T      : ", "energyTotal"));

	for (unsigned int i = 0; i < hud->numChildren(); ++i) {
		boost::shared_ptr<TextDisplay> display = shared_from_dynamic_cast<TextDisplay>(hud->getChild(i));
		display->setFontSize(14);
		display->setFontColor(G3D::Color4(G3D::Color3::purple()));
		display->setBorderColor(G3D::Color4::clear());
		display->setGuiSize(G3D::Vector2(260, 22));
	}

	hud->setVisible(false);

	return hud;
}

// FUNCTION: ROBLOX 0x00616d20
void GuiBuilder::buildGui(Adorn* adorn, DataModel* dataModel, Workspace* workspace)
{
	this->dataModel = dataModel;
	this->workspace = workspace;

	this->dataModel->getGuiRoot()->addGuiItem(buildStatsHud1());
	this->dataModel->getGuiRoot()->addGuiItem(buildStatsHud2());
	this->dataModel->getGuiRoot()->addGuiItem(buildChatHud());
	this->dataModel->getGuiRoot()->addGuiItem(buildChatMenu(adorn));
	this->dataModel->getGuiRoot()->addGuiItem(buildScoreHud());
	this->dataModel->getGuiRoot()->addGuiItem(buildHealthHud());
	this->dataModel->getGuiRoot()->addGuiItem(buildMenu());
	this->dataModel->getGuiRoot()->addGuiItem(buildRightPalette(adorn));
	this->dataModel->getGuiRoot()->addGuiItem(buildControlHud(adorn, 20));
}

} // namespace RBX
