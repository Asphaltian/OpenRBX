#ifndef CLASSTREEVIEW_H
#define CLASSTREEVIEW_H

#include "decomp.h"
#include "reflection/EnumConverter.h"
#include "reflection/object.h"

namespace RBX {
namespace Reflection {
namespace Metadata {
class Item;
}
} // namespace Reflection
} // namespace RBX

class CDeclarationView;
class CMemberTreeView;

// SIZE 0xb0
class CClassTreeView : public CXTTreeView
{
	DECLARE_DYNCREATE(CClassTreeView)

	// SYNTHETIC: ROBLOX 0x00437570
	// ?GetThisClass@CClassTreeView@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x00438220
	// ?CreateObject@CClassTreeView@@SGPAVCObject@@XZ

protected:
	CClassTreeView();

public:
	virtual ~CClassTreeView();

	virtual void OnInitialUpdate();

	void FUN_00437f90(const RBX::Reflection::EnumDescriptor* descriptor);
	void FUN_00438130(HTREEITEM item, RBX::Reflection::Metadata::Item* metadata);
	void FUN_00439690(const RBX::Reflection::ClassDescriptor* descriptor);

	// SYNTHETIC: ROBLOX 0x00437ba0
	// CClassTreeView::`scalar deleting destructor'

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void FUN_0043a280(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	// SYNTHETIC: ROBLOX 0x0043a430
	// ?GetMessageMap@CClassTreeView@@MBEPBUAFX_MSGMAP@@XZ

private:
	CImageList m_unk0xa0; // 0xa0

public:
	CMemberTreeView* m_unk0xa8;  // 0xa8
	CDeclarationView* m_unk0xac; // 0xac
};

DECOMP_SIZE_ASSERT(CClassTreeView, 0xb0)

#endif // CLASSTREEVIEW_H
