#ifndef ROBLOX_H
#define ROBLOX_H

#include "ErrorUploader.h"
#include "LogManager.h"
#include "decomp.h"

#include <boost/scoped_ptr.hpp>
#include <string>

// SIZE 0x110
class CRobloxApp : public CWinApp
{
public:
	CRobloxApp();
	~CRobloxApp();

	virtual BOOL InitInstance(); // vtable+0x58
	virtual int Run();           // vtable+0x5c
	virtual int ExitInstance();  // vtable+0x70

	// SYNTHETIC: ROBLOX 0x00451530
	// CRobloxApp::`scalar deleting destructor'

	BOOL FUN_0044dac0(bool p_unk0x04);
	void FUN_0044e250();

	afx_msg void OnAppAbout();
	afx_msg void FUN_0044dc90();
	afx_msg void FUN_0044e270();
	afx_msg void FUN_0044e290();
	afx_msg void FUN_004510b0(CCmdUI* p_unk0x04);
	afx_msg void FUN_00451100();
	afx_msg void FUN_00453eb0(CCmdUI* p_unk0x04);
	afx_msg void FUN_0044d450(WPARAM p_unk0x04, LPARAM p_unk0x08);

	DECLARE_MESSAGE_MAP()

private:
	MainLogManager m_unk0xa4;                    // 0xa4
	std::string m_unk0xf0;                       // 0xf0
	boost::scoped_ptr<ErrorUploader> m_unk0x10c; // 0x10c
};

DECOMP_SIZE_ASSERT(CRobloxApp, 0x110)

extern CRobloxApp theApp;

#endif // ROBLOX_H
