#ifndef V8DATAMODEL_MESSAGE_H
#define V8DATAMODEL_MESSAGE_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/IRenderable.h"
#include "v8tree/Instance.h"

#include <string>

namespace RBX {

extern char sHint[];

extern char sMessage[];

// SIZE 0x124
class Message : public DescribedCreatable<Message, Instance, sMessage>, public IRenderable
{
protected:
	std::string text; // 0x108

	void renderFullScreen(Adorn* adorn);
	void renderPersonalMsg(Adorn* adorn);

public:
	Message();

	// FUNCTION: WEBSERVICE 0x100e6150 FOLDED
	// FUNCTION: ROBLOX 0x005df250 FOLDED
	virtual bool shouldRender2d() const { return true; }

	virtual void render2d(Adorn* adorn);

	// FUNCTION: WEBSERVICE 0x100e44b0 FOLDED
	// FUNCTION: ROBLOX 0x00633110 FOLDED
	virtual bool askSetParent(const Instance* instance) const { return true; }

	// FUNCTION: WEBSERVICE 0x100ee460
	const std::string& getText() const { return text; }

	void setText(const std::string& value);
};

DECOMP_SIZE_ASSERT(Message, 0x124)

// SIZE 0x124
class Hint : public DescribedCreatable<Hint, Message, sHint>
{
private:
	virtual void render2d(Adorn* adorn);

public:
	// SYNTHETIC: WEBSERVICE 0x1008e400
	// SYNTHETIC: ROBLOX 0x005cdcc0
	// RBX::Hint::Hint
};

DECOMP_SIZE_ASSERT(Hint, 0x124)

// clang-format off
// FUNCTION: WEBSERVICE 0x1008c790
// FUNCTION: ROBLOX 0x005ccab0
// RBX::Message::~Message
// FUNCTION: WEBSERVICE 0x100eeb90
// FUNCTION: ROBLOX 0x0062dca0
// RBX::Message::`scalar deleting destructor'
// clang-format on

// clang-format off
// FUNCTION: WEBSERVICE 0x1021d530
// RBX::`dynamic initializer for 'desc_Text''
// SYNTHETIC: WEBSERVICE 0x10224770
// RBX::`dynamic atexit destructor for 'desc_Text''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_MESSAGE_H
