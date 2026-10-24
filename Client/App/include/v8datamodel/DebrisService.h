#ifndef V8DATAMODEL_DEBRISSERVICE_H
#define V8DATAMODEL_DEBRISSERVICE_H

#include "decomp.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <queue>

namespace RBX {

extern char sDebrisService[];

class TimerService;

// SIZE 0x114
class DebrisService : public DescribedCreatable<DebrisService, Instance, sDebrisService>, public Service
{
private:
	std::queue<boost::weak_ptr<Instance> > queue; // 0x0f8
	int maxItems;                                 // 0x10c
	TimerService* timer;                          // 0x110

public:
	DebrisService();

	// SYNTHETIC: WEBSERVICE 0x100faf40
	// RBX::DebrisService::~DebrisService

	void addItem(boost::shared_ptr<Instance> item, double lifetime);
	void setMaxItems(int value);

	// FUNCTION: WEBSERVICE 0x100f9de0 FOLDED
	int getMaxItems() const { return maxItems; }

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

private:
	void cleanup();
};

DECOMP_SIZE_ASSERT(DebrisService, 0x114)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021db30
// RBX::`dynamic initializer for 'prop_MaxItems''
// SYNTHETIC: WEBSERVICE 0x1021db70
// RBX::`dynamic initializer for 'func_AddItem''
// SYNTHETIC: WEBSERVICE 0x10224c10
// RBX::`dynamic atexit destructor for 'prop_MaxItems''
// SYNTHETIC: WEBSERVICE 0x10224c30
// RBX::`dynamic atexit destructor for 'func_AddItem''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_DEBRISSERVICE_H
