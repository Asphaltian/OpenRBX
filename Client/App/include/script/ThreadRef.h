#ifndef SCRIPT_THREADREF_H
#define SCRIPT_THREADREF_H

#include "lua/LuaBridge.h"
#include "placeholder.h"
#include "reflection/type.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

struct lua_State;

namespace RBX {
namespace Lua {

class ThreadRef
{
public:
	class Node
	{
	public:
		static boost::shared_ptr<Node> create(lua_State* L);
		static boost::shared_ptr<Node> get(lua_State* L);

		void eraseAllRefs();

		~Node();

	private:
		friend class ThreadRef;

		Node() : first(NULL), sync(syncSingleton) {}

		ThreadRef* first;
		const boost::shared_ptr<boost::mutex> sync;
	};

	ThreadRef() : sync(syncSingleton), node(NULL), previous(NULL), next(NULL), L(NULL), threadId(0) {}

	ThreadRef(const ThreadRef& other);
	ThreadRef(lua_State* L);

	~ThreadRef();

	ThreadRef& operator=(const ThreadRef& other);

	bool operator==(const ThreadRef& other) const;
	bool operator!=(const ThreadRef& other) const;

	void reset();

	bool empty() const;

	lua_State* thread() const { return L; }

protected:
	virtual void removeRef();

private:
	friend class Node;

	void addRef();
	void addToNode();
	void removeFromNode();

	static boost::shared_ptr<boost::mutex> syncSingleton;

	const boost::shared_ptr<boost::mutex> sync;
	Node* node;
	ThreadRef* previous;
	ThreadRef* next;
	lua_State* L;
	int threadId;
};

class FunctionRef : public ThreadRef
{
public:
	FunctionRef() : functionId(0) {}
	FunctionRef(const FunctionRef& other);
	FunctionRef(lua_State* L, int index);

	~FunctionRef();

	FunctionRef& operator=(const FunctionRef& other);
	bool operator==(const FunctionRef& other) const;
	bool operator!=(const FunctionRef& other) const;

protected:
	virtual void removeRef();

private:
	friend void lua_pushfunction(lua_State* L, const FunctionRef& ref);

	int functionId;
};

void lua_pushfunction(lua_State* L, const FunctionRef& ref);

template <>
int Bridge<boost::shared_ptr<ThreadRef::Node>, 1>::on_index(
	const boost::shared_ptr<ThreadRef::Node>& value, const char* name, lua_State* L
);

template <>
void Bridge<boost::shared_ptr<ThreadRef::Node>, 1>::on_newindex(
	boost::shared_ptr<ThreadRef::Node>& value, const char* name, lua_State* L
);

template <>
inline int Bridge<boost::shared_ptr<ThreadRef::Node>, 1>::on_tostring(
	const boost::shared_ptr<ThreadRef::Node>& value, lua_State* L
)
{
	lua_pushstring(L, "ThreadRef");
	return 1;
}

FunctionRef lua_tofunction(lua_State* L, int index);

} // namespace Lua

namespace Reflection {

template <>
Lua::FunctionRef& Value::convert<Lua::FunctionRef>();

} // namespace Reflection
} // namespace RBX

#endif // SCRIPT_THREADREF_H
