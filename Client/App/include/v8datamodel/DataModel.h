#ifndef V8DATAMODEL_DATAMODEL_H
#define V8DATAMODEL_DATAMODEL_H

#include "decomp.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace RBX {

class LocalBackpack;
class Workspace;

extern const char sDataModel[];

// SIZE 0x1fc
class DataModel : public DescribedNonCreatable<DataModel, ServiceProvider, sDataModel>
{
private:
	undefined m_unk0x15c[0x190 - 0x15c];             // 0x15c
	boost::shared_ptr<LocalBackpack> localBackpack;  // 0x190
	boost::shared_ptr<Workspace> workspace;          // 0x198
	undefined m_unk0x1a0[0x1f0 - 0x1a0];             // 0x1a0
	boost::shared_ptr<boost::recursive_mutex> mutex; // 0x1f0
	undefined m_unk0x1f8[0x1fc - 0x1f8];             // 0x1f8

public:
	// SIZE 0xc
	class Lock
	{
	public:
		Lock(const DataModel* dataModel);

		// FUNCTION: WEBSERVICE 0x1000cd40
		Lock(boost::shared_ptr<const DataModel> dataModel) : lock(NULL) { doLock(dataModel.get()); }

		~Lock();

	private:
		void doLock(const DataModel* dataModel);

		boost::shared_ptr<boost::recursive_mutex> mutex; // 0x00
		boost::recursive_mutex::scoped_lock* lock;       // 0x08
	};

	static boost::shared_ptr<DataModel> createDataModel();
	static void closeDataModel(boost::shared_ptr<DataModel> dataModel, bool joinRunThread);

	Workspace* getWorkspace() const { return workspace.get(); }
	float step(float distributedGameTime);
};

DECOMP_SIZE_ASSERT(DataModel::Lock, 0xc)
DECOMP_SIZE_ASSERT(DataModel, 0x1fc)

} // namespace RBX

#endif // V8DATAMODEL_DATAMODEL_H
