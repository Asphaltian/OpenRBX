#ifndef RBXVIEW_PART_H
#define RBXVIEW_PART_H

#include "decomp.h"
#include "renderlib/Chunk.h"
#include "renderlib/Material.h"
#include "renderlib/Mesh.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/ReferenceCount.h>
#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

namespace RBX {

class Instance;
class PartInstance;
class SpecialShape;

namespace Reflection {
class PropertyDescriptor;
}

namespace View {

class View;

// SIZE 0xcc
class __declspec(novtable) PartChunk : public Render::Chunk
{
private:
	G3D::CoordinateFrame coordinateFrame; // 0x1c

	virtual void updateMesh() = 0; // vtable+0x20

	boost::signals::scoped_connection childAddedConnection;           // 0x4c
	boost::signals::scoped_connection childRemovedConnection;         // 0x60
	boost::signals::scoped_connection ancestorChangedConnection;      // 0x74
	boost::signals::scoped_connection propertyChangedConnection;      // 0x88
	boost::signals::scoped_connection shapePropertyChangedConnection; // 0x9c

public:
	virtual ~PartChunk();

	virtual bool castsShadows() const;
	virtual bool cullable() const;

protected:
	boost::shared_ptr<PartInstance> partInstance;            // 0xb0
	G3D::ReferenceCountedPointer<Render::Material> material; // 0xb8
	bool materialInvalid;                                    // 0xbc
	G3D::ReferenceCountedPointer<Render::Mesh> mesh;         // 0xc0
	View* view;                                              // 0xc4
	SpecialShape* specialShape;                              // 0xc8

	PartChunk(float polygonOffset, const boost::shared_ptr<PartInstance>& partInstance, View* view);

	void invalidateMaterial();
	void invalidateMesh();

	virtual void onPropertyChanged(const Reflection::PropertyDescriptor* descriptor); // vtable+0x24

	virtual G3D::ReferenceCountedPointer<Render::Mesh> getMesh();
	virtual const G3D::CoordinateFrame& cframe();

private:
	void onAncestorChanged(boost::shared_ptr<Instance> ancestor);
	void onChildAdded(boost::shared_ptr<Instance> child);
	void onChildRemoved(boost::shared_ptr<Instance> child);
	void onSpecialShapeChanged();
};

DECOMP_SIZE_ASSERT(PartChunk, 0xcc)

} // namespace View
} // namespace RBX

#endif // RBXVIEW_PART_H
