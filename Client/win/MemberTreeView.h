#ifndef MEMBERTREEVIEW_H
#define MEMBERTREEVIEW_H

#include "placeholder.h"
#include "reflection/EnumConverter.h"
#include "reflection/function.h"
#include "reflection/object.h"
#include "reflection/property.h"
#include "reflection/signal.h"

namespace RBX {
namespace Reflection {
namespace Metadata {
class Item;
}
} // namespace Reflection
} // namespace RBX

class CDeclarationView;

class CMemberTreeView : public CXTTreeView
{
	DECLARE_DYNCREATE(CMemberTreeView)

protected:
	CMemberTreeView();

public:
	virtual ~CMemberTreeView();

	virtual void OnInitialUpdate();

	void FUN_004380e0(const RBX::Reflection::EnumDescriptor::Item* item);
	void FUN_00438130(HTREEITEM item, RBX::Reflection::Metadata::Item* metadata);
	void FUN_00438300(const RBX::Reflection::PropertyDescriptor* descriptor);
	void FUN_00438420(const RBX::Reflection::SignalDescriptor* descriptor);
	void FUN_004397f0(const RBX::Reflection::EnumDescriptor* descriptor);
	void FUN_00439a70(const RBX::Reflection::FunctionDescriptor* descriptor);
	void FUN_0043a060(const RBX::Reflection::ClassDescriptor* descriptor);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void FUN_0043a240(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

private:
	CImageList m_unk0xa0;
	bool m_unk0xa8;
	const RBX::Reflection::Descriptor* m_unk0xac;

public:
	CDeclarationView* m_unk0xb0;
};

#endif // MEMBERTREEVIEW_H
