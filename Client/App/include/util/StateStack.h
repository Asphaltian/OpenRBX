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

// clang-format off
// STUB: ROBLOX 0x00411e90
// ?pushState@?$StateStack@VXmlState@RBX@@@RBX@@QAEXV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PAVXmlState@2@1@Z
// clang-format on

template <class T>
void StateStack<T>::pushState(std::string name, T* undoState, T* redoState)
{
	clearStackAfter(currentState);
	StateEntry entry = {name, undoState, redoState};
	stack.push_back(entry);
	currentState++;
	while (isStackTooBig()) {
		delete stack.front().redoState;
		delete stack.front().undoState;
		stack.erase(stack.begin());
		currentState--;
	}
}

// TEMPLATE: ROBLOX 0x00455280
// RBX::StateStack<RBX::XmlState>::canUndo

template <class T>
bool StateStack<T>::canUndo(std::string& name) const
{
	if (currentState >= 0) {
		name = stack[currentState].name;
		return true;
	}
	return false;
}

template <class T>
inline T* StateStack<T>::undo()
{
	return stack[currentState--].undoState;
}

// TEMPLATE: ROBLOX 0x004552e0
// RBX::StateStack<RBX::XmlState>::canRedo

template <class T>
bool StateStack<T>::canRedo(std::string& name) const
{
	if (currentState + 1 < (int) stack.size()) {
		name = stack[currentState + 1].name;
		return true;
	}
	return false;
}

template <class T>
inline T* StateStack<T>::redo()
{
	return stack[++currentState].redoState;
}

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
