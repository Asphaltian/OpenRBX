#ifndef UTIL_STATESTACK_H
#define UTIL_STATESTACK_H

#include "placeholder.h"
#include "util/Name.h"

#include <string>
#include <vector>

namespace RBX {

template <class T>
class StateStack
{
public:
	struct StateEntry
	{
		std::string name;
		T* undoState;
		T* redoState;
	};

private:
	int currentState;
	std::vector<StateEntry> stack;

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

	virtual bool isStackTooBig() const { return false; }

private:
	void clearStackAfter(int index);
};

template <class T>
inline void StateStack<T>::pushState(const Name& name, T* undoState, T* redoState)
{
	pushState(name.toString(), undoState, redoState);
}

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
