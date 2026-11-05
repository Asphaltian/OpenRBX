#ifndef VERBBINDER_H
#define VERBBINDER_H

#include "decomp.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Verb.h"

#include <boost/shared_ptr.hpp>
#include <exception>
#include <map>

// clang-format off
// TEMPLATE: ROBLOX 0x0045dfc0
// ??0?$VerbBinder@VCRobloxView@@VCView@@@@QAE@XZ
// STUB: ROBLOX 0x0045e040
// ?OnCmdMsg@?$VerbBinder@VCRobloxView@@VCView@@@@MAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
// TEMPLATE: ROBLOX 0x0045e230
// ??1?$VerbBinder@VCRobloxView@@VCView@@@@UAE@XZ
// SYNTHETIC: ROBLOX 0x0045e2c0
// VerbBinder<CRobloxView,CView>::`scalar deleting destructor'
// clang-format on

template <class T, class Base>
class VerbBinder : public Base
{
protected:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
	{
		if (nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI) {
			typename std::map<UINT, const RBX::Name*>::iterator it = m_unk0x58.find(nID);
			if (it != m_unk0x58.end() && it->second) {
				boost::shared_ptr<RBX::DataModel> dataModel = static_cast<T*>(this)->FUN_0045d8e0();
				RBX::DataModel::Lock lock(dataModel);
				RBX::Verb* verb = static_cast<RBX::VerbContainer*>(static_cast<T*>(this))->getVerb(*it->second);
				if (verb) {
					if (nCode == CN_COMMAND) {
						try {
							verb->doIt(dataModel.get());
						}
						catch (std::exception& e) {
							AfxMessageBox(e.what());
						}
					}
					else {
						CCmdUI* cmdUI = static_cast<CCmdUI*>(pExtra);
						cmdUI->Enable(verb->isEnabled());
						cmdUI->SetCheck(verb->isChecked());
					}
				}
				return TRUE;
			}
		}
		return Base::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}

	std::map<UINT, const RBX::Name*> m_unk0x58; // 0x58
};

#endif // VERBBINDER_H
