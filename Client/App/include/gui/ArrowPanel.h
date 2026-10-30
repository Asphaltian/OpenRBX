#ifndef GUI_ARROWPANEL_H
#define GUI_ARROWPANEL_H

#include "decomp.h"
#include "gui/GUI.h"
#include "v8world/Controller.h"

#include <SDL_keysym.h>
#include <string>

namespace RBX {

class Adorn;
class DataModel;

// SIZE 0x12c
class ArrowPanel : public TopMenuBar
{
protected:
	DataModel* dataModel;                  // 0x124
	Controller::ControllerType controller; // 0x128

	void init(
		Adorn* adorn,
		const std::string& image,
		const std::string& forward,
		const std::string& backward,
		const std::string& left,
		const std::string& right,
		SDLKey forwardKey,
		SDLKey backwardKey,
		SDLKey leftKey,
		SDLKey rightKey
	);

public:
	ArrowPanel(Adorn* adorn, DataModel* dataModel, Controller::ControllerType controller);

	virtual bool isVisible() const;
	virtual G3D::Vector2 getSize() const;
};

DECOMP_SIZE_ASSERT(ArrowPanel, 0x12c)

} // namespace RBX

#endif // GUI_ARROWPANEL_H
