#include "ActiveDocView.h"

// GLOBAL: ROBLOX 0x00952d8c
CRobloxDoc* ActiveDocView::g_unk0x00952d8c;

// GLOBAL: ROBLOX 0x00952d90
std::set<ActiveDocView*> ActiveDocView::g_unk0x00952d90;

// GLOBAL: ROBLOX 0x00952d9c
boost::shared_ptr<RBX::DataModel> ActiveDocView::g_unk0x00952d9c;

// FUNCTION: ROBLOX 0x004054f0
void ActiveDocView::FUN_004054f0(bool activate, CRobloxDoc* doc)
{
	boost::shared_ptr<RBX::DataModel> oldDataModel = g_unk0x00952d9c;

	if (activate) {
		g_unk0x00952d8c = doc;
		g_unk0x00952d9c = doc->FUN_00403290();
	}
	else {
		if (doc == g_unk0x00952d8c) {
			g_unk0x00952d8c = NULL;
		}

		if (doc->FUN_00403290() == g_unk0x00952d9c) {
			g_unk0x00952d9c.reset();
		}
	}

	if (g_unk0x00952d9c != oldDataModel) {
		for (std::set<ActiveDocView*>::iterator it = g_unk0x00952d90.begin(); it != g_unk0x00952d90.end(); ++it) {
			(*it)->VTable0x00(g_unk0x00952d9c, oldDataModel);
		}
	}
}

// FUNCTION: ROBLOX 0x00406090
ActiveDocView::ActiveDocView()
{
	g_unk0x00952d90.insert(this);
}

// FUNCTION: ROBLOX 0x004060c0
ActiveDocView::~ActiveDocView()
{
	g_unk0x00952d90.erase(this);
}
