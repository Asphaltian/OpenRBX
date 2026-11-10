#include "DHTMLWindow.h"

char sDHTMLWindowService[] = "HtmlService";
char sDHTMLWindow[] = "HtmlWindow";

template const RBX::FactoryProduct<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>::Creator
	RBX::FactoryProduct<DHTMLWindowService, RBX::Instance, sDHTMLWindowService>::creator;

// GLOBAL: ROBLOX 0x00952f1c
static Marshaller* g_unk0x00952f1c;

// STUB: ROBLOX 0x0041a4a0
Marshaller* FUN_0041a4a0()
{
	STUB(0x41a4a0);
	return NULL;
}

// STUB: ROBLOX 0x0041ac10
void FUN_0041ac10()
{
	g_unk0x00952f1c = FUN_0041a4a0();
}

// FUNCTION: ROBLOX 0x0041bec0
DHTMLWindowService::DHTMLWindowService()
{
	setName("WindowService");
}

// FUNCTION: ROBLOX 0x0041bfc0
DHTMLWindow::DHTMLWindow() : m_unk0x104(NULL)
{
	setName("Window");
}
