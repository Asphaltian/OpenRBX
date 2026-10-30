#ifndef SCRIPT_LUAARGUMENTS_H
#define SCRIPT_LUAARGUMENTS_H

#include "decomp.h"
#include "reflection/function.h"
#include "reflection/type.h"
#include "script/LuaAtomicClasses.h"
#include "script/LuaInstanceBridge.h"
#include "script/ThreadRef.h"
#include "util/ContentProvider.h"

#include <lua.h>
#include <string>
#include <vector>

namespace RBX {
namespace Lua {

// SIZE 0x14
class LuaArguments : public Reflection::FunctionDescriptor::Arguments
{
	const int offset;   // 0x0c
	lua_State* const L; // 0x10

public:
	LuaArguments(lua_State* L, int offset) : offset(offset), L(L) {}

	// FUNCTION: WEBSERVICE 0x100ab2d0
	// FUNCTION: ROBLOX 0x005fc760
	virtual unsigned int size() const { return lua_gettop(L) - 1; }

	// STUB: WEBSERVICE 0x10064ef0
	// STUB: ROBLOX 0x00572030
	static bool get(lua_State* L, int luaIndex, Reflection::Value& value)
	{
		if (luaIndex > lua_gettop(L)) {
			return false;
		}

		switch (lua_type(L, luaIndex)) {
		case LUA_TNUMBER:
			value = lua_tonumber(L, luaIndex);
			break;

		case LUA_TBOOLEAN:
			value = lua_toboolean(L, luaIndex) != 0;
			break;

		case LUA_TSTRING:
			value = std::string(lua_tostring(L, luaIndex));
			break;

		case LUA_TTABLE: {
			const int n = lua_objlen(L, luaIndex);

			value = std::vector<Reflection::Value>(n);

			std::vector<Reflection::Value>& values = value.cast<std::vector<Reflection::Value>&>();

			for (int i = 1; i <= n; i++) {
				Reflection::Value& item = values[i - 1];
				lua_rawgeti(L, luaIndex, i);
				get(L, -1, item);
				lua_pop(L, 1);
			}
			break;
		}

		case LUA_TNIL:
			value = Reflection::Value();
			break;

		case LUA_TFUNCTION:
			value = lua_tofunction(L, luaIndex);
			break;

		default:
			value = Reflection::Value();
			break;

		case LUA_TUSERDATA:
			if (Bridge<G3D::CoordinateFrame, 1>::getValue(L, luaIndex, value)) {
				break;
			}
			if (Bridge<G3D::Vector3, 1>::getValue(L, luaIndex, value)) {
				break;
			}
			if (Bridge<G3D::Color3, 1>::getValue(L, luaIndex, value)) {
				break;
			}
			if (Bridge<RBX::BrickColor, 1>::getValue(L, luaIndex, value)) {
				break;
			}
			if (SharedPtrBridge<Reflection::DescribedBase>::getPtr(L, luaIndex, value)) {
				break;
			}

			value = Reflection::Value();
			break;
		}

		return true;
	}

	// FUNCTION: WEBSERVICE 0x100ad2c0
	// FUNCTION: ROBLOX 0x005fdda0
	virtual void get(int index, Reflection::Value& value) const { get(L, offset + index, value); }

	template <class Iterator>
	static int pushTable(Iterator _First, Iterator _Last, lua_State* const L)
	{
		lua_createtable(L, _Last - _First, 0);

		int index = 0;
		for (; _First != _Last; ++_First) {
			push(*_First, L);
			lua_rawseti(L, -2, ++index);
		}

		return 1;
	}

	int push(const Reflection::Value& value) const { return push(value, L); }

	// FUNCTION: WEBSERVICE 0x10062a40
	// FUNCTION: ROBLOX 0x0056fe60
	static int push(const Reflection::Value& value, lua_State* const L)
	{
		if (value.isType<void>()) {
			return 0;
		}

		if (value.isType<int>()) {
			lua_pushinteger(L, value.cast<int>());
			return 1;
		}

		if (value.isType<bool>()) {
			lua_pushboolean(L, value.cast<bool>());
			return 1;
		}

		if (value.isType<float>()) {
			lua_pushnumber(L, value.cast<float>());
			return 1;
		}

		if (value.isType<double>()) {
			lua_pushnumber(L, value.cast<double>());
			return 1;
		}

		if (value.isType<FunctionRef>()) {
			lua_pushfunction(L, value.cast<const FunctionRef&>());
			return 1;
		}

		if (value.isType<std::string>()) {
			lua_pushstring(L, value.cast<const std::string&>().c_str());
			return 1;
		}

		if (value.isType<G3D::Vector3>()) {
			Vector3Bridge::pushVector3(L, value.cast<const G3D::Vector3&>());
			return 1;
		}

		if (value.isType<G3D::CoordinateFrame>()) {
			CoordinateFrameBridge::pushCoordinateFrame(L, value.cast<const G3D::CoordinateFrame&>());
			return 1;
		}

		if (value.isType<G3D::Color3>()) {
			Color3Bridge::pushColor3(L, value.cast<const G3D::Color3&>());
			return 1;
		}

		if (value.isType<RBX::BrickColor>()) {
			Bridge<RBX::BrickColor, 1>::pushNewObject(L, value.cast<const RBX::BrickColor&>());
			return 1;
		}

		if (value.isType<ContentId>()) {
			lua_pushstring(L, value.cast<const ContentId&>().c_str());
			return 1;
		}

		if (value.isType<boost::shared_ptr<Instance> >()) {
			SharedPtrBridge<Reflection::DescribedBase>::push(L, value.cast<const boost::shared_ptr<Instance>&>());
			return 1;
		}

		if (value.isType<boost::shared_ptr<Reflection::DescribedBase> >()) {
			SharedPtrBridge<Reflection::DescribedBase>::push(
				L, value.cast<const boost::shared_ptr<Reflection::DescribedBase>&>()
			);
			return 1;
		}

		if (value.isType<std::vector<Reflection::Value> >()) {
			const std::vector<Reflection::Value>& values = value.cast<const std::vector<Reflection::Value>&>();
			return pushTable(values.begin(), values.end(), L);
		}

		if (value.isType<boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > >()) {
			boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > instances =
				value.cast<boost::shared_ptr<const std::vector<boost::shared_ptr<Instance> > > >();
			if (instances) {
				return pushTable(instances->begin(), instances->end(), L);
			}

			lua_newtable(L);
			return 1;
		}

		return 0;
	}

	// clang-format off
	// TEMPLATE: WEBSERVICE 0x10060490
	// TEMPLATE: ROBLOX 0x0056d740
	// ??$pushTable@V?$_Vector_const_iterator@VValue@Reflection@RBX@@V?$allocator@VValue@Reflection@RBX@@@std@@@std@@@LuaArguments@Lua@RBX@@SAHV?$_Vector_const_iterator@VValue@Reflection@RBX@@V?$allocator@VValue@Reflection@RBX@@@std@@@std@@0QAUlua_State@@@Z
	// TEMPLATE: WEBSERVICE 0x100632c0
	// TEMPLATE: ROBLOX 0x00570720
	// ??$pushTable@V?$_Vector_const_iterator@V?$shared_ptr@VInstance@RBX@@@boost@@V?$allocator@V?$shared_ptr@VInstance@RBX@@@boost@@@std@@@std@@@LuaArguments@Lua@RBX@@SAHV?$_Vector_const_iterator@V?$shared_ptr@VInstance@RBX@@@boost@@V?$allocator@V?$shared_ptr@VInstance@RBX@@@boost@@@std@@@std@@0QAUlua_State@@@Z
	// clang-format on

	int pushReturnValue() const { return push(returnValue); }
};

DECOMP_SIZE_ASSERT(LuaArguments, 0x14)

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_LUAARGUMENTS_H
