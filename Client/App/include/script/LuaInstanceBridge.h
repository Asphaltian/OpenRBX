#ifndef SCRIPT_LUAINSTANCEBRIDGE_H
#define SCRIPT_LUAINSTANCEBRIDGE_H

#include "decomp.h"
#include "lua/LuaBridge.h"
#include "reflection/reflection.h"
#include "v8tree/Instance.h"

#include <boost/shared_ptr.hpp>

struct lua_State;

namespace RBX {

namespace Lua {

// FUNCTION: WEBSERVICE 0x10064440
template <>
inline int Bridge<boost::shared_ptr<Reflection::DescribedBase>, 0>::on_tostring(
	const boost::shared_ptr<Reflection::DescribedBase>& value,
	lua_State* L
)
{
	if (Instance* instance = dynamic_cast<Instance*>(value.get())) {
		lua_pushstring(L, instance->getName().c_str());
	}
	else {
		lua_pushstring(L, Reflection::ClassDescriptor::rootDescriptor().name.toString().c_str());
	}
	return 1;
}

template <>
int Bridge<boost::shared_ptr<Reflection::DescribedBase>, 0>::on_index(
	const boost::shared_ptr<Reflection::DescribedBase>& value,
	const char* name,
	lua_State* L
);

template <>
void Bridge<boost::shared_ptr<Reflection::DescribedBase>, 0>::on_newindex(
	boost::shared_ptr<Reflection::DescribedBase>& value,
	const char* name,
	lua_State* L
);

void newweaktable(lua_State* L, const char* mode);

template <class T>
void SharedPtrBridge<T>::registerClassLibrary(lua_State* L)
{
	lua_pushlightuserdata(L, &push);
	newweaktable(L, "v");
	lua_rawset(L, LUA_REGISTRYINDEX);
}

// SIZE 0x1
class ObjectBridge : public SharedPtrBridge<Reflection::DescribedBase>
{
public:
	static int callMemberFunction(lua_State* L);

	static void registerInstanceClassLibrary(lua_State* L)
	{
		luaL_register(L, "Instance", classLibrary);
		lua_pop(L, 1);
	}

	static int newInstance(lua_State* L);

	static boost::shared_ptr<Instance> getInstance(lua_State* L, unsigned int index);

private:
	static const luaL_Reg classLibrary[];
};

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_LUAINSTANCEBRIDGE_H
