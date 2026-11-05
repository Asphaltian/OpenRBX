#ifndef ROBLOXWND_H
#define ROBLOXWND_H

#include "DxUserInput.h"
#include "decomp.h"
#include "util/IMetric.h"
#include "util/RunStateOwner.h"

// SIZE 0xb8
class CRobloxWnd : public CWnd, public RBX::IRunView, public RBX::IMetric
{
public:
	CRobloxWnd(bool p_unk0x80);
	virtual ~CRobloxWnd();

	void FUN_0045fc50(bool active);

	virtual void InvalidateRunView();
	virtual void AdjustThrottle(double frameRate);

	virtual std::string getMetric(const std::string& metric) const;

private:
	undefined m_unk0x5c[0x7c - 0x5c]; // 0x5c
	DxUserInput* m_unk0x7c;           // 0x7c
	undefined m_unk0x80[0x88 - 0x80]; // 0x80
	double m_unk0x88;                 // 0x88
	undefined m_unk0x90[0xb8 - 0x90]; // 0x90
};

DECOMP_SIZE_ASSERT(CRobloxWnd, 0xb8)

#endif // ROBLOXWND_H
