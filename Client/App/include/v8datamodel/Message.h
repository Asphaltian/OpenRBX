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
public:
	// FUNCTION: WEBSERVICE 0x100ee460
	const std::string& getText() const { return text; }

	void setText(const std::string& value);

private:
	std::string text; // 0x108
};

DECOMP_SIZE_ASSERT(Message, 0x124)

// SIZE 0x124
class Hint : public DescribedCreatable<Hint, Message, sHint>
{
};

DECOMP_SIZE_ASSERT(Hint, 0x124)

// clang-format off
// FUNCTION: WEBSERVICE 0x1008c790
// RBX::Message::~Message
// FUNCTION: WEBSERVICE 0x100eeb90
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
