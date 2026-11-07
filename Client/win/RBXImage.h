#ifndef RBXIMAGE_H
#define RBXIMAGE_H

#include "decomp.h"

#include <windows.h>

// VTABLE: ROBLOX 0x008090e8
// SIZE 0x30
class RBXImage
{
public:
	enum
	{
		c_createAlphaChannel = 0x01
	};

	RBXImage();
	virtual ~RBXImage(); // vtable+0x00

	// SYNTHETIC: ROBLOX 0x00445f00
	// RBXImage::`scalar deleting destructor'

	void* FUN_00445c40(int x, int y);
	int FUN_00445c70();
	BOOL FUN_00445dd0(int width, int height, int bpp, DWORD compression, const DWORD* masks, DWORD flags);
	BOOL FUN_00445f50(int width, int height, int bpp, DWORD flags);
	operator HBITMAP();

private:
	void FUN_00445cc0(int orientation);

	HBITMAP m_unk0x04;    // 0x04
	void* m_unk0x08;      // 0x08
	int m_unk0x0c;        // 0x0c
	int m_unk0x10;        // 0x10
	int m_unk0x14;        // 0x14
	int m_unk0x18;        // 0x18
	bool m_unk0x1c;       // 0x1c
	bool m_unk0x1d;       // 0x1d
	int m_unk0x20;        // 0x20
	undefined4 m_unk0x24; // 0x24
	undefined4 m_unk0x28; // 0x28
	undefined4 m_unk0x2c; // 0x2c
};

#endif // RBXIMAGE_H
