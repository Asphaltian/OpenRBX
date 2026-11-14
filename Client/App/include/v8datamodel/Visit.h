#ifndef V8DATAMODEL_VISIT_H
#define V8DATAMODEL_VISIT_H

#include "placeholder.h"
#include "util/Handle.h"
#include "util/boost.hpp"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/scoped_ptr.hpp>
#include <string>

namespace RBX {

extern char sVisit[];
class Visit : public DescribedCreatable<Visit, Instance, sVisit>, public Service
{
private:
	std::string uploadUrl;
	boost::scoped_ptr<worker_thread> pingThread;

public:
	Visit();
	virtual ~Visit();

	void setPing(std::string url, int interval);
	static worker_thread::work_result ping(std::string url, int interval);

	void setUploadUrl(std::string value) { uploadUrl = value; }

	std::string getUploadUrl() { return uploadUrl; }

	virtual XmlElement* write() { return NULL; }
};

} // namespace RBX

#endif // V8DATAMODEL_VISIT_H
