#ifndef ROBLOXDOC_H
#define ROBLOXDOC_H

#include "VerbBinder.h"
#include "Workspace.h"
#include "decomp.h"
#include "v8datamodel/Unknown005d2700.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>
#include <string>
#include <vector>

class CRobloxDoc;

// VTABLE: ROBLOX 0x008010c4
// SIZE 0x14
class PasteVerb : public RBX::Verb
{
public:
	// FUNCTION: ROBLOX 0x00410000
	PasteVerb(RBX::DataModel* dataModel, bool pasteInto)
		: RBX::Verb(dataModel, pasteInto ? "PasteInto" : "Paste"), m_unk0x0c(dataModel), m_unk0x10(pasteInto)
	{
	}

	virtual bool isEnabled() const;
	virtual void doIt(RBX::IDataState* dataState);

private:
	RBX::DataModel* m_unk0x0c; // 0x0c
	bool m_unk0x10;            // 0x10
};

DECOMP_SIZE_ASSERT(PasteVerb, 0x14)

// VTABLE: ROBLOX 0x00801120
// SIZE 0x28
class CutVerb : public RBX::DeleteSelectionVerb
{
public:
	// FUNCTION: ROBLOX 0x00410470
	CutVerb(RBX::DataModel* dataModel) : RBX::DeleteSelectionVerb(dataModel, dataModel, "Cut") {}

	virtual void doIt(RBX::IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CutVerb, 0x28)

// VTABLE: ROBLOX 0x008010f0
// SIZE 0x24
class CopyVerb : public RBX::EditSelectionVerb
{
public:
	// FUNCTION: ROBLOX 0x00410060
	CopyVerb(RBX::DataModel* dataModel) : RBX::EditSelectionVerb("Copy", dataModel) {}

	virtual ~CopyVerb() {}

	// SYNTHETIC: ROBLOX 0x00410440
	// CopyVerb::`scalar deleting destructor'

	virtual void doIt(RBX::IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CopyVerb, 0x24)

// SIZE 0x10
class ReportAbuseVerb : public RBX::Verb
{
public:
	ReportAbuseVerb(RBX::VerbContainer* p_unk0x04, CRobloxDoc* p_unk0x08)
		: RBX::Verb(p_unk0x04, "ReportAbuse"), m_unk0x0c(p_unk0x08)
	{
	}

	virtual bool isEnabled() const;
	virtual void doIt(RBX::IDataState* dataState);

private:
	CRobloxDoc* m_unk0x0c; // 0x0c
};

DECOMP_SIZE_ASSERT(ReportAbuseVerb, 0x10)

// SIZE 0x10
class ExitCommand : public RBX::Verb
{
public:
	ExitCommand(RBX::VerbContainer* p_unk0x04, CRobloxDoc* p_unk0x08)
		: RBX::Verb(p_unk0x04, "Exit"), m_unk0x0c(p_unk0x08)
	{
	}

	virtual void doIt(RBX::IDataState* dataState);

private:
	CRobloxDoc* m_unk0x0c; // 0x0c
};

DECOMP_SIZE_ASSERT(ExitCommand, 0x10)

// SIZE 0x7c
class CRobloxDoc : public VerbBinder<CRobloxDoc, CDocument>
{
public:
	// FUNCTION: ROBLOX 0x00403290
	boost::shared_ptr<RBX::DataModel> FUN_00403290() const
	{
		if (m_unk0x78) {
			return m_unk0x78->FUN_0046edc0();
		}

		return boost::shared_ptr<RBX::DataModel>();
	}

	DECLARE_DYNCREATE(CRobloxDoc)

	// SYNTHETIC: ROBLOX 0x00453ea0
	// ?GetThisClass@CRobloxDoc@@SGPAUCRuntimeClass@@XZ

protected:
	CRobloxDoc();

public:
	virtual ~CRobloxDoc();

	RBX::VerbContainer* FUN_004558a0();
	void FUN_00456c00(int id, RBX::Verb* verb);
	void FUN_00456c50(int id, RBX::Verb* verb);
	void FUN_00456d30();

	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	BOOL FUN_00459950(std::string p_unk0x04);
	void FUN_00459c80();

protected:
	afx_msg void FUN_00453eb0(CCmdUI* pCmdUI);
	afx_msg void FUN_004588a0(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

private:
	friend class CRobloxView;

	std::auto_ptr<XmlElement> m_unk0x60;               // 0x60
	std::vector<RBX::Verb*> m_unk0x64;                 // 0x64
	boost::scoped_ptr<RBX::Unknown005d2700> m_unk0x74; // 0x74
	CComObject<CWorkspace>* m_unk0x78;                 // 0x78
};

DECOMP_SIZE_ASSERT(CRobloxDoc, 0x7c)

#endif // ROBLOXDOC_H
