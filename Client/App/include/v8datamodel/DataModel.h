#ifndef V8DATAMODEL_DATAMODEL_H
#define V8DATAMODEL_DATAMODEL_H

#include "decomp.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

class LocalBackpack;
class Workspace;

extern const char sDataModel[];

// SIZE 0x1fc
class DataModel : public DescribedNonCreatable<DataModel, ServiceProvider, sDataModel>
{
private:
	undefined m_unk0x15c[0x190 - 0x15c];            // 0x15c
	boost::shared_ptr<LocalBackpack> localBackpack; // 0x190
	boost::shared_ptr<Workspace> workspace;         // 0x198
	undefined m_unk0x1a0[0x1fc - 0x1a0];            // 0x1a0

public:
	// SIZE 0xc
	class Lock
	{
	public:
		Lock(boost::shared_ptr<const DataModel> dataModel);
		~Lock();

	private:
		undefined m_unk0x00[0xc]; // 0x00
	};

	Workspace* getWorkspace() const { return workspace.get(); }
	float step(float distributedGameTime);
};

DECOMP_SIZE_ASSERT(DataModel::Lock, 0xc)
DECOMP_SIZE_ASSERT(DataModel, 0x1fc)

} // namespace RBX

#endif // V8DATAMODEL_DATAMODEL_H
