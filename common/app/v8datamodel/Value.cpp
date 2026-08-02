#include "v8datamodel/Value.h"

#include "util/Name.h"

namespace RBX {

char sBrickColorValue[] = "BrickColorValue";

char sColor3Value[] = "Color3Value";

char sCFrameValue[] = "CFrameValue";

char sVector3Value[] = "Vector3Value";

char sStringValue[] = "StringValue";

char sDoubleValue[] = "NumberValue";

char sBoolValue[] = "BoolValue";

char sIntValue[] = "IntValue";

char sObjectValue[] = "ObjectValue";

} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sIntValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBoolValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sDoubleValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sStringValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sVector3Value>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sCFrameValue>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sColor3Value>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sBrickColorValue>();
template void RBX::Name::callDoDeclare<RBX::sIntValue>();
template void RBX::Name::callDoDeclare<RBX::sBoolValue>();
template void RBX::Name::callDoDeclare<RBX::sDoubleValue>();
template void RBX::Name::callDoDeclare<RBX::sStringValue>();
template void RBX::Name::callDoDeclare<RBX::sVector3Value>();
template void RBX::Name::callDoDeclare<RBX::sCFrameValue>();
template void RBX::Name::callDoDeclare<RBX::sColor3Value>();
template void RBX::Name::callDoDeclare<RBX::sBrickColorValue>();
