#ifndef ACTIVEDOCVIEW_H
#define ACTIVEDOCVIEW_H

#include "RobloxDoc.h"
#include "decomp.h"
#include "v8datamodel/DataModel.h"

#include <boost/shared_ptr.hpp>
#include <set>

// SIZE 0x4
class ActiveDocView
{
public:
	// FUNCTION: ROBLOX 0x004054f0
	static void FUN_004054f0(bool activate, CRobloxDoc* doc)
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

protected:
	// FUNCTION: ROBLOX 0x00406090
	ActiveDocView() { g_unk0x00952d90.insert(this); }

	// FUNCTION: ROBLOX 0x004060c0
	~ActiveDocView() { g_unk0x00952d90.erase(this); }

	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel,
		boost::shared_ptr<RBX::DataModel> oldDataModel
	) = 0; // vtable+0x00

private:
	static CRobloxDoc* g_unk0x00952d8c;
	static std::set<ActiveDocView*> g_unk0x00952d90;
	static boost::shared_ptr<RBX::DataModel> g_unk0x00952d9c;
};

DECOMP_SIZE_ASSERT(ActiveDocView, 0x4)

#endif // ACTIVEDOCVIEW_H
