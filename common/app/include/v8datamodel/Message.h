#ifndef V8DATAMODEL_MESSAGE_H
#define V8DATAMODEL_MESSAGE_H

#include "decomp.h"
#include "v8tree/Instance.h"

#include <string>

namespace RBX {

// SIZE 0x124
class Message : public Instance
{
public:
	void setText(const std::string& value);

private:
	undefined m_unk0x0f8[0x108 - 0x0f8]; // 0x0f8
	std::string text;                    // 0x108
};

DECOMP_SIZE_ASSERT(Message, 0x124)

} // namespace RBX

#endif // V8DATAMODEL_MESSAGE_H
