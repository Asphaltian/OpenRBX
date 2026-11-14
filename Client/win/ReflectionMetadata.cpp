#include "ReflectionMetadata.h"

#include "v8xml/SerializerV2.h"
#include "v8xml/XmlSerializer.h"

#include <fstream>
#include <shlwapi.h>

namespace RBX {
namespace Reflection {
namespace Metadata {

char sReflection[] = "ReflectionMetadata";

char sClasses[] = "ReflectionMetadataClasses";

char sItem[] = "ReflectionMetadataItem";

char sClass[] = "ReflectionMetadataClass";

char sProperties[] = "ReflectionMetadataProperties";

char sFunctions[] = "ReflectionMetadataFunctions";

char sEvents[] = "ReflectionMetadataEvents";

char sMember[] = "ReflectionMetadataMember";

bool Reflection::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const Classes*>(instance) != NULL;
}

bool Classes::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const Class*>(instance) != NULL;
}

bool Class::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const Members*>(instance) != NULL;
}

bool Members::askAddChild(const Instance* instance) const
{
	return dynamic_cast<const Member*>(instance) != NULL;
}

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

Class* Reflection::FUN_00446220(const ClassDescriptor* descriptor, bool recurse)
{
	if (m_unk0xf8) {
		return m_unk0xf8->FUN_00446130(descriptor, recurse);
	}
	return NULL;
}

Member* Reflection::FUN_00446920(const PropertyDescriptor* descriptor)
{
	Class* item = m_unk0xf8 ? m_unk0xf8->FUN_00446130(&descriptor->owner, false) : NULL;
	if (item) {
		Properties* members = item->findFirstChildOfType<Properties>();
		if (members) {
			return static_cast<Member*>(members->findFirstChildByName(descriptor->name.toString()));
		}
	}
	return NULL;
}

Member* Reflection::FUN_00446970(const FunctionDescriptor* descriptor)
{
	Class* item = m_unk0xf8 ? m_unk0xf8->FUN_00446130(&descriptor->owner, false) : NULL;
	if (item) {
		Functions* members = item->findFirstChildOfType<Functions>();
		if (members) {
			return static_cast<Member*>(members->findFirstChildByName(descriptor->name.toString()));
		}
	}
	return NULL;
}

Member* Reflection::FUN_004469c0(const SignalDescriptor* descriptor)
{
	Class* item = m_unk0xf8 ? m_unk0xf8->FUN_00446130(&descriptor->owner, false) : NULL;
	if (item) {
		Events* members = item->findFirstChildOfType<Events>();
		if (members) {
			return static_cast<Member*>(members->findFirstChildByName(descriptor->name.toString()));
		}
	}
	return NULL;
}

void FUN_00447680()
{
	Reflection::classDescriptor();
	Classes::classDescriptor();
	Functions::classDescriptor();
	Member::classDescriptor();
	Properties::classDescriptor();
	Events::classDescriptor();
}

Reflection::Reflection() : m_unk0xf8(NULL)
{
}

void Classes::FUN_00447d00(const ClassDescriptor& descriptor)
{
	Instance* existing = findFirstChildByName(descriptor.name.toString());
	if (existing) {
		static_cast<Item*>(existing)->browsable = false;
		return;
	}

	boost::shared_ptr<Class> metadata = Creatable<Instance>::create<Class>();
	metadata->setName(descriptor.name.toString());
	metadata->browsable = false;
	metadata->setParent(this);
}

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

RBX::Reflection::BoundProp<bool, 1> Item::prop_Browsable("Browsable", "Reflection", &Item::browsable);

RBX::Reflection::BoundProp<bool, 1> Item::prop_Deprecated("Deprecated", "Reflection", &Item::deprecated);

RBX::Reflection::BoundProp<bool, 1> Item::prop_Preliminary("Preliminary", "Reflection", &Item::preliminary);

RBX::Reflection::BoundProp<std::string, 1> Item::prop_summary("summary", "Reflection", &Item::summary);

RBX::Reflection::BoundProp<int, 1> Class::prop_ExplorerOrder("ExplorerOrder", "Reflection", &Class::explorerOrder);

RBX::Reflection::BoundProp<int, 1> Class::prop_ExplorerImageIndex(
	"ExplorerImageIndex", "Reflection", &Class::explorerImageIndex
);

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
