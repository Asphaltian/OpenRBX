#ifndef APPDRAW_UNKNOWN0067EB00_H
#define APPDRAW_UNKNOWN0067EB00_H

#include "decomp.h"

#include <OgreColourValue.h>
#include <OgreManualObject.h>
#include <OgreSceneManager.h>
#include <OgreString.h>
#include <OgreVector3.h>

namespace Ogre {

// SIZE 0x2c
class Unknown0067eb00
{
public:
	Unknown0067eb00();

	void FUN_0067eae0(const Vector3& p_unk0x04);
	ManualObject* FUN_0067ecc0(
		SceneManager* sceneManager,
		const String& name,
		ManualObject* manual,
		float baseRadius,
		float topRadius,
		float height,
		int slices,
		int stacks
	);
	ManualObject* FUN_00680b30(
		SceneManager* sceneManager,
		const String& name,
		ManualObject* manual,
		float innerRadius,
		float outerRadius,
		int slices,
		int loops,
		float startAngle,
		float sweepAngle
	);
	ManualObject* FUN_00682ad0(
		SceneManager* sceneManager, const String& name, ManualObject* manual, float radius, int slices, int stacks
	);
	ManualObject* FUN_00685580(
		SceneManager* sceneManager,
		const String& name,
		ManualObject* manual,
		float innerRadius,
		float outerRadius,
		int slices,
		int loops
	);

	int normals;           // 0x00
	bool textureCoords;    // 0x04
	int orientation;       // 0x08
	int drawStyle;         // 0x0c
	ColourValue m_unk0x10; // 0x10
	Vector3 m_unk0x20;     // 0x20
};

DECOMP_SIZE_ASSERT(Unknown0067eb00, 0x2c)

} // namespace Ogre

#endif // APPDRAW_UNKNOWN0067EB00_H
