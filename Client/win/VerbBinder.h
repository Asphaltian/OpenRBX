#ifndef VERBBINDER_H
#define VERBBINDER_H

#include "placeholder.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Verb.h"

#include <boost/shared_ptr.hpp>
#include <map>

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

	void bind(UINT id, const RBX::Name& name) { m_unk0x58[id] = &name; }

	void bind(int id, RBX::Verb* verb)
	{
		if (verb) {
			const RBX::Name* name = &verb->getName();
			m_unk0x58[id] = name;
		}
		else {
			const RBX::Name* name = &RBX::Name::getNullName();
			m_unk0x58[id] = name;
		}
	}

	std::map<UINT, const RBX::Name*> m_unk0x58;
};

#endif // VERBBINDER_H
