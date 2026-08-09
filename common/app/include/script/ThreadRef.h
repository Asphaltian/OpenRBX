#ifndef SCRIPT_THREADREF_H
#define SCRIPT_THREADREF_H

#include "decomp.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>

struct lua_State;

namespace RBX {
namespace Lua {

// VTABLE: WEBSERVICE 0x1022f964
// SIZE 0x20
class ThreadRef
{
public:
	// SIZE 0xc
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

		ThreadRef* first;                           // 0x00
		const boost::shared_ptr<boost::mutex> sync; // 0x04
	};

	ThreadRef();
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
	virtual void removeRef(); // vtable+0x00

private:
	friend class Node;

	void addRef();
	void addToNode();
	void removeFromNode();

	static boost::shared_ptr<boost::mutex> syncSingleton;

	const boost::shared_ptr<boost::mutex> sync; // 0x04
	Node* node;                                 // 0x0c
	ThreadRef* previous;                        // 0x10
	ThreadRef* next;                            // 0x14
	lua_State* L;                               // 0x18
	int threadId;                               // 0x1c
};

DECOMP_SIZE_ASSERT(ThreadRef, 0x20)
DECOMP_SIZE_ASSERT(ThreadRef::Node, 0xc)

// VTABLE: WEBSERVICE 0x10237ddc
// SIZE 0x24
class FunctionRef : public ThreadRef
{
public:
	FunctionRef();
	FunctionRef(const FunctionRef& other);
	FunctionRef(lua_State* L, int index);

	~FunctionRef();

protected:
	virtual void removeRef(); // vtable+0x00

private:
	friend void lua_pushfunction(lua_State* L, const FunctionRef& ref);

	int functionId; // 0x20
};

DECOMP_SIZE_ASSERT(FunctionRef, 0x24)

void lua_pushfunction(lua_State* L, const FunctionRef& ref);

FunctionRef lua_tofunction(lua_State* L, int index);

} // namespace Lua
} // namespace RBX

#endif // SCRIPT_THREADREF_H
