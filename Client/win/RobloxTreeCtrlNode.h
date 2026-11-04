#ifndef ROBLOXTREECTRLNODE_H
#define ROBLOXTREECTRLNODE_H

#include "decomp.h"
#include "util/Events.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>
#include <map>
#include <set>

class CRobloxTreeCtrl;

int FUN_004230c0(RBX::Instance* instance);

// SIZE 0x50
class CRobloxTreeCtrlNode : public RBX::Listener<RBX::Instance, RBX::PropertyChanged>,
							public RBX::Listener<RBX::Instance, RBX::ChildAdded>,
							public RBX::Listener<RBX::Instance, RBX::ChildRemoved>
{
public:
	CRobloxTreeCtrlNode(CRobloxTreeCtrlNode* parent, boost::shared_ptr<RBX::Instance> instance, CRobloxTreeCtrl* tree);
	virtual ~CRobloxTreeCtrlNode();

	virtual void VTable0x08();                       // vtable+0x08
	virtual void VTable0x0c();                       // vtable+0x0c
	virtual void VTable0x10(undefined4 p_unk0x04);   // vtable+0x10
	virtual void VTable0x14(NMTVDISPINFO* dispInfo); // vtable+0x14
	virtual HTREEITEM VTable0x18();                  // vtable+0x18

	void FUN_004221a0();
	void FUN_00425c40();
	void FUN_00426960();

protected:
	friend class CSelectionTreeCtrl;

	virtual void onEvent(const RBX::Instance* source, RBX::PropertyChanged event);
	virtual void onEvent(const RBX::Instance* source, RBX::ChildAdded event);
	virtual void onEvent(const RBX::Instance* source, RBX::ChildRemoved event);

	boost::shared_ptr<RBX::Instance> m_unk0x0c;               // 0x0c
	std::map<RBX::Instance*, CRobloxTreeCtrlNode*> m_unk0x14; // 0x14
	std::set<CRobloxTreeCtrlNode*> m_unk0x20;                 // 0x20
	undefined m_unk0x2c_bit0 : 1;                             // 0x2c
	undefined m_unk0x2c_bit1 : 1;                             // 0x2c
	undefined m_unk0x2d;                                      // 0x2d
	CRobloxTreeCtrl* m_unk0x30;                               // 0x30
	CRobloxTreeCtrlNode* m_unk0x34;                           // 0x34
	std::set<boost::shared_ptr<RBX::Instance> > m_unk0x38;    // 0x38
	HTREEITEM m_unk0x44;                                      // 0x44
	int m_unk0x48;                                            // 0x48
	const RBX::Reflection::ClassDescriptor* m_unk0x4c;        // 0x4c
};

DECOMP_SIZE_ASSERT(CRobloxTreeCtrlNode, 0x50)

// SYNTHETIC: ROBLOX 0x00425ed0
// ??_ERootNode@@UAEPAXI@Z

#endif // ROBLOXTREECTRLNODE_H
