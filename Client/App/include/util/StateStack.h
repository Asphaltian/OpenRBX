#ifndef UTIL_STATESTACK_H
#define UTIL_STATESTACK_H

#include "decomp.h"

#include <string>
#include <vector>

namespace RBX {

class Name;

// SIZE 0x18
template <class T>
class StateStack
{
public:
	// SIZE 0x24
	struct StateEntry
	{
		std::string name; // 0x00
		T* undoState;     // 0x1c
		T* redoState;     // 0x20
	};

private:
	int currentState;              // 0x04
	std::vector<StateEntry> stack; // 0x08

public:
	StateStack() { currentState = -1; }

	~StateStack() { clearStack(); }

	void pushState(const Name&, T*, T*);
	void pushState(std::string, T*, T*);

	void clearStack()
	{
		clearStackAfter(-1);
		currentState = -1;
	}

	bool canUndo(std::string&) const;
	T* undo();
	bool canRedo(std::string&) const;
	T* redo();

protected:
	unsigned int getStackSize() const { return stack.size(); }

	virtual bool isStackTooBig() const { return false; } // vtable+0x00

private:
	void clearStackAfter(int index);
};

template <class T>
inline void StateStack<T>::clearStackAfter(int index)
{
	for (int i = (int) stack.size() - 1; i > index; --i) {
		delete stack[i].redoState;
		delete stack[i].undoState;
	}

	stack.resize(index + 1);
}

} // namespace RBX

#endif // UTIL_STATESTACK_H
