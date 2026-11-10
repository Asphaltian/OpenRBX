#include "LuaFunction.h"

#include "Content.h"
#include "LogManager.h"
#include "script/ScriptContext.h"
#include "util/StandardOut.h"
#include "v8datamodel/DataModel.h"

#include <exception>
#include <memory>

// FUNCTION: ROBLOX 0x00431ad0
STDMETHODIMP CLuaFunction::CallEx(SAFEARRAY* args, SAFEARRAY** result)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	std::auto_ptr<std::vector<RBX::Reflection::Value> > results;
	if (result != NULL) {
		*result = FUN_00413500(*results);
	}

	return E_NOTIMPL;
}

// FUNCTION: ROBLOX 0x004321b0
STDMETHODIMP CLuaFunction::Call(VARIANT arg1, VARIANT arg2, VARIANT arg3, VARIANT arg4, VARIANT* result)
{
	AFX_MANAGE_STATE(AfxGetAppModuleState());

	try {
		Unknown00431c00 arguments;
		arguments.FUN_00431c00(arg1);
		arguments.FUN_00431c00(arg2);
		arguments.FUN_00431c00(arg3);
		arguments.FUN_00431c00(arg4);

		std::auto_ptr<std::vector<RBX::Reflection::Value> > results;
		{
			RBX::DataModel::Lock lock(m_unk0x0c);
			results = (m_unk0x0c ? m_unk0x0c->create<RBX::ScriptContext>() : NULL)->call(m_unk0x14, arguments);
		}

		if (results->size() > 0) {
			FUN_004132b0(results->front(), result);
		}

		return S_OK;
	}
	catch (std::exception& e) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_WARNING, "CLuaFunction::Call failed, %s", e.what());
		return LogManager::ReportExceptionAsCOMError(CLSID_LuaFunction, e);
	}
}
