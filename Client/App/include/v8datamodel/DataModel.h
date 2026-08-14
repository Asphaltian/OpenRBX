#ifndef V8DATAMODEL_DATAMODEL_H
#define V8DATAMODEL_DATAMODEL_H

#include "decomp.h"

#include <boost/shared_ptr.hpp>

namespace RBX {

extern const char sDataModel[];

class DataModel
{
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

	float step(float distributedGameTime);
};

DECOMP_SIZE_ASSERT(DataModel::Lock, 0xc)

} // namespace RBX

#endif // V8DATAMODEL_DATAMODEL_H
