#ifndef SETTINGSPROPGRID_H
#define SETTINGSPROPGRID_H

#include "PropGrid.h"
#include "decomp.h"

// SIZE 0x1e8
class CSettingsPropGrid : public CSelectionPropGrid
{
public:
	virtual void FUN_00442e20(
		const RBX::Reflection::ClassDescriptor* classDescriptor,
		const RBX::Reflection::PropertyDescriptor* descriptor
	); // vtable+0x160

	// SYNTHETIC: ROBLOX 0x00434950
	// CSelectionPropGrid::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(CSettingsPropGrid, 0x1e8)

#endif // SETTINGSPROPGRID_H
