#ifndef RENDERLIBNEW_PART_H
#define RENDERLIBNEW_PART_H

#include "placeholder.h"
#include "renderlibnew/Chunk.h"
#include "renderlibnew/Material.h"
#include "renderlibnew/Mesh.h"
#include "util/Events.h"
#include "v8world/IMoving.h"

#include <G3D/CoordinateFrame.h>
#include <G3D/ReferenceCount.h>
#include <G3D/Vector3.h>
#include <boost/shared_ptr.hpp>
#include <boost/signals/connection.hpp>

namespace RBX {

class Decal;
class Instance;
class PartInstance;
class SpecialShape;
class Texture;

namespace Reflection {
class PropertyDescriptor;
}

namespace ViewNew {

class View;

float primaryComponent(const G3D::Vector3& v);

class PartChunk : public RenderNew::Chunk
{
private:
	G3D::CoordinateFrame coordinateFrame;

	virtual void updateMesh() = 0;

	boost::signals::scoped_connection childAddedConnection;
	boost::signals::scoped_connection childRemovedConnection;
	boost::signals::scoped_connection ancestorChangedConnection;
	boost::signals::scoped_connection propertyChangedConnection;
	boost::signals::scoped_connection shapePropertyChangedConnection;

public:
	virtual ~PartChunk();

	virtual bool castsShadows() const;
	virtual bool cullable() const;

protected:
	boost::shared_ptr<PartInstance> partInstance;
	G3D::ReferenceCountedPointer<RenderNew::Material> material;
	bool materialInvalid;
	G3D::ReferenceCountedPointer<RenderNew::Mesh> mesh;
	View* view;
	SpecialShape* specialShape;

	PartChunk(float polygonOffset, const boost::shared_ptr<PartInstance>& partInstance, View* view);

	void invalidateMaterial();
	void invalidateMesh();

	virtual void onPropertyChanged(const Reflection::PropertyDescriptor* descriptor);

	virtual G3D::ReferenceCountedPointer<RenderNew::Mesh> getMesh();
	virtual const G3D::CoordinateFrame& cframe();

private:
	void onAncestorChanged(boost::shared_ptr<Instance> ancestor);
	void onChildAdded(boost::shared_ptr<Instance> child);
	void onChildRemoved(boost::shared_ptr<Instance> child);
	void onSpecialShapeChanged();
};

class Part : public PartChunk, public Listener<PartInstance, CanAggregateChanged>
{
public:
	Part(const boost::shared_ptr<PartInstance>& partInstance, View* view);

	virtual ~Part();

	virtual G3D::ReferenceCountedPointer<RenderNew::Material> getMaterial();

protected:
	virtual void onPropertyChanged(const Reflection::PropertyDescriptor* descriptor);
	virtual void onEvent(const PartInstance* source, CanAggregateChanged event);

private:
	bool usesMegaTexture() const;

	virtual void updateMesh();
};

class Decal : public PartChunk
{
private:
	boost::shared_ptr<RBX::Decal> decal;
	boost::signals::scoped_connection decalAncestorChangedConnection;
	boost::signals::scoped_connection decalPropertyChangedConnection;

public:
	Decal(RBX::Decal& decal, PartInstance& parentPart, View* view);

	virtual ~Decal();

	virtual G3D::ReferenceCountedPointer<RenderNew::Material> getMaterial();

protected:
	void onDecalPropertyChanged(const Reflection::PropertyDescriptor* descriptor);
	void onDecalAncestorChanged(boost::shared_ptr<Instance> ancestor);

private:
	virtual void updateMesh();
};

class Texture : public PartChunk
{
private:
	boost::shared_ptr<RBX::Texture> texture;
	boost::signals::scoped_connection textureAncestorChangedConnection;
	boost::signals::scoped_connection texturePropertyChangedConnection;

public:
	Texture(RBX::Texture& texture, PartInstance& parentPart, View* view);

	virtual ~Texture();

	virtual G3D::ReferenceCountedPointer<RenderNew::Material> getMaterial();

protected:
	void onTexturePropertyChanged(const Reflection::PropertyDescriptor* descriptor);
	void onTextureAncestorChanged(boost::shared_ptr<Instance> ancestor);

private:
	virtual void updateMesh();
};

} // namespace ViewNew
} // namespace RBX

#endif // RENDERLIBNEW_PART_H
