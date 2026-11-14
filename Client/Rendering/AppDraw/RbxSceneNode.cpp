#include "appdraw/RbxSceneNode.h"

#include <OgreMovableObject.h>
#include <OgreRenderQueue.h>
#include <OgreSceneManager.h>
#include <stdexcept>

namespace Ogre {

std::map<const SubMesh*, Sphere>* g_unk0x009613c4;

void RbxSceneNode::_update(bool updateChildren, bool parentHasChanged)
{
	mDerivedOrientation = mOrientation;
	mDerivedPosition = mPosition;
	mDerivedScale = mScale;
	mWorldAABB.setExtents(-10000.0f, -10000.0f, -10000.0f, 10000.0f, 10000.0f, 10000.0f);
}

void RbxSceneNode::_findVisibleObjects(
	Camera* cam,
	RenderQueue* queue,
	VisibleObjectsBoundsInfo* visibleBounds,
	bool includeChildren,
	bool displayNodes,
	bool onlyShadowCasters
)
{
	ObjectMap::iterator iobj;
	ObjectMap::iterator iobjend = mObjectsByName.end();
	for (iobj = mObjectsByName.begin(); iobj != iobjend; ++iobj) {
		iobj->second->_notifyCurrentCamera(cam);
		if (iobj->second->isVisible() && (!onlyShadowCasters || iobj->second->getCastShadows())) {
			iobj->second->_updateRenderQueue(queue);

			if (visibleBounds) {
				visibleBounds->merge(
					iobj->second->getWorldBoundingBox(true), iobj->second->getWorldBoundingSphere(true), cam
				);
			}
		}
	}

	if (includeChildren) {
		ChildNodeMap::iterator child, childend;
		childend = mChildren.end();
		for (child = mChildren.begin(); child != childend; ++child) {
			SceneNode* sceneChild = static_cast<SceneNode*>(child->second);
			sceneChild->_findVisibleObjects(
				cam, queue, visibleBounds, includeChildren, displayNodes, onlyShadowCasters
			);
		}
	}

	if (displayNodes) {
		queue->addRenderable(this);
	}

	if (mShowBoundingBox || (mCreator && mCreator->getShowBoundingBoxes())) {
		_addBoundingBoxToQueue(queue);
	}
}

void RbxSceneNode::_updateBounds(void)
{
	mWorldAABB.setNull();

	ObjectMap::iterator i;
	for (i = mObjectsByName.begin(); i != mObjectsByName.end(); ++i) {
		mWorldAABB.merge(i->second->getWorldBoundingBox(true));
	}

	ChildNodeMap::iterator child;
	for (child = mChildren.begin(); child != mChildren.end(); ++child) {
		SceneNode* sceneChild = static_cast<SceneNode*>(child->second);
		mWorldAABB.merge(sceneChild->_getWorldAABB());
	}
}

const Matrix4& RbxSceneNode::FUN_0067ac90(SubMesh* subMesh)
{
	if (subMesh != NULL) {
		std::map<const SubMesh*, Matrix4*>::iterator it = m_unk0x1c0.find(subMesh);
		if (it == m_unk0x1c0.end()) {
			throw std::runtime_error("Transform map error");
		}
		return *it->second;
	}
	if (mCachedTransformOutOfDate) {
		mCachedTransform.makeTransform(_getDerivedPosition(), _getDerivedScale(), _getDerivedOrientation());
		mCachedTransformOutOfDate = false;
	}
	return mCachedTransform;
}

bool RbxSceneNode::FUN_0067ada0(SubMesh* subMesh) const
{
	std::map<const SubMesh*, Matrix4*>::const_iterator it = m_unk0x1c0.find(subMesh);
	return it != m_unk0x1c0.end();
}

void RbxSceneNode::FUN_0067aeb0(SubMesh* subMesh, const Matrix4& transform)
{
	const SubMesh* key = subMesh;
	std::map<const SubMesh*, Matrix4*>::iterator it = m_unk0x1c0.find(key);
	if (it == m_unk0x1c0.end()) {
		std::pair<std::map<const SubMesh*, Matrix4*>::iterator, bool> inserted;
		inserted = m_unk0x1c0.insert(std::map<const SubMesh*, Matrix4*>::value_type(key, new Matrix4));
		it = inserted.first;
	}
	*it->second = transform;
}

const Sphere& RbxSceneNode::FUN_0067af60(SubMesh* subMesh)
{
	std::map<const SubMesh*, Sphere>::const_iterator it = g_unk0x009613c4->find(subMesh);
	if (it == g_unk0x009613c4->end()) {
		throw std::runtime_error("bsphere map error");
	}
	return it->second;
}

bool RbxSceneNode::FUN_0067b020(SubMesh* subMesh) const
{
	std::map<const SubMesh*, Sphere>::const_iterator it = g_unk0x009613c4->find(subMesh);
	return it != g_unk0x009613c4->end();
}

bool RbxSceneNode::FUN_0067b070(SubMesh* subMesh)
{
	if (g_unk0x009613c4 == NULL) {
		return false;
	}
	std::map<const SubMesh*, Sphere>::iterator it = g_unk0x009613c4->find(subMesh);
	if (it != g_unk0x009613c4->end()) {
		g_unk0x009613c4->erase(it);
		return true;
	}
	return false;
}

RbxSceneNode::~RbxSceneNode()
{
	for (std::map<const SubMesh*, Matrix4*>::iterator it = m_unk0x1c0.begin(); it != m_unk0x1c0.end(); ++it) {
		delete it->second;
	}
	m_unk0x1c0.clear();
}

void RbxSceneNode::FUN_0067b220()
{
	if (g_unk0x009613c4 == NULL) {
		g_unk0x009613c4 = new std::map<const SubMesh*, Sphere>;
	}
}

void RbxSceneNode::FUN_0067b2b0(SubMesh* subMesh, const Sphere& sphere)
{
	FUN_0067b220();
	const SubMesh* key = subMesh;
	std::map<const SubMesh*, Sphere>::iterator it = g_unk0x009613c4->find(key);
	if (it == g_unk0x009613c4->end()) {
		std::pair<std::map<const SubMesh*, Sphere>::iterator, bool> inserted;
		inserted = g_unk0x009613c4->insert(std::map<const SubMesh*, Sphere>::value_type(key, Sphere()));
		it = inserted.first;
	}
	it->second = sphere;
}

} // namespace Ogre
