#include "ReflectionMetadata.h"

#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <fstream>
#include <shlwapi.h>

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

// FUNCTION: ROBLOX 0x00446130
Class* Classes::FUN_00446130(const ClassDescriptor* descriptor, bool recurse)
{
	Class* result = static_cast<Class*>(findFirstChildByName(descriptor->name.toString()));
	if (result) {
		return result;
	}
	if (recurse && descriptor->getBase()) {
		return FUN_00446130(descriptor->getBase(), true);
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x00446220
Class* Reflection::FUN_00446220(const ClassDescriptor* descriptor, bool recurse)
{
	if (m_unk0xf8) {
		return m_unk0xf8->FUN_00446130(descriptor, recurse);
	}
	return NULL;
}

// FUNCTION: ROBLOX 0x004479b0
Reflection::Reflection() : m_unk0xf8(NULL)
{
}

// FUNCTION: ROBLOX 0x00447d00
void Classes::FUN_00447d00(const ClassDescriptor& descriptor)
{
	Instance* existing = findFirstChildByName(descriptor.name.toString());
	if (existing) {
		static_cast<Item*>(existing)->m_unk0xf8 = false;
		return;
	}

	boost::shared_ptr<Class> metadata = Creatable<Instance>::create<Class>();
	metadata->setName(descriptor.name.toString());
	metadata->m_unk0xf8 = false;
	metadata->setParent(this);
}

// FUNCTION: ROBLOX 0x00447e50
void Reflection::FUN_00447e50(std::string fileName)
{
	if (fileName.size() == 0) {
		return;
	}

	std::ifstream stream(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
	TextXmlParser parser(stream.rdbuf());
	std::auto_ptr<XmlElement> root(parser.parse());

	MergeBinder binder;
	readChildren(root.get(), binder);
	binder.resolveRefs();

	m_unk0xf8 = findFirstChildOfType<Classes>();
	if (m_unk0xf8) {
		m_unk0xf8->FUN_00447d00(ClassDescriptor::rootDescriptor());
		m_unk0xf8->FUN_00447d00(Instance::classDescriptor());
		m_unk0xf8->FUN_00447d00(Reflection::classDescriptor());
		m_unk0xf8->FUN_00447d00(Classes::classDescriptor());
		m_unk0xf8->FUN_00447d00(Item::classDescriptor());
		m_unk0xf8->FUN_00447d00(Class::classDescriptor());
		m_unk0xf8->FUN_00447d00(Member::classDescriptor());
		m_unk0xf8->FUN_00447d00(Properties::classDescriptor());
		m_unk0xf8->FUN_00447d00(Functions::classDescriptor());
		m_unk0xf8->FUN_00447d00(Events::classDescriptor());
	}
}

// FUNCTION: ROBLOX 0x004480c0
boost::shared_ptr<Reflection> Reflection::FUN_004480c0()
{
	static boost::shared_ptr<Reflection> g_unk0x00953a24;
	if (!g_unk0x00953a24) {
		g_unk0x00953a24 = Creatable<Instance>::create<Reflection>();

		CString fileName;
		AfxGetModuleShortFileName(AfxGetApp()->m_hInstance, fileName);
		CString directory((LPCTSTR) fileName);
		PathRemoveFileSpec(directory.GetBuffer(directory.GetLength()));
		directory.ReleaseBuffer();
		g_unk0x00953a24->FUN_00447e50((LPCTSTR) (directory + "\\ReflectionMetadata.xml"));
	}
	return g_unk0x00953a24;
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
