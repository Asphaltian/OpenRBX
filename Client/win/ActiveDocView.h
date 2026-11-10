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
	static void FUN_004054f0(bool activate, CRobloxDoc* doc);

	// FUNCTION: ROBLOX 0x00434840
	static boost::shared_ptr<RBX::DataModel> FUN_00434840() { return g_unk0x00952d9c; }

	// FUNCTION: ROBLOX 0x004204e0
	void FUN_004204e0() { VTable0x00(g_unk0x00952d9c, boost::shared_ptr<RBX::DataModel>()); }

protected:
	ActiveDocView();
	~ActiveDocView();

	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel,
		boost::shared_ptr<RBX::DataModel> oldDataModel
	) = 0; // vtable+0x00

private:
	friend class CRobloxDoc;

	static CRobloxDoc* g_unk0x00952d8c;
	static std::set<ActiveDocView*> g_unk0x00952d90;
	static boost::shared_ptr<RBX::DataModel> g_unk0x00952d9c;
};

DECOMP_SIZE_ASSERT(ActiveDocView, 0x4)

#endif // ACTIVEDOCVIEW_H
