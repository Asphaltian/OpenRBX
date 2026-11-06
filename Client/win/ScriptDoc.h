#ifndef SCRIPTDOC_H
#define SCRIPTDOC_H

#include "MarshaledListener.h"
#include "NullDoc.h"
#include "ScintillaDocView.h"
#include "decomp.h"
#include "util/RunStateOwner.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace RBX {
class Script;
} // namespace RBX

// SIZE 0x5c
class CScriptDoc : public CNullDoc
{
	DECLARE_DYNCREATE(CScriptDoc)

	// SYNTHETIC: ROBLOX 0x00467db0
	// ?GetThisClass@CScriptDoc@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00468d30
	// ?CreateObject@CScriptDoc@@SGPAVCObject@@XZ

protected:
	CScriptDoc();

public:
	// SYNTHETIC: ROBLOX 0x00468be0
	// ??1CScriptDoc@@UAE@XZ

	// SYNTHETIC: ROBLOX 0x00468c70
	// CScriptDoc::`scalar deleting destructor'

	void FUN_0044eaf0(boost::shared_ptr<RBX::Script> script);

private:
	boost::shared_ptr<RBX::Script> m_unk0x54; // 0x54
};

DECOMP_SIZE_ASSERT(CScriptDoc, 0x5c)

// SIZE 0x150
class CScriptEditor
	: public Scintilla::CScintillaView,
	  public RBX::Listener<RBX::Instance, RBX::AncestorChanged>,
	  public RBX::MarshaledListener<RBX::Instance, RBX::PropertyChanged, RBX::Instance, RBX::PropertyChanged>,
	  public RBX::MarshaledListener<RBX::RunService, RBX::RunState, RBX::RunService, RBX::RunState>
{
	DECLARE_DYNCREATE(CScriptEditor)

	// SYNTHETIC: ROBLOX 0x00467dd0
	// ?GetThisClass@CScriptEditor@@SGPAUCRuntimeClass@@XZ

protected:
	CScriptEditor();

public:
	virtual ~CScriptEditor();

	void FUN_00469b30(boost::shared_ptr<RBX::Script> script);

protected:
	virtual void onEvent(const RBX::Instance* source, RBX::AncestorChanged event);
	virtual void VTable0x08(const RBX::RunService* source, RBX::RunState event);
	virtual void VTable0x08(const RBX::Instance* source, RBX::PropertyChanged event);
	virtual void VTable0x0c(const RBX::Instance* source, RBX::PropertyChanged event) throw();

private:
	boost::shared_ptr<boost::recursive_mutex> m_unk0x128; // 0x128
	undefined4 m_unk0x130;                                // 0x130
	boost::shared_ptr<RBX::Script> m_unk0x134;            // 0x134
	boost::shared_ptr<RBX::RunService> m_unk0x13c;        // 0x13c
	bool m_unk0x144;                                      // 0x144
	bool m_unk0x145;                                      // 0x145
	bool m_unk0x146;                                      // 0x146
	CFont m_unk0x148;                                     // 0x148
};

DECOMP_SIZE_ASSERT(CScriptEditor, 0x150)

#endif // SCRIPTDOC_H
