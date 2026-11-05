#ifndef DXUSERINPUT_H
#define DXUSERINPUT_H

#include "decomp.h"
#include "util/Events.h"
#include "util/RunStateOwner.h"
#include "util/UIEvent.h"
#include "util/UserInputBase.h"

#include <G3D/Rect2D.h>
#include <atlsync.h>
#include <boost/shared_ptr.hpp>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace RBX {
class DataModel;
}

// VTABLE: ROBLOX 0x0080f9dc RBX::UserInputBase
// VTABLE: ROBLOX 0x0080f9d0 RBX::Listener<RBX::RunService, class RBX::RunTransition>
// VTABLE: ROBLOX 0x0080f9c4 RBX::Listener<RBX::RunService, class RBX::Heartbeat>
// SIZE 0x19c
class DxUserInput : public RBX::UserInputBase,
					public RBX::Listener<RBX::RunService, RBX::RunTransition>,
					public RBX::Listener<RBX::RunService, RBX::Heartbeat>
{
private:
	undefined4 m_unk0x30;                         // 0x30
	mutable ATL::CCriticalSection m_unk0x34;      // 0x34
	CFrameWnd* m_unk0x4c;                         // 0x4c
	ACCEL* m_unk0x50;                             // 0x50
	int m_unk0x54;                                // 0x54
	G3D::Rect2D m_unk0x58;                        // 0x58
	bool m_unk0x68;                               // 0x68
	WrapMode m_unk0x6c;                           // 0x6c
	G3D::Vector2 m_unk0x70;                       // 0x70
	bool m_unk0x78;                               // 0x78
	bool m_unk0x79;                               // 0x79
	bool m_unk0x7a;                               // 0x7a
	BYTE m_unk0x7b[0x100];                        // 0x7b
	SDLKey m_unk0x17c;                            // 0x17c
	CWnd* m_unk0x180;                             // 0x180
	HWND m_unk0x184;                              // 0x184
	CComPtr<IDirectInput8> m_unk0x188;            // 0x188
	CComPtr<IDirectInputDevice8> m_unk0x18c;      // 0x18c
	CComPtr<IDirectInputDevice8> m_unk0x190;      // 0x190
	boost::shared_ptr<RBX::DataModel> m_unk0x194; // 0x194

	bool FUN_0046b7f0();
	bool FUN_0046b920();
	void FUN_0046b9d0();
	void FUN_0046ba80();
	void FUN_0046bb10();
	void FUN_0046bc20(const G3D::Vector2& position);
	void FUN_0046bd60();
	bool FUN_0046bed0(DIDEVICEOBJECTDATA* data, DWORD* count, IDirectInputDevice8* device);
	void FUN_0046c090();
	void FUN_0046c3c0(RBX::UIEvent::EventType eventType, const G3D::Vector2& position);
	G3D::Vector2 FUN_0046c4f0();
	G3D::Vector2 FUN_0046c590();
	void FUN_0046cd90();

protected:
	virtual G3D::Vector2 getCursorPosition();
	virtual G3D::ReferenceCountedPointer<RBX::TextureProxyBase> getGameCursor(RBX::Adorn* adorn);

	virtual void onEvent(const RBX::RunService* source, RBX::Heartbeat event);
	virtual void onEvent(const RBX::RunService* source, RBX::RunTransition event);

public:
	DxUserInput(CWnd* wnd, RBX::DataModel* dataModel);
	virtual ~DxUserInput();

	virtual void setWrapMode(WrapMode wrapMode);
	virtual WrapMode getWrapMode() const { return m_unk0x6c; }
	virtual void centerCursor();
	virtual bool keyDown(SDLKey key) const;
	virtual void setKeyState(SDLKey key, bool down);

	void FUN_0046be50(bool p_unk0x78);
	void FUN_0046ca90();
	void FUN_0046d0a0();

	// SYNTHETIC: ROBLOX 0x0046d650
	// DxUserInput::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(DxUserInput, 0x19c)

#endif // DXUSERINPUT_H
