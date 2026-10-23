#ifndef TOOL_DRAGGER_H
#define TOOL_DRAGGER_H

#include "util/Extents.h"
#include "v8world/Primitive.h"

#include <G3D/Array.h>
#include <G3D/Matrix3.h>
#include <G3D/Vector3.h>

namespace RBX {

class ContactManager;

class Dragger
{
private:
	static bool intersectingWorldOrOthers(const G3D::Array<Primitive*>& primitives, ContactManager& contactManager);
	static void movePrimitives(const G3D::Array<Primitive*>& primitives, const G3D::Vector3& delta);
	static void movePrimitivesDelta(
		const G3D::Array<Primitive*>& primitives,
		const G3D::Vector3& delta,
		G3D::Vector3& movedSoFar
	);
	static void searchUpFine(
		const G3D::Array<Primitive*>& primitives,
		G3D::Vector3& movedSoFar,
		ContactManager& contactManager
	);
	static void searchDownFine(
		const G3D::Array<Primitive*>& primitives,
		G3D::Vector3& movedSoFar,
		ContactManager& contactManager
	);
	static void searchUpGross(
		const G3D::Array<Primitive*>& primitives,
		G3D::Vector3& movedSoFar,
		ContactManager& contactManager
	);
	static void searchDownGross(
		const G3D::Array<Primitive*>& primitives,
		G3D::Vector3& movedSoFar,
		ContactManager& contactManager
	);

public:
	static const G3D::Vector3& dragSnap()
	{
		static G3D::Vector3 v(1.0f, 0.1f, 1.0f);

		return v;
	}

	static G3D::Vector3 toGrid(const G3D::Vector3& v);
	static G3D::Vector3 safeMoveNoDrop(
		const G3D::Array<Primitive*>& primitives,
		const G3D::Vector3& tryMove,
		ContactManager& contactManager
	);
	static G3D::Vector3 safeMoveYDrop(
		const G3D::Array<Primitive*>& primitives,
		const G3D::Vector3& tryMove,
		ContactManager& contactManager
	);
	static void safeRotate(
		const G3D::Array<Primitive*>& primitives,
		const G3D::Matrix3& rotate,
		ContactManager& contactManager
	);
	static Extents computeExtents(const G3D::Array<Primitive*>& primitives);
};

} // namespace RBX

#endif // TOOL_DRAGGER_H
