#ifndef REFLECTIONMETADATA_H
#define REFLECTIONMETADATA_H

#include "decomp.h"
#include "reflection/property.h"
#include "v8tree/Instance.h"

#include <algorithm>
#include <string>

namespace RBX {
namespace Reflection {
namespace Metadata {

extern char sReflection[];
extern char sClasses[];
extern char sItem[];
extern char sClass[];
extern char sProperties[];
extern char sFunctions[];
extern char sEvents[];
extern char sMember[];

class Class;

// SIZE 0xf8
class Members : public Instance
{
public:
	// FUNCTION: ROBLOX 0x00427a60
	Members() {}
};

DECOMP_SIZE_ASSERT(Members, 0xf8)

// SIZE 0xf8
class Classes : public DescribedCreatable<Classes, Instance, sClasses>
{
public:
	// FUNCTION: ROBLOX 0x00429280
	Classes() {}

	Class* FUN_00446130(const ClassDescriptor* descriptor, bool recurse);
};

DECOMP_SIZE_ASSERT(Classes, 0xf8)

// SIZE 0xf8
class Properties : public DescribedCreatable<Properties, Members, sProperties>
{
public:
	// FUNCTION: ROBLOX 0x00429330
	Properties() {}
};

DECOMP_SIZE_ASSERT(Properties, 0xf8)

// SIZE 0xf8
class Functions : public DescribedCreatable<Functions, Members, sFunctions>
{
public:
	// FUNCTION: ROBLOX 0x004293a0
	Functions() {}
};

DECOMP_SIZE_ASSERT(Functions, 0xf8)

// SIZE 0xf8
class Events : public DescribedCreatable<Events, Members, sEvents>
{
public:
	// FUNCTION: ROBLOX 0x00429450
	Events() {}
};

DECOMP_SIZE_ASSERT(Events, 0xf8)

// SIZE 0x118
class Item : public DescribedNonCreatable<Item, Instance, sItem>
{
public:
	// FUNCTION: ROBLOX 0x00429500
	Item() : m_unk0xf8(true), m_unk0xf9(false), m_unk0xfa(false) {}

	static RBX::Reflection::BoundProp<bool, 1> g_unk0x00953a34;
	static RBX::Reflection::BoundProp<bool, 1> g_unk0x00953a50;
	static RBX::Reflection::BoundProp<bool, 1> g_unk0x00953a6c;
	static RBX::Reflection::BoundProp<std::string, 1> g_unk0x00953a88;

private:
	bool m_unk0xf8;        // 0xf8
	bool m_unk0xf9;        // 0xf9
	bool m_unk0xfa;        // 0xfa
	std::string m_unk0xfc; // 0xfc
};

DECOMP_SIZE_ASSERT(Item, 0x118)

// SIZE 0x120
class Class : public DescribedCreatable<Class, Item, sClass>
{
public:
	// FUNCTION: ROBLOX 0x00429e20
	Class() : m_unk0x118(-1), m_unk0x11c(0) {}

	// FUNCTION: ROBLOX 0x00422100
	int FUN_00422100() const
	{
		if (m_unk0x118) {
			return m_unk0x118;
		}
		return dynamic_cast<Class*>(getParent()) && dynamic_cast<Class*>(getParent())->FUN_00422100();
	}

	// FUNCTION: ROBLOX 0x004251c0
	int FUN_004251c0() const
	{
		if (m_unk0x11c == -1) {
			return dynamic_cast<Class*>(getParent()) && dynamic_cast<Class*>(getParent())->FUN_004251c0();
		}
		return std::max<int>(m_unk0x11c, 0);
	}

	static RBX::Reflection::BoundProp<int, 1> g_unk0x00953aa4;
	static RBX::Reflection::BoundProp<int, 1> g_unk0x00953ac0;

private:
	int m_unk0x118; // 0x118
	int m_unk0x11c; // 0x11c
};

DECOMP_SIZE_ASSERT(Class, 0x120)

// SIZE 0x118
class Member : public DescribedCreatable<Member, Item, sMember>
{
public:
	// FUNCTION: ROBLOX 0x00429f90
	Member() {}
};

DECOMP_SIZE_ASSERT(Member, 0x118)

// SIZE 0xfc
class Reflection : public DescribedCreatable<Reflection, Instance, sReflection>
{
public:
	Reflection();

	Class* FUN_00446220(const ClassDescriptor* descriptor, bool recurse);

	static boost::shared_ptr<Reflection> FUN_004480c0();

private:
	Classes* m_unk0xf8; // 0xf8
};

DECOMP_SIZE_ASSERT(Reflection, 0xfc)

} // namespace Metadata
} // namespace Reflection
} // namespace RBX

// clang-format off
// TEMPLATE: ROBLOX 0x00422700
// ??$doDeclare@$1?sReflection@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAABV01@XZ
// TEMPLATE: ROBLOX 0x00422780
// ??$doDeclare@$1?sClasses@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAABV01@XZ
// TEMPLATE: ROBLOX 0x00422800
// ??$doDeclare@$1?sClass@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAABV01@XZ
// TEMPLATE: ROBLOX 0x00422880
// ??$doDeclare@$1?sProperties@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAABV01@XZ
// TEMPLATE: ROBLOX 0x00422900
// ??$doDeclare@$1?sFunctions@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAABV01@XZ
// TEMPLATE: ROBLOX 0x00422980
// ??$doDeclare@$1?sEvents@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAABV01@XZ
// TEMPLATE: ROBLOX 0x00422a00
// ??$doDeclare@$1?sMember@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAABV01@XZ
// TEMPLATE: ROBLOX 0x00423050
// ??$callDoDeclare@$1?sReflection@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAXXZ
// TEMPLATE: ROBLOX 0x00423060
// ??$callDoDeclare@$1?sClasses@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAXXZ
// TEMPLATE: ROBLOX 0x00423070
// ??$callDoDeclare@$1?sClass@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAXXZ
// TEMPLATE: ROBLOX 0x00423080
// ??$callDoDeclare@$1?sProperties@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAXXZ
// TEMPLATE: ROBLOX 0x00423090
// ??$callDoDeclare@$1?sFunctions@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAXXZ
// TEMPLATE: ROBLOX 0x004230a0
// ??$callDoDeclare@$1?sEvents@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAXXZ
// TEMPLATE: ROBLOX 0x004230b0
// ??$callDoDeclare@$1?sMember@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAXXZ
// TEMPLATE: ROBLOX 0x004273c0
// ??$create@VReflection@Metadata@1RBX@@@?$Creatable@VInstance@RBX@@@RBX@@SA?AV?$shared_ptr@VReflection@Metadata@1RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00427450
// ?create@Creator@?$FactoryProduct@VReflection@Metadata@1RBX@@VInstance@3@$1?sReflection@213@3PADA@RBX@@UBE?AV?$shared_ptr@VObject@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00427560
// ??0?$FactoryProduct@VClasses@Metadata@Reflection@RBX@@VInstance@4@$1?sClasses@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00427720
// ??$declare@$1?sClasses@Metadata@Reflection@RBX@@3PADA@Name@RBX@@SAABV01@XZ
// TEMPLATE: ROBLOX 0x00427740
// ??0?$NonFactoryProduct@VInstance@RBX@@$1?sItem@Metadata@Reflection@2@3PADA@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x004277a0
// ??$doDeclare@$1?sItem@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAABV01@XZ
// TEMPLATE: ROBLOX 0x00427820
// ??$callDoDeclare@$1?sItem@Metadata@Reflection@RBX@@3PADA@Name@RBX@@CAXXZ
// SYNTHETIC: ROBLOX 0x004278c0
// ??1Item@Metadata@Reflection@RBX@@UAE@XZ
// TEMPLATE: ROBLOX 0x00427a20
// ?className@?$NonFactoryProduct@VInstance@RBX@@$1?sItem@Metadata@Reflection@2@3PADA@RBX@@SAABVName@2@XZ
// TEMPLATE: ROBLOX 0x00427a40
// ?getClassName@?$FactoryProduct@VClass@Metadata@Reflection@RBX@@VItem@234@$1?sClass@234@3PADA@RBX@@UBEABVName@2@XZ
// TEMPLATE: ROBLOX 0x00427ac0
// ??1?$FactoryProduct@VProperties@Metadata@Reflection@RBX@@VMembers@234@$1?sProperties@234@3PADA@RBX@@MAE@XZ
// SYNTHETIC: ROBLOX 0x00427b40
// ??_G?$FactoryProduct@VProperties@Metadata@Reflection@RBX@@VMembers@234@$1?sProperties@234@3PADA@RBX@@MAEPAXI@Z
// TEMPLATE: ROBLOX 0x00427b60
// ??0?$FactoryProduct@VProperties@Metadata@Reflection@RBX@@VMembers@234@$1?sProperties@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00427d00
// ??$declare@$1?sProperties@Metadata@Reflection@RBX@@3PADA@Name@RBX@@SAABV01@XZ
// TEMPLATE: ROBLOX 0x00427dc0
// ??0?$FactoryProduct@VFunctions@Metadata@Reflection@RBX@@VMembers@234@$1?sFunctions@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00427f60
// ?className@?$FactoryProduct@VFunctions@Metadata@Reflection@RBX@@VMembers@234@$1?sFunctions@234@3PADA@RBX@@SAABVName@2@XZ
// TEMPLATE: ROBLOX 0x00427f80
// ??1?$FactoryProduct@VEvents@Metadata@Reflection@RBX@@VMembers@234@$1?sEvents@234@3PADA@RBX@@MAE@XZ
// TEMPLATE: ROBLOX 0x00427ff0
// ??0?$FactoryProduct@VEvents@Metadata@Reflection@RBX@@VMembers@234@$1?sEvents@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00428190
// ??$declare@$1?sEvents@Metadata@Reflection@RBX@@3PADA@Name@RBX@@SAABV01@XZ
// TEMPLATE: ROBLOX 0x00428240
// ??1?$FactoryProduct@VMember@Metadata@Reflection@RBX@@VItem@234@$1?sMember@234@3PADA@RBX@@MAE@XZ
// TEMPLATE: ROBLOX 0x00428360
// ??$declare@$1?sMember@Metadata@Reflection@RBX@@3PADA@Name@RBX@@SAABV01@XZ
// TEMPLATE: ROBLOX 0x00428380
// ?classDescriptor@?$Described@VClasses@Metadata@Reflection@RBX@@$1?sClasses@234@3PADAV?$FactoryProduct@VClasses@Metadata@Reflection@RBX@@VInstance@4@$1?sClasses@234@3PADA@4@@Reflection@RBX@@SAAAVClassDescriptor@23@XZ
// TEMPLATE: ROBLOX 0x00428400
// ?classDescriptor@?$Described@VProperties@Metadata@Reflection@RBX@@$1?sProperties@234@3PADAV?$FactoryProduct@VProperties@Metadata@Reflection@RBX@@VMembers@234@$1?sProperties@234@3PADA@4@@Reflection@RBX@@SAAAVClassDescriptor@23@XZ
// TEMPLATE: ROBLOX 0x00428480
// ?classDescriptor@?$Described@VFunctions@Metadata@Reflection@RBX@@$1?sFunctions@234@3PADAV?$FactoryProduct@VFunctions@Metadata@Reflection@RBX@@VMembers@234@$1?sFunctions@234@3PADA@4@@Reflection@RBX@@SAAAVClassDescriptor@23@XZ
// TEMPLATE: ROBLOX 0x00428500
// ?classDescriptor@?$Described@VEvents@Metadata@Reflection@RBX@@$1?sEvents@234@3PADAV?$FactoryProduct@VEvents@Metadata@Reflection@RBX@@VMembers@234@$1?sEvents@234@3PADA@4@@Reflection@RBX@@SAAAVClassDescriptor@23@XZ
// TEMPLATE: ROBLOX 0x00428880
// ?classDescriptor@?$Described@VItem@Metadata@Reflection@RBX@@$1?sItem@234@3PADAV?$NonFactoryProduct@VInstance@RBX@@$1?sItem@Metadata@Reflection@2@3PADA@4@@Reflection@RBX@@SAAAVClassDescriptor@23@XZ
// TEMPLATE: ROBLOX 0x00428900
// ?classDescriptor@?$Described@VClass@Metadata@Reflection@RBX@@$1?sClass@234@3PADAV?$FactoryProduct@VClass@Metadata@Reflection@RBX@@VItem@234@$1?sClass@234@3PADA@4@@Reflection@RBX@@SAAAVClassDescriptor@23@XZ
// TEMPLATE: ROBLOX 0x00428980
// ??0?$Described@VClasses@Metadata@Reflection@RBX@@$1?sClasses@234@3PADAV?$FactoryProduct@VClasses@Metadata@Reflection@RBX@@VInstance@4@$1?sClasses@234@3PADA@4@@Reflection@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x004289d0
// ??0?$Described@VProperties@Metadata@Reflection@RBX@@$1?sProperties@234@3PADAV?$FactoryProduct@VProperties@Metadata@Reflection@RBX@@VMembers@234@$1?sProperties@234@3PADA@4@@Reflection@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00428a20
// ??0?$Described@VFunctions@Metadata@Reflection@RBX@@$1?sFunctions@234@3PADAV?$FactoryProduct@VFunctions@Metadata@Reflection@RBX@@VMembers@234@$1?sFunctions@234@3PADA@4@@Reflection@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00428a70
// ??0?$Described@VEvents@Metadata@Reflection@RBX@@$1?sEvents@234@3PADAV?$FactoryProduct@VEvents@Metadata@Reflection@RBX@@VMembers@234@$1?sEvents@234@3PADA@4@@Reflection@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00428ac0
// ?classDescriptor@?$Described@VMember@Metadata@Reflection@RBX@@$1?sMember@234@3PADAV?$FactoryProduct@VMember@Metadata@Reflection@RBX@@VItem@234@$1?sMember@234@3PADA@4@@Reflection@RBX@@SAAAVClassDescriptor@23@XZ
// TEMPLATE: ROBLOX 0x00428da0
// ??0?$Described@VItem@Metadata@Reflection@RBX@@$1?sItem@234@3PADAV?$NonFactoryProduct@VInstance@RBX@@$1?sItem@Metadata@Reflection@2@3PADA@4@@Reflection@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00428df0
// ??0?$DescribedCreatable@VClasses@Metadata@Reflection@RBX@@VInstance@4@$1?sClasses@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00428e80
// ??0?$DescribedCreatable@VProperties@Metadata@Reflection@RBX@@VMembers@234@$1?sProperties@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00428f20
// ??0?$DescribedCreatable@VFunctions@Metadata@Reflection@RBX@@VMembers@234@$1?sFunctions@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00428f80
// ??0?$DescribedCreatable@VEvents@Metadata@Reflection@RBX@@VMembers@234@$1?sEvents@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00429220
// ??0?$DescribedNonCreatable@VItem@Metadata@Reflection@RBX@@VInstance@4@$1?sItem@234@3PADA@RBX@@IAE@XZ
// STUB: ROBLOX 0x00429310
// ??_E?$DescribedCreatable@VReflection@Metadata@1RBX@@VInstance@3@$1?sReflection@213@3PADA@RBX@@UAEPAXI@Z
// STUB: ROBLOX 0x00429430
// ??_E?$FactoryProduct@VClass@Metadata@Reflection@RBX@@VItem@234@$1?sClass@234@3PADA@RBX@@MAEPAXI@Z
// SYNTHETIC: ROBLOX 0x004294e0
// ??_E?$DescribedCreatable@VEvents@Metadata@Reflection@RBX@@VMembers@234@$1?sEvents@234@3PADA@RBX@@UAEPAXI@Z
// SYNTHETIC: ROBLOX 0x00429630
// ??_EItem@Metadata@Reflection@RBX@@UAEPAXI@Z
// TEMPLATE: ROBLOX 0x00429650
// ??0?$FactoryProduct@VClass@Metadata@Reflection@RBX@@VItem@234@$1?sClass@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x004296b0
// ??$create@VClasses@Metadata@Reflection@RBX@@@?$Creatable@VInstance@RBX@@@RBX@@SA?AV?$shared_ptr@VClasses@Metadata@Reflection@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00429740
// ??$create@VProperties@Metadata@Reflection@RBX@@@?$Creatable@VInstance@RBX@@@RBX@@SA?AV?$shared_ptr@VProperties@Metadata@Reflection@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x004297d0
// ??$create@VFunctions@Metadata@Reflection@RBX@@@?$Creatable@VInstance@RBX@@@RBX@@SA?AV?$shared_ptr@VFunctions@Metadata@Reflection@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00429860
// ??$create@VEvents@Metadata@Reflection@RBX@@@?$Creatable@VInstance@RBX@@@RBX@@SA?AV?$shared_ptr@VEvents@Metadata@Reflection@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x004298f0
// ??0?$FactoryProduct@VMember@Metadata@Reflection@RBX@@VItem@234@$1?sMember@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00429960
// ?create@Creator@?$FactoryProduct@VClasses@Metadata@Reflection@RBX@@VInstance@4@$1?sClasses@234@3PADA@RBX@@UBE?AV?$shared_ptr@VObject@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00429a20
// ?create@Creator@?$FactoryProduct@VProperties@Metadata@Reflection@RBX@@VMembers@234@$1?sProperties@234@3PADA@RBX@@UBE?AV?$shared_ptr@VObject@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00429ae0
// ?create@Creator@?$FactoryProduct@VFunctions@Metadata@Reflection@RBX@@VMembers@234@$1?sFunctions@234@3PADA@RBX@@UBE?AV?$shared_ptr@VObject@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00429ba0
// ?create@Creator@?$FactoryProduct@VEvents@Metadata@Reflection@RBX@@VMembers@234@$1?sEvents@234@3PADA@RBX@@UBE?AV?$shared_ptr@VObject@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00429c60
// ??0?$Described@VClass@Metadata@Reflection@RBX@@$1?sClass@234@3PADAV?$FactoryProduct@VClass@Metadata@Reflection@RBX@@VItem@234@$1?sClass@234@3PADA@4@@Reflection@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00429cb0
// ??0?$Described@VMember@Metadata@Reflection@RBX@@$1?sMember@234@3PADAV?$FactoryProduct@VMember@Metadata@Reflection@RBX@@VItem@234@$1?sMember@234@3PADA@4@@Reflection@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x00429d00
// ??0?$DescribedCreatable@VClass@Metadata@Reflection@RBX@@VItem@234@$1?sClass@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00429d90
// ??0?$DescribedCreatable@VMember@Metadata@Reflection@RBX@@VItem@234@$1?sMember@234@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00429f00
// ??$create@VClass@Metadata@Reflection@RBX@@@?$Creatable@VInstance@RBX@@@RBX@@SA?AV?$shared_ptr@VClass@Metadata@Reflection@RBX@@@boost@@XZ
// SYNTHETIC: ROBLOX 0x0042a020
// ??_G?$DescribedCreatable@VMember@Metadata@Reflection@RBX@@VItem@234@$1?sMember@234@3PADA@RBX@@UAEPAXI@Z
// TEMPLATE: ROBLOX 0x0042a040
// ?create@Creator@?$FactoryProduct@VClass@Metadata@Reflection@RBX@@VItem@234@$1?sClass@234@3PADA@RBX@@UBE?AV?$shared_ptr@VObject@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x0042a100
// ??$create@VMember@Metadata@Reflection@RBX@@@?$Creatable@VInstance@RBX@@@RBX@@SA?AV?$shared_ptr@VMember@Metadata@Reflection@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x0042a190
// ?create@Creator@?$FactoryProduct@VMember@Metadata@Reflection@RBX@@VItem@234@$1?sMember@234@3PADA@RBX@@UBE?AV?$shared_ptr@VObject@RBX@@@boost@@XZ
// TEMPLATE: ROBLOX 0x00446080
// ??0?$FactoryProduct@VReflection@Metadata@1RBX@@VInstance@3@$1?sReflection@213@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x004461e0
// ?getValue@?$BoundPropGetSet@VItem@Metadata@Reflection@RBX@@@?$BoundProp@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$00@Reflection@RBX@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PBVDescribedBase@34@@Z
// TEMPLATE: ROBLOX 0x00446a10
// ?getClassName@Creator@?$FactoryProduct@VReflection@Metadata@1RBX@@VInstance@3@$1?sReflection@213@3PADA@RBX@@QBEABVName@3@XZ
// TEMPLATE: ROBLOX 0x00447280
// ?classDescriptor@?$Described@VReflection@Metadata@1RBX@@$1?sReflection@213@3PADAV?$FactoryProduct@VReflection@Metadata@1RBX@@VInstance@3@$1?sReflection@213@3PADA@3@@Reflection@RBX@@SAAAVClassDescriptor@23@XZ
// TEMPLATE: ROBLOX 0x00447300
// ??0?$Described@VReflection@Metadata@1RBX@@$1?sReflection@213@3PADAV?$FactoryProduct@VReflection@Metadata@1RBX@@VInstance@3@$1?sReflection@213@3PADA@3@@Reflection@RBX@@QAE@XZ
// TEMPLATE: ROBLOX 0x004473e0
// ??$?0VItem@Metadata@Reflection@RBX@@@?$BoundProp@_N$00@Reflection@RBX@@QAE@PBD0PQItem@Metadata@12@_NW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x004474c0
// ??$?0VItem@Metadata@Reflection@RBX@@@?$BoundProp@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$00@Reflection@RBX@@QAE@PBD0PQItem@Metadata@12@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x004475a0
// ??$?0VClass@Metadata@Reflection@RBX@@@?$BoundProp@H$00@Reflection@RBX@@QAE@PBD0PQClass@Metadata@12@HW4Functionality@PropertyDescriptor@12@@Z
// TEMPLATE: ROBLOX 0x004476a0
// ??0?$DescribedCreatable@VReflection@Metadata@1RBX@@VInstance@3@$1?sReflection@213@3PADA@RBX@@IAE@XZ
// TEMPLATE: ROBLOX 0x00447df0
// ?setValue@?$BoundPropGetSet@VItem@Metadata@Reflection@RBX@@@?$BoundProp@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$00@Reflection@RBX@@UBEXPAVDescribedBase@34@ABV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z
// SYNTHETIC: ROBLOX 0x007e4860
// `dynamic initializer for 'RBX::Reflection::Metadata::Item::g_unk0x00953a34''
// SYNTHETIC: ROBLOX 0x007e4890
// `dynamic initializer for 'RBX::Reflection::Metadata::Item::g_unk0x00953a50''
// SYNTHETIC: ROBLOX 0x007e48c0
// `dynamic initializer for 'RBX::Reflection::Metadata::Item::g_unk0x00953a6c''
// SYNTHETIC: ROBLOX 0x007e48f0
// `dynamic initializer for 'RBX::Reflection::Metadata::Item::g_unk0x00953a88''
// SYNTHETIC: ROBLOX 0x007e4920
// `dynamic initializer for 'RBX::Reflection::Metadata::Class::g_unk0x00953aa4''
// SYNTHETIC: ROBLOX 0x007e4950
// `dynamic initializer for 'RBX::Reflection::Metadata::Class::g_unk0x00953ac0''
// SYNTHETIC: ROBLOX 0x007ef7b0
// `RBX::Reflection::Described<RBX::Reflection::Metadata::Class,&RBX::Reflection::Metadata::sClass,RBX::FactoryProduct<RBX::Reflection::Metadata::Class,RBX::Reflection::Metadata::Item,&RBX::Reflection::Metadata::sClass> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007ef7c0
// `RBX::Reflection::Described<RBX::Reflection::Metadata::Item,&RBX::Reflection::Metadata::sItem,RBX::NonFactoryProduct<RBX::Instance,&RBX::Reflection::Metadata::sItem> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007ef7d0
// `RBX::Reflection::Described<RBX::Reflection::Metadata::Member,&RBX::Reflection::Metadata::sMember,RBX::FactoryProduct<RBX::Reflection::Metadata::Member,RBX::Reflection::Metadata::Item,&RBX::Reflection::Metadata::sMember> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007ef7e0
// `RBX::Reflection::Described<RBX::Reflection::Metadata::Events,&RBX::Reflection::Metadata::sEvents,RBX::FactoryProduct<RBX::Reflection::Metadata::Events,RBX::Reflection::Metadata::Members,&RBX::Reflection::Metadata::sEvents> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007ef7f0
// `RBX::Reflection::Described<RBX::Reflection::Metadata::Functions,&RBX::Reflection::Metadata::sFunctions,RBX::FactoryProduct<RBX::Reflection::Metadata::Functions,RBX::Reflection::Metadata::Members,&RBX::Reflection::Metadata::sFunctions> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007ef800
// `RBX::Reflection::Described<RBX::Reflection::Metadata::Properties,&RBX::Reflection::Metadata::sProperties,RBX::FactoryProduct<RBX::Reflection::Metadata::Properties,RBX::Reflection::Metadata::Members,&RBX::Reflection::Metadata::sProperties> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007ef810
// `RBX::Reflection::Described<RBX::Reflection::Metadata::Classes,&RBX::Reflection::Metadata::sClasses,RBX::FactoryProduct<RBX::Reflection::Metadata::Classes,RBX::Instance,&RBX::Reflection::Metadata::sClasses> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007efc60
// `RBX::Reflection::Described<RBX::Reflection::Metadata::Reflection,&RBX::Reflection::Metadata::sReflection,RBX::FactoryProduct<RBX::Reflection::Metadata::Reflection,RBX::Instance,&RBX::Reflection::Metadata::sReflection> >::classDescriptor'::`2'::`dynamic atexit destructor for 'foo''
// SYNTHETIC: ROBLOX 0x007efb60
// `dynamic atexit destructor for 'RBX::Reflection::Metadata::Item::g_unk0x00953a6c''
// SYNTHETIC: ROBLOX 0x007efbc0
// `dynamic atexit destructor for 'RBX::Reflection::Metadata::Item::g_unk0x00953a50''
// SYNTHETIC: ROBLOX 0x007efbe0
// `dynamic atexit destructor for 'RBX::Reflection::Metadata::Item::g_unk0x00953a34''
// SYNTHETIC: ROBLOX 0x007efc00
// `dynamic atexit destructor for 'RBX::Reflection::Metadata::Class::g_unk0x00953ac0''
// SYNTHETIC: ROBLOX 0x007efc20
// `dynamic atexit destructor for 'RBX::Reflection::Metadata::Class::g_unk0x00953aa4''
// SYNTHETIC: ROBLOX 0x007efc40
// `dynamic atexit destructor for 'RBX::Reflection::Metadata::Item::g_unk0x00953a88''
// clang-format on

#endif // REFLECTIONMETADATA_H
