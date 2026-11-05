#ifndef SELECTIONCAPTION_H
#define SELECTIONCAPTION_H

#include "ActiveDocView.h"
#include "FilteredListener.h"
#include "decomp.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Selection.h"

#include <boost/shared_ptr.hpp>

// SIZE 0x1c0
class CSelectionCaption
	: public CXTCaption,
	  public ActiveDocView,
	  public RBX::FilteredListener<RBX::Selection, RBX::SelectionChanged, RBX::Selection, RBX::SelectionChanged>
{
	DECLARE_DYNAMIC(CSelectionCaption)

	// SYNTHETIC: ROBLOX 0x0046a220
	// ?_GetBaseClass@CSelectionCaption@@KGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0046a230
	// ?GetThisClass@CSelectionCaption@@SGPAUCRuntimeClass@@XZ

public:
	CSelectionCaption();
	virtual ~CSelectionCaption();

	// SYNTHETIC: ROBLOX 0x0046abe0
	// CSelectionCaption::`scalar deleting destructor'

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x0046a280
	// ?GetMessageMap@CSelectionCaption@@MBEPBUAFX_MSGMAP@@XZ

	virtual void VTable0x00(
		boost::shared_ptr<RBX::DataModel> newDataModel, boost::shared_ptr<RBX::DataModel> oldDataModel
	);
	virtual void VTable0x08(const RBX::Selection* source, RBX::SelectionChanged event);
	virtual bool VTable0x10(
		const RBX::Selection* source,
		const RBX::SelectionChanged& event,
		const RBX::Selection* oldSource,
		const RBX::SelectionChanged& oldEvent
	);

private:
	void FUN_0046a290();

	boost::shared_ptr<RBX::DataModel> m_unk0x1b0; // 0x1b0
	boost::shared_ptr<RBX::Selection> m_unk0x1b8; // 0x1b8
};

DECOMP_SIZE_ASSERT(CSelectionCaption, 0x1c0)

#endif // SELECTIONCAPTION_H
