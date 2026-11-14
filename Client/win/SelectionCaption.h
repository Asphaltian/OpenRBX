#ifndef SELECTIONCAPTION_H
#define SELECTIONCAPTION_H

#include "ActiveDocView.h"
#include "FilteredListener.h"
#include "placeholder.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Selection.h"

#include <boost/shared_ptr.hpp>

class CSelectionCaption
	: public CXTCaption,
	  public ActiveDocView,
	  public RBX::FilteredListener<RBX::Selection, RBX::SelectionChanged, RBX::Selection, RBX::SelectionChanged>
{
	DECLARE_DYNAMIC(CSelectionCaption)

public:
	CSelectionCaption();
	virtual ~CSelectionCaption();

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

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

	boost::shared_ptr<RBX::DataModel> m_unk0x1b0;
	boost::shared_ptr<RBX::Selection> m_unk0x1b8;
};

#endif // SELECTIONCAPTION_H
