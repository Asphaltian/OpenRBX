#include "script/ThreadRef.h"

#include "lua/LuaBridge.h"
#include "reflection/type.h"

#include <lauxlib.h>
#include <lua.h>

namespace RBX {
namespace Lua {

boost::shared_ptr<boost::mutex> ThreadRef::syncSingleton(new boost::mutex());

template <>
const char* Bridge<boost::shared_ptr<ThreadRef::Node>, 1>::className = "shared_ptr<ThreadRef::Node>";

void ThreadRef::addRef()
{
	if (L != NULL) {
		lua_pushthread(L);

		threadId = luaL_ref(L, LUA_REGISTRYINDEX);
	}
}

void ThreadRef::addToNode()
{
	if (node != NULL) {
		ThreadRef* first = node->first;

		if (first != NULL) {
			next = first;
			first->previous = this;
		}
		else {
			next = NULL;
		}

		previous = NULL;
		node->first = this;
	}
}

// FUNCTION: WEBSERVICE 0x100b1340
void ThreadRef::removeRef()
{
	if (L != NULL) {
		luaL_unref(L, LUA_REGISTRYINDEX, threadId);

		threadId = 0;
		L = NULL;
	}
}

void ThreadRef::removeFromNode()
{
	if (node != NULL) {
		if (next != NULL) {
			next->previous = previous;
		}

		if (previous != NULL) {
			previous->next = next;
		}

		if (node->first == this) {
			node->first = next;
		}

		next = NULL;
		previous = NULL;
		node = NULL;
	}
}

// FUNCTION: WEBSERVICE 0x100adb90 FOLDED
// FUNCTION: ROBLOX 0x005a4b00 FOLDED
template <>
void Bridge<boost::shared_ptr<ThreadRef::Node>, 1>::on_newindex(
	boost::shared_ptr<ThreadRef::Node>& value, const char* name, lua_State* L
)
{
	throw std::runtime_error(G3D::format("%s cannot be assigned to", name));
}

// FUNCTION: WEBSERVICE 0x100b1370
// FUNCTION: ROBLOX 0x005a4aa0
template <>
int Bridge<boost::shared_ptr<ThreadRef::Node>, 1>::on_index(
	const boost::shared_ptr<ThreadRef::Node>& value, const char* name, lua_State* L
)
{
	throw std::runtime_error(G3D::format("%s is not a valid member", name));
}

// FUNCTION: WEBSERVICE 0x100b13d0
// FUNCTION: ROBLOX 0x005a4b60
void lua_pushfunction(lua_State* L, const FunctionRef& function)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, function.functionId);
}

// FUNCTION: WEBSERVICE 0x100b13f0
// FUNCTION: ROBLOX 0x005a4b80
void FunctionRef::removeRef()
{
	if (functionId != 0 && thread() != NULL) {
		luaL_unref(thread(), LUA_REGISTRYINDEX, functionId);

		functionId = 0;
	}

	ThreadRef::removeRef();
}

// FUNCTION: WEBSERVICE 0x100b1440
// FUNCTION: ROBLOX 0x005a4bd0
boost::shared_ptr<ThreadRef::Node> ThreadRef::Node::get(lua_State* thread)
{
	lua_pushlightuserdata(thread, (void*) 0x4e);
	lua_gettable(thread, LUA_GLOBALSINDEX);

	boost::shared_ptr<Node>& node = Bridge<boost::shared_ptr<Node>, 1>::getObject(thread, lua_gettop(thread));

	lua_settop(thread, -2);

	return node;
}

} // namespace Lua

namespace Reflection {

// FUNCTION: WEBSERVICE 0x100b14b0
// FUNCTION: ROBLOX 0x005a4c40
template <>
const Type& Type::singleton<Lua::FunctionRef>()
{
	static Type type("Function", typeid(Lua::FunctionRef));

	return type;
}

} // namespace Reflection

namespace Lua {

// FUNCTION: WEBSERVICE 0x100b15f0
// FUNCTION: ROBLOX 0x005a4d60
ThreadRef::ThreadRef(lua_State* L) : sync(syncSingleton), node(NULL), previous(NULL), next(NULL), L(L), threadId(0)
{
	boost::mutex::scoped_lock lock(*sync);

	node = Node::get(L).get();

	addToNode();

	addRef();
}

// FUNCTION: WEBSERVICE 0x100b1720
// FUNCTION: ROBLOX 0x005a4e90
ThreadRef::ThreadRef(const ThreadRef& other) : sync(syncSingleton), node(other.node), L(other.L)
{
	boost::mutex::scoped_lock lock(*sync);

	addRef();

	addToNode();
}

// FUNCTION: WEBSERVICE 0x100b1810
// FUNCTION: ROBLOX 0x005a4f80
void ThreadRef::reset()
{
	boost::mutex::scoped_lock lock(*sync);

	removeFromNode();

	removeRef();
}

// FUNCTION: WEBSERVICE 0x100b18b0
// FUNCTION: ROBLOX 0x005a5020
ThreadRef& ThreadRef::operator=(const ThreadRef& other)
{
	if (L != other.L) {
		boost::mutex::scoped_lock lock(*sync);

		removeRef();

		L = other.L;

		addRef();
	}

	if (node != other.node) {
		boost::mutex::scoped_lock lock(*sync);

		removeFromNode();

		node = other.node;

		addToNode();
	}

	return *this;
}

// FUNCTION: WEBSERVICE 0x100b19c0
// FUNCTION: ROBLOX 0x005a5130
void ThreadRef::Node::eraseAllRefs()
{
	boost::mutex::scoped_lock lock(*sync);

	ThreadRef* ref = first;

	while (ref != NULL) {
		ref->removeRef();
		ref->node = NULL;

		ref = ref->next;
	}

	first = NULL;
}

// FUNCTION: WEBSERVICE 0x100b1a40
// FUNCTION: ROBLOX 0x005a51b0
ThreadRef::Node::~Node()
{
	eraseAllRefs();
}

// FUNCTION: WEBSERVICE 0x100b1ac0
// FUNCTION: ROBLOX 0x005a52a0
ThreadRef::~ThreadRef()
{
	reset();
}

// FUNCTION: WEBSERVICE 0x100b1b40
// FUNCTION: ROBLOX 0x005a5320
FunctionRef::FunctionRef(lua_State* L, int index) : ThreadRef(L)
{
	lua_pushvalue(L, index);

	functionId = luaL_ref(L, LUA_REGISTRYINDEX);
}

// FUNCTION: WEBSERVICE 0x100b1bb0
// FUNCTION: ROBLOX 0x005a5390
FunctionRef::~FunctionRef()
{
	if (functionId != 0 && thread() != NULL) {
		luaL_unref(thread(), LUA_REGISTRYINDEX, functionId);
	}
}

// FUNCTION: WEBSERVICE 0x100b1c20
// FUNCTION: ROBLOX 0x005a5400
FunctionRef::FunctionRef(const FunctionRef& other) : ThreadRef(other)
{
	if (thread() == NULL) {
		functionId = 0;
	}
	else {
		lua_rawgeti(thread(), LUA_REGISTRYINDEX, other.functionId);

		functionId = luaL_ref(thread(), LUA_REGISTRYINDEX);
	}
}

// FUNCTION: WEBSERVICE 0x100b1cd0
// FUNCTION: ROBLOX 0x005a54b0
FunctionRef lua_tofunction(lua_State* L, int index)
{
	return FunctionRef(L, index);
}

// FUNCTION: WEBSERVICE 0x100b1db0
// FUNCTION: ROBLOX 0x005a5690
boost::shared_ptr<ThreadRef::Node> ThreadRef::Node::create(lua_State* thread)
{
	lua_pushlightuserdata(thread, (void*) 0x4e);

	boost::shared_ptr<Node>& node = Bridge<boost::shared_ptr<Node>, 1>::pushNewObject(thread);

	lua_settable(thread, LUA_GLOBALSINDEX);

	node = boost::shared_ptr<Node>(new Node());

	return node;
}

} // namespace Lua
} // namespace RBX
