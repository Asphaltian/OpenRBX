#include "ErrorUploader.h"

#include "util/Http.h"
#include "util/StandardOut.h"

#include <fstream>
#include <sstream>

// STUB: WEBSERVICE 0x10001100
void ErrorUploader::Upload(std::string url, bool synchronous)
{
	STUB(0x10001100);
}

// STUB: WEBSERVICE 0x10001350
RBX::worker_thread::work_result ErrorUploader::run(boost::shared_ptr<data> _data)
{
	std::string file;
	{
		boost::recursive_mutex::scoped_lock lock(_data->sync);
		if (_data->files.empty()) {
			return RBX::worker_thread::done;
		}
		file = _data->files.front();
	}

	try {
		std::string url = _data->url;
		url += "?filename=";
		url += file;

		std::fstream data(file.c_str(), std::ios_base::in | std::ios_base::binary);
		unsigned int begin = data.tellg();
		data.seekg(0, std::ios_base::end);
		unsigned int end = data.tellg();
		if (end > begin) {
			data.seekg(0, std::ios_base::beg);
			std::string response;
			RBX::Http(url).post(data, true, response);
		}
		else {
			std::stringstream data(std::string("Empty!!!"));
			std::string response;
			RBX::Http(url).post(data, true, response);
		}
	}
	catch (std::exception& e) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, e);
	}

	{
		boost::recursive_mutex::scoped_lock lock(_data->sync);
		_data->files.pop();
	}

	return RBX::worker_thread::more;
}
