#ifndef ROBLOXDOC_H
#define ROBLOXDOC_H

#include "VerbBinder.h"
#include "Workspace.h"
#include "decomp.h"

#include <boost/shared_ptr.hpp>
#include <memory>
#include <string>
#include <vector>

class CRobloxDoc;

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
	BOOL FUN_00459950(std::string p_unk0x04);
	void FUN_00459c80();

private:
	friend class CRobloxView;

	std::auto_ptr<XmlElement> m_unk0x60; // 0x60
	std::vector<undefined4*> m_unk0x64;  // 0x64
	undefined4 m_unk0x74;                // 0x74
	CWorkspace* m_unk0x78;               // 0x78
};

DECOMP_SIZE_ASSERT(CRobloxDoc, 0x7c)

#endif // ROBLOXDOC_H
