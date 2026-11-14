#ifndef ROBLOXTREECTRL_H
#define ROBLOXTREECTRL_H

#include "placeholder.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>

namespace RBX {
class DataModel;
class Instance;
class Selection;
} // namespace RBX

class CRobloxTreeCtrlNode;

class CRobloxTreeCtrl : public CXTTreeCtrl
{
	DECLARE_DYNAMIC(CRobloxTreeCtrl)

public:
	CRobloxTreeCtrl();

	virtual RBX::Selection* VTable0x148() = 0;
	virtual boost::shared_ptr<RBX::DataModel> VTable0x14c() const = 0;
	virtual void VTable0x150(RBX::Instance* instance) = 0;
	virtual void VTable0x154(CRobloxTreeCtrlNode* node) = 0;
	virtual void VTable0x158(CRobloxTreeCtrlNode* node) = 0;
	virtual void VTable0x15c();

	CRobloxTreeCtrlNode* FUN_00426e00(const boost::shared_ptr<RBX::Instance>& instance);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void FUN_004221f0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void FUN_00422210(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void FUN_00422b80(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void FUN_00422de0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void FUN_004234f0(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void FUN_00428720(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

	friend class CRobloxTreeCtrlNode;

	CRobloxTreeCtrlNode* m_unk0x94;
	std::map<RBX::Instance*, CRobloxTreeCtrlNode*> m_unk0x98;
	undefined m_unk0xa4;
	CImageList m_unk0xa8;
	boost::recursive_mutex m_unk0xb0;
};

#endif // ROBLOXTREECTRL_H
