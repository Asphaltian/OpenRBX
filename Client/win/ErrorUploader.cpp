#include "ErrorUploader.h"

// STUB: WEBSERVICE 0x10001100
void ErrorUploader::Upload(std::string url, bool synchronous)
{
	STUB(0x10001100);
}

// STUB: WEBSERVICE 0x10001350
RBX::worker_thread::work_result ErrorUploader::run(boost::shared_ptr<data> _data)
{
	STUB(0x10001350);
	return RBX::worker_thread::done;
}
