#ifndef V8WORLD_CLUMP2_H
#define V8WORLD_CLUMP2_H

#include "decomp.h"

#include <cstddef>

namespace RBX {

class Edge;
class Joint;
class Primitive;

enum SearchType
{
	IN_CLUMP = 0,
	IN_ASSEMBLY = 1,
};

// SIZE 0x08
class PrimIterator
{
private:
	static bool isParent(Primitive* parentCandidate, Primitive* child, Joint* joint, SearchType searchType);
	static Primitive* findFirstChild(Primitive* primitive, SearchType searchType);
	static Primitive* findNextSibling(Primitive* parent, Primitive* child, SearchType searchType);
	static Primitive* findNextRelative(Primitive* parent, Primitive* child, SearchType searchType);

public:
	PrimIterator(Primitive* primitive, SearchType searchType) : primitive(primitive), searchType(searchType) {}

	Primitive* operator*() const { return primitive; }

	bool operator==(const PrimIterator& other) const { return primitive == other.primitive; }
	bool operator!=(const PrimIterator& other) const { return primitive != other.primitive; }

	PrimIterator& operator++();

	static Primitive* findParent(Primitive* primitive, SearchType searchType);

private:
	Primitive* primitive;  // 0x00
	SearchType searchType; // 0x04
};

DECOMP_SIZE_ASSERT(PrimIterator, 0x08)

// SIZE 0x08
class EdgeIterator
{
public:
	static EdgeIterator begin(Primitive* primitive);

	EdgeIterator() : primitive(NULL), edge(NULL) {}

	EdgeIterator(Primitive* primitive, Edge* edge) : primitive(primitive), edge(edge) {}

	static EdgeIterator& end();

	EdgeIterator& operator++();

	Edge* operator*() const { return edge; }

	bool operator==(const EdgeIterator& other) const { return edge == other.edge && primitive == other.primitive; }

	bool operator!=(const EdgeIterator& other) const { return !(*this == other); }

private:
	static Edge* getNextExternalUtil(Primitive* primitive, Edge* edge);

	void findEdgeOnNextPrimitive();

	Primitive* primitive; // 0x00
	Edge* edge;           // 0x04
};

DECOMP_SIZE_ASSERT(EdgeIterator, 0x08)

inline EdgeIterator& EdgeIterator::end()
{
	static EdgeIterator temp;
	return temp;
}

} // namespace RBX

#endif // V8WORLD_CLUMP2_H
