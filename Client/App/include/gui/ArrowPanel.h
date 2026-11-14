#ifndef GUI_ARROWPANEL_H
#define GUI_ARROWPANEL_H

#include "gui/GUI.h"
#include "placeholder.h"
#include "v8world/Controller.h"

#include <SDL_keysym.h>
#include <string>

namespace RBX {

class Adorn;
class DataModel;

class ArrowPanel : public TopMenuBar
{
protected:
	DataModel* dataModel;
	Controller::ControllerType controller;

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

} // namespace RBX

#endif // GUI_ARROWPANEL_H
