#ifndef V8DATAMODEL_DEBRISSERVICE_H
#define V8DATAMODEL_DEBRISSERVICE_H

#include "placeholder.h"
#include "util/Handle.h"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <queue>

namespace RBX {

extern char sDebrisService[];

class TimerService;

class DebrisService : public DescribedCreatable<DebrisService, Instance, sDebrisService>, public Service
{
private:
	std::queue<boost::weak_ptr<Instance> > queue;
	int maxItems;
	TimerService* timer;

public:
	DebrisService();

	void addItem(boost::shared_ptr<Instance> item, double lifetime);
	void setMaxItems(int value);

	int getMaxItems() const { return maxItems; }

protected:
	virtual void onServiceProvider(const ServiceProvider* oldProvider, const ServiceProvider* newProvider);

private:
	void cleanup();
};

} // namespace RBX

#endif // V8DATAMODEL_DEBRISSERVICE_H
