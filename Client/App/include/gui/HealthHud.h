#ifndef GUI_HEALTHHUD_H
#define GUI_HEALTHHUD_H

#include "decomp.h"
#include "gui/GUI.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

class Visit;

// SIZE 0x114
class HealthHud : public GuiItem
{
private:
	boost::shared_ptr<Visit> visit; // 0x10c

	virtual void render2d(Adorn* adorn);

public:
	HealthHud();

	// SYNTHETIC: ROBLOX 0x0065f150
	// RBX::HealthHud::~HealthHud

	// SYNTHETIC: ROBLOX 0x0065f1d0
	// RBX::HealthHud::`scalar deleting destructor'

	// SYNTHETIC: ROBLOX 0x0065f360
	// ??$create@VVisit@RBX@@@ServiceProvider@RBX@@SAPAVVisit@1@PBVInstance@1@@Z
};

DECOMP_SIZE_ASSERT(HealthHud, 0x114)

} // namespace RBX

#endif // GUI_HEALTHHUD_H
