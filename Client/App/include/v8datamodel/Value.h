#ifndef V8DATAMODEL_VALUE_H
#define V8DATAMODEL_VALUE_H

#include "decomp.h"
#include "v8tree/Instance.h"

namespace RBX {

extern char sBrickColorValue[];

extern char sColor3Value[];

extern char sCFrameValue[];

extern char sVector3Value[];

extern char sStringValue[];

extern char sDoubleValue[];

extern char sBoolValue[];

extern char sIntValue[];

extern char sObjectValue[];
// SIZE 0x100
class ObjectValue : public DescribedCreatable<ObjectValue, Instance, sObjectValue>
{
private:
	undefined m_unk0x0f8[0x100 - 0x0f8]; // 0x0f8
};

DECOMP_SIZE_ASSERT(ObjectValue, 0x100)

} // namespace RBX

#endif // V8DATAMODEL_VALUE_H
