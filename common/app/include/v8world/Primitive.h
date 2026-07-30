#ifndef V8WORLD_PRIMITIVE_H
#define V8WORLD_PRIMITIVE_H

#include "util/Guid.h"
#include "v8world/Geometry.h"
#include "v8world/IPipelined.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>

namespace RBX {

class Assembly;
class Primitive;
class Body;
class Velocity;
class Clump;
class Controller;
class PartInstance;
class SpatialNode;
class World;

using G3D::CoordinateFrame;
using G3D::Vector3;

namespace Sim {

enum EdgeState
{
	UNDEFINED = 0,
	STEPPING = 1,
	SLEEPING = 2,
	TOUCHING = 3,
	TOUCHING_SLEEPING = 4,
};

} // namespace Sim

class Edge : public IPipelined
{
public:
	enum EdgeType
	{
		JOINT = 0,
		CONTACT = 1,
	};

	Edge(Primitive* prim0, Primitive* prim1);

	virtual EdgeType getEdgeType() const = 0;
	virtual void setPrimitive(int index, Primitive* primitive);

	Edge* getNext(const Primitive* primitive) const { return primitive == prim0 ? next0 : next1; }

	Primitive* getPrimitive(int index) const { return index == 0 ? prim0 : prim1; }

private:
	Sim::EdgeState edgeState; // 0x08
	Primitive* prim0;         // 0x0c
	Primitive* prim1;         // 0x10
	Edge* next0;              // 0x14
	Edge* next1;              // 0x18
	bool inEdgeList;          // 0x1c
};

class EdgeList
{
public:
	Edge* first; // 0x00
	int num;     // 0x04
};

class Primitive : public IPipelined
{
public:
	const Guid& getGuid() const;

	static Primitive* downstreamPrimitive(Edge* edge);

	void setClump(Clump* value);

	void setSizeMultiplier(unsigned int multiplier);
	Edge* getFirstEdge() const;
	Edge* getNextEdge(Edge* edge) const;
	Edge* getFirstContact() const;
	// FUNCTION: WEBSERVICE 0x100a8240
	float getRadius() const { return geometry->getRadius(); }

private:
	Guid guid;                    // 0x08
	bool guidSetExternally;       // 0x10
	unsigned int sizeMultiplier;  // 0x14
	EdgeList contacts;            // 0x18
	EdgeList joints;              // 0x20
	int worldIndex;               // 0x28
	World* world;                 // 0x2c
	Clump* clump;                 // 0x30
	int clumpDepth;               // 0x34
	int traverseId;               // 0x38
	SpatialNode* spatialNodes;    // 0x3c
	Vector3 oldSpatialMin;        // 0x40
	Vector3 oldSpatialMax;        // 0x4c
	CoordinateFrame fuzzyExtents; // 0x58
	int fuzzyExtentsStateId;      // 0x70
	Geometry* geometry;           // 0x74
	Body* body;                   // 0x78
	PartInstance* myOwner;        // 0x7c
	void* anchorObject;           // 0x80
	bool dragging;                // 0x84
	bool anchored;                // 0x85
	bool canCollide;              // 0x86
	bool canSleep;                // 0x87
	float friction;               // 0x88
	float elasticity;             // 0x8c
	int surfaceType[6];           // 0x90
	int surfaceData[6];           // 0xa8
	Controller* controller;       // 0xc0
	float jointK;                 // 0xc4
};

} // namespace RBX

#endif // V8WORLD_PRIMITIVE_H
