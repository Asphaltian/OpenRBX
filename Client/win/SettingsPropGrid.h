#ifndef SETTINGSPROPGRID_H
#define SETTINGSPROPGRID_H

#include "PropGrid.h"
#include "placeholder.h"

class CSettingsPropGrid : public CSelectionPropGrid
{
public:
	virtual void FUN_00442e20(
		const RBX::Reflection::ClassDescriptor* classDescriptor, const RBX::Reflection::PropertyDescriptor* descriptor
	);
};

#endif // SETTINGSPROPGRID_H
