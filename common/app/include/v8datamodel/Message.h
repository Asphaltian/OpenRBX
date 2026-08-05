#ifndef V8DATAMODEL_MESSAGE_H
#define V8DATAMODEL_MESSAGE_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"

#include <string>

namespace RBX {

extern char sHint[];

extern char sMessage[];

// SIZE 0x124
class Message : public DescribedCreatable<Message, Instance, sMessage>
{
public:
	// FUNCTION: WEBSERVICE 0x100ee460
	const std::string& getText() const { return text; }

	void setText(const std::string& value);

private:
	undefined m_unk0x0f8[0x108 - 0x0f8]; // 0x0f8
	std::string text;                    // 0x108
};

DECOMP_SIZE_ASSERT(Message, 0x124)

// SIZE 0x124
class Hint : public DescribedCreatable<Hint, Message, sHint>
{
};

DECOMP_SIZE_ASSERT(Hint, 0x124)

// clang-format off
// STUB: WEBSERVICE 0x1008c790
// RBX::Message::~Message
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_MESSAGE_H
