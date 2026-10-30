#ifndef ERRORUPLOADER_H
#define ERRORUPLOADER_H

#include "decomp.h"
#include "util/boost.hpp"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <queue>
#include <string>

// SIZE 0x1c
class ErrorUploader
{
	// SIZE 0x3c
	struct data
	{
		std::queue<std::string> files; // 0x00
		boost::recursive_mutex sync;   // 0x14
		std::string url;               // 0x20

		// SYNTHETIC: WEBSERVICE 0x1000cf10
		// ErrorUploader::data::data
		// SYNTHETIC: WEBSERVICE 0x1001ddc0
		// ErrorUploader::data::~data
	};

	boost::shared_ptr<data> _data; // 0x00
	RBX::worker_thread thread;     // 0x08

public:
	// FUNCTION: WEBSERVICE 0x1000cdf0
	// FUNCTION: ROBLOX 0x004520c0
	ErrorUploader() : _data(new data()), thread(boost::bind(&ErrorUploader::run, _data), "rbx_erroruploader") {}

	// SYNTHETIC: WEBSERVICE 0x1000da60
	// SYNTHETIC: ROBLOX 0x0044e870
	// ErrorUploader::~ErrorUploader

	void Cancel();
	void Upload(std::string url, bool synchronous);
	bool IsUploading();

private:
	static RBX::worker_thread::work_result run(boost::shared_ptr<data> _data);
};

DECOMP_SIZE_ASSERT(ErrorUploader, 0x1c)

#endif // ERRORUPLOADER_H
