#ifndef ROBLOXTREECTRL_H
#define ROBLOXTREECTRL_H

#include "decomp.h"

#include <boost/thread/recursive_mutex.hpp>
#include <map>

namespace RBX {
class Instance;
}

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

protected:
	undefined4 m_unk0x94;                                     // 0x94
	std::map<RBX::Instance*, CRobloxTreeCtrlNode*> m_unk0x98; // 0x98
	undefined m_unk0xa4;                                      // 0xa4
	CImageList m_unk0xa8;                                     // 0xa8
	boost::recursive_mutex m_unk0xb0;                         // 0xb0
};

DECOMP_SIZE_ASSERT(CRobloxTreeCtrl, 0xbc)

#endif // ROBLOXTREECTRL_H
