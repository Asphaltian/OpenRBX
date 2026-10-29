#ifndef WEBSERVICE_WEBSERVICE_H
#define WEBSERVICE_WEBSERVICE_H

// clang-format off
// winsock2.h before anything that pulls in windows.h.
#include <winsock2.h>
// clang-format on

#include "decomp.h"

#include <G3D/System.h>
#include <atlisapi.h>
#include <atlsoap.h>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <map>
#include <stdexcept>
#include <string>
#include <unknwn.h>

namespace RBX {
class DataModel;
namespace Reflection {
class Value;
}
} // namespace RBX

namespace Roblox {

enum LuaType
{
	LUA_TNIL = 0,
	LUA_TBOOLEAN = 1,
	LUA_TNUMBER = 3,
	LUA_TSTRING = 4,
	LUA_TTABLE = 5
};

enum MessageType
{
	MESSAGE_OUTPUT = 0,
	MESSAGE_INFO = 1,
	MESSAGE_WARNING = 2,
	MESSAGE_ERROR = 3
};

struct LuaValue10
{
	LuaType type;   // 0x00
	wchar_t* value; // 0x04
};

struct LuaValue9
{
	LuaType type;      // 0x00
	wchar_t* value;    // 0x04
	int tableLength;   // 0x08
	LuaValue10* table; // 0x0c
};

struct LuaValue8
{
	LuaType type;     // 0x00
	wchar_t* value;   // 0x04
	int tableLength;  // 0x08
	LuaValue9* table; // 0x0c
};

struct LuaValue7
{
	LuaType type;     // 0x00
	wchar_t* value;   // 0x04
	int tableLength;  // 0x08
	LuaValue8* table; // 0x0c
};

struct LuaValue6
{
	LuaType type;     // 0x00
	wchar_t* value;   // 0x04
	int tableLength;  // 0x08
	LuaValue7* table; // 0x0c
};

struct LuaValue5
{
	LuaType type;     // 0x00
	wchar_t* value;   // 0x04
	int tableLength;  // 0x08
	LuaValue6* table; // 0x0c
};

struct LuaValue4
{
	LuaType type;     // 0x00
	wchar_t* value;   // 0x04
	int tableLength;  // 0x08
	LuaValue5* table; // 0x0c
};

struct LuaValue3
{
	LuaType type;     // 0x00
	wchar_t* value;   // 0x04
	int tableLength;  // 0x08
	LuaValue4* table; // 0x0c
};

struct LuaValue2
{
	LuaType type;     // 0x00
	wchar_t* value;   // 0x04
	int tableLength;  // 0x08
	LuaValue3* table; // 0x0c
};

struct LuaValue1
{
	LuaType type;     // 0x00
	wchar_t* value;   // 0x04
	int tableLength;  // 0x08
	LuaValue2* table; // 0x0c
};

struct LuaArguments
{
	int count;        // 0x00
	LuaValue1* items; // 0x04
};

struct ScriptExecution
{
	wchar_t* name;          // 0x00
	wchar_t* script;        // 0x04
	LuaArguments arguments; // 0x08
};

struct Status
{
	wchar_t* version;     // 0x00
	int environmentCount; // 0x04
};

struct Strings
{
	int count;       // 0x00
	wchar_t** items; // 0x04
};

struct StandardOutMessage
{
	MessageType type; // 0x00
	__int64 time;     // 0x08
	wchar_t* text;    // 0x10
};

struct StandardOutMessages
{
	int count;                 // 0x00
	StandardOutMessage* items; // 0x04
};

struct IWebService : public IUnknown
{
	virtual HRESULT __stdcall HelloWorld(BSTR* bstrOutput) = 0; // vtable+0x0c
	virtual HRESULT __stdcall GetVersion(BSTR* result) = 0;     // vtable+0x10
	virtual HRESULT __stdcall GetStatus(Status* result) = 0;    // vtable+0x14
	virtual HRESULT __stdcall Update(BSTR url) = 0;             // vtable+0x18
	virtual HRESULT __stdcall OpenJob(
		BSTR jobID,
		ScriptExecution script,
		double timeout,
		LuaArguments* result
	) = 0;                                                                                           // vtable+0x1c
	virtual HRESULT __stdcall TouchJob(BSTR jobID, double timeout) = 0;                              // vtable+0x20
	virtual HRESULT __stdcall Execute(BSTR jobID, ScriptExecution script, LuaArguments* result) = 0; // vtable+0x24
	virtual HRESULT __stdcall CloseJob(BSTR jobID) = 0;                                              // vtable+0x28
	virtual HRESULT __stdcall BatchJob(
		BSTR jobID,
		ScriptExecution script,
		double timeout,
		LuaArguments* result
	) = 0;                                                                                           // vtable+0x2c
	virtual HRESULT __stdcall GetTimeout(BSTR jobID, double* timeout) = 0;                           // vtable+0x30
	virtual HRESULT __stdcall GetAllJobs(Strings* result) = 0;                                       // vtable+0x34
	virtual HRESULT __stdcall CloseOrphanedJobs(Strings activeJobs, int* result) = 0;                // vtable+0x38
	virtual HRESULT __stdcall CloseTimedoutJobs(int* result) = 0;                                    // vtable+0x3c
	virtual HRESULT __stdcall CloseAllJobs() = 0;                                                    // vtable+0x40
	virtual HRESULT __stdcall GetStandardOutMessages(int maxCount, StandardOutMessages* result) = 0; // vtable+0x44
};

// SIZE 0x180
class CWebService : public IWebService, public ATL::CSoapHandler<CWebService>
{
	// SIZE 0x10
	struct Job
	{
		boost::shared_ptr<RBX::DataModel> dataModel; // 0x00
		double expiration;                           // 0x08
	};

	typedef std::map<std::string, Job> Jobs;

	static Jobs jobs;
	static boost::mutex sync;

	CStringW errorMessage; // 0x17c

	HRESULT ReportException(const std::exception& e);

public:
	CWebService() {}
	virtual ~CWebService() {}

protected:
	virtual HRESULT GenerateAppError(ATL::IWriteStream* pStream, HRESULT hr);

private:
	Jobs::iterator getJob(wchar_t* jobID);
	void touch(Job& job, double timeout) { job.expiration = G3D::System::getLocalTime() + timeout; }
	double secondsToTimeout(const Job& job) { return job.expiration - G3D::System::getLocalTime(); }

	template <class T>
	T* allocate(T*, int count)
	{
		if (count == 0) {
			return NULL;
		}

		T* items = (T*) GetMemMgr()->Allocate(count * sizeof(T));
		memset(items, 0, count * sizeof(T));
		return items;
	}

	template <class T>
	void convertToLua(const RBX::Reflection::Value& source, T& dest);

public:
	static bool hasOpenEnvironments() { return jobs.size() > 0; }

	HRESULT __stdcall HelloWorld(BSTR* bstrOutput);
	HRESULT __stdcall GetVersion(BSTR* result);
	HRESULT __stdcall GetStatus(Status* result);
	HRESULT __stdcall Update(BSTR url);
	HRESULT __stdcall OpenJob(BSTR jobID, ScriptExecution script, double timeout, LuaArguments* result);
	HRESULT __stdcall TouchJob(BSTR jobID, double timeout);
	HRESULT __stdcall Execute(BSTR jobID, ScriptExecution script, LuaArguments* result);
	HRESULT __stdcall CloseJob(BSTR jobID);
	HRESULT __stdcall BatchJob(BSTR jobID, ScriptExecution script, double timeout, LuaArguments* result);
	HRESULT __stdcall GetTimeout(BSTR jobID, double* timeout);
	HRESULT __stdcall CloseOrphanedJobs(Strings activeJobs, int* result);
	HRESULT __stdcall CloseAllJobs();
	HRESULT __stdcall CloseTimedoutJobs(int* result);
	HRESULT __stdcall GetAllJobs(Strings* result);
	HRESULT __stdcall GetStandardOutMessages(int maxCount, StandardOutMessages* result);

	const _soapmap** GetFunctionMap() throw();
	const _soapmap** GetHeaderMap() throw();
	void* GetHeaderValue() throw();
	const wchar_t* GetNamespaceUri() throw();
	const char* GetNamespaceUriA() throw();
	const char* GetServiceName() throw();
	HRESULT CallFunction(void* pvParam, const wchar_t* wszLocalName, int cchLocalName, size_t nItem);
};

__if_not_exists(___Roblox_LuaType_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaType_entries[] = {
		{0x380E5A7D, "LUA_TTABLE", L"LUA_TTABLE", sizeof("LUA_TTABLE") - 1, 5, SOAPFLAG_FIELD, 0, NULL, NULL, -1},
		{0x38E50DCC, "LUA_TSTRING", L"LUA_TSTRING", sizeof("LUA_TSTRING") - 1, 4, SOAPFLAG_FIELD, 0, NULL, NULL, -1},
		{0x2D4A7E5E, "LUA_TNUMBER", L"LUA_TNUMBER", sizeof("LUA_TNUMBER") - 1, 3, SOAPFLAG_FIELD, 0, NULL, NULL, -1},
		{0x2D0A2835, "LUA_TBOOLEAN", L"LUA_TBOOLEAN", sizeof("LUA_TBOOLEAN") - 1, 1, SOAPFLAG_FIELD, 0, NULL, NULL, -1},
		{0x51272CF8, "LUA_TNIL", L"LUA_TNIL", sizeof("LUA_TNIL") - 1, 0, SOAPFLAG_FIELD, 0, NULL, NULL, -1},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaType_map = {
		0xF2410524,
		"LuaType",
		L"LuaType",
		sizeof("LuaType") - 1,
		sizeof("LuaType") - 1,
		SOAPMAP_ENUM,
		___Roblox_LuaType_entries,
		sizeof(::Roblox::LuaType),
		1,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_MessageType_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_MessageType_entries[] = {
		{0x59A0014E,
		 "MESSAGE_ERROR",
		 L"MESSAGE_ERROR",
		 sizeof("MESSAGE_ERROR") - 1,
		 3,
		 SOAPFLAG_FIELD,
		 0,
		 NULL,
		 NULL,
		 -1},
		{0x8395775A,
		 "MESSAGE_WARNING",
		 L"MESSAGE_WARNING",
		 sizeof("MESSAGE_WARNING") - 1,
		 2,
		 SOAPFLAG_FIELD,
		 0,
		 NULL,
		 NULL,
		 -1},
		{0xBCE7F070, "MESSAGE_INFO", L"MESSAGE_INFO", sizeof("MESSAGE_INFO") - 1, 1, SOAPFLAG_FIELD, 0, NULL, NULL, -1},
		{0xA52B2895,
		 "MESSAGE_OUTPUT",
		 L"MESSAGE_OUTPUT",
		 sizeof("MESSAGE_OUTPUT") - 1,
		 0,
		 SOAPFLAG_FIELD,
		 0,
		 NULL,
		 NULL,
		 -1},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_MessageType_map = {
		0x4CEF5547,
		"MessageType",
		L"MessageType",
		sizeof("MessageType") - 1,
		sizeof("MessageType") - 1,
		SOAPMAP_ENUM,
		___Roblox_MessageType_entries,
		sizeof(::Roblox::MessageType),
		1,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_Status_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_Status_entries[] = {
		{0x6F40CEE6,
		 "version",
		 L"version",
		 sizeof("version") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::Status, version),
		 NULL,
		 NULL,
		 0},
		{0x2967FF9E,
		 "environmentCount",
		 L"environmentCount",
		 sizeof("environmentCount") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::Status, environmentCount),
		 NULL,
		 NULL,
		 0},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_Status_map = {
		0xCA067464,
		"Status",
		L"Status",
		sizeof("Status") - 1,
		sizeof("Status") - 1,
		SOAPMAP_STRUCT,
		___Roblox_Status_entries,
		sizeof(::Roblox::Status),
		2,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue10_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue10_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue10, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue10, value),
		 NULL,
		 NULL,
		 0},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue10_map = {
		0xBB2D6540,
		"LuaValue10",
		L"LuaValue10",
		sizeof("LuaValue10") - 1,
		sizeof("LuaValue10") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue10_entries,
		sizeof(::Roblox::LuaValue10),
		2,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue9_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue9_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue9, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue9, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue9, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue9, table),
		 NULL,
		 &___Roblox_LuaValue10_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue9_map = {
		0x898D0318,
		"LuaValue9",
		L"LuaValue9",
		sizeof("LuaValue9") - 1,
		sizeof("LuaValue9") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue9_entries,
		sizeof(::Roblox::LuaValue9),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue8_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue8_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue8, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue8, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue8, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue8, table),
		 NULL,
		 &___Roblox_LuaValue9_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue8_map = {
		0x898D0317,
		"LuaValue8",
		L"LuaValue8",
		sizeof("LuaValue8") - 1,
		sizeof("LuaValue8") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue8_entries,
		sizeof(::Roblox::LuaValue8),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue7_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue7_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue7, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue7, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue7, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue7, table),
		 NULL,
		 &___Roblox_LuaValue8_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue7_map = {
		0x898D0316,
		"LuaValue7",
		L"LuaValue7",
		sizeof("LuaValue7") - 1,
		sizeof("LuaValue7") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue7_entries,
		sizeof(::Roblox::LuaValue7),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue6_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue6_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue6, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue6, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue6, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue6, table),
		 NULL,
		 &___Roblox_LuaValue7_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue6_map = {
		0x898D0315,
		"LuaValue6",
		L"LuaValue6",
		sizeof("LuaValue6") - 1,
		sizeof("LuaValue6") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue6_entries,
		sizeof(::Roblox::LuaValue6),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue5_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue5_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue5, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue5, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue5, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue5, table),
		 NULL,
		 &___Roblox_LuaValue6_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue5_map = {
		0x898D0314,
		"LuaValue5",
		L"LuaValue5",
		sizeof("LuaValue5") - 1,
		sizeof("LuaValue5") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue5_entries,
		sizeof(::Roblox::LuaValue5),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue4_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue4_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue4, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue4, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue4, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue4, table),
		 NULL,
		 &___Roblox_LuaValue5_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue4_map = {
		0x898D0313,
		"LuaValue4",
		L"LuaValue4",
		sizeof("LuaValue4") - 1,
		sizeof("LuaValue4") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue4_entries,
		sizeof(::Roblox::LuaValue4),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue3_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue3_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue3, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue3, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue3, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue3, table),
		 NULL,
		 &___Roblox_LuaValue4_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue3_map = {
		0x898D0312,
		"LuaValue3",
		L"LuaValue3",
		sizeof("LuaValue3") - 1,
		sizeof("LuaValue3") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue3_entries,
		sizeof(::Roblox::LuaValue3),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue2_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue2_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue2, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue2, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue2, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue2, table),
		 NULL,
		 &___Roblox_LuaValue3_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue2_map = {
		0x898D0311,
		"LuaValue2",
		L"LuaValue2",
		sizeof("LuaValue2") - 1,
		sizeof("LuaValue2") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue2_entries,
		sizeof(::Roblox::LuaValue2),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaValue1_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaValue1_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::LuaValue1, type),
		 NULL,
		 &___Roblox_LuaType_map,
		 0},
		{0x088E571D,
		 "value",
		 L"value",
		 sizeof("value") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue1, value),
		 NULL,
		 NULL,
		 0},
		{0x50CFFA4A,
		 "tableLength",
		 L"tableLength",
		 sizeof("tableLength") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaValue1, tableLength),
		 NULL,
		 NULL,
		 0},
		{0x0869FA68,
		 "table",
		 L"table",
		 sizeof("table") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaValue1, table),
		 NULL,
		 &___Roblox_LuaValue2_map,
		 2},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaValue1_map = {
		0x898D0310,
		"LuaValue1",
		L"LuaValue1",
		sizeof("LuaValue1") - 1,
		sizeof("LuaValue1") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaValue1_entries,
		sizeof(::Roblox::LuaValue1),
		4,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_LuaArguments_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_LuaArguments_entries[] = {
		{0x073E5849,
		 "count",
		 L"count",
		 sizeof("count") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::LuaArguments, count),
		 NULL,
		 NULL,
		 0},
		{0x07AD6502,
		 "items",
		 L"items",
		 sizeof("items") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::LuaArguments, items),
		 NULL,
		 &___Roblox_LuaValue1_map,
		 0},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_LuaArguments_map = {
		0xDE10C238,
		"LuaArguments",
		L"LuaArguments",
		sizeof("LuaArguments") - 1,
		sizeof("LuaArguments") - 1,
		SOAPMAP_STRUCT,
		___Roblox_LuaArguments_entries,
		sizeof(::Roblox::LuaArguments),
		2,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_ScriptExecution_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_ScriptExecution_entries[] = {
		{0x003DFCC1,
		 "name",
		 L"name",
		 sizeof("name") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::ScriptExecution, name),
		 NULL,
		 NULL,
		 0},
		{0x13810AF5,
		 "script",
		 L"script",
		 sizeof("script") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::ScriptExecution, script),
		 NULL,
		 NULL,
		 0},
		{0x69FF5556,
		 "arguments",
		 L"arguments",
		 sizeof("arguments") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::ScriptExecution, arguments),
		 NULL,
		 &___Roblox_LuaArguments_map,
		 0},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_ScriptExecution_map = {
		0xE610FFE9,
		"ScriptExecution",
		L"ScriptExecution",
		sizeof("ScriptExecution") - 1,
		sizeof("ScriptExecution") - 1,
		SOAPMAP_STRUCT,
		___Roblox_ScriptExecution_entries,
		sizeof(::Roblox::ScriptExecution),
		3,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_Strings_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_Strings_entries[] = {
		{0x073E5849,
		 "count",
		 L"count",
		 sizeof("count") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::Strings, count),
		 NULL,
		 NULL,
		 0},
		{0x07AD6502,
		 "items",
		 L"items",
		 sizeof("items") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::Strings, items),
		 NULL,
		 NULL,
		 0},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_Strings_map = {
		0x0C027A6A,
		"Strings",
		L"Strings",
		sizeof("Strings") - 1,
		sizeof("Strings") - 1,
		SOAPMAP_STRUCT,
		___Roblox_Strings_entries,
		sizeof(::Roblox::Strings),
		2,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_StandardOutMessage_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_StandardOutMessage_entries[] = {
		{0x0041AD82,
		 "type",
		 L"type",
		 sizeof("type") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::StandardOutMessage, type),
		 NULL,
		 &___Roblox_MessageType_map,
		 0},
		{0x0041690F,
		 "time",
		 L"time",
		 sizeof("time") - 1,
		 SOAPTYPE_LONG,
		 SOAPFLAG_FIELD,
		 offsetof(::Roblox::StandardOutMessage, time),
		 NULL,
		 NULL,
		 0},
		{0x00415985,
		 "text",
		 L"text",
		 sizeof("text") - 1,
		 SOAPTYPE_STRING,
		 SOAPFLAG_FIELD | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::StandardOutMessage, text),
		 NULL,
		 NULL,
		 0},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_StandardOutMessage_map = {
		0x46A169AE,
		"StandardOutMessage",
		L"StandardOutMessage",
		sizeof("StandardOutMessage") - 1,
		sizeof("StandardOutMessage") - 1,
		SOAPMAP_STRUCT,
		___Roblox_StandardOutMessage_entries,
		sizeof(::Roblox::StandardOutMessage),
		3,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

__if_not_exists(___Roblox_StandardOutMessages_entries)
{
	extern __declspec(selectany) const _soapmapentry ___Roblox_StandardOutMessages_entries[] = {
		{0x073E5849,
		 "count",
		 L"count",
		 sizeof("count") - 1,
		 SOAPTYPE_INT,
		 SOAPFLAG_FIELD | SOAPFLAG_SIZEIS,
		 offsetof(::Roblox::StandardOutMessages, count),
		 NULL,
		 NULL,
		 0},
		{0x07AD6502,
		 "items",
		 L"items",
		 sizeof("items") - 1,
		 SOAPTYPE_UNK,
		 SOAPFLAG_FIELD | SOAPFLAG_DYNARR | SOAPFLAG_NULLABLE,
		 offsetof(::Roblox::StandardOutMessages, items),
		 NULL,
		 &___Roblox_StandardOutMessage_map,
		 0},
		{0x00000000}
	};

	extern __declspec(selectany) const _soapmap ___Roblox_StandardOutMessages_map = {
		0x1ACE9FE1,
		"StandardOutMessages",
		L"StandardOutMessages",
		sizeof("StandardOutMessages") - 1,
		sizeof("StandardOutMessages") - 1,
		SOAPMAP_STRUCT,
		___Roblox_StandardOutMessages_entries,
		sizeof(::Roblox::StandardOutMessages),
		2,
		-1,
		SOAPFLAG_NONE,
		0x74911DC5,
		"urn:Roblox",
		L"urn:Roblox",
		sizeof("urn:Roblox") - 1
	};
}

struct ___Roblox_CWebService_HelloWorld_struct
{
	BSTR bstrOutput;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_HelloWorld_entries[] = {

	{
		0xF6041A8C,
		"bstrOutput",
		L"bstrOutput",
		sizeof("bstrOutput") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_HelloWorld_struct, bstrOutput),
		NULL,
		NULL,
		-1,
	},

	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_HelloWorld_map = {
	0x46BA99FC,
	"HelloWorld",
	L"HelloWorld",
	sizeof("HelloWorld") - 1,
	sizeof("HelloWorld") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_HelloWorld_entries,
	sizeof(___Roblox_CWebService_HelloWorld_struct),
	0,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_HelloWorld_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_HelloWorld_atlsoapheader_map = {
	0x46BA99FC,
	"HelloWorld",
	L"HelloWorld",
	sizeof("HelloWorld") - 1,
	sizeof("HelloWorld") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_HelloWorld_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_GetVersion_struct
{
	BSTR result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_GetVersion_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_GetVersion_struct, result),
		NULL,
		NULL,
		-1,
	},

	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetVersion_map = {
	0xF1D48246,
	"GetVersion",
	L"GetVersion",
	sizeof("GetVersion") - 1,
	sizeof("GetVersion") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_GetVersion_entries,
	sizeof(___Roblox_CWebService_GetVersion_struct),
	0,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_GetVersion_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetVersion_atlsoapheader_map = {
	0xF1D48246,
	"GetVersion",
	L"GetVersion",
	sizeof("GetVersion") - 1,
	sizeof("GetVersion") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_GetVersion_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_GetStatus_struct
{
	::Roblox::Status result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_GetStatus_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_GetStatus_struct, result),
		NULL,
		&___Roblox_Status_map,
		-1,
	},

	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetStatus_map = {
	0x2FE64FE4,
	"GetStatus",
	L"GetStatus",
	sizeof("GetStatus") - 1,
	sizeof("GetStatus") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_GetStatus_entries,
	sizeof(___Roblox_CWebService_GetStatus_struct),
	0,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_GetStatus_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetStatus_atlsoapheader_map = {
	0x2FE64FE4,
	"GetStatus",
	L"GetStatus",
	sizeof("GetStatus") - 1,
	sizeof("GetStatus") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_GetStatus_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_Update_struct
{
	BSTR url;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_Update_entries[] = {

	{
		0x000200D3,
		"url",
		L"url",
		sizeof("url") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_Update_struct, url),
		NULL,
		NULL,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_Update_map = {
	0xCE69B7C3,
	"Update",
	L"Update",
	sizeof("Update") - 1,
	sizeof("Update") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_Update_entries,
	sizeof(___Roblox_CWebService_Update_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_Update_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_Update_atlsoapheader_map = {
	0xCE69B7C3,
	"Update",
	L"Update",
	sizeof("Update") - 1,
	sizeof("Update") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_Update_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_OpenJob_struct
{
	BSTR jobID;
	::Roblox::ScriptExecution script;
	double timeout;
	::Roblox::LuaArguments result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_OpenJob_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_OpenJob_struct, result),
		NULL,
		&___Roblox_LuaArguments_map,
		-1,
	},

	{
		0x07BCAE08,
		"jobID",
		L"jobID",
		sizeof("jobID") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_OpenJob_struct, jobID),
		NULL,
		NULL,
		-1,
	},
	{
		0x13810AF5,
		"script",
		L"script",
		sizeof("script") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_NONE | SOAPFLAG_IN,
		offsetof(___Roblox_CWebService_OpenJob_struct, script),
		NULL,
		&___Roblox_ScriptExecution_map,
		-1,
	},
	{
		0xDE3EEA67,
		"timeout",
		L"timeout",
		sizeof("timeout") - 1,
		SOAPTYPE_DOUBLE,
		SOAPFLAG_NONE | SOAPFLAG_IN,
		offsetof(___Roblox_CWebService_OpenJob_struct, timeout),
		NULL,
		NULL,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_OpenJob_map = {
	0xCDDBCC0D,
	"OpenJob",
	L"OpenJob",
	sizeof("OpenJob") - 1,
	sizeof("OpenJob") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_OpenJob_entries,
	sizeof(___Roblox_CWebService_OpenJob_struct),
	3,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_OpenJob_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_OpenJob_atlsoapheader_map = {
	0xCDDBCC0D,
	"OpenJob",
	L"OpenJob",
	sizeof("OpenJob") - 1,
	sizeof("OpenJob") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_OpenJob_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_TouchJob_struct
{
	BSTR jobID;
	double timeout;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_TouchJob_entries[] = {

	{
		0x07BCAE08,
		"jobID",
		L"jobID",
		sizeof("jobID") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_TouchJob_struct, jobID),
		NULL,
		NULL,
		-1,
	},
	{
		0xDE3EEA67,
		"timeout",
		L"timeout",
		sizeof("timeout") - 1,
		SOAPTYPE_DOUBLE,
		SOAPFLAG_NONE | SOAPFLAG_IN,
		offsetof(___Roblox_CWebService_TouchJob_struct, timeout),
		NULL,
		NULL,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_TouchJob_map = {
	0xFE3F8DDE,
	"TouchJob",
	L"TouchJob",
	sizeof("TouchJob") - 1,
	sizeof("TouchJob") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_TouchJob_entries,
	sizeof(___Roblox_CWebService_TouchJob_struct),
	2,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_TouchJob_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_TouchJob_atlsoapheader_map = {
	0xFE3F8DDE,
	"TouchJob",
	L"TouchJob",
	sizeof("TouchJob") - 1,
	sizeof("TouchJob") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_TouchJob_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_Execute_struct
{
	BSTR jobID;
	::Roblox::ScriptExecution script;
	::Roblox::LuaArguments result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_Execute_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_Execute_struct, result),
		NULL,
		&___Roblox_LuaArguments_map,
		-1,
	},

	{
		0x07BCAE08,
		"jobID",
		L"jobID",
		sizeof("jobID") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_Execute_struct, jobID),
		NULL,
		NULL,
		-1,
	},
	{
		0x13810AF5,
		"script",
		L"script",
		sizeof("script") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_NONE | SOAPFLAG_IN,
		offsetof(___Roblox_CWebService_Execute_struct, script),
		NULL,
		&___Roblox_ScriptExecution_map,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_Execute_map = {
	0xDEB960F3,
	"Execute",
	L"Execute",
	sizeof("Execute") - 1,
	sizeof("Execute") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_Execute_entries,
	sizeof(___Roblox_CWebService_Execute_struct),
	2,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_Execute_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_Execute_atlsoapheader_map = {
	0xDEB960F3,
	"Execute",
	L"Execute",
	sizeof("Execute") - 1,
	sizeof("Execute") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_Execute_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_CloseJob_struct
{
	BSTR jobID;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_CloseJob_entries[] = {

	{
		0x07BCAE08,
		"jobID",
		L"jobID",
		sizeof("jobID") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_CloseJob_struct, jobID),
		NULL,
		NULL,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_CloseJob_map = {
	0x5A12A3D1,
	"CloseJob",
	L"CloseJob",
	sizeof("CloseJob") - 1,
	sizeof("CloseJob") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_CloseJob_entries,
	sizeof(___Roblox_CWebService_CloseJob_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_CloseJob_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_CloseJob_atlsoapheader_map = {
	0x5A12A3D1,
	"CloseJob",
	L"CloseJob",
	sizeof("CloseJob") - 1,
	sizeof("CloseJob") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_CloseJob_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_BatchJob_struct
{
	BSTR jobID;
	::Roblox::ScriptExecution script;
	double timeout;
	::Roblox::LuaArguments result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_BatchJob_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_BatchJob_struct, result),
		NULL,
		&___Roblox_LuaArguments_map,
		-1,
	},

	{
		0x07BCAE08,
		"jobID",
		L"jobID",
		sizeof("jobID") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_BatchJob_struct, jobID),
		NULL,
		NULL,
		-1,
	},
	{
		0x13810AF5,
		"script",
		L"script",
		sizeof("script") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_NONE | SOAPFLAG_IN,
		offsetof(___Roblox_CWebService_BatchJob_struct, script),
		NULL,
		&___Roblox_ScriptExecution_map,
		-1,
	},
	{
		0xDE3EEA67,
		"timeout",
		L"timeout",
		sizeof("timeout") - 1,
		SOAPTYPE_DOUBLE,
		SOAPFLAG_NONE | SOAPFLAG_IN,
		offsetof(___Roblox_CWebService_BatchJob_struct, timeout),
		NULL,
		NULL,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_BatchJob_map = {
	0x299A1ADD,
	"BatchJob",
	L"BatchJob",
	sizeof("BatchJob") - 1,
	sizeof("BatchJob") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_BatchJob_entries,
	sizeof(___Roblox_CWebService_BatchJob_struct),
	3,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_BatchJob_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_BatchJob_atlsoapheader_map = {
	0x299A1ADD,
	"BatchJob",
	L"BatchJob",
	sizeof("BatchJob") - 1,
	sizeof("BatchJob") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_BatchJob_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_GetTimeout_struct
{
	BSTR jobID;
	double timeout;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_GetTimeout_entries[] = {

	{
		0xDE3EEA67,
		"timeout",
		L"timeout",
		sizeof("timeout") - 1,
		SOAPTYPE_DOUBLE,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_GetTimeout_struct, timeout),
		NULL,
		NULL,
		-1,
	},

	{
		0x07BCAE08,
		"jobID",
		L"jobID",
		sizeof("jobID") - 1,
		SOAPTYPE_STRING,
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_NULLABLE,
		offsetof(___Roblox_CWebService_GetTimeout_struct, jobID),
		NULL,
		NULL,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetTimeout_map = {
	0x60D29DC7,
	"GetTimeout",
	L"GetTimeout",
	sizeof("GetTimeout") - 1,
	sizeof("GetTimeout") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_GetTimeout_entries,
	sizeof(___Roblox_CWebService_GetTimeout_struct),
	1,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_GetTimeout_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetTimeout_atlsoapheader_map = {
	0x60D29DC7,
	"GetTimeout",
	L"GetTimeout",
	sizeof("GetTimeout") - 1,
	sizeof("GetTimeout") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_GetTimeout_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_CloseOrphanedJobs_struct
{
	::Roblox::Strings activeJobs;
	int result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_CloseOrphanedJobs_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_INT,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_CloseOrphanedJobs_struct, result),
		NULL,
		NULL,
		-1,
	},

	{
		0x4F26F06A,
		"activeJobs",
		L"activeJobs",
		sizeof("activeJobs") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_NONE | SOAPFLAG_IN,
		offsetof(___Roblox_CWebService_CloseOrphanedJobs_struct, activeJobs),
		NULL,
		&___Roblox_Strings_map,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_CloseOrphanedJobs_map = {
	0x70A02135,
	"CloseOrphanedJobs",
	L"CloseOrphanedJobs",
	sizeof("CloseOrphanedJobs") - 1,
	sizeof("CloseOrphanedJobs") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_CloseOrphanedJobs_entries,
	sizeof(___Roblox_CWebService_CloseOrphanedJobs_struct),
	1,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_CloseOrphanedJobs_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_CloseOrphanedJobs_atlsoapheader_map = {
	0x70A02135,
	"CloseOrphanedJobs",
	L"CloseOrphanedJobs",
	sizeof("CloseOrphanedJobs") - 1,
	sizeof("CloseOrphanedJobs") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_CloseOrphanedJobs_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_CloseAllJobs_struct
{
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_CloseAllJobs_entries[] = {

	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_CloseAllJobs_map = {
	0xEC85F5FD,
	"CloseAllJobs",
	L"CloseAllJobs",
	sizeof("CloseAllJobs") - 1,
	sizeof("CloseAllJobs") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_CloseAllJobs_entries,
	sizeof(___Roblox_CWebService_CloseAllJobs_struct),
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_CloseAllJobs_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_CloseAllJobs_atlsoapheader_map = {
	0xEC85F5FD,
	"CloseAllJobs",
	L"CloseAllJobs",
	sizeof("CloseAllJobs") - 1,
	sizeof("CloseAllJobs") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_CloseAllJobs_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_CloseTimedoutJobs_struct
{
	int result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_CloseTimedoutJobs_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_INT,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_CloseTimedoutJobs_struct, result),
		NULL,
		NULL,
		-1,
	},

	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_CloseTimedoutJobs_map = {
	0x83BD5BEF,
	"CloseTimedoutJobs",
	L"CloseTimedoutJobs",
	sizeof("CloseTimedoutJobs") - 1,
	sizeof("CloseTimedoutJobs") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_CloseTimedoutJobs_entries,
	sizeof(___Roblox_CWebService_CloseTimedoutJobs_struct),
	0,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_CloseTimedoutJobs_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_CloseTimedoutJobs_atlsoapheader_map = {
	0x83BD5BEF,
	"CloseTimedoutJobs",
	L"CloseTimedoutJobs",
	sizeof("CloseTimedoutJobs") - 1,
	sizeof("CloseTimedoutJobs") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_CloseTimedoutJobs_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_GetAllJobs_struct
{
	::Roblox::Strings result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_GetAllJobs_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_GetAllJobs_struct, result),
		NULL,
		&___Roblox_Strings_map,
		-1,
	},

	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetAllJobs_map = {
	0xB11EAC27,
	"GetAllJobs",
	L"GetAllJobs",
	sizeof("GetAllJobs") - 1,
	sizeof("GetAllJobs") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_GetAllJobs_entries,
	sizeof(___Roblox_CWebService_GetAllJobs_struct),
	0,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_GetAllJobs_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetAllJobs_atlsoapheader_map = {
	0xB11EAC27,
	"GetAllJobs",
	L"GetAllJobs",
	sizeof("GetAllJobs") - 1,
	sizeof("GetAllJobs") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_GetAllJobs_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
struct ___Roblox_CWebService_GetStandardOutMessages_struct
{
	int maxCount;
	::Roblox::StandardOutMessages result;
};

extern __declspec(selectany) const _soapmapentry ___Roblox_CWebService_GetStandardOutMessages_entries[] = {

	{
		0x1150D23F,
		"result",
		L"result",
		sizeof("result") - 1,
		SOAPTYPE_UNK,
		SOAPFLAG_RETVAL | SOAPFLAG_OUT,
		offsetof(___Roblox_CWebService_GetStandardOutMessages_struct, result),
		NULL,
		&___Roblox_StandardOutMessages_map,
		-1,
	},

	{
		0xDFBEFC8F,
		"maxCount",
		L"maxCount",
		sizeof("maxCount") - 1,
		SOAPTYPE_INT,
		SOAPFLAG_NONE | SOAPFLAG_IN,
		offsetof(___Roblox_CWebService_GetStandardOutMessages_struct, maxCount),
		NULL,
		NULL,
		-1,
	},
	{0x00000000}
};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetStandardOutMessages_map = {
	0x64772B61,
	"GetStandardOutMessages",
	L"GetStandardOutMessages",
	sizeof("GetStandardOutMessages") - 1,
	sizeof("GetStandardOutMessages") - 1,
	SOAPMAP_FUNC,
	___Roblox_CWebService_GetStandardOutMessages_entries,
	sizeof(___Roblox_CWebService_GetStandardOutMessages_struct),
	1,
	0,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};

extern __declspec(selectany)
const _soapmapentry ___Roblox_CWebService_GetStandardOutMessages_atlsoapheader_entries[] = {{0x00000000}};

extern __declspec(selectany) const _soapmap ___Roblox_CWebService_GetStandardOutMessages_atlsoapheader_map = {
	0x64772B61,
	"GetStandardOutMessages",
	L"GetStandardOutMessages",
	sizeof("GetStandardOutMessages") - 1,
	sizeof("GetStandardOutMessages") - 1,
	SOAPMAP_HEADER,
	___Roblox_CWebService_GetStandardOutMessages_atlsoapheader_entries,
	0,
	0,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x74911DC5,
	"urn:Roblox",
	L"urn:Roblox",
	sizeof("urn:Roblox") - 1
};
extern __declspec(selectany) const _soapmap* ___Roblox_CWebService_funcs[] = {
	&___Roblox_CWebService_HelloWorld_map,
	&___Roblox_CWebService_GetVersion_map,
	&___Roblox_CWebService_GetStatus_map,
	&___Roblox_CWebService_Update_map,
	&___Roblox_CWebService_OpenJob_map,
	&___Roblox_CWebService_TouchJob_map,
	&___Roblox_CWebService_Execute_map,
	&___Roblox_CWebService_CloseJob_map,
	&___Roblox_CWebService_BatchJob_map,
	&___Roblox_CWebService_GetTimeout_map,
	&___Roblox_CWebService_CloseOrphanedJobs_map,
	&___Roblox_CWebService_CloseAllJobs_map,
	&___Roblox_CWebService_CloseTimedoutJobs_map,
	&___Roblox_CWebService_GetAllJobs_map,
	&___Roblox_CWebService_GetStandardOutMessages_map,
	NULL
};

extern __declspec(selectany) const _soapmap* ___Roblox_CWebService_headers[] = {
	&___Roblox_CWebService_HelloWorld_atlsoapheader_map,
	&___Roblox_CWebService_GetVersion_atlsoapheader_map,
	&___Roblox_CWebService_GetStatus_atlsoapheader_map,
	&___Roblox_CWebService_Update_atlsoapheader_map,
	&___Roblox_CWebService_OpenJob_atlsoapheader_map,
	&___Roblox_CWebService_TouchJob_atlsoapheader_map,
	&___Roblox_CWebService_Execute_atlsoapheader_map,
	&___Roblox_CWebService_CloseJob_atlsoapheader_map,
	&___Roblox_CWebService_BatchJob_atlsoapheader_map,
	&___Roblox_CWebService_GetTimeout_atlsoapheader_map,
	&___Roblox_CWebService_CloseOrphanedJobs_atlsoapheader_map,
	&___Roblox_CWebService_CloseAllJobs_atlsoapheader_map,
	&___Roblox_CWebService_CloseTimedoutJobs_atlsoapheader_map,
	&___Roblox_CWebService_GetAllJobs_atlsoapheader_map,
	&___Roblox_CWebService_GetStandardOutMessages_atlsoapheader_map,
	NULL
};

// FUNCTION: WEBSERVICE 0x1000dcc0
ATL_NOINLINE inline const _soapmap** CWebService::GetFunctionMap()
{
	return ___Roblox_CWebService_funcs;
}

// FUNCTION: WEBSERVICE 0x1000dcd0
ATL_NOINLINE inline const _soapmap** CWebService::GetHeaderMap()
{
	return ___Roblox_CWebService_headers;
}

// FUNCTION: WEBSERVICE 0x1000dce0
ATL_NOINLINE inline void* CWebService::GetHeaderValue()
{
	return this;
}

// FUNCTION: WEBSERVICE 0x1000dcf0
ATL_NOINLINE inline HRESULT CWebService::CallFunction(
	void* pvParam,
	const wchar_t* wszLocalName,
	int cchLocalName,
	size_t nItem
)
{
	wszLocalName;
	cchLocalName;

	HRESULT hr = S_OK;

	switch (nItem) {
	case 0: {
		___Roblox_CWebService_HelloWorld_struct* p = (___Roblox_CWebService_HelloWorld_struct*) pvParam;
		hr = HelloWorld(&p->bstrOutput);
		break;
	}
	case 1: {
		___Roblox_CWebService_GetVersion_struct* p = (___Roblox_CWebService_GetVersion_struct*) pvParam;
		hr = GetVersion(&p->result);
		break;
	}
	case 2: {
		___Roblox_CWebService_GetStatus_struct* p = (___Roblox_CWebService_GetStatus_struct*) pvParam;
		hr = GetStatus(&p->result);
		break;
	}
	case 3: {
		___Roblox_CWebService_Update_struct* p = (___Roblox_CWebService_Update_struct*) pvParam;
		hr = Update(p->url);
		break;
	}
	case 4: {
		___Roblox_CWebService_OpenJob_struct* p = (___Roblox_CWebService_OpenJob_struct*) pvParam;
		hr = OpenJob(p->jobID, p->script, p->timeout, &p->result);
		break;
	}
	case 5: {
		___Roblox_CWebService_TouchJob_struct* p = (___Roblox_CWebService_TouchJob_struct*) pvParam;
		hr = TouchJob(p->jobID, p->timeout);
		break;
	}
	case 6: {
		___Roblox_CWebService_Execute_struct* p = (___Roblox_CWebService_Execute_struct*) pvParam;
		hr = Execute(p->jobID, p->script, &p->result);
		break;
	}
	case 7: {
		___Roblox_CWebService_CloseJob_struct* p = (___Roblox_CWebService_CloseJob_struct*) pvParam;
		hr = CloseJob(p->jobID);
		break;
	}
	case 8: {
		___Roblox_CWebService_BatchJob_struct* p = (___Roblox_CWebService_BatchJob_struct*) pvParam;
		hr = BatchJob(p->jobID, p->script, p->timeout, &p->result);
		break;
	}
	case 9: {
		___Roblox_CWebService_GetTimeout_struct* p = (___Roblox_CWebService_GetTimeout_struct*) pvParam;
		hr = GetTimeout(p->jobID, &p->timeout);
		break;
	}
	case 10: {
		___Roblox_CWebService_CloseOrphanedJobs_struct* p = (___Roblox_CWebService_CloseOrphanedJobs_struct*) pvParam;
		hr = CloseOrphanedJobs(p->activeJobs, &p->result);
		break;
	}
	case 11: {
		___Roblox_CWebService_CloseAllJobs_struct* p = (___Roblox_CWebService_CloseAllJobs_struct*) pvParam;
		hr = CloseAllJobs();
		break;
	}
	case 12: {
		___Roblox_CWebService_CloseTimedoutJobs_struct* p = (___Roblox_CWebService_CloseTimedoutJobs_struct*) pvParam;
		hr = CloseTimedoutJobs(&p->result);
		break;
	}
	case 13: {
		___Roblox_CWebService_GetAllJobs_struct* p = (___Roblox_CWebService_GetAllJobs_struct*) pvParam;
		hr = GetAllJobs(&p->result);
		break;
	}
	case 14: {
		___Roblox_CWebService_GetStandardOutMessages_struct* p =
			(___Roblox_CWebService_GetStandardOutMessages_struct*) pvParam;
		hr = GetStandardOutMessages(p->maxCount, &p->result);
		break;
	}
	default:
		hr = E_FAIL;
	}

	return hr;
}

// FUNCTION: WEBSERVICE 0x1000df50
ATL_NOINLINE inline const wchar_t* CWebService::GetNamespaceUri()
{
	return L"urn:Roblox";
}

// FUNCTION: WEBSERVICE 0x1000df60
ATL_NOINLINE inline const char* CWebService::GetNamespaceUriA()
{
	return "urn:Roblox";
}

// FUNCTION: WEBSERVICE 0x1000df70
ATL_NOINLINE inline const char* CWebService::GetServiceName()
{
	return "Service";
}

// FUNCTION: WEBSERVICE 0x1000df80
inline HRESULT CWebService::GenerateAppError(ATL::IWriteStream* pStream, HRESULT hr)
{
	if (errorMessage.GetLength() > 0) {
		return SoapFault(SOAP_E_SERVER, errorMessage, -1);
	}

	return ATL::CSoapHandler<CWebService>::GenerateAppError(pStream, hr);
}

// FUNCTION: WEBSERVICE 0x1000dfb0
inline HRESULT __stdcall CWebService::HelloWorld(BSTR* bstrOutput)
{
	ATL::CComBSTR answer(L"Hello World!");
	*bstrOutput = answer.Detach();

	return S_OK;
}

DECOMP_SIZE_ASSERT(CWebService, 0x180)

// clang-format off
// TEMPLATE: WEBSERVICE 0x100195e0
// Roblox::convert<Roblox::LuaValue1>
// STUB: WEBSERVICE 0x10019890
// Roblox::CWebService::convertToLua<Roblox::LuaValue1>
// TEMPLATE: WEBSERVICE 0x1001bfd0
// Roblox::convert<Roblox::LuaValue2>
// STUB: WEBSERVICE 0x1001c470
// Roblox::CWebService::convertToLua<Roblox::LuaValue2>
// TEMPLATE: WEBSERVICE 0x1001d370
// Roblox::convert<Roblox::LuaValue3>
// STUB: WEBSERVICE 0x1001d890
// Roblox::CWebService::convertToLua<Roblox::LuaValue3>
// TEMPLATE: WEBSERVICE 0x1001e0d0
// Roblox::convert<Roblox::LuaValue4>
// STUB: WEBSERVICE 0x1001e490
// Roblox::CWebService::convertToLua<Roblox::LuaValue4>
// TEMPLATE: WEBSERVICE 0x1001eb60
// Roblox::convert<Roblox::LuaValue5>
// STUB: WEBSERVICE 0x1001edd0
// Roblox::CWebService::convertToLua<Roblox::LuaValue5>
// TEMPLATE: WEBSERVICE 0x1001f2b0
// Roblox::convert<Roblox::LuaValue6>
// STUB: WEBSERVICE 0x1001f520
// Roblox::CWebService::convertToLua<Roblox::LuaValue6>
// TEMPLATE: WEBSERVICE 0x1001f8c0
// Roblox::convert<Roblox::LuaValue7>
// STUB: WEBSERVICE 0x1001fb30
// Roblox::CWebService::convertToLua<Roblox::LuaValue7>
// TEMPLATE: WEBSERVICE 0x1001ff50
// Roblox::convert<Roblox::LuaValue8>
// STUB: WEBSERVICE 0x100201c0
// Roblox::CWebService::convertToLua<Roblox::LuaValue8>
// TEMPLATE: WEBSERVICE 0x10020560
// Roblox::convert<Roblox::LuaValue9>
// STUB: WEBSERVICE 0x10020800
// Roblox::CWebService::convertToLua<Roblox::LuaValue9>
// TEMPLATE: WEBSERVICE 0x10020b70
// Roblox::convert<Roblox::LuaValue10>
// TEMPLATE: WEBSERVICE 0x10020db0
// Roblox::CWebService::convertToLua<Roblox::LuaValue10>
// clang-format on

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10217fe0
// `dynamic initializer for 'mainLogManager''
// SYNTHETIC: WEBSERVICE 0x10217ff0
// `dynamic initializer for 'standardOutLog''
// SYNTHETIC: WEBSERVICE 0x10218000
// `dynamic initializer for 'dotVersion''
// SYNTHETIC: WEBSERVICE 0x10218020
// `dynamic initializer for '_AtlModule''
// SYNTHETIC: WEBSERVICE 0x10218040
// `dynamic initializer for 'theExtension''
// SYNTHETIC: WEBSERVICE 0x10218070
// `dynamic initializer for 'Roblox::CWebService::jobs''
// SYNTHETIC: WEBSERVICE 0x102180b0
// `dynamic initializer for 'Roblox::CWebService::sync''
// SYNTHETIC: WEBSERVICE 0x10221270
// `dynamic atexit destructor for 'mainLogManager''
// SYNTHETIC: WEBSERVICE 0x10221290
// `dynamic atexit destructor for 'standardOutLog''
// SYNTHETIC: WEBSERVICE 0x102212b0
// `dynamic atexit destructor for 'dotVersion''
// SYNTHETIC: WEBSERVICE 0x102212d0
// `dynamic atexit destructor for '_AtlModule''
// SYNTHETIC: WEBSERVICE 0x102212e0
// `dynamic atexit destructor for 'theExtension''
// SYNTHETIC: WEBSERVICE 0x102212f0
// `dynamic atexit destructor for 'Roblox::CWebService::jobs''
// SYNTHETIC: WEBSERVICE 0x10221300
// `dynamic atexit destructor for 'Roblox::CWebService::sync''
// clang-format on

} // namespace Roblox

HANDLER_ENTRY_SDL("Default", CWebService, ::Roblox::CWebService, WSDL)

#endif // WEBSERVICE_WEBSERVICE_H
