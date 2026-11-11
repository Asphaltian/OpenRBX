#include "appdraw/Unknown0067eb00.h"

namespace Ogre {

// FUNCTION: ROBLOX 0x0067eae0
void Unknown0067eb00::FUN_0067eae0(const Vector3& p_unk0x04)
{
	m_unk0x20 = p_unk0x04;
}

// FUNCTION: ROBLOX 0x0067eb00
Unknown0067eb00::Unknown0067eb00()
{
	m_unk0x00 = 0;
	m_unk0x04 = false;
	m_unk0x08 = 0;
	m_unk0x0c = 2;
	m_unk0x10 = ColourValue(-1.0f, -1.0f, -1.0f, -1.0f);
	m_unk0x20 = Vector3(0.0f, 0.0f, 0.0f);
}

// STUB: ROBLOX 0x0067ecc0
ManualObject* Unknown0067eb00::FUN_0067ecc0(
	SceneManager* sceneManager,
	const String& name,
	ManualObject* manual,
	float baseRadius,
	float topRadius,
	float height,
	int slices,
	int stacks
)
{
	STUB(0x67ecc0);
	return NULL;
}

// STUB: ROBLOX 0x00680b30
ManualObject* Unknown0067eb00::FUN_00680b30(
	SceneManager* sceneManager,
	const String& name,
	ManualObject* manual,
	float innerRadius,
	float outerRadius,
	int slices,
	int loops,
	float startAngle,
	float sweepAngle
)
{
	STUB(0x680b30);
	return NULL;
}

// STUB: ROBLOX 0x00682ad0
ManualObject* Unknown0067eb00::FUN_00682ad0(
	SceneManager* sceneManager, const String& name, ManualObject* manual, float radius, int slices, int stacks
)
{
	STUB(0x682ad0);
	return NULL;
}

// STUB: ROBLOX 0x00685580
ManualObject* Unknown0067eb00::FUN_00685580(
	SceneManager* sceneManager,
	const String& name,
	ManualObject* manual,
	float innerRadius,
	float outerRadius,
	int slices,
	int loops
)
{
	return FUN_00680b30(sceneManager, name, manual, innerRadius, outerRadius, slices, loops, 0.0f, 360.0f);
}

} // namespace Ogre
