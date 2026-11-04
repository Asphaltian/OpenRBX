#ifndef ROBLOXDOC_H
#define ROBLOXDOC_H

#include "Workspace.h"
#include "decomp.h"

#include <boost/shared_ptr.hpp>

// SIZE 0x7c
class CRobloxDoc : public CDocument
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

private:
	undefined m_unk0x54[0x78 - 0x54]; // 0x54
	CWorkspace* m_unk0x78;            // 0x78
};

DECOMP_SIZE_ASSERT(CRobloxDoc, 0x7c)

#endif // ROBLOXDOC_H
