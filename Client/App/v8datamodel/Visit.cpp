#include <time.h>
#undef TIME_UTC

#include "reflection/reflection.h"
#include "util/Http.h"
#include "util/StandardOut.h"
#include "v8datamodel/Visit.h"

#include <G3D/format.h>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>

namespace RBX {

char sVisit[] = "Visit";

} // namespace RBX

static RBX::Reflection::BoundFuncDesc<RBX::Visit, void(std::string), 1> UploadUrlFunction(
	&RBX::Visit::setUploadUrl,
	"SetUploadUrl",
	"url",
	RBX::Reflection::FunctionDescriptor::NeedTrustedCaller
);
static RBX::Reflection::BoundFuncDesc<RBX::Visit, std::string(), 0> getUploadUrl(
	&RBX::Visit::getUploadUrl,
	"GetUploadUrl",
	RBX::Reflection::FunctionDescriptor::NeedTrustedCaller
);
static RBX::Reflection::BoundFuncDesc<RBX::Visit, void(std::string, int), 2> desc_setPing(
	&RBX::Visit::setPing,
	"SetPing",
	"pingUrl",
	"interval",
	RBX::Reflection::FunctionDescriptor::NeedTrustedCaller
);

namespace RBX {

// FUNCTION: WEBSERVICE 0x100f8d60
worker_thread::work_result Visit::ping(std::string url, int interval)
{
	try {
		std::string response;
		Http(url).get(response);
	}
	catch (std::exception& exp) {
		std::string message = G3D::format("ping %s failed: %s", url.c_str(), exp.what());
		StandardOut::singleton()->print(MESSAGE_WARNING, message.c_str());
	}

	boost::xtime xt;
	boost::xtime_get(&xt, boost::TIME_UTC);
	xt.sec += interval;
	boost::thread::sleep(xt);

	return worker_thread::more;
}

// FUNCTION: WEBSERVICE 0x100f8f20
Visit::~Visit()
{
}

// FUNCTION: WEBSERVICE 0x100f9a70
Visit::Visit() : uploadUrl("NoVisit")
{
}

// FUNCTION: WEBSERVICE 0x100f9c80
void Visit::setPing(std::string url, int interval)
{
	if (url != "") {
		pingThread.reset(new worker_thread(boost::bind(&Visit::ping, url, interval), "rbx_visit"));
	}
	else {
		pingThread.reset();
	}
}

} // namespace RBX

template class RBX::FactoryProduct<RBX::Visit, RBX::Instance, RBX::sVisit>;
template class RBX::Reflection::
	Described<RBX::Visit, RBX::sVisit, RBX::FactoryProduct<RBX::Visit, RBX::Instance, RBX::sVisit> >;
template class RBX::DescribedCreatable<RBX::Visit, RBX::Instance, RBX::sVisit>;
