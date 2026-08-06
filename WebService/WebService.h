#ifndef WEBSERVICE_WEBSERVICE_H
#define WEBSERVICE_WEBSERVICE_H

// clang-format off
// winsock2.h before anything that pulls in windows.h.
#include <winsock2.h>
// clang-format on

#include <unknwn.h>

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

struct OpenJobParams
{
	wchar_t* jobID;         // 0x00
	ScriptExecution script; // 0x04
	double expiration;      // 0x18
	LuaArguments result;    // 0x20
};

struct TouchJobParams
{
	wchar_t* jobID;    // 0x00
	double expiration; // 0x08
};

struct ExecuteParams
{
	wchar_t* jobID;         // 0x00
	ScriptExecution script; // 0x04
	LuaArguments result;    // 0x14
};

struct GetTimeoutParams
{
	wchar_t* jobID; // 0x00
	double timeout; // 0x08
};

struct CloseOrphanedJobsParams
{
	Strings jobs; // 0x00
	int result;   // 0x08
};

struct GetStandardOutMessagesParams
{
	int lastId;                 // 0x00
	StandardOutMessages result; // 0x04
};

struct __declspec(uuid("6d7e5c9a-0000-0000-0000-000000000000")) IWebService : public IUnknown
{
	virtual HRESULT __stdcall HelloWorld(wchar_t** result) = 0; // vtable+0x0c
	virtual HRESULT __stdcall GetVersion(wchar_t** result) = 0; // vtable+0x10
	virtual HRESULT __stdcall GetStatus(Status* result) = 0;    // vtable+0x14
	virtual HRESULT __stdcall Update(wchar_t* url) = 0;         // vtable+0x18
	virtual HRESULT __stdcall OpenJob(
		wchar_t* jobID,
		ScriptExecution script,
		double expiration,
		LuaArguments* result
	) = 0;                                                                     // vtable+0x1c
	virtual HRESULT __stdcall TouchJob(wchar_t* jobID, double expiration) = 0; // vtable+0x20
	virtual HRESULT __stdcall Execute(wchar_t* jobID, ScriptExecution script,
									  LuaArguments* result) = 0; // vtable+0x24
	virtual HRESULT __stdcall CloseJob(wchar_t* jobID) = 0;      // vtable+0x28
	virtual HRESULT __stdcall BatchJob(
		wchar_t* jobID,
		ScriptExecution script,
		double expiration,
		LuaArguments* result
	) = 0;                                                                      // vtable+0x2c
	virtual HRESULT __stdcall GetTimeout(wchar_t* jobID, double* timeout) = 0;  // vtable+0x30
	virtual HRESULT __stdcall GetAllJobs(Strings* result) = 0;                  // vtable+0x34
	virtual HRESULT __stdcall CloseOrphanedJobs(Strings jobs, int* result) = 0; // vtable+0x38
	virtual HRESULT __stdcall CloseTimedoutJobs(int* result) = 0;               // vtable+0x3c
	virtual HRESULT __stdcall CloseAllJobs() = 0;                               // vtable+0x40
	virtual HRESULT __stdcall GetStandardOutMessages(int lastId,
													 StandardOutMessages* result) = 0; // vtable+0x44
};

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10217ff0
// `dynamic initializer for 'standardOutLog''
// SYNTHETIC: WEBSERVICE 0x10218000
// `dynamic initializer for 'dotVersion''
// SYNTHETIC: WEBSERVICE 0x10218020
// `dynamic initializer for '_AtlModule''
// SYNTHETIC: WEBSERVICE 0x10218040
// `dynamic initializer for 'theExtension''
// SYNTHETIC: WEBSERVICE 0x102180b0
// `dynamic initializer for 'Roblox::CWebService::sync''
// SYNTHETIC: WEBSERVICE 0x10221290
// `dynamic atexit destructor for 'standardOutLog''
// SYNTHETIC: WEBSERVICE 0x102212b0
// `dynamic atexit destructor for 'dotVersion''
// SYNTHETIC: WEBSERVICE 0x102212d0
// `dynamic atexit destructor for '_AtlModule''
// SYNTHETIC: WEBSERVICE 0x102212e0
// `dynamic atexit destructor for 'theExtension''
// SYNTHETIC: WEBSERVICE 0x10221300
// `dynamic atexit destructor for 'Roblox::CWebService::sync''
// clang-format on

} // namespace Roblox

#endif // WEBSERVICE_WEBSERVICE_H
