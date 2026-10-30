#ifndef SECUREHTMLVIEW_H
#define SECUREHTMLVIEW_H

#include "decomp.h"

#include <afxhtml.h>

// SIZE 0xf4
class CSecureHtmlView : public CHtmlView
{
	DECLARE_DYNAMIC(CSecureHtmlView)

protected:
	CSecureHtmlView();

public:
	virtual ~CSecureHtmlView();

private:
	undefined4 m_unk0xf0; // 0xf0
};

DECOMP_SIZE_ASSERT(CSecureHtmlView, 0xf4)

#endif // SECUREHTMLVIEW_H
