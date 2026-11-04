#ifndef STANDARDOUTPUTVIEW_H
#define STANDARDOUTPUTVIEW_H

#include "OutputView.h"
#include "decomp.h"
#include "util/StandardOut.h"

#include <boost/thread/mutex.hpp>
#include <deque>

// SIZE 0x130
class CStandardOutputView : public COutputView, public RBX::Listener<RBX::StandardOut, RBX::StandardOutMessage>
{
	DECLARE_DYNCREATE(CStandardOutputView)

	// SYNTHETIC: ROBLOX 0x0043a460
	// ?GetThisClass@CStandardOutputView@@SGPAUCRuntimeClass@@XZ

	// SYNTHETIC: ROBLOX 0x0043bab0
	// ?CreateObject@CStandardOutputView@@SGPAVCObject@@XZ

protected:
	CStandardOutputView();

public:
	virtual ~CStandardOutputView();

	// SYNTHETIC: ROBLOX 0x0043b470
	// CStandardOutputView::`scalar deleting destructor'

protected:
	virtual void onEvent(const RBX::StandardOut* source, RBX::StandardOutMessage message);

private:
	std::deque<RBX::StandardOutMessage> m_unk0x114; // 0x114
	boost::mutex m_unk0x128;                        // 0x128
};

DECOMP_SIZE_ASSERT(CStandardOutputView, 0x130)

#endif // STANDARDOUTPUTVIEW_H
