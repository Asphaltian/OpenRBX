#ifndef ACTIVEDOCVIEW_H
#define ACTIVEDOCVIEW_H

#include "RobloxDoc.h"
#include "placeholder.h"
#include "v8datamodel/DataModel.h"

#include <boost/shared_ptr.hpp>
#include <set>

class ActiveDocView
{
public:
	static void FUN_004054f0(bool activate, CRobloxDoc* doc);

	static boost::shared_ptr<RBX::DataModel> FUN_00434840() { return g_unk0x00952d9c; }

	void FUN_004204e0() { VTable0x00(g_unk0x00952d9c, boost::shared_ptr<RBX::DataModel>()); }

	static CRobloxDoc* g_unk0x00952d8c;

protected:
	ActiveDocView();
	~ActiveDocView();

	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
	) = 0;

	friend class CRobloxDoc;

	static std::set<ActiveDocView*> g_unk0x00952d90;
	static boost::shared_ptr<RBX::DataModel> g_unk0x00952d9c;
};

#endif // ACTIVEDOCVIEW_H
