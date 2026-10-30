#ifndef V8DATAMODEL_GUIBUILDER_H
#define V8DATAMODEL_GUIBUILDER_H

#include "decomp.h"
#include "v8datamodel/Workspace.h"

#include <boost/shared_ptr.hpp>
#include <string>

namespace RBX {

class Adorn;
class ChatOption;
class DataModel;
class GuiRoot;
class TopMenuBar;
class UnifiedWidget;
class Verb;

// SIZE 0x8
class GuiBuilder
{
private:
	boost::shared_ptr<TopMenuBar> buildMenu();
	boost::shared_ptr<TopMenuBar> buildRightPalette(Adorn* adorn);
	boost::shared_ptr<TopMenuBar> buildPanelControlPalette(Adorn* adorn, GuiRoot* guiRoot);
	boost::shared_ptr<TopMenuBar> buildScoreHud();
	boost::shared_ptr<TopMenuBar> buildScoreHud(int offset);
	boost::shared_ptr<TopMenuBar> buildControlHud(Adorn* adorn, int offset);
	boost::shared_ptr<TopMenuBar> buildMessageHud(int offset);
	boost::shared_ptr<TopMenuBar> buildChatHud();
	void buildChatMenu(ChatOption* option, std::string prefix, boost::shared_ptr<UnifiedWidget> parent);
	boost::shared_ptr<TopMenuBar> buildChatMenu(Adorn* adorn);
	boost::shared_ptr<TopMenuBar> buildHealthHud();
	boost::shared_ptr<TopMenuBar> buildStatsHud1();
	boost::shared_ptr<TopMenuBar> buildStatsHud2();
	void buildRightPaletteItems(Adorn* adorn, boost::shared_ptr<TopMenuBar> palette);
	TopMenuBar* buildColorPanel(Adorn* adorn, GuiRoot* guiRoot);

	DataModel* dataModel; // 0x00
	Workspace* workspace; // 0x04

	Verb* getVerb(const std::string& name) { return workspace->getVerb(name); }

public:
	void buildGui(Adorn* adorn, DataModel* dataModel, Workspace* workspace);
};

DECOMP_SIZE_ASSERT(GuiBuilder, 0x8)

} // namespace RBX

#endif // V8DATAMODEL_GUIBUILDER_H
