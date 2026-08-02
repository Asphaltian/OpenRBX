#ifndef UTIL_ASSOCIATION_H
#define UTIL_ASSOCIATION_H

#include <vector>

namespace RBX {

template <class T>
// SIZE 0x10
class Association
{
public:
	class Item;

private:
	std::vector<Item*> items; // 0x00
};

} // namespace RBX

#endif // UTIL_ASSOCIATION_H
