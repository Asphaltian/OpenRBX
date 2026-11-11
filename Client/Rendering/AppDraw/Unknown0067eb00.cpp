#include "appdraw/Unknown0067eb00.h"

#include <OgreMath.h>
#include <OgreRenderOperation.h>
#include <math.h>
#include <vector>

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
	if (manual == NULL) {
		manual = sceneManager->createManualObject(name);
	}

	int i, j;
	float sinCache1a[240];
	float cosCache1a[240];
	float sinCache2a[240];
	float cosCache2a[240];
	float sinCache3a[240];
	float cosCache3a[240];
	float sinCache1b[240];
	float cosCache1b[240];
	float sinCache2b[240];
	float cosCache2b[240];
	float sinCache3b[240];
	float cosCache3b[240];
	float angle;
	float zLow, zHigh;
	float sintemp1, sintemp2, sintemp3 = 0.0, sintemp4 = 0.0;
	float costemp1, costemp2 = 0.0, costemp3 = 0.0, costemp4 = 0.0;
	bool needCache2, needCache3;
	int start, finish;
	int index;
	int size;

	if (slices >= 240) {
		slices = 239;
	}
	if (stacks >= 240) {
		stacks = 239;
	}
	if (slices < 2 || stacks < 1 || radius < 0.0) {
		return NULL;
	}

	needCache2 = needCache3 = false;

	if (m_unk0x00 == 0) {
		needCache2 = true;
	}

	if (m_unk0x00 == 1) {
		if (m_unk0x0c != 0) {
			needCache3 = true;
		}
		if (m_unk0x0c == 1) {
			needCache2 = true;
		}
	}

	for (i = 0; i < slices; i++) {
		angle = 2 * Math::PI * i / slices;
		sinCache1a[i] = sin(angle);
		cosCache1a[i] = cos(angle);
		if (needCache2) {
			sinCache2a[i] = sinCache1a[i];
			cosCache2a[i] = cosCache1a[i];
		}
	}

	for (j = 0; j <= stacks; j++) {
		angle = Math::PI * j / stacks;
		if (needCache2) {
			if (m_unk0x08 == 0) {
				sinCache2b[j] = sin(angle);
				cosCache2b[j] = cos(angle);
			}
			else {
				sinCache2b[j] = -sin(angle);
				cosCache2b[j] = -cos(angle);
			}
		}
		sinCache1b[j] = radius * sin(angle);
		cosCache1b[j] = radius * cos(angle);
	}
	sinCache1b[0] = 0;
	sinCache1b[stacks] = 0;

	if (needCache3) {
		for (i = 0; i < slices; i++) {
			angle = 2 * Math::PI * (i - 0.5) / slices;
			sinCache3a[i] = sin(angle);
			cosCache3a[i] = cos(angle);
		}
		for (j = 0; j <= stacks; j++) {
			angle = Math::PI * (j - 0.5) / stacks;
			if (m_unk0x08 == 0) {
				sinCache3b[j] = sin(angle);
				cosCache3b[j] = cos(angle);
			}
			else {
				sinCache3b[j] = -sin(angle);
				cosCache3b[j] = -cos(angle);
			}
		}
	}

	sinCache1a[slices] = sinCache1a[0];
	cosCache1a[slices] = cosCache1a[0];
	if (needCache2) {
		sinCache2a[slices] = sinCache2a[0];
		cosCache2a[slices] = cosCache2a[0];
	}
	if (needCache3) {
		sinCache3a[slices] = sinCache3a[0];
		cosCache3a[slices] = cosCache3a[0];
	}

	switch (m_unk0x0c) {
	case 2:
		if (!m_unk0x04) {
			start = 1;
			finish = stacks - 1;

			sintemp2 = sinCache1b[1];
			zHigh = cosCache1b[1];
			manual->begin("", RenderOperation::OT_TRIANGLE_FAN);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			manual->position(0.0f + m_unk0x20.x, 0.0f + m_unk0x20.y, radius + m_unk0x20.z);
			manual->index(0);
			index = 1;
			switch (m_unk0x00) {
			case 1:
				sintemp3 = sinCache3b[1];
				costemp3 = cosCache3b[1];
				break;
			case 0:
				sintemp3 = sinCache2b[1];
				costemp3 = cosCache2b[1];
				manual->normal(sinCache2a[0] * sinCache2b[0], cosCache2a[0] * sinCache2b[0], cosCache2b[0]);
				break;
			default:
				break;
			}
			if (m_unk0x08 == 0) {
				for (i = slices; i >= 0; i--) {
					manual->position(
						sintemp2 * sinCache1a[i] + m_unk0x20.x,
						sintemp2 * cosCache1a[i] + m_unk0x20.y,
						zHigh + m_unk0x20.z
					);
					manual->index(index++);
					switch (m_unk0x00) {
					case 0:
						manual->normal(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
						break;
					case 1:
						if (i != slices) {
							manual->normal(sinCache3a[i + 1] * sintemp3, cosCache3a[i + 1] * sintemp3, costemp3);
						}
						break;
					default:
						break;
					}
				}
			}
			else {
				for (i = 0; i <= slices; i++) {
					manual->position(
						sintemp2 * sinCache1a[i] + m_unk0x20.x,
						sintemp2 * cosCache1a[i] + m_unk0x20.y,
						zHigh + m_unk0x20.z
					);
					manual->index(index++);
					switch (m_unk0x00) {
					case 0:
						manual->normal(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
						break;
					case 1:
						manual->normal(sinCache3a[i] * sintemp3, cosCache3a[i] * sintemp3, costemp3);
						break;
					default:
						break;
					}
				}
			}
			manual->end();

			manual->begin("", RenderOperation::OT_TRIANGLE_FAN);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			manual->position(0.0f + m_unk0x20.x, 0.0f + m_unk0x20.y, m_unk0x20.z - radius);
			manual->index(0);
			index = 1;
			sintemp2 = sinCache1b[stacks - 1];
			zHigh = cosCache1b[stacks - 1];
			switch (m_unk0x00) {
			case 1:
				sintemp3 = sinCache3b[stacks];
				costemp3 = cosCache3b[stacks];
				break;
			case 0:
				sintemp3 = sinCache2b[stacks - 1];
				costemp3 = cosCache2b[stacks - 1];
				manual->normal(
					sinCache2a[stacks] * sinCache2b[stacks], cosCache2a[stacks] * sinCache2b[stacks], cosCache2b[stacks]
				);
				break;
			default:
				break;
			}
			if (m_unk0x08 == 0) {
				for (i = 0; i <= slices; i++) {
					manual->position(
						sintemp2 * sinCache1a[i] + m_unk0x20.x,
						sintemp2 * cosCache1a[i] + m_unk0x20.y,
						zHigh + m_unk0x20.z
					);
					manual->index(index++);
					switch (m_unk0x00) {
					case 0:
						manual->normal(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
						break;
					case 1:
						manual->normal(sinCache3a[i] * sintemp3, cosCache3a[i] * sintemp3, costemp3);
						break;
					default:
						break;
					}
				}
			}
			else {
				for (i = slices; i >= 0; i--) {
					manual->position(
						sintemp2 * sinCache1a[i] + m_unk0x20.x,
						sintemp2 * cosCache1a[i] + m_unk0x20.y,
						zHigh + m_unk0x20.z
					);
					manual->index(index++);
					switch (m_unk0x00) {
					case 0:
						manual->normal(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
						break;
					case 1:
						if (i != slices) {
							manual->normal(sinCache3a[i + 1] * sintemp3, cosCache3a[i + 1] * sintemp3, costemp3);
						}
						break;
					default:
						break;
					}
				}
			}
			manual->end();
		}
		else {
			start = 0;
			finish = stacks;
		}
		for (j = start; j < finish; j++) {
			zLow = cosCache1b[j];
			zHigh = cosCache1b[j + 1];
			sintemp1 = sinCache1b[j];
			sintemp2 = sinCache1b[j + 1];
			switch (m_unk0x00) {
			case 1:
				sintemp4 = sinCache3b[j + 1];
				costemp4 = cosCache3b[j + 1];
				break;
			case 0:
				if (m_unk0x08 == 0) {
					sintemp3 = sinCache2b[j + 1];
					costemp3 = cosCache2b[j + 1];
					sintemp4 = sinCache2b[j];
					costemp4 = cosCache2b[j];
				}
				else {
					sintemp3 = sinCache2b[j];
					costemp3 = cosCache2b[j];
					sintemp4 = sinCache2b[j + 1];
					costemp4 = cosCache2b[j + 1];
				}
				break;
			default:
				break;
			}

			std::vector<Vector3> positions;
			std::vector<Vector3> normals;
			if (j == start) {
				manual->begin("", RenderOperation::OT_TRIANGLE_LIST);
				index = 0;
			}
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (i = 0; i <= slices; i++) {
				Vector3 normal;
				if (m_unk0x00 == 0) {
					normal = Vector3(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
				}
				if (m_unk0x08 == 0) {
					if (m_unk0x04) {
						manual->textureCoord(1 - (float) i / slices, 1 - (float) (j + 1) / stacks);
					}
					size = positions.size();
					if (size < 2) {
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp2 * sinCache1a[i],
							m_unk0x20.y + sintemp2 * cosCache1a[i],
							m_unk0x20.z + zHigh
						));
					}
					else if (size % 2 != 0) {
						manual->position(positions[size - 1]);
						manual->index(index++);
						manual->normal(normals[size - 1]);
						manual->position(positions[size - 2]);
						manual->index(index++);
						manual->normal(normals[size - 2]);
						manual->position(
							sintemp2 * sinCache1a[i] + m_unk0x20.x,
							sintemp2 * cosCache1a[i] + m_unk0x20.y,
							zHigh + m_unk0x20.z
						);
						manual->index(index++);
						manual->normal(normal);
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp2 * sinCache1a[i],
							m_unk0x20.y + sintemp2 * cosCache1a[i],
							m_unk0x20.z + zHigh
						));
					}
					else {
						manual->position(positions[size - 2]);
						manual->index(index++);
						manual->normal(normals[size - 2]);
						manual->position(positions[size - 1]);
						manual->index(index++);
						manual->normal(normals[size - 1]);
						manual->position(
							sintemp2 * sinCache1a[i] + m_unk0x20.x,
							sintemp2 * cosCache1a[i] + m_unk0x20.y,
							zHigh + m_unk0x20.z
						);
						manual->index(index++);
						manual->normal(normal);
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp2 * sinCache1a[i],
							m_unk0x20.y + sintemp2 * cosCache1a[i],
							m_unk0x20.z + zHigh
						));
					}
					normals.push_back(normal);
				}
				else {
					if (m_unk0x04) {
						manual->textureCoord(1 - (float) i / slices, 1 - (float) j / stacks);
					}
					size = positions.size();
					if (size < 2) {
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp1 * sinCache1a[i],
							m_unk0x20.y + sintemp1 * cosCache1a[i],
							m_unk0x20.z + zLow
						));
					}
					else if (size % 2 != 0) {
						manual->position(positions[size - 1]);
						manual->index(index++);
						manual->normal(normals[size - 1]);
						manual->position(positions[size - 2]);
						manual->index(index++);
						manual->normal(normals[size - 2]);
						manual->position(
							sintemp1 * sinCache1a[i] + m_unk0x20.x,
							sintemp1 * cosCache1a[i] + m_unk0x20.y,
							zLow + m_unk0x20.z
						);
						manual->index(index++);
						manual->normal(normal);
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp1 * sinCache1a[i],
							m_unk0x20.y + sintemp1 * cosCache1a[i],
							m_unk0x20.z + zLow
						));
					}
					else {
						manual->position(positions[size - 2]);
						manual->index(index++);
						manual->normal(normals[size - 2]);
						manual->position(positions[size - 1]);
						manual->index(index++);
						manual->normal(normals[size - 1]);
						manual->position(
							sintemp1 * sinCache1a[i] + m_unk0x20.x,
							sintemp1 * cosCache1a[i] + m_unk0x20.y,
							zLow + m_unk0x20.z
						);
						manual->index(index++);
						manual->normal(normal);
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp1 * sinCache1a[i],
							m_unk0x20.y + sintemp1 * cosCache1a[i],
							m_unk0x20.z + zLow
						));
					}
					normals.push_back(normal);
				}
				switch (m_unk0x00) {
				case 0:
					normal = Vector3(sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
					break;
				case 1:
					normal = Vector3(sinCache3a[i] * sintemp4, cosCache3a[i] * sintemp4, costemp4);
					break;
				default:
					break;
				}
				if (m_unk0x08 == 0) {
					if (m_unk0x04) {
						manual->textureCoord(1 - (float) i / slices, 1 - (float) j / stacks);
					}
					size = positions.size();
					if (size < 2) {
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp1 * sinCache1a[i],
							m_unk0x20.y + sintemp1 * cosCache1a[i],
							m_unk0x20.z + zLow
						));
					}
					else if (size % 2 != 0) {
						manual->position(positions[size - 1]);
						manual->index(index++);
						manual->normal(normals[size - 1]);
						manual->position(positions[size - 2]);
						manual->index(index++);
						manual->normal(normals[size - 2]);
						manual->position(
							sintemp1 * sinCache1a[i] + m_unk0x20.x,
							sintemp1 * cosCache1a[i] + m_unk0x20.y,
							zLow + m_unk0x20.z
						);
						manual->index(index++);
						manual->normal(normal);
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp1 * sinCache1a[i],
							m_unk0x20.y + sintemp1 * cosCache1a[i],
							m_unk0x20.z + zLow
						));
					}
					else {
						manual->position(positions[size - 2]);
						manual->index(index++);
						manual->normal(normals[size - 2]);
						manual->position(positions[size - 1]);
						manual->index(index++);
						manual->normal(normals[size - 1]);
						manual->position(
							sintemp1 * sinCache1a[i] + m_unk0x20.x,
							sintemp1 * cosCache1a[i] + m_unk0x20.y,
							zLow + m_unk0x20.z
						);
						manual->index(index++);
						manual->normal(normal);
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp1 * sinCache1a[i],
							m_unk0x20.y + sintemp1 * cosCache1a[i],
							m_unk0x20.z + zLow
						));
					}
					normals.push_back(normal);
				}
				else {
					if (m_unk0x04) {
						manual->textureCoord(1 - (float) i / slices, 1 - (float) (j + 1) / stacks);
					}
					size = positions.size();
					if (size < 2) {
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp2 * sinCache1a[i],
							m_unk0x20.y + sintemp2 * cosCache1a[i],
							m_unk0x20.z + zHigh
						));
					}
					else if (size % 2 != 0) {
						manual->position(positions[size - 1]);
						manual->index(index++);
						manual->normal(normals[size - 1]);
						manual->position(positions[size - 2]);
						manual->index(index++);
						manual->normal(normals[size - 2]);
						manual->position(
							sintemp2 * sinCache1a[i] + m_unk0x20.x,
							sintemp2 * cosCache1a[i] + m_unk0x20.y,
							zHigh + m_unk0x20.z
						);
						manual->index(index++);
						manual->normal(normal);
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp2 * sinCache1a[i],
							m_unk0x20.y + sintemp2 * cosCache1a[i],
							m_unk0x20.z + zHigh
						));
					}
					else {
						manual->position(positions[size - 2]);
						manual->index(index++);
						manual->normal(normals[size - 2]);
						manual->position(positions[size - 1]);
						manual->index(index++);
						manual->normal(normals[size - 1]);
						manual->position(
							sintemp2 * sinCache1a[i] + m_unk0x20.x,
							sintemp2 * cosCache1a[i] + m_unk0x20.y,
							zHigh + m_unk0x20.z
						);
						manual->index(index++);
						manual->normal(normal);
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp2 * sinCache1a[i],
							m_unk0x20.y + sintemp2 * cosCache1a[i],
							m_unk0x20.z + zHigh
						));
					}
					normals.push_back(normal);
				}
			}
			if (j == finish - 1) {
				manual->end();
			}
		}
		break;
	case 0:
		manual->begin("", RenderOperation::OT_POINT_LIST);
		if (m_unk0x10.r >= 0.0f) {
			manual->colour(m_unk0x10);
		}
		index = 0;
		for (j = 0; j <= stacks; j++) {
			sintemp1 = sinCache1b[j];
			costemp1 = cosCache1b[j];
			switch (m_unk0x00) {
			case 1:
			case 0:
				sintemp2 = sinCache2b[j];
				costemp2 = cosCache2b[j];
				break;
			default:
				break;
			}
			for (i = 0; i < slices; i++) {
				manual->position(
					sintemp1 * sinCache1a[i] + m_unk0x20.x,
					sintemp1 * cosCache1a[i] + m_unk0x20.y,
					costemp1 + m_unk0x20.z
				);
				manual->index(index++);
				switch (m_unk0x00) {
				case 1:
				case 0:
					manual->normal(sinCache2a[i] * sintemp2, cosCache2a[i] * sintemp2, costemp2);
					break;
				default:
					break;
				}

				if (m_unk0x04) {
					manual->textureCoord(1 - (float) i / slices, 1 - (float) j / stacks);
				}
			}
		}
		manual->end();
		break;
	case 1:
	case 3:
		for (j = 1; j < stacks; j++) {
			sintemp1 = sinCache1b[j];
			costemp1 = cosCache1b[j];
			switch (m_unk0x00) {
			case 1:
			case 0:
				sintemp2 = sinCache2b[j];
				costemp2 = cosCache2b[j];
				break;
			default:
				break;
			}

			manual->begin("", RenderOperation::OT_LINE_STRIP);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			index = 0;
			for (i = 0; i <= slices; i++) {
				manual->position(
					sintemp1 * sinCache1a[i] + m_unk0x20.x,
					sintemp1 * cosCache1a[i] + m_unk0x20.y,
					costemp1 + m_unk0x20.z
				);
				manual->index(index++);
				switch (m_unk0x00) {
				case 1:
					manual->normal(sinCache3a[i] * sintemp2, cosCache3a[i] * sintemp2, costemp2);
					break;
				case 0:
					manual->normal(sinCache2a[i] * sintemp2, cosCache2a[i] * sintemp2, costemp2);
					break;
				default:
					break;
				}
				if (m_unk0x04) {
					manual->textureCoord(1 - (float) i / slices, 1 - (float) j / stacks);
				}
			}
			manual->end();
		}
		for (i = 0; i < slices; i++) {
			sintemp1 = sinCache1a[i];
			costemp1 = cosCache1a[i];
			switch (m_unk0x00) {
			case 1:
			case 0:
				sintemp2 = sinCache2a[i];
				costemp2 = cosCache2a[i];
				break;
			default:
				break;
			}

			manual->begin("", RenderOperation::OT_LINE_STRIP);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			index = 0;
			for (j = 0; j <= stacks; j++) {
				manual->position(
					sintemp1 * sinCache1b[j] + m_unk0x20.x,
					costemp1 * sinCache1b[j] + m_unk0x20.y,
					cosCache1b[j] + m_unk0x20.z
				);
				manual->index(index++);
				switch (m_unk0x00) {
				case 1:
					manual->normal(sintemp2 * sinCache3b[j], costemp2 * sinCache3b[j], cosCache3b[j]);
					break;
				case 0:
					manual->normal(sintemp2 * sinCache2b[j], costemp2 * sinCache2b[j], cosCache2b[j]);
					break;
				default:
					break;
				}

				if (m_unk0x04) {
					manual->textureCoord(1 - (float) i / slices, 1 - (float) j / stacks);
				}
			}
			manual->end();
		}
		break;
	default:
		break;
	}

	return manual;
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
