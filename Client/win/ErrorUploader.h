#ifndef ERRORUPLOADER_H
#define ERRORUPLOADER_H

#include "placeholder.h"
#include "util/boost.hpp"

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <queue>
#include <string>

class ErrorUploader
{
	struct data
	{
		std::queue<std::string> files;
		boost::recursive_mutex sync;
		std::string url;
	};

	boost::shared_ptr<data> _data;
	RBX::worker_thread thread;

public:
	ErrorUploader() : _data(new data()), thread(boost::bind(&ErrorUploader::run, _data), "rbx_erroruploader") {}

	void Cancel();
	void Upload(std::string url, bool synchronous);
	bool IsUploading() { return !_data->files.empty(); }

private:
	static RBX::worker_thread::work_result run(boost::shared_ptr<data> _data);
};

#endif // ERRORUPLOADER_H
