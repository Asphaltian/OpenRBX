#ifndef STANDARDOUTPUTVIEW_H
#define STANDARDOUTPUTVIEW_H

#include "OutputView.h"
#include "placeholder.h"
#include "util/StandardOut.h"

#include <boost/thread/mutex.hpp>
#include <deque>

class CStandardOutputView : public COutputView, public RBX::Listener<RBX::StandardOut, RBX::StandardOutMessage>
{
	DECLARE_DYNCREATE(CStandardOutputView)

protected:
	CStandardOutputView();

public:
	virtual ~CStandardOutputView();

protected:
	virtual void onEvent(const RBX::StandardOut* source, RBX::StandardOutMessage message);

	afx_msg LRESULT FUN_0043b8a0(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	std::deque<RBX::StandardOutMessage> m_unk0x114;
	boost::mutex m_unk0x128;
};

#endif // STANDARDOUTPUTVIEW_H
