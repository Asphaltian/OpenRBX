#include "ReflectionMetadata.h"

namespace RBX {
namespace Reflection {
namespace Metadata {

// GLOBAL: ROBLOX 0x00919c5c
char sReflection[] = "ReflectionMetadata";

// GLOBAL: ROBLOX 0x00919c70
char sClasses[] = "ReflectionMetadataClasses";

// GLOBAL: ROBLOX 0x00919c8c
char sItem[] = "ReflectionMetadataItem";

// GLOBAL: ROBLOX 0x00919ca4
char sClass[] = "ReflectionMetadataClass";

// GLOBAL: ROBLOX 0x00919cbc
char sProperties[] = "ReflectionMetadataProperties";

// GLOBAL: ROBLOX 0x00919cdc
char sFunctions[] = "ReflectionMetadataFunctions";

// GLOBAL: ROBLOX 0x00919cf8
char sEvents[] = "ReflectionMetadataEvents";

// GLOBAL: ROBLOX 0x00919d14
char sMember[] = "ReflectionMetadataMember";

// FUNCTION: ROBLOX 0x004479b0
Reflection::Reflection() : m_unk0xf8(0)
{
}

RBX::Reflection::BoundProp<bool, 1> Item::g_unk0x00953a34("Browsable", "Reflection", &Item::m_unk0xf8);

RBX::Reflection::BoundProp<bool, 1> Item::g_unk0x00953a50("Deprecated", "Reflection", &Item::m_unk0xf9);

RBX::Reflection::BoundProp<bool, 1> Item::g_unk0x00953a6c("Preliminary", "Reflection", &Item::m_unk0xfa);

RBX::Reflection::BoundProp<std::string, 1> Item::g_unk0x00953a88("summary", "Reflection", &Item::m_unk0xfc);

RBX::Reflection::BoundProp<int, 1> Class::g_unk0x00953aa4("ExplorerOrder", "Reflection", &Class::m_unk0x118);

RBX::Reflection::BoundProp<int, 1> Class::g_unk0x00953ac0("ExplorerImageIndex", "Reflection", &Class::m_unk0x11c);

} // namespace Metadata
} // namespace Reflection
} // namespace RBX

template class RBX::
	FactoryProduct<RBX::Reflection::Metadata::Reflection, RBX::Instance, RBX::Reflection::Metadata::sReflection>;
template class RBX::
	DescribedCreatable<RBX::Reflection::Metadata::Reflection, RBX::Instance, RBX::Reflection::Metadata::sReflection>;
template class RBX::
	FactoryProduct<RBX::Reflection::Metadata::Classes, RBX::Instance, RBX::Reflection::Metadata::sClasses>;
template class RBX::
	DescribedCreatable<RBX::Reflection::Metadata::Classes, RBX::Instance, RBX::Reflection::Metadata::sClasses>;
template class RBX::FactoryProduct<
	RBX::Reflection::Metadata::Class,
	RBX::Reflection::Metadata::Item,
	RBX::Reflection::Metadata::sClass>;
template class RBX::DescribedCreatable<
	RBX::Reflection::Metadata::Class,
	RBX::Reflection::Metadata::Item,
	RBX::Reflection::Metadata::sClass>;
template class RBX::FactoryProduct<
	RBX::Reflection::Metadata::Properties,
	RBX::Reflection::Metadata::Members,
	RBX::Reflection::Metadata::sProperties>;
template class RBX::DescribedCreatable<
	RBX::Reflection::Metadata::Properties,
	RBX::Reflection::Metadata::Members,
	RBX::Reflection::Metadata::sProperties>;
template class RBX::FactoryProduct<
	RBX::Reflection::Metadata::Functions,
	RBX::Reflection::Metadata::Members,
	RBX::Reflection::Metadata::sFunctions>;
template class RBX::DescribedCreatable<
	RBX::Reflection::Metadata::Functions,
	RBX::Reflection::Metadata::Members,
	RBX::Reflection::Metadata::sFunctions>;
template class RBX::FactoryProduct<
	RBX::Reflection::Metadata::Events,
	RBX::Reflection::Metadata::Members,
	RBX::Reflection::Metadata::sEvents>;
template class RBX::DescribedCreatable<
	RBX::Reflection::Metadata::Events,
	RBX::Reflection::Metadata::Members,
	RBX::Reflection::Metadata::sEvents>;
template class RBX::FactoryProduct<
	RBX::Reflection::Metadata::Member,
	RBX::Reflection::Metadata::Item,
	RBX::Reflection::Metadata::sMember>;
template class RBX::DescribedCreatable<
	RBX::Reflection::Metadata::Member,
	RBX::Reflection::Metadata::Item,
	RBX::Reflection::Metadata::sMember>;
