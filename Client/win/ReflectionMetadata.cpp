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
