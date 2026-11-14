#ifndef GUI_HEALTHHUD_H
#define GUI_HEALTHHUD_H

#include "gui/GUI.h"
#include "placeholder.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

class Visit;

class HealthHud : public GuiItem
{
private:
	boost::shared_ptr<Visit> visit;

	virtual void render2d(Adorn* adorn);

public:
	HealthHud();
};

} // namespace RBX

#endif // GUI_HEALTHHUD_H
