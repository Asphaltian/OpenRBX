#ifndef V8DATAMODEL_VISIT_H
#define V8DATAMODEL_VISIT_H

#include "decomp.h"
#include "util/Handle.h"
#include "util/boost.hpp"
#include "v8tree/Instance.h"
#include "v8tree/Service.h"

#include <boost/scoped_ptr.hpp>
#include <string>

namespace RBX {

extern char sVisit[];
// SIZE 0x118
class Visit : public DescribedCreatable<Visit, Instance, sVisit>, public Service
{
private:
	std::string uploadUrl;                       // 0x0f8
	boost::scoped_ptr<worker_thread> pingThread; // 0x114

public:
	Visit();
	virtual ~Visit();

	void setPing(std::string url, int interval);
	static worker_thread::work_result ping(std::string url, int interval);

	// FUNCTION: WEBSERVICE 0x100f8b10
	void setUploadUrl(std::string value) { uploadUrl = value; }

	// FUNCTION: WEBSERVICE 0x100f8b60
	std::string getUploadUrl() { return uploadUrl; }

	// FUNCTION: WEBSERVICE 0x10059d20 FOLDED
	virtual XmlElement* write() { return NULL; }

	// SYNTHETIC: WEBSERVICE 0x100f9060
	// RBX::Visit::`scalar deleting destructor'
};

DECOMP_SIZE_ASSERT(Visit, 0x118)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021daa0
// `dynamic initializer for 'UploadUrlFunction''
// SYNTHETIC: WEBSERVICE 0x1021dad0
// `dynamic initializer for 'getUploadUrl''
// SYNTHETIC: WEBSERVICE 0x1021db00
// `dynamic initializer for 'desc_setPing''
// SYNTHETIC: WEBSERVICE 0x10224bf0
// `dynamic atexit destructor for 'desc_setPing''
// SYNTHETIC: WEBSERVICE 0x10224c00
// `dynamic atexit destructor for 'UploadUrlFunction''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_VISIT_H
