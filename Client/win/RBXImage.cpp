#include "RBXImage.h"

#include <atlalloc.h>

DECOMP_SIZE_ASSERT(RBXImage, 0x30)

// FUNCTION: ROBLOX 0x00445c40
void* RBXImage::FUN_00445c40(int x, int y)
{
	return static_cast<BYTE*>(m_unk0x08) + m_unk0x14 * y + x * m_unk0x18 / 8;
}

// FUNCTION: ROBLOX 0x00445c70
int RBXImage::FUN_00445c70()
{
	return m_unk0x0c;
}

// FUNCTION: ROBLOX 0x00445c80
RBXImage::RBXImage()
	: m_unk0x04(NULL), m_unk0x08(NULL), m_unk0x0c(0), m_unk0x10(0), m_unk0x14(0), m_unk0x18(0), m_unk0x1c(false),
	  m_unk0x1d(false), m_unk0x20(-1), m_unk0x24(0), m_unk0x28(0), m_unk0x2c(0)
{
}

// STUB: ROBLOX 0x00445cc0
void RBXImage::FUN_00445cc0(int orientation)
{
	DIBSECTION section;
	if (GetObject(m_unk0x04, sizeof(DIBSECTION), &section) == sizeof(DIBSECTION)) {
		m_unk0x18 = section.dsBmih.biBitCount;
		m_unk0x0c = section.dsBmih.biWidth;
		m_unk0x10 = abs(section.dsBmih.biHeight);
		m_unk0x14 = (m_unk0x18 * m_unk0x0c + 31) / 32 * 4;
		m_unk0x1c = true;
		m_unk0x08 = section.dsBm.bmBits;
		if (orientation == 0) {
			orientation = section.dsBmih.biHeight > 0 ? 2 : 1;
		}
		m_unk0x20 = -1;
		m_unk0x1d = false;
		if (orientation == 2) {
			m_unk0x08 = static_cast<BYTE*>(m_unk0x08) + (m_unk0x10 - 1) * m_unk0x14;
			m_unk0x14 = -m_unk0x14;
		}
	}
	else {
		m_unk0x1c = false;
		m_unk0x0c = section.dsBm.bmWidth;
		m_unk0x10 = section.dsBm.bmHeight;
		m_unk0x18 = section.dsBm.bmBitsPixel;
		m_unk0x14 = 0;
		m_unk0x08 = NULL;
		m_unk0x20 = -1;
		m_unk0x1d = false;
	}
}

// FUNCTION: ROBLOX 0x00445d90
RBXImage::~RBXImage()
{
	if (m_unk0x04 != NULL) {
		HBITMAP bitmap = m_unk0x04;
		m_unk0x04 = NULL;
		m_unk0x08 = NULL;
		m_unk0x0c = 0;
		m_unk0x10 = 0;
		m_unk0x18 = 0;
		m_unk0x14 = 0;
		m_unk0x20 = -1;
		m_unk0x1d = false;
		m_unk0x1c = false;
		DeleteObject(bitmap);
	}
}

// STUB: ROBLOX 0x00445dd0
BOOL RBXImage::FUN_00445dd0(int width, int height, int bpp, DWORD compression, const DWORD* masks, DWORD flags)
{
	USES_ATL_SAFE_ALLOCA;
	BITMAPINFO* info = static_cast<BITMAPINFO*>(
		_ATL_SAFE_ALLOCA(sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD), _ATL_SAFE_ALLOCA_DEF_THRESHOLD)
	);
	if (info == NULL) {
		return FALSE;
	}

	memset(&info->bmiHeader, 0, sizeof(info->bmiHeader));
	info->bmiHeader.biSize = sizeof(info->bmiHeader);
	info->bmiHeader.biWidth = width;
	info->bmiHeader.biHeight = height;
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biBitCount = static_cast<WORD>(bpp);
	info->bmiHeader.biCompression = compression;
	if (bpp <= 8) {
		memset(info->bmiColors, 0, 256 * sizeof(RGBQUAD));
	}
	else if (compression == BI_BITFIELDS) {
		memcpy(info->bmiColors, masks, 3 * sizeof(DWORD));
	}

	HBITMAP bitmap = CreateDIBSection(NULL, info, DIB_RGB_COLORS, &m_unk0x08, NULL, 0);
	if (bitmap == NULL) {
		return FALSE;
	}
	m_unk0x04 = bitmap;
	FUN_00445cc0(height < 0 ? 1 : 2);
	if (flags & c_createAlphaChannel) {
		m_unk0x1d = true;
	}
	return TRUE;
}

// FUNCTION: ROBLOX 0x00445f50
BOOL RBXImage::FUN_00445f50(int width, int height, int bpp, DWORD flags)
{
	return FUN_00445dd0(width, height, bpp, BI_RGB, NULL, flags);
}

// FUNCTION: ROBLOX 0x006b31e0
RBXImage::operator HBITMAP()
{
	return m_unk0x04;
}
