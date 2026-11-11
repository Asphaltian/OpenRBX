#ifndef VERBBINDER_H
#define VERBBINDER_H

#include "decomp.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Verb.h"

#include <boost/shared_ptr.hpp>
#include <map>

// clang-format off
// TEMPLATE: ROBLOX 0x00456820
// ??0?$VerbBinder@VCRobloxDoc@@VCDocument@@@@QAE@XZ
// TEMPLATE: ROBLOX 0x004568b0
// ?OnCmdMsg@?$VerbBinder@VCRobloxDoc@@VCDocument@@@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
// TEMPLATE: ROBLOX 0x00456aa0
// ??1?$VerbBinder@VCRobloxDoc@@VCDocument@@@@UAE@XZ
// SYNTHETIC: ROBLOX 0x00456b30
// VerbBinder<CRobloxDoc,CDocument>::`scalar deleting destructor'
// TEMPLATE: ROBLOX 0x0045dfc0
// ??0?$VerbBinder@VCRobloxView@@VCView@@@@QAE@XZ
// TEMPLATE: ROBLOX 0x0045e040
// ?OnCmdMsg@?$VerbBinder@VCRobloxView@@VCView@@@@UAEHIHPAXPAUAFX_CMDHANDLERINFO@@@Z
// TEMPLATE: ROBLOX 0x0045e230
// ??1?$VerbBinder@VCRobloxView@@VCView@@@@UAE@XZ
// SYNTHETIC: ROBLOX 0x0045e2c0
// VerbBinder<CRobloxView,CView>::`scalar deleting destructor'
// clang-format on

template <class T, class Base>
class VerbBinder : public Base
{
public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
	{
		if (nCode == CN_COMMAND || nCode == CN_UPDATE_COMMAND_UI) {
			typename std::map<UINT, const RBX::Name*>::iterator it = m_unk0x58.find(nID);
			if (it != m_unk0x58.end() && it->second) {
				boost::shared_ptr<RBX::DataModel> dataModel = static_cast<T*>(this)->FUN_00403290();
				RBX::DataModel::Lock lock(dataModel);
				const RBX::Name* name = it->second;
				RBX::VerbContainer* container = static_cast<T*>(this)->FUN_004558a0();
				if (container) {
					RBX::Verb* verb = container->getVerb(*name);
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
							cmdUI->Enable(verb->isEnabled() ? TRUE : FALSE);
							cmdUI->SetCheck(verb->isChecked() ? 1 : 0);
						}
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
