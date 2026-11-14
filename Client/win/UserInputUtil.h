#ifndef USERINPUTUTIL_H
#define USERINPUTUTIL_H

#include "util/UIEvent.h"
#include "util/UserInputBase.h"

#include <G3D/Vector2.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

SDLKey FUN_0046d6f0(DWORD scanCode);
int FUN_0046db70(SDLKey key);
int FUN_0046dff0(SDLKey key);
RBX::UIEvent::EventType FUN_0046e290(UINT message);
SDLMod FUN_0046e2e0(const BYTE* keys);
void FUN_0046d670(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position);
void FUN_0046d6a0(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position);
G3D::Vector2 FUN_0046d6c0(const DIDEVICEOBJECTDATA& data);
void FUN_0046e530(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
);
void FUN_0046e560(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
);
void FUN_0046e5f0(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
);

#endif // USERINPUTUTIL_H
