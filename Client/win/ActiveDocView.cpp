#include "ActiveDocView.h"

#include "script/Script.h"
#include "script/ScriptContext.h"
#include "util/RunStateOwner.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Selection.h"

CRobloxDoc* ActiveDocView::g_unk0x00952d8c;

std::set<ActiveDocView*> ActiveDocView::g_unk0x00952d90;

boost::shared_ptr<RBX::DataModel> ActiveDocView::g_unk0x00952d9c;

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

ActiveDocView::ActiveDocView()
{
	g_unk0x00952d90.insert(this);
}

ActiveDocView::~ActiveDocView()
{
	g_unk0x00952d90.erase(this);
}

template const RBX::FactoryProduct<RBX::ModelInstance, RBX::PVInstance, RBX::sModel>::Creator
	RBX::FactoryProduct<RBX::ModelInstance, RBX::PVInstance, RBX::sModel>::creator;

template const RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>::Creator
	RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>::creator;

template const RBX::FactoryProduct<RBX::RunService, RBX::Instance, RBX::sRunService>::Creator
	RBX::FactoryProduct<RBX::RunService, RBX::Instance, RBX::sRunService>::creator;

template const RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>::Creator
	RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>::creator;

template const RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript>::Creator
	RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript>::creator;

template const RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript>::Creator
	RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript>::creator;
