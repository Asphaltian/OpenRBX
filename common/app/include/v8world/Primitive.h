#ifndef V8WORLD_PRIMITIVE_H
#define V8WORLD_PRIMITIVE_H

#include "decomp.h"
#include "util/ComputeProp.h"
#include "util/Extents.h"
#include "util/Face.h"
#include "util/Guid.h"
#include "util/NormalId.h"
#include "util/Vector3int32.h"
#include "v8world/Geometry.h"
#include "v8world/IPipelined.h"
#include "v8world/SurfaceData.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/Ray.h>
#include <G3D/Vector3.h>
#include <cstddef>

namespace RBX {

class Anchor;
class Assembly;
class Body;
class Clump;
class Contact;
class Controller;
class IMoving;
class Joint;
class Primitive;
class RigidJoint;
class SpatialNode;
class Velocity;
class World;

using G3D::CoordinateFrame;
using G3D::Ray;
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

// SIZE 0x20
class __declspec(novtable) Edge : public IPipelined
{
public:
	enum EdgeType
	{
		JOINT = 0,
		CONTACT = 1,
	};

	Edge(Primitive* prim0, Primitive* prim1);

	virtual EdgeType getEdgeType() const = 0;                   // vtable+0x0c
	virtual void setPrimitive(int index, Primitive* primitive); // vtable+0x10

	Edge* getNext(const Primitive* primitive) const { return primitive == prim0 ? next0 : next1; }

	void setNext(Primitive* primitive, Edge* edge)
	{
		if (primitive == prim0) {
			next0 = edge;
		}
		else {
			next1 = edge;
		}
	}

	Primitive* getPrimitive(int index) const { return (&prim0)[index]; }

	Primitive* otherPrimitive(const Primitive* primitive) const { return primitive == prim0 ? prim1 : prim0; }

	bool getInEdgeList() const { return inEdgeList; }
	void setInEdgeList(bool value) { inEdgeList = value; }

	Sim::EdgeState getEdgeState() const { return edgeState; }
	void setEdgeState(Sim::EdgeState value) { edgeState = value; }

private:
	Sim::EdgeState edgeState; // 0x08
	Primitive* prim0;         // 0x0c
	Primitive* prim1;         // 0x10
	Edge* next0;              // 0x14
	Edge* next1;              // 0x18
	bool inEdgeList;          // 0x1c
};

DECOMP_SIZE_ASSERT(Edge, 0x20)

// SIZE 0x08
class EdgeList
{
public:
	static void insertEdge(Primitive* primitive, Edge* edge, EdgeList& list);
	static void removeEdge(Primitive* primitive, Edge* edge, EdgeList& list);

	EdgeList() : first(NULL), num(0) {}

	Edge* first; // 0x00
	int num;     // 0x04
};

DECOMP_SIZE_ASSERT(EdgeList, 0x08)

// VTABLE: WEBSERVICE 0x102376a4
// SIZE 0xd4
class Primitive : public IPipelined
{
public:
	Primitive(Geometry::GeometryType geometryType);

	static bool disableSleep;

private:
	friend class SpatialHash;

	static bool ignoreBool;

	Guid guid;                   // 0x08
	bool guidSetExternally;      // 0x10
	unsigned int sizeMultiplier; // 0x14
	EdgeList contacts;           // 0x18
	EdgeList joints;             // 0x20
	int worldIndex;              // 0x28
	World* world;                // 0x2c
	Clump* clump;                // 0x30
	int clumpDepth;              // 0x34
	int traverseId;              // 0x38

	SpatialNode* spatialNodes;       // 0x3c
	Vector3int32 oldSpatialMin;      // 0x40
	Vector3int32 oldSpatialMax;      // 0x4c
	mutable Extents fuzzyExtents;    // 0x58
	mutable int fuzzyExtentsStateId; // 0x70

protected:
	Geometry* geometry;          // 0x74
	Body* body;                  // 0x78
	IMoving* myOwner;            // 0x7c
	Anchor* anchorObject;        // 0x80
	bool dragging;               // 0x84
	bool anchored;               // 0x85
	bool canCollide;             // 0x86
	bool canSleep;               // 0x87
	float friction;              // 0x88
	float elasticity;            // 0x8c
	SurfaceType surfaceType[6];  // 0x90
	SurfaceData* surfaceData[6]; // 0xa8
	Controller* controller;      // 0xc0

	ComputeProp<float, Primitive> JointK; // 0xc4

	static Geometry* newGeometry(Geometry::GeometryType geometryType);

public:
	virtual ~Primitive(); // vtable+0x00

	// SYNTHETIC: WEBSERVICE 0x100a8740
	// RBX::Primitive::`scalar deleting destructor'

	Body* getBody() const { return body; }
	const Geometry* getGeometry() const { return geometry; }
	Geometry* getGeometry() { return geometry; }

	const Vector3& getGridSize() const { return geometry->getGridSize(); }
	Geometry::GeometryType getPrimitiveType() const { return geometry->getGeometryType(); }

	Clump* getClump() const { return clump; }
	IMoving* getOwner() const { return myOwner; }
	bool getCanSleep() const { return canSleep; }
	Anchor* getAnchorObject() { return anchorObject; }
	bool getAnchor() const { return anchorObject != NULL; }

	bool getDragging() const { return dragging; }
	bool getCanCollide() const { return canCollide; }
	float getFriction() const { return friction; }
	float getElasticity() const { return elasticity; }

	const Guid& getGuid() const;
	void setGuid(const Guid& value);

	int& worldIndexFunc() { return worldIndex; }

	unsigned int getSizeMultiplier() const { return sizeMultiplier; }
	void setSizeMultiplier(unsigned int multiplier);

	static void insertEdge(Edge* edge);
	static void removeEdge(Edge* edge);

	void setClump(Clump* value);

	int getClumpDepth() const { return clumpDepth; }
	void setClumpDepth(int value) { clumpDepth = value; }

	Assembly* getAssembly() const;

	void setDragging(bool value);
	void setAnchor(bool value);
	void setCanSleep(bool value);
	void setCanCollide(bool value);
	void setFriction(float value);
	void setElasticity(float value);

	const CoordinateFrame& getCoordinateFrame() const;
	void setVelocity(const Velocity& velocity);

	static void onNewTouch(Primitive* p0, Primitive* p1);

	bool hitTest(const Ray& ray, Vector3& hitPoint, bool& inside);

	CoordinateFrame getFaceCoordInObject(NormalId normalId);
	Face getFaceInObject(NormalId normalId);
	Face getFaceInWorld(NormalId normalId);

	float computeJointK() const;

	Extents computeFuzzyExtents() const;
	const Extents& getFastFuzzyExtents() const;

	void setSurfaceData(NormalId normalId, const SurfaceData& value);
	void setSurfaceType(NormalId normalId, SurfaceType surfaceType);

	SurfaceType getSurfaceType(NormalId normalId) const { return surfaceType[normalId]; }

	float getJointK() const { return JointK; }

	const SurfaceData& getSurfaceData(NormalId normalId) const
	{
		return surfaceData[normalId] != NULL ? *surfaceData[normalId] : SurfaceData::empty();
	}

	Controller* getController() const { return controller; }

	CoordinateFrame getGridCorner() const;
	void setGridCorner(const CoordinateFrame& corner);

	void setCoordinateFrame(const CoordinateFrame& value);

	Vector3 clipToSafeSize(const Vector3& size);
	void setGridSize(const Vector3& size);

	void setController(Controller* value);
	void setPrimitiveType(Geometry::GeometryType geometryType);

	// FUNCTION: WEBSERVICE 0x100a8240
	virtual float getRadius() const { return geometry->getRadius(); } // vtable+0x0c

	RigidJoint* getFirstRigidAt(Edge* edge) const;

	Edge* getFirstEdge() const;
	Edge* getNextEdge(Edge* edge) const;
	Joint* getFirstJoint() const;
	Joint* getNextJoint(Joint* joint) const;
	Contact* getFirstContact() const;
	Contact* getNextContact(Contact* contact) const;
	RigidJoint* getFirstRigid() const;
	RigidJoint* getNextRigid(RigidJoint* joint) const;

	static Joint* getJoint(Primitive* p0, Primitive* p1);
	static Contact* getContact(Primitive* p0, Primitive* p1);

	static Primitive* downstreamPrimitive(Joint* joint);
};

DECOMP_SIZE_ASSERT(Primitive, 0xd4)

} // namespace RBX

#endif // V8WORLD_PRIMITIVE_H
