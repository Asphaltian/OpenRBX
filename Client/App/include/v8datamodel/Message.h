#ifndef V8DATAMODEL_MESSAGE_H
#define V8DATAMODEL_MESSAGE_H

#include "placeholder.h"
#include "util/Handle.h"
#include "util/IRenderable.h"
#include "v8tree/Instance.h"

#include <string>

namespace RBX {

extern char sHint[];

extern char sMessage[];

class Message : public DescribedCreatable<Message, Instance, sMessage>, public IRenderable
{
protected:
	std::string text;

	void renderFullScreen(Adorn* adorn);
	void renderPersonalMsg(Adorn* adorn);

public:
	Message();

	virtual bool shouldRender2d() const { return true; }

	virtual void render2d(Adorn* adorn);

	virtual bool askSetParent(const Instance* instance) const { return true; }

	const std::string& getText() const { return text; }

	void setText(const std::string& value);
};

class Hint : public DescribedCreatable<Hint, Message, sHint>
{
private:
	virtual void render2d(Adorn* adorn);

public:
};

} // namespace RBX

#endif // V8DATAMODEL_MESSAGE_H
