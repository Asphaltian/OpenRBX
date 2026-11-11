#include "UserInputUtil.h"

#include "util/Rect.h"

static SDLKey g_unk0x009544f0[0x100];
static bool g_unk0x009548f0;

// FUNCTION: ROBLOX 0x0046d670
void FUN_0046d670(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position)
{
	wrapMouseDelta = G3D::Vector2::zero();
	position += mouseDelta;
}

// FUNCTION: ROBLOX 0x0046d6a0
void FUN_0046d6a0(const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position)
{
	wrapMouseDelta += mouseDelta;
}

// FUNCTION: ROBLOX 0x0046d6c0
G3D::Vector2 FUN_0046d6c0(const DIDEVICEOBJECTDATA& data)
{
	G3D::Vector2 delta(0.0f, 0.0f);
	float value = (int) data.dwData;
	if (data.dwOfs == DIMOFS_X) {
		delta.x = value;
	}
	else {
		delta.y = value;
	}
	return delta;
}

// FUNCTION: ROBLOX 0x0046d6f0
SDLKey FUN_0046d6f0(DWORD scanCode)
{
	if (!g_unk0x009548f0) {
		for (int i = 0; i < 0x100; i++) {
			g_unk0x009544f0[i] = SDLK_UNKNOWN;
		}

		g_unk0x009544f0[DIK_ESCAPE] = SDLK_ESCAPE;
		g_unk0x009544f0[DIK_1] = SDLK_1;
		g_unk0x009544f0[DIK_2] = SDLK_2;
		g_unk0x009544f0[DIK_3] = SDLK_3;
		g_unk0x009544f0[DIK_4] = SDLK_4;
		g_unk0x009544f0[DIK_5] = SDLK_5;
		g_unk0x009544f0[DIK_6] = SDLK_6;
		g_unk0x009544f0[DIK_7] = SDLK_7;
		g_unk0x009544f0[DIK_8] = SDLK_8;
		g_unk0x009544f0[DIK_9] = SDLK_9;
		g_unk0x009544f0[DIK_0] = SDLK_0;
		g_unk0x009544f0[DIK_MINUS] = SDLK_MINUS;
		g_unk0x009544f0[DIK_EQUALS] = SDLK_EQUALS;
		g_unk0x009544f0[DIK_BACK] = SDLK_BACKSPACE;
		g_unk0x009544f0[DIK_TAB] = SDLK_TAB;
		g_unk0x009544f0[DIK_Q] = SDLK_q;
		g_unk0x009544f0[DIK_W] = SDLK_w;
		g_unk0x009544f0[DIK_E] = SDLK_e;
		g_unk0x009544f0[DIK_R] = SDLK_r;
		g_unk0x009544f0[DIK_T] = SDLK_t;
		g_unk0x009544f0[DIK_Y] = SDLK_y;
		g_unk0x009544f0[DIK_U] = SDLK_u;
		g_unk0x009544f0[DIK_I] = SDLK_i;
		g_unk0x009544f0[DIK_O] = SDLK_o;
		g_unk0x009544f0[DIK_P] = SDLK_p;
		g_unk0x009544f0[DIK_LBRACKET] = SDLK_LEFTBRACKET;
		g_unk0x009544f0[DIK_RBRACKET] = SDLK_RIGHTBRACKET;
		g_unk0x009544f0[DIK_RETURN] = SDLK_RETURN;
		g_unk0x009544f0[DIK_LCONTROL] = SDLK_LCTRL;
		g_unk0x009544f0[DIK_A] = SDLK_a;
		g_unk0x009544f0[DIK_S] = SDLK_s;
		g_unk0x009544f0[DIK_D] = SDLK_d;
		g_unk0x009544f0[DIK_F] = SDLK_f;
		g_unk0x009544f0[DIK_G] = SDLK_g;
		g_unk0x009544f0[DIK_H] = SDLK_h;
		g_unk0x009544f0[DIK_J] = SDLK_j;
		g_unk0x009544f0[DIK_K] = SDLK_k;
		g_unk0x009544f0[DIK_L] = SDLK_l;
		g_unk0x009544f0[DIK_SEMICOLON] = SDLK_SEMICOLON;
		g_unk0x009544f0[DIK_APOSTROPHE] = SDLK_QUOTE;
		g_unk0x009544f0[DIK_GRAVE] = SDLK_BACKQUOTE;
		g_unk0x009544f0[DIK_LSHIFT] = SDLK_LSHIFT;
		g_unk0x009544f0[DIK_BACKSLASH] = SDLK_BACKSLASH;
		g_unk0x009544f0[DIK_OEM_102] = SDLK_BACKSLASH;
		g_unk0x009544f0[DIK_Z] = SDLK_z;
		g_unk0x009544f0[DIK_X] = SDLK_x;
		g_unk0x009544f0[DIK_C] = SDLK_c;
		g_unk0x009544f0[DIK_V] = SDLK_v;
		g_unk0x009544f0[DIK_B] = SDLK_b;
		g_unk0x009544f0[DIK_N] = SDLK_n;
		g_unk0x009544f0[DIK_M] = SDLK_m;
		g_unk0x009544f0[DIK_COMMA] = SDLK_COMMA;
		g_unk0x009544f0[DIK_PERIOD] = SDLK_PERIOD;
		g_unk0x009544f0[DIK_SLASH] = SDLK_SLASH;
		g_unk0x009544f0[DIK_RSHIFT] = SDLK_RSHIFT;
		g_unk0x009544f0[DIK_MULTIPLY] = SDLK_KP_MULTIPLY;
		g_unk0x009544f0[DIK_LMENU] = SDLK_LALT;
		g_unk0x009544f0[DIK_SPACE] = SDLK_SPACE;
		g_unk0x009544f0[DIK_CAPITAL] = SDLK_CAPSLOCK;
		g_unk0x009544f0[DIK_F1] = SDLK_F1;
		g_unk0x009544f0[DIK_F2] = SDLK_F2;
		g_unk0x009544f0[DIK_F3] = SDLK_F3;
		g_unk0x009544f0[DIK_F4] = SDLK_F4;
		g_unk0x009544f0[DIK_F5] = SDLK_F5;
		g_unk0x009544f0[DIK_F6] = SDLK_F6;
		g_unk0x009544f0[DIK_F7] = SDLK_F7;
		g_unk0x009544f0[DIK_F8] = SDLK_F8;
		g_unk0x009544f0[DIK_F9] = SDLK_F9;
		g_unk0x009544f0[DIK_F10] = SDLK_F10;
		g_unk0x009544f0[DIK_NUMLOCK] = SDLK_NUMLOCK;
		g_unk0x009544f0[DIK_SCROLL] = SDLK_SCROLLOCK;
		g_unk0x009544f0[DIK_NUMPAD7] = SDLK_KP7;
		g_unk0x009544f0[DIK_NUMPAD8] = SDLK_KP8;
		g_unk0x009544f0[DIK_NUMPAD9] = SDLK_KP9;
		g_unk0x009544f0[DIK_SUBTRACT] = SDLK_KP_MINUS;
		g_unk0x009544f0[DIK_NUMPAD4] = SDLK_KP4;
		g_unk0x009544f0[DIK_NUMPAD5] = SDLK_KP5;
		g_unk0x009544f0[DIK_NUMPAD6] = SDLK_KP6;
		g_unk0x009544f0[DIK_ADD] = SDLK_KP_PLUS;
		g_unk0x009544f0[DIK_NUMPAD1] = SDLK_KP1;
		g_unk0x009544f0[DIK_NUMPAD2] = SDLK_KP2;
		g_unk0x009544f0[DIK_NUMPAD3] = SDLK_KP3;
		g_unk0x009544f0[DIK_NUMPAD0] = SDLK_KP0;
		g_unk0x009544f0[DIK_DECIMAL] = SDLK_KP_PERIOD;
		g_unk0x009544f0[DIK_F11] = SDLK_F11;
		g_unk0x009544f0[DIK_F12] = SDLK_F12;
		g_unk0x009544f0[DIK_F13] = SDLK_F13;
		g_unk0x009544f0[DIK_F14] = SDLK_F14;
		g_unk0x009544f0[DIK_F15] = SDLK_F15;
		g_unk0x009544f0[DIK_NUMPADEQUALS] = SDLK_KP_EQUALS;
		g_unk0x009544f0[DIK_NUMPADENTER] = SDLK_KP_ENTER;
		g_unk0x009544f0[DIK_RCONTROL] = SDLK_RCTRL;
		g_unk0x009544f0[DIK_DIVIDE] = SDLK_KP_DIVIDE;
		g_unk0x009544f0[DIK_SYSRQ] = SDLK_SYSREQ;
		g_unk0x009544f0[DIK_RMENU] = SDLK_RALT;
		g_unk0x009544f0[DIK_PAUSE] = SDLK_PAUSE;
		g_unk0x009544f0[DIK_HOME] = SDLK_HOME;
		g_unk0x009544f0[DIK_UP] = SDLK_UP;
		g_unk0x009544f0[DIK_PRIOR] = SDLK_PAGEUP;
		g_unk0x009544f0[DIK_LEFT] = SDLK_LEFT;
		g_unk0x009544f0[DIK_RIGHT] = SDLK_RIGHT;
		g_unk0x009544f0[DIK_END] = SDLK_END;
		g_unk0x009544f0[DIK_DOWN] = SDLK_DOWN;
		g_unk0x009544f0[DIK_NEXT] = SDLK_PAGEDOWN;
		g_unk0x009544f0[DIK_INSERT] = SDLK_INSERT;
		g_unk0x009544f0[DIK_DELETE] = SDLK_DELETE;
		g_unk0x009544f0[DIK_LWIN] = SDLK_LMETA;
		g_unk0x009544f0[DIK_RWIN] = SDLK_RMETA;
		g_unk0x009544f0[DIK_APPS] = SDLK_MENU;
		g_unk0x009548f0 = true;
	}

	return g_unk0x009544f0[scanCode];
}

static int g_unk0x009548f8[SDLK_LAST];
static bool g_unk0x00954e04;

// FUNCTION: ROBLOX 0x0046db70
int FUN_0046db70(SDLKey key)
{
	if (!g_unk0x00954e04) {
		for (int i = 0; i < SDLK_LAST; i++) {
			g_unk0x009548f8[i] = 0;
		}

		g_unk0x009548f8[SDLK_ESCAPE] = DIK_ESCAPE;
		g_unk0x009548f8[SDLK_1] = DIK_1;
		g_unk0x009548f8[SDLK_2] = DIK_2;
		g_unk0x009548f8[SDLK_3] = DIK_3;
		g_unk0x009548f8[SDLK_4] = DIK_4;
		g_unk0x009548f8[SDLK_5] = DIK_5;
		g_unk0x009548f8[SDLK_6] = DIK_6;
		g_unk0x009548f8[SDLK_7] = DIK_7;
		g_unk0x009548f8[SDLK_8] = DIK_8;
		g_unk0x009548f8[SDLK_9] = DIK_9;
		g_unk0x009548f8[SDLK_0] = DIK_0;
		g_unk0x009548f8[SDLK_MINUS] = DIK_MINUS;
		g_unk0x009548f8[SDLK_EQUALS] = DIK_EQUALS;
		g_unk0x009548f8[SDLK_BACKSPACE] = DIK_BACK;
		g_unk0x009548f8[SDLK_TAB] = DIK_TAB;
		g_unk0x009548f8[SDLK_q] = DIK_Q;
		g_unk0x009548f8[SDLK_w] = DIK_W;
		g_unk0x009548f8[SDLK_e] = DIK_E;
		g_unk0x009548f8[SDLK_r] = DIK_R;
		g_unk0x009548f8[SDLK_t] = DIK_T;
		g_unk0x009548f8[SDLK_y] = DIK_Y;
		g_unk0x009548f8[SDLK_u] = DIK_U;
		g_unk0x009548f8[SDLK_i] = DIK_I;
		g_unk0x009548f8[SDLK_o] = DIK_O;
		g_unk0x009548f8[SDLK_p] = DIK_P;
		g_unk0x009548f8[SDLK_LEFTBRACKET] = DIK_LBRACKET;
		g_unk0x009548f8[SDLK_RIGHTBRACKET] = DIK_RBRACKET;
		g_unk0x009548f8[SDLK_RETURN] = DIK_RETURN;
		g_unk0x009548f8[SDLK_LCTRL] = DIK_LCONTROL;
		g_unk0x009548f8[SDLK_a] = DIK_A;
		g_unk0x009548f8[SDLK_s] = DIK_S;
		g_unk0x009548f8[SDLK_d] = DIK_D;
		g_unk0x009548f8[SDLK_f] = DIK_F;
		g_unk0x009548f8[SDLK_g] = DIK_G;
		g_unk0x009548f8[SDLK_h] = DIK_H;
		g_unk0x009548f8[SDLK_j] = DIK_J;
		g_unk0x009548f8[SDLK_k] = DIK_K;
		g_unk0x009548f8[SDLK_l] = DIK_L;
		g_unk0x009548f8[SDLK_SEMICOLON] = DIK_SEMICOLON;
		g_unk0x009548f8[SDLK_QUOTE] = DIK_APOSTROPHE;
		g_unk0x009548f8[SDLK_BACKQUOTE] = DIK_GRAVE;
		g_unk0x009548f8[SDLK_LSHIFT] = DIK_LSHIFT;
		g_unk0x009548f8[SDLK_BACKSLASH] = DIK_OEM_102;
		g_unk0x009548f8[SDLK_z] = DIK_Z;
		g_unk0x009548f8[SDLK_x] = DIK_X;
		g_unk0x009548f8[SDLK_c] = DIK_C;
		g_unk0x009548f8[SDLK_v] = DIK_V;
		g_unk0x009548f8[SDLK_b] = DIK_B;
		g_unk0x009548f8[SDLK_n] = DIK_N;
		g_unk0x009548f8[SDLK_m] = DIK_M;
		g_unk0x009548f8[SDLK_COMMA] = DIK_COMMA;
		g_unk0x009548f8[SDLK_PERIOD] = DIK_PERIOD;
		g_unk0x009548f8[SDLK_SLASH] = DIK_SLASH;
		g_unk0x009548f8[SDLK_RSHIFT] = DIK_RSHIFT;
		g_unk0x009548f8[SDLK_KP_MULTIPLY] = DIK_MULTIPLY;
		g_unk0x009548f8[SDLK_LALT] = DIK_LMENU;
		g_unk0x009548f8[SDLK_SPACE] = DIK_SPACE;
		g_unk0x009548f8[SDLK_CAPSLOCK] = DIK_CAPITAL;
		g_unk0x009548f8[SDLK_F1] = DIK_F1;
		g_unk0x009548f8[SDLK_F2] = DIK_F2;
		g_unk0x009548f8[SDLK_F3] = DIK_F3;
		g_unk0x009548f8[SDLK_F4] = DIK_F4;
		g_unk0x009548f8[SDLK_F5] = DIK_F5;
		g_unk0x009548f8[SDLK_F6] = DIK_F6;
		g_unk0x009548f8[SDLK_F7] = DIK_F7;
		g_unk0x009548f8[SDLK_F8] = DIK_F8;
		g_unk0x009548f8[SDLK_F9] = DIK_F9;
		g_unk0x009548f8[SDLK_F10] = DIK_F10;
		g_unk0x009548f8[SDLK_NUMLOCK] = DIK_NUMLOCK;
		g_unk0x009548f8[SDLK_SCROLLOCK] = DIK_SCROLL;
		g_unk0x009548f8[SDLK_KP7] = DIK_NUMPAD7;
		g_unk0x009548f8[SDLK_KP8] = DIK_NUMPAD8;
		g_unk0x009548f8[SDLK_KP9] = DIK_NUMPAD9;
		g_unk0x009548f8[SDLK_KP_MINUS] = DIK_SUBTRACT;
		g_unk0x009548f8[SDLK_KP4] = DIK_NUMPAD4;
		g_unk0x009548f8[SDLK_KP5] = DIK_NUMPAD5;
		g_unk0x009548f8[SDLK_KP6] = DIK_NUMPAD6;
		g_unk0x009548f8[SDLK_KP_PLUS] = DIK_ADD;
		g_unk0x009548f8[SDLK_KP1] = DIK_NUMPAD1;
		g_unk0x009548f8[SDLK_KP2] = DIK_NUMPAD2;
		g_unk0x009548f8[SDLK_KP3] = DIK_NUMPAD3;
		g_unk0x009548f8[SDLK_KP0] = DIK_NUMPAD0;
		g_unk0x009548f8[SDLK_KP_PERIOD] = DIK_DECIMAL;
		g_unk0x009548f8[SDLK_F11] = DIK_F11;
		g_unk0x009548f8[SDLK_F12] = DIK_F12;
		g_unk0x009548f8[SDLK_F13] = DIK_F13;
		g_unk0x009548f8[SDLK_F14] = DIK_F14;
		g_unk0x009548f8[SDLK_F15] = DIK_F15;
		g_unk0x009548f8[SDLK_KP_EQUALS] = DIK_NUMPADEQUALS;
		g_unk0x009548f8[SDLK_KP_ENTER] = DIK_NUMPADENTER;
		g_unk0x009548f8[SDLK_RCTRL] = DIK_RCONTROL;
		g_unk0x009548f8[SDLK_KP_DIVIDE] = DIK_DIVIDE;
		g_unk0x009548f8[SDLK_SYSREQ] = DIK_SYSRQ;
		g_unk0x009548f8[SDLK_RALT] = DIK_RMENU;
		g_unk0x009548f8[SDLK_PAUSE] = DIK_PAUSE;
		g_unk0x009548f8[SDLK_HOME] = DIK_HOME;
		g_unk0x009548f8[SDLK_UP] = DIK_UP;
		g_unk0x009548f8[SDLK_PAGEUP] = DIK_PRIOR;
		g_unk0x009548f8[SDLK_LEFT] = DIK_LEFT;
		g_unk0x009548f8[SDLK_RIGHT] = DIK_RIGHT;
		g_unk0x009548f8[SDLK_END] = DIK_END;
		g_unk0x009548f8[SDLK_DOWN] = DIK_DOWN;
		g_unk0x009548f8[SDLK_PAGEDOWN] = DIK_NEXT;
		g_unk0x009548f8[SDLK_INSERT] = DIK_INSERT;
		g_unk0x009548f8[SDLK_DELETE] = DIK_DELETE;
		g_unk0x009548f8[SDLK_LMETA] = DIK_LWIN;
		g_unk0x009548f8[SDLK_RMETA] = DIK_RWIN;
		g_unk0x009548f8[SDLK_MENU] = DIK_APPS;

		g_unk0x00954e04 = true;
	}

	return g_unk0x009548f8[key];
}

static int g_unk0x00954e08[SDLK_LAST];
static bool g_unk0x00955314;

// FUNCTION: ROBLOX 0x0046dff0
int FUN_0046dff0(SDLKey key)
{
	if (!g_unk0x00955314) {
		for (int i = 0; i < SDLK_LAST; i++) {
			g_unk0x00954e08[i] = 0;
		}

		g_unk0x00954e08[SDLK_RETURN] = VK_RETURN;
		g_unk0x00954e08[SDLK_KP_ENTER] = VK_RETURN;
		g_unk0x00954e08[SDLK_PRINT] = VK_PRINT;
		g_unk0x00954e08[SDLK_SYSREQ] = VK_SNAPSHOT;
		g_unk0x00954e08[SDLK_ESCAPE] = VK_ESCAPE;
		g_unk0x00954e08[SDLK_BACKSPACE] = VK_BACK;
		g_unk0x00954e08[SDLK_TAB] = VK_TAB;
		g_unk0x00954e08[SDLK_LCTRL] = VK_LCONTROL;
		g_unk0x00954e08[SDLK_LSHIFT] = VK_LSHIFT;
		g_unk0x00954e08[SDLK_BACKSLASH] = VK_OEM_102;
		g_unk0x00954e08[SDLK_RSHIFT] = VK_RSHIFT;
		g_unk0x00954e08[SDLK_KP_MULTIPLY] = VK_MULTIPLY;
		g_unk0x00954e08[SDLK_LALT] = VK_LMENU;
		g_unk0x00954e08[SDLK_SPACE] = VK_SPACE;
		g_unk0x00954e08[SDLK_CAPSLOCK] = VK_CAPITAL;
		g_unk0x00954e08[SDLK_F1] = VK_F1;
		g_unk0x00954e08[SDLK_F2] = VK_F2;
		g_unk0x00954e08[SDLK_F3] = VK_F3;
		g_unk0x00954e08[SDLK_F4] = VK_F4;
		g_unk0x00954e08[SDLK_F5] = VK_F5;
		g_unk0x00954e08[SDLK_F6] = VK_F6;
		g_unk0x00954e08[SDLK_F7] = VK_F7;
		g_unk0x00954e08[SDLK_F8] = VK_F8;
		g_unk0x00954e08[SDLK_F9] = VK_F9;
		g_unk0x00954e08[SDLK_F10] = VK_F10;
		g_unk0x00954e08[SDLK_NUMLOCK] = VK_NUMLOCK;
		g_unk0x00954e08[SDLK_SCROLLOCK] = VK_SCROLL;
		g_unk0x00954e08[SDLK_KP7] = VK_NUMPAD7;
		g_unk0x00954e08[SDLK_KP8] = VK_NUMPAD8;
		g_unk0x00954e08[SDLK_KP9] = VK_NUMPAD9;
		g_unk0x00954e08[SDLK_KP_MINUS] = VK_SUBTRACT;
		g_unk0x00954e08[SDLK_KP4] = VK_NUMPAD4;
		g_unk0x00954e08[SDLK_KP5] = VK_NUMPAD5;
		g_unk0x00954e08[SDLK_KP6] = VK_NUMPAD6;
		g_unk0x00954e08[SDLK_KP_PLUS] = VK_ADD;
		g_unk0x00954e08[SDLK_KP1] = VK_NUMPAD1;
		g_unk0x00954e08[SDLK_KP2] = VK_NUMPAD2;
		g_unk0x00954e08[SDLK_KP3] = VK_NUMPAD3;
		g_unk0x00954e08[SDLK_KP0] = VK_NUMPAD0;
		g_unk0x00954e08[SDLK_KP_PERIOD] = VK_DECIMAL;
		g_unk0x00954e08[SDLK_F11] = VK_F11;
		g_unk0x00954e08[SDLK_F12] = VK_F12;
		g_unk0x00954e08[SDLK_F13] = VK_F13;
		g_unk0x00954e08[SDLK_F14] = VK_F14;
		g_unk0x00954e08[SDLK_F15] = VK_F15;
		g_unk0x00954e08[SDLK_RCTRL] = VK_RCONTROL;
		g_unk0x00954e08[SDLK_KP_DIVIDE] = VK_DIVIDE;
		g_unk0x00954e08[SDLK_RALT] = VK_RMENU;
		g_unk0x00954e08[SDLK_HOME] = VK_HOME;
		g_unk0x00954e08[SDLK_UP] = VK_UP;
		g_unk0x00954e08[SDLK_PAGEUP] = VK_PRIOR;
		g_unk0x00954e08[SDLK_LEFT] = VK_LEFT;
		g_unk0x00954e08[SDLK_RIGHT] = VK_RIGHT;
		g_unk0x00954e08[SDLK_END] = VK_END;
		g_unk0x00954e08[SDLK_DOWN] = VK_DOWN;
		g_unk0x00954e08[SDLK_PAGEDOWN] = VK_NEXT;
		g_unk0x00954e08[SDLK_INSERT] = VK_INSERT;
		g_unk0x00954e08[SDLK_DELETE] = VK_DELETE;
		g_unk0x00954e08[SDLK_LMETA] = VK_LWIN;
		g_unk0x00954e08[SDLK_RMETA] = VK_RWIN;
		g_unk0x00954e08[SDLK_MENU] = VK_APPS;
		g_unk0x00955314 = true;
	}

	return g_unk0x00954e08[key];
}

// FUNCTION: ROBLOX 0x0046e290
RBX::UIEvent::EventType FUN_0046e290(UINT message)
{
	switch (message) {
	case WM_MOUSEMOVE:
		return RBX::UIEvent::MOUSE_MOVE;
	case WM_LBUTTONDOWN:
		return RBX::UIEvent::MOUSE_LEFT_BUTTON_DOWN;
	case WM_RBUTTONDOWN:
		return RBX::UIEvent::MOUSE_RIGHT_BUTTON_DOWN;
	case WM_LBUTTONUP:
		return RBX::UIEvent::MOUSE_LEFT_BUTTON_UP;
	case WM_RBUTTONUP:
		return RBX::UIEvent::MOUSE_RIGHT_BUTTON_UP;
	default:
		return RBX::UIEvent::NO_EVENT;
	}
}

// FUNCTION: ROBLOX 0x0046e2e0
SDLMod FUN_0046e2e0(const BYTE* keys)
{
	int mod = KMOD_NONE;
	if (keys[DIK_LSHIFT] & 0x80) {
		mod = KMOD_LSHIFT;
	}
	if (keys[DIK_RSHIFT] & 0x80) {
		mod |= KMOD_RSHIFT;
	}
	if (keys[DIK_LCONTROL] & 0x80) {
		mod |= KMOD_LCTRL;
	}
	if (keys[DIK_RCONTROL] & 0x80) {
		mod |= KMOD_RCTRL;
	}
	if (keys[DIK_LMENU] & 0x80) {
		mod |= KMOD_LALT;
	}
	if (keys[DIK_LMENU] & 0x80) {
		mod |= KMOD_RALT;
	}
	return static_cast<SDLMod>(mod);
}

// STUB: ROBLOX 0x0046e3e0
void FUN_0046e3e0(
	const G3D::Vector2& mouseDelta,
	G3D::Vector2& wrapMouseDelta,
	G3D::Vector2& position,
	const G3D::Vector2& size,
	int border,
	float stickiness
)
{
	G3D::Vector2 half(size.x * 0.5, size.y * 0.5);
	float inset = border;
	RBX::Rect bounds(inset - half.x, inset - half.y, half.x - inset, half.y - inset);

	G3D::Vector2 oldPosition = position;
	bounds.unionWith(RBX::Rect(oldPosition, oldPosition));

	G3D::Vector2 newPosition = mouseDelta + oldPosition;
	G3D::Vector2 clamped = newPosition.clamp(bounds.low, bounds.high);
	G3D::Vector2 overflow = newPosition - clamped;

	position = clamped + overflow * stickiness;
	wrapMouseDelta += overflow;
}

// FUNCTION: ROBLOX 0x0046e530
void FUN_0046e530(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
)
{
	FUN_0046e3e0(mouseDelta, wrapMouseDelta, position, size, 6, 0.0f);
}

// FUNCTION: ROBLOX 0x0046e560
void FUN_0046e560(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
)
{
	float y = mouseDelta.y + position.y;
	FUN_0046e3e0(mouseDelta, wrapMouseDelta, position, size, 6, 0.0f);
	wrapMouseDelta.y = 0.0f;
	position.y = y;

	float inset = size.y * 0.5 - 2.0;
	float top = -inset;
	position.y = std::max(top, position.y);
}

// FUNCTION: ROBLOX 0x0046e5f0
void FUN_0046e5f0(
	const G3D::Vector2& mouseDelta, G3D::Vector2& wrapMouseDelta, G3D::Vector2& position, const G3D::Vector2& size
)
{
	float y = mouseDelta.y + position.y;
	FUN_0046e3e0(mouseDelta, wrapMouseDelta, position, size, 20, 0.05f);
	wrapMouseDelta.y = 0.0f;
	position.y = y;
}
