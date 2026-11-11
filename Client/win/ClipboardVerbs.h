#ifndef CLIPBOARDVERBS_H
#define CLIPBOARDVERBS_H

#include "decomp.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/DataModel.h"
#include "v8tree/Verb.h"

// VTABLE: ROBLOX 0x008010c4
// SIZE 0x14
class PasteVerb : public RBX::Verb
{
public:
	PasteVerb(RBX::DataModel* dataModel, bool pasteInto);

	virtual bool isEnabled() const;
	virtual void doIt(RBX::IDataState* dataState);

	// SYNTHETIC: ROBLOX 0x00410420
	// PasteVerb::`scalar deleting destructor'

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
	CopyVerb(RBX::DataModel* dataModel);

	virtual ~CopyVerb() {}

	// SYNTHETIC: ROBLOX 0x00410440
	// CopyVerb::`scalar deleting destructor'

	virtual void doIt(RBX::IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CopyVerb, 0x24)

#endif // CLIPBOARDVERBS_H
