#include "appdraw/RbxEntity.h"

#include "appdraw/RbxSceneNode.h"
#include "appdraw/RbxSubEntity.h"

#include <OgreCamera.h>
#include <OgreLight.h>
#include <OgreLogManager.h>
#include <OgreMath.h>
#include <OgreMesh.h>
#include <OgrePass.h>
#include <OgrePlane.h>
#include <OgreRay.h>
#include <OgreRenderQueue.h>
#include <OgreStringConverter.h>
#include <OgreSubMesh.h>
#include <OgreTechnique.h>
#include <algorithm>
#include <string.h>

namespace Ogre {

// FUNCTION: ROBLOX 0x00674e30
void RbxEntity::FUN_00674e30(VisualEngine::Unk0x18* p_unk0x240)
{
	m_unk0x240 = p_unk0x240;
}

// FUNCTION: ROBLOX 0x00674e40
void RbxEntity::FUN_00674e40(MeshPtr mesh)
{
	mMesh = mesh;
}

// FUNCTION: ROBLOX 0x00674ea0
void RbxEntity::_notifyCurrentCamera(Camera* cam)
{
	if (!m_unk0x245) {
		FUN_00676870(cam);
		m_unk0x245 = true;
	}
}

// FUNCTION: ROBLOX 0x00674fd0
void RbxEntity::FUN_00674fd0(bool visible)
{
	for (int i = 0; i < (int) mSubEntityList.size(); i++) {
		mSubEntityList[i]->setVisible(visible);
	}
}

// FUNCTION: ROBLOX 0x006754f0
void RbxEntity::_updateRenderQueue(RenderQueue* queue)
{
	if (!mInitialised) {
		return;
	}

	if (mMeshLodIndex > 0 && mMesh->isLodManual()) {
		mLodEntityList[mMeshLodIndex - 1]->_updateRenderQueue(queue);
		return;
	}

	SubEntityList::iterator i, iend;
	iend = mSubEntityList.end();
	for (i = mSubEntityList.begin(); i != iend; ++i) {
		if ((*i)->isVisible()) {
			if (mRenderQueueIDSet) {
				queue->addRenderable(*i, mRenderQueueID);
			}
			else {
				queue->addRenderable(*i);
			}
		}
	}
}

// STUB: ROBLOX 0x00675a70
void RbxEntity::FUN_00675a70(RbxSubEntity* subEntity)
{
	SubEntityList::iterator it;
	for (it = mSubEntityList.begin(); it != mSubEntityList.end(); ++it) {
		if (*it == subEntity) {
			mSubEntityList.erase(it);
			return;
		}
	}

	if (it == mSubEntityList.end()) {
		delete subEntity;
	}
}

// FUNCTION: ROBLOX 0x00675bc0
RbxEntity::RbxEntity()
{
	m_unk0x268 = 20.0f;
	mInitialised = true;
	m_unk0x244 = true;
	m_unk0x246 = true;
	m_unk0x240 = 0;
	m_unk0x26c = 888;
}

// FUNCTION: ROBLOX 0x00675c90
void RbxEntity::FUN_00675c90()
{
	mSubEntityList.clear();
}

// FUNCTION: ROBLOX 0x00675cd0
RbxEntity::~RbxEntity()
{
	for (int i = 0; i < (int) getNumSubEntities(); i++) {
		RbxSubEntity* subEntity = dynamic_cast<RbxSubEntity*>(mSubEntityList[i]);
		if (subEntity) {
			delete subEntity;
		}
	}
	mSubEntityList.clear();
}

// FUNCTION: ROBLOX 0x00676680
RbxSubEntity* RbxEntity::FUN_00676680(SubMesh* subMesh, int lodIndex)
{
	RbxSubEntity* subEntity = new RbxSubEntity(this, subMesh, lodIndex);
	if (subMesh->isMatInitialised()) {
		subEntity->setMaterialName(subMesh->getMaterialName());
	}
	mSubEntityList.push_back(subEntity);
	return subEntity;
}

// FUNCTION: ROBLOX 0x00676730
void RbxEntity::FUN_00676730(RbxSubEntity* subEntity)
{
	mSubEntityList.push_back(subEntity);
}

// STUB: ROBLOX 0x00676870
void RbxEntity::FUN_00676870(Camera* cam)
{
	RbxSceneNode* node = dynamic_cast<RbxSceneNode*>(getParentNode());
	std::vector<Unknown00676870> candidates;

	bool managed;
	if (!g_unk0x00949dd0 || !m_unk0x240) {
		managed = false;
	}
	else {
		managed = true;
		candidates.resize(mSubEntityList.size(), Unknown00676870());
		candidates.erase(candidates.begin(), candidates.end());
	}

	if (!mSubEntityList.empty()) {
		m_unk0x270.resize(mSubEntityList.size());
		memset(&m_unk0x270[0], 0, m_unk0x270.size());
	}

	for (int i = 0; i < (int) mSubEntityList.size(); i++) {
		RbxSubEntity* subEntity = dynamic_cast<RbxSubEntity*>(mSubEntityList[i]);
		Vector3 offset = subEntity->m_unk0xdc.getTrans();
		bool visible = true;

		SubMesh* subMesh = subEntity->getSubMesh();
		if (!subMesh) {
			subMesh = subEntity->FUN_0067b4e0(1);
		}

		Sphere sphere = node->FUN_0067af60(subMesh);
		sphere.setCenter(sphere.getCenter() + offset);

		if (m_unk0x244) {
			visible = cam->isVisible(sphere, NULL);
		}
		subEntity->setVisible(visible);

		if (!visible) {
			m_unk0x270[i] = 2;
		}
		else {
			m_unk0x270[i] = 1;
			if (managed) {
				Unknown00676870 candidate;
				candidate.m_unk0x00 = i;
				candidate.m_unk0x04 = Math::Sqrt(subEntity->getSquaredViewDepth(cam)) - sphere.getRadius() -
									  subEntity->FUN_0067bcd0() * 1000000.0;
				candidates.push_back(candidate);
			}
		}
	}

	if (managed) {
		int count = candidates.size();
		int mode = -1;

		if (m_unk0x240->FUN_00671ea0() > 30) {
			VisualEngine::Unk0x18::Unk0x08& last = m_unk0x240->FUN_00671ef0(0);
			Real average = m_unk0x240->FUN_00672980(20);

			if (!(Math::Abs((average - last.m_unk0x00) / average) <= 0.2)) {
				if (last.m_unk0x08 < last.m_unk0x04) {
					Real limit = (Real) candidates.size() < last.m_unk0x08 ? (Real) candidates.size() : last.m_unk0x08;
					count = limit;
				}
				else {
					count = candidates.size();
				}
				mode = 1;
			}
			else if (average > 50.0) {
				Real budget = m_unk0x268;
				int minimum = budget / 100.0;
				Real fraction = minimum;
				if (average > 100.0) {
					count = last.m_unk0x08 * fraction;
				}
				else {
					Real scale = 1.0f - (1.0f - fraction) * Math::Sin((average - 50.0) * Math::HALF_PI / 50.0);
					count = last.m_unk0x08 * scale;
				}

				if (count < mSubEntityList.size() * fraction) {
					count = mSubEntityList.size() * fraction;
				}
				if (Math::Abs(last.m_unk0x08 - count) < 40.0f) {
					count = last.m_unk0x08;
				}
				if (count < m_unk0x26c) {
					count = m_unk0x26c;
				}
				if (count > (int) candidates.size()) {
					count = candidates.size();
				}
				mode = 2;
			}
			else if (last.m_unk0x04 > last.m_unk0x08) {
				count = last.m_unk0x08;
				Real growth = (50.0 - average) / average;
				if (growth > 1.2f) {
					growth = 1.2f;
				}
				int grown = count * growth;
				if (grown >= count + 40) {
					count = grown;
				}
				if (count > (int) candidates.size()) {
					count = candidates.size();
				}
				mode = 3;
			}
			else {
				count = candidates.size();
				mode = 4;
			}
		}

		if (count < (int) candidates.size()) {
			std::sort(candidates.begin(), candidates.end());
			for (int i = count; i < (int) candidates.size(); i++) {
				dynamic_cast<RbxSubEntity*>(mSubEntityList[candidates[i].m_unk0x00])->setVisible(false);
				m_unk0x270[candidates[i].m_unk0x00] = 3;
			}

			LogManager::getSingleton().logMessage(
				StringConverter::toString(count) + "/" + StringConverter::toString(candidates.size()) +
				" p=" + StringConverter::toString(mode)
			);
		}

		if (m_unk0x246) {
			m_unk0x240->FUN_00671eb0(VisualEngine::Unk0x18::Unk0x08());
			VisualEngine::Unk0x18::Unk0x08& latest = m_unk0x240->FUN_00671ef0(0);
			latest.m_unk0x04 = candidates.size();
			latest.m_unk0x08 = count;
		}
	}
}

// STUB: ROBLOX 0x006772d0
void RbxEntity::FUN_006772d0(Camera* camera, const Light* light, std::vector<ShadowCaster*>* casters)
{
	casters->clear();
	if (!m_unk0x245) {
		FUN_00676870(camera);
		m_unk0x245 = true;
	}

	const Vector3* corners = m_unk0x248.getAllCorners();
	for (int i = 0; i < 8; i++) {
		if (camera->isVisible(corners[i])) {
			corners[i].distance(camera->getDerivedPosition());
		}
	}

	Real farDistance;
	{
		std::vector<Plane> planes;
		planes.push_back(Plane(Vector3(1.0f, 0.0f, 0.0f), Vector3(m_unk0x248.getMaximum().x, 0.0f, 0.0f)));
		planes.push_back(Plane(Vector3(-1.0f, 0.0f, 0.0f), Vector3(m_unk0x248.getMinimum().x, 0.0f, 0.0f)));
		planes.push_back(Plane(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, m_unk0x248.getMaximum().y, 0.0f)));
		planes.push_back(Plane(Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, m_unk0x248.getMinimum().y, 0.0f)));
		planes.push_back(Plane(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, m_unk0x248.getMaximum().z)));
		planes.push_back(Plane(Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, 0.0f, m_unk0x248.getMinimum().z)));

		const Vector3* frustumCorners = camera->getWorldSpaceCorners();
		Ray rays[4];
		for (int i = 0; i < 4; i++) {
			rays[i].setOrigin(frustumCorners[i]);
			Vector3 direction = frustumCorners[i + 4] - frustumCorners[i];
			direction.normalise();
			rays[i].setDirection(direction);
		}

		Real maxDistance = -1.0f;
		for (int i = 0; i < 4; i++) {
			Real minDistance = 1000000000.0f;
			for (unsigned int j = 0; j < 6; j++) {
				if (planes[j].getSide(rays[i].getOrigin()) == Plane::NEGATIVE_SIDE) {
					std::pair<bool, Real> hit = Math::intersects(rays[i], planes[j]);
					if (hit.first) {
						minDistance = std::min(minDistance, hit.second);
					}
				}
			}
			if (minDistance != 1000000000.0 && minDistance < 10000.0f) {
				maxDistance = std::max(minDistance, maxDistance);
			}
		}

		farDistance = camera->getNearClipDistance() + maxDistance;
	}

	Real savedFarDistance;
	if (farDistance > 0.0f) {
		savedFarDistance = camera->getFarClipDistance();
		camera->setFarClipDistance(farDistance);
	}

	bool lightInFrustum;
	const PlaneBoundedVolumeList* volList;
	if (light->getType() == Light::LT_DIRECTIONAL) {
		AxisAlignedBox aabb;
		const Vector3* frustumCorners = camera->getWorldSpaceCorners();
		Vector3 min, max;
		Vector3 extrude = light->getDerivedDirection() * -10000.0;

		min = max = frustumCorners[0];
		min.makeFloor(frustumCorners[0] + extrude);
		max.makeCeil(frustumCorners[0] + extrude);
		for (size_t c = 1; c < 8; ++c) {
			min.makeFloor(frustumCorners[c]);
			max.makeCeil(frustumCorners[c]);
			min.makeFloor(frustumCorners[c] + extrude);
			max.makeCeil(frustumCorners[c] + extrude);
		}
		aabb.setExtents(min, max);

		lightInFrustum = false;
		volList = &light->_getFrustumClipVolumes(camera);
	}
	else {
		Sphere s(light->getDerivedPosition(), light->getAttenuationRange());
		if (camera->isVisible(s)) {
			lightInFrustum = camera->isVisible(light->getDerivedPosition());
			if (!lightInFrustum) {
				volList = &light->_getFrustumClipVolumes(camera);
			}
			else {
				volList = NULL;
			}
		}
	}

	for (int i = 0; i < (int) getNumSubEntities(); i++) {
		RbxSubEntity* subEntity = dynamic_cast<RbxSubEntity*>(mSubEntityList[i]);
		if (subEntity->getCastShadows()) {
			if (!subEntity->getMaterial()->getTechnique(0)->getPass(0)->isTransparent()) {
				if (subEntity->isVisible()) {
					casters->push_back(subEntity);
				}
				else if (m_unk0x270[i] != 3 && (!lightInFrustum || light->getType() == Light::LT_DIRECTIONAL)) {
					PlaneBoundedVolumeList::const_iterator pi, piend;
					piend = volList->end();
					for (pi = volList->begin(); pi != piend; ++pi) {
						if (pi->intersects(subEntity->getWorldBoundingBox())) {
							casters->push_back(subEntity);
							break;
						}
					}
				}
			}
		}
	}

	if (farDistance > 0.0f) {
		camera->setFarClipDistance(savedFarDistance);
	}
}

String RbxEntityFactory::FACTORY_TYPE_NAME = "Entity";

} // namespace Ogre
