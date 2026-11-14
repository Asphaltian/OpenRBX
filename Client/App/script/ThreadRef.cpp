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

template <>
void Bridge<boost::shared_ptr<ThreadRef::Node>, 1>::on_newindex(
	boost::shared_ptr<ThreadRef::Node>& value, const char* name, lua_State* L
)
{
	throw std::runtime_error(G3D::format("%s cannot be assigned to", name));
}

template <>
int Bridge<boost::shared_ptr<ThreadRef::Node>, 1>::on_index(
	const boost::shared_ptr<ThreadRef::Node>& value, const char* name, lua_State* L
)
{
	throw std::runtime_error(G3D::format("%s is not a valid member", name));
}

void lua_pushfunction(lua_State* L, const FunctionRef& function)
{
	lua_rawgeti(L, LUA_REGISTRYINDEX, function.functionId);
}

void FunctionRef::removeRef()
{
	if (functionId != 0 && thread() != NULL) {
		luaL_unref(thread(), LUA_REGISTRYINDEX, functionId);

		functionId = 0;
	}

	ThreadRef::removeRef();
}

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

template <>
const Type& Type::singleton<Lua::FunctionRef>()
{
	static Type type("Function", typeid(Lua::FunctionRef));

	return type;
}

} // namespace Reflection

namespace Lua {

ThreadRef::ThreadRef(lua_State* L) : sync(syncSingleton), node(NULL), previous(NULL), next(NULL), L(L), threadId(0)
{
	boost::mutex::scoped_lock lock(*sync);

	node = Node::get(L).get();

	addToNode();

	addRef();
}

ThreadRef::ThreadRef(const ThreadRef& other) : sync(syncSingleton), node(other.node), L(other.L)
{
	boost::mutex::scoped_lock lock(*sync);

	addRef();

	addToNode();
}

void ThreadRef::reset()
{
	boost::mutex::scoped_lock lock(*sync);

	removeFromNode();

	removeRef();
}

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

ThreadRef::Node::~Node()
{
	eraseAllRefs();
}

FunctionRef& FunctionRef::operator=(const FunctionRef& other)
{
	if (thread() != NULL) {
		luaL_unref(thread(), LUA_REGISTRYINDEX, functionId);
	}

	ThreadRef::operator=(other);

	if (thread() == NULL) {
		functionId = 0;
	}
	else {
		lua_rawgeti(thread(), LUA_REGISTRYINDEX, other.functionId);

		functionId = luaL_ref(thread(), LUA_REGISTRYINDEX);
	}

	return *this;
}

ThreadRef::~ThreadRef()
{
	reset();
}

FunctionRef::FunctionRef(lua_State* L, int index) : ThreadRef(L)
{
	lua_pushvalue(L, index);

	functionId = luaL_ref(L, LUA_REGISTRYINDEX);
}

FunctionRef::~FunctionRef()
{
	if (functionId != 0 && thread() != NULL) {
		luaL_unref(thread(), LUA_REGISTRYINDEX, functionId);
	}
}

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

FunctionRef lua_tofunction(lua_State* L, int index)
{
	return FunctionRef(L, index);
}

} // namespace Lua

namespace Reflection {

template <>
Lua::FunctionRef& Value::convert<Lua::FunctionRef>()
{
	if (isType<void>()) {
		value = Lua::FunctionRef();
		_type = &Type::singleton<Lua::FunctionRef>();
	}

	if (Lua::FunctionRef* function = boost::any_cast<Lua::FunctionRef>(&value)) {
		return *function;
	}

	throw std::runtime_error("Unable to cast value to RBX::Lua::FunctionRef");
}

} // namespace Reflection

namespace Lua {

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
