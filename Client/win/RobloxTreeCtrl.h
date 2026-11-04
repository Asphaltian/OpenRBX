#ifndef ROBLOXTREECTRL_H
#define ROBLOXTREECTRL_H

#include "decomp.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <map>

namespace RBX {
class DataModel;
class Instance;
class Selection;
} // namespace RBX

class CRobloxTreeCtrlNode;

// SIZE 0xbc
class CRobloxTreeCtrl : public CXTTreeCtrl
{
public:
	CRobloxTreeCtrl();

	// SYNTHETIC: ROBLOX 0x00424840
	// CRobloxTreeCtrl::~CRobloxTreeCtrl

	// SYNTHETIC: ROBLOX 0x00424df0
	// CRobloxTreeCtrl::`scalar deleting destructor'

	virtual RBX::Selection* VTable0x148() = 0;                         // vtable+0x148
	virtual boost::shared_ptr<RBX::DataModel> VTable0x14c() const = 0; // vtable+0x14c
	virtual void VTable0x150(RBX::Instance* instance) = 0;             // vtable+0x150
	virtual void VTable0x154(CRobloxTreeCtrlNode* node) = 0;           // vtable+0x154
	virtual void VTable0x158(CRobloxTreeCtrlNode* node) = 0;           // vtable+0x158
	virtual void VTable0x15c();                                        // vtable+0x15c

	CRobloxTreeCtrlNode* FUN_00426e00(const boost::shared_ptr<RBX::Instance>& instance);

protected:
	friend class CRobloxTreeCtrlNode;

	CRobloxTreeCtrlNode* m_unk0x94;                           // 0x94
	std::map<RBX::Instance*, CRobloxTreeCtrlNode*> m_unk0x98; // 0x98
	undefined m_unk0xa4;                                      // 0xa4
	CImageList m_unk0xa8;                                     // 0xa8
	boost::recursive_mutex m_unk0xb0;                         // 0xb0
};

DECOMP_SIZE_ASSERT(CRobloxTreeCtrl, 0xbc)

#endif // ROBLOXTREECTRL_H
