#ifndef ROBLOXDOC_H
#define ROBLOXDOC_H

#include "VerbBinder.h"
#include "Workspace.h"
#include "decomp.h"

#include <boost/shared_ptr.hpp>
#include <memory>
#include <vector>

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

private:
	std::auto_ptr<XmlElement> m_unk0x60; // 0x60
	std::vector<undefined4*> m_unk0x64;  // 0x64
	undefined4 m_unk0x74;                // 0x74
	CWorkspace* m_unk0x78;               // 0x78
};

DECOMP_SIZE_ASSERT(CRobloxDoc, 0x7c)

#endif // ROBLOXDOC_H
