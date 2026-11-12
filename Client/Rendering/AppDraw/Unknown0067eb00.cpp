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
	normals = 0;
	textureCoords = false;
	orientation = 0;
	drawStyle = 2;
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
	if (manual == NULL) {
		manual = sceneManager->createManualObject(name);
	}

	int i, j;
	float sinCache[240];
	float cosCache[240];
	float sinCache2[240];
	float cosCache2[240];
	float sinCache3[240];
	float cosCache3[240];
	float angle;
	float zLow, zHigh;
	float sintemp, costemp;
	float length;
	float deltaRadius;
	float zNormal;
	float xyNormalRatio;
	float radiusLow, radiusHigh;
	int needCache2, needCache3;
	int index;
	int size;

	if (slices >= 240) {
		slices = 239;
	}
	if (slices < 2 || stacks < 1 || baseRadius < 0.0 || topRadius < 0.0 || height < 0.0) {
		return NULL;
	}

	deltaRadius = baseRadius - topRadius;
	length = sqrt(deltaRadius * deltaRadius + height * height);
	if (length == 0.0) {
		return NULL;
	}

	needCache2 = needCache3 = 0;
	if (normals == 0) {
		needCache2 = 1;
	}

	if (normals == 1) {
		if (drawStyle != 0) {
			needCache3 = 1;
		}
		if (drawStyle == 1) {
			needCache2 = 1;
		}
	}

	zNormal = deltaRadius / length;
	xyNormalRatio = height / length;

	for (i = 0; i < slices; i++) {
		angle = 2 * Math::PI * i / slices;
		if (needCache2) {
			if (orientation == 0) {
				sinCache2[i] = xyNormalRatio * sin(angle);
				cosCache2[i] = xyNormalRatio * cos(angle);
			}
			else {
				sinCache2[i] = -xyNormalRatio * sin(angle);
				cosCache2[i] = -xyNormalRatio * cos(angle);
			}
		}
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	if (needCache3) {
		for (i = 0; i < slices; i++) {
			angle = 2 * Math::PI * (i - 0.5) / slices;
			if (orientation == 0) {
				sinCache3[i] = xyNormalRatio * sin(angle);
				cosCache3[i] = xyNormalRatio * cos(angle);
			}
			else {
				sinCache3[i] = -xyNormalRatio * sin(angle);
				cosCache3[i] = -xyNormalRatio * cos(angle);
			}
		}
	}

	sinCache[slices] = sinCache[0];
	cosCache[slices] = cosCache[0];
	if (needCache2) {
		sinCache2[slices] = sinCache2[0];
		cosCache2[slices] = cosCache2[0];
	}
	if (needCache3) {
		sinCache3[slices] = sinCache3[0];
		cosCache3[slices] = cosCache3[0];
	}

	switch (drawStyle) {
	case 2:
		for (j = 0; j < stacks; j++) {
			zLow = j * height / stacks;
			zHigh = (j + 1) * height / stacks;
			radiusLow = baseRadius - deltaRadius * ((float) j / stacks);
			radiusHigh = baseRadius - deltaRadius * ((float) (j + 1) / stacks);
			index = 0;

			std::vector<Vector3> positions;
			std::vector<Vector3> vertexNormals;
			manual->begin("", RenderOperation::OT_TRIANGLE_LIST);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (i = 0; i <= slices; i++) {
				Vector3 normal;
				switch (normals) {
				case 1:
					normal = Vector3(sinCache3[i], cosCache3[i], zNormal);
					break;
				case 0:
					normal = Vector3(sinCache2[i], cosCache2[i], zNormal);
					break;
				default:
					break;
				}
				if (orientation == 0) {
					if (textureCoords) {
						manual->textureCoord(1 - (float) i / slices, (float) j / stacks);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
							manual->position(
								radiusLow * sinCache[i] + m_unk0x20.x,
								radiusLow * cosCache[i] + m_unk0x20.y,
								zLow + m_unk0x20.z
							);
							manual->index(index++);
							manual->normal(normal);
							positions.push_back(Vector3(
								m_unk0x20.x + radiusLow * sinCache[i],
								m_unk0x20.y + radiusLow * cosCache[i],
								m_unk0x20.z + zLow
							));
						}
						else {
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(
								radiusLow * sinCache[i] + m_unk0x20.x,
								radiusLow * cosCache[i] + m_unk0x20.y,
								zLow + m_unk0x20.z
							);
							manual->index(index++);
							manual->normal(normal);
							positions.push_back(Vector3(
								m_unk0x20.x + radiusLow * sinCache[i],
								m_unk0x20.y + radiusLow * cosCache[i],
								m_unk0x20.z + zLow
							));
						}
					}
					else {
						positions.push_back(Vector3(
							m_unk0x20.x + radiusLow * sinCache[i],
							m_unk0x20.y + radiusLow * cosCache[i],
							m_unk0x20.z + zLow
						));
					}
					vertexNormals.push_back(normal);
					if (textureCoords) {
						manual->textureCoord(1 - (float) i / slices, (float) (j + 1) / stacks);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
							manual->position(
								radiusHigh * sinCache[i] + m_unk0x20.x,
								radiusHigh * cosCache[i] + m_unk0x20.y,
								zHigh + m_unk0x20.z
							);
							manual->index(index++);
							manual->normal(normal);
							positions.push_back(Vector3(
								m_unk0x20.x + radiusHigh * sinCache[i],
								m_unk0x20.y + radiusHigh * cosCache[i],
								m_unk0x20.z + zHigh
							));
						}
						else {
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(
								radiusHigh * sinCache[i] + m_unk0x20.x,
								radiusHigh * cosCache[i] + m_unk0x20.y,
								zHigh + m_unk0x20.z
							);
							manual->index(index++);
							manual->normal(normal);
							positions.push_back(Vector3(
								m_unk0x20.x + radiusHigh * sinCache[i],
								m_unk0x20.y + radiusHigh * cosCache[i],
								m_unk0x20.z + zHigh
							));
						}
					}
					else {
						positions.push_back(Vector3(
							m_unk0x20.x + radiusHigh * sinCache[i],
							m_unk0x20.y + radiusHigh * cosCache[i],
							m_unk0x20.z + zHigh
						));
					}
				}
				else {
					if (textureCoords) {
						manual->textureCoord(1 - (float) i / slices, (float) (j + 1) / stacks);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
							manual->position(
								radiusHigh * sinCache[i] + m_unk0x20.x,
								radiusHigh * cosCache[i] + m_unk0x20.y,
								zHigh + m_unk0x20.z
							);
							manual->index(index++);
							manual->normal(normal);
							positions.push_back(Vector3(
								m_unk0x20.x + radiusHigh * sinCache[i],
								m_unk0x20.y + radiusHigh * cosCache[i],
								m_unk0x20.z + zHigh
							));
						}
						else {
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(
								radiusHigh * sinCache[i] + m_unk0x20.x,
								radiusHigh * cosCache[i] + m_unk0x20.y,
								zHigh + m_unk0x20.z
							);
							manual->index(index++);
							manual->normal(normal);
							positions.push_back(Vector3(
								m_unk0x20.x + radiusHigh * sinCache[i],
								m_unk0x20.y + radiusHigh * cosCache[i],
								m_unk0x20.z + zHigh
							));
						}
					}
					else {
						positions.push_back(Vector3(
							m_unk0x20.x + radiusHigh * sinCache[i],
							m_unk0x20.y + radiusHigh * cosCache[i],
							m_unk0x20.z + zHigh
						));
					}
					vertexNormals.push_back(normal);
					if (textureCoords) {
						manual->textureCoord(1 - (float) i / slices, (float) j / stacks);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
							manual->position(
								radiusLow * sinCache[i] + m_unk0x20.x,
								radiusLow * cosCache[i] + m_unk0x20.y,
								zLow + m_unk0x20.z
							);
							manual->index(index++);
							manual->normal(normal);
							positions.push_back(Vector3(
								m_unk0x20.x + radiusLow * sinCache[i],
								m_unk0x20.y + radiusLow * cosCache[i],
								m_unk0x20.z + zLow
							));
						}
						else {
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(
								radiusLow * sinCache[i] + m_unk0x20.x,
								radiusLow * cosCache[i] + m_unk0x20.y,
								zLow + m_unk0x20.z
							);
							manual->index(index++);
							manual->normal(normal);
							positions.push_back(Vector3(
								m_unk0x20.x + radiusLow * sinCache[i],
								m_unk0x20.y + radiusLow * cosCache[i],
								m_unk0x20.z + zLow
							));
						}
					}
					else {
						positions.push_back(Vector3(
							m_unk0x20.x + radiusLow * sinCache[i],
							m_unk0x20.y + radiusLow * cosCache[i],
							m_unk0x20.z + zLow
						));
					}
				}
				vertexNormals.push_back(normal);
			}
			manual->end();
		}
		break;
	case 0:
		manual->begin("", RenderOperation::OT_POINT_LIST);
		if (m_unk0x10.r >= 0.0f) {
			manual->colour(m_unk0x10);
		}
		for (i = 0; i < slices; i++) {
			sintemp = sinCache[i];
			costemp = cosCache[i];
			for (j = 0; j <= stacks; j++) {
				zLow = j * height / stacks;
				radiusLow = baseRadius - deltaRadius * ((float) j / stacks);
				manual->position(
					radiusLow * sintemp + m_unk0x20.x, radiusLow * costemp + m_unk0x20.y, zLow + m_unk0x20.z
				);
				switch (normals) {
				case 1:
				case 0:
					manual->normal(sinCache2[i], cosCache2[i], zNormal);
					break;
				default:
					break;
				}
				if (textureCoords) {
					manual->textureCoord(1 - (float) i / slices, (float) j / stacks);
				}
			}
		}
		manual->end();
		break;
	case 1:
		for (j = 1; j < stacks; j++) {
			zLow = j * height / stacks;
			radiusLow = baseRadius - deltaRadius * ((float) j / stacks);

			manual->begin("", RenderOperation::OT_LINE_STRIP);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (i = 0; i <= slices; i++) {
				manual->position(
					radiusLow * sinCache[i] + m_unk0x20.x, radiusLow * cosCache[i] + m_unk0x20.y, zLow + m_unk0x20.z
				);
				switch (normals) {
				case 1:
					manual->normal(sinCache3[i], cosCache3[i], zNormal);
					break;
				case 0:
					manual->normal(sinCache2[i], cosCache2[i], zNormal);
					break;
				default:
					break;
				}
				if (textureCoords) {
					manual->textureCoord(1 - (float) i / slices, (float) j / stacks);
				}
			}
			manual->end();
		}
	case 3:
		for (j = 0; j <= stacks; j += stacks) {
			zLow = j * height / stacks;
			radiusLow = baseRadius - deltaRadius * ((float) j / stacks);

			manual->begin("", RenderOperation::OT_LINE_STRIP);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (i = 0; i <= slices; i++) {
				manual->position(
					radiusLow * sinCache[i] + m_unk0x20.x, radiusLow * cosCache[i] + m_unk0x20.y, zLow + m_unk0x20.z
				);
				switch (normals) {
				case 1:
					manual->normal(sinCache3[i], cosCache3[i], zNormal);
					break;
				case 0:
					manual->normal(sinCache2[i], cosCache2[i], zNormal);
					break;
				default:
					break;
				}
				if (textureCoords) {
					manual->textureCoord(1 - (float) i / slices, (float) j / stacks);
				}
			}
			manual->end();
		}
		for (i = 0; i < slices; i++) {
			switch (normals) {
			case 1:
			case 0:
				manual->normal(sinCache2[i], cosCache2[i], 0.0f);
				break;
			default:
				break;
			}
			sintemp = sinCache[i];
			costemp = cosCache[i];
			manual->begin("", RenderOperation::OT_LINE_STRIP);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (j = 0; j <= stacks; j++) {
				manual->position(
					radiusLow * sintemp + m_unk0x20.x, radiusLow * costemp + m_unk0x20.y, zLow + m_unk0x20.z
				);
				zLow = j * height / stacks;
				radiusLow = baseRadius - deltaRadius * ((float) j / stacks);
				if (textureCoords) {
					manual->textureCoord(1 - (float) i / slices, (float) j / stacks);
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
	if (manual == NULL) {
		manual = sceneManager->createManualObject(name);
	}

	int i, j;
	float sinCache[240];
	float cosCache[240];
	float angle;
	float sintemp, costemp;
	float deltaRadius;
	float radiusLow, radiusHigh;
	float texLow = 0.0, texHigh = 0.0;
	float angleOffset;
	int slices2;
	int finish;
	int index;
	int size;

	if (slices >= 240) {
		slices = 239;
	}
	if (slices < 2 || loops < 1 || outerRadius <= 0.0 || innerRadius < 0.0 || innerRadius > outerRadius) {
		return NULL;
	}

	if (sweepAngle < -360.0) {
		sweepAngle = 360.0;
	}
	if (sweepAngle > 360.0) {
		sweepAngle = 360.0;
	}
	if (sweepAngle < 0) {
		startAngle += sweepAngle;
		sweepAngle = -sweepAngle;
	}

	if (sweepAngle == 360.0) {
		slices2 = slices;
	}
	else {
		slices2 = slices + 1;
	}

	deltaRadius = outerRadius - innerRadius;

	angleOffset = startAngle / 180.0 * Math::PI;
	for (i = 0; i <= slices; i++) {
		angle = angleOffset + ((Math::PI * sweepAngle) / 180.0) * i / slices;
		sinCache[i] = sin(angle);
		cosCache[i] = cos(angle);
	}

	if (sweepAngle == 360.0) {
		sinCache[slices] = sinCache[0];
		cosCache[slices] = cosCache[0];
	}

	switch (drawStyle) {
	case 2:
		if (innerRadius == 0.0) {
			finish = loops - 1;
			manual->begin("", RenderOperation::OT_TRIANGLE_FAN);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			manual->position(0.0f + m_unk0x20.x, 0.0f + m_unk0x20.y, 0.0f + m_unk0x20.z);
			manual->index(0);
			index = 1;
			switch (normals) {
			case 1:
			case 0:
				if (orientation == 0) {
					manual->normal(0.0f, 0.0f, 1.0f);
				}
				else {
					manual->normal(0.0f, 0.0f, -1.0f);
				}
				break;
			default:
				break;
			}
			if (textureCoords) {
				manual->textureCoord(0.5f, 0.5f);
			}
			radiusLow = outerRadius - deltaRadius * ((float) (loops - 1) / loops);
			if (textureCoords) {
				texLow = radiusLow / outerRadius / 2;
			}

			if (orientation == 0) {
				for (i = slices; i >= 0; i--) {
					manual->position(
						radiusLow * sinCache[i] + m_unk0x20.x, radiusLow * cosCache[i] + m_unk0x20.y, 0.0 + m_unk0x20.z
					);
					manual->index(index++);
					if (textureCoords) {
						manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
					}
				}
			}
			else {
				for (i = 0; i <= slices; i++) {
					manual->position(
						radiusLow * sinCache[i] + m_unk0x20.x, radiusLow * cosCache[i] + m_unk0x20.y, 0.0 + m_unk0x20.z
					);
					manual->index(index++);
					if (textureCoords) {
						manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
					}
				}
			}
			manual->end();
		}
		else {
			finish = loops;
		}
		for (j = 0; j < finish; j++) {
			radiusLow = outerRadius - deltaRadius * ((float) j / loops);
			radiusHigh = outerRadius - deltaRadius * ((float) (j + 1) / loops);
			if (textureCoords) {
				texLow = radiusLow / outerRadius / 2;
				texHigh = radiusHigh / outerRadius / 2;
			}
			index = 0;

			std::vector<Vector3> positions;
			manual->begin("", RenderOperation::OT_TRIANGLE_LIST);
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			switch (normals) {
			case 1:
			case 0:
				if (orientation == 0) {
					manual->normal(0.0f, 0.0f, 1.0f);
				}
				else {
					manual->normal(0.0f, 0.0f, -1.0f);
				}
				break;
			default:
				break;
			}
			for (i = 0; i <= slices; i++) {
				if (orientation == 0) {
					if (textureCoords) {
						manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->position(
								radiusLow * sinCache[i] + m_unk0x20.x,
								radiusLow * cosCache[i] + m_unk0x20.y,
								0.0 + m_unk0x20.z
							);
							manual->index(index++);
							positions.push_back(Vector3(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0f));
						}
						else {
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->position(
								radiusLow * sinCache[i] + m_unk0x20.x,
								radiusLow * cosCache[i] + m_unk0x20.y,
								0.0 + m_unk0x20.z
							);
							manual->index(index++);
							positions.push_back(Vector3(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0f));
						}
					}
					else {
						positions.push_back(Vector3(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0f));
					}

					if (textureCoords) {
						manual->textureCoord(texHigh * sinCache[i] + 0.5, texHigh * cosCache[i] + 0.5);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->position(
								radiusHigh * sinCache[i] + m_unk0x20.x,
								radiusHigh * cosCache[i] + m_unk0x20.y,
								0.0 + m_unk0x20.z
							);
							manual->index(index++);
							positions.push_back(Vector3(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0f));
						}
						else {
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->position(
								radiusHigh * sinCache[i] + m_unk0x20.x,
								radiusHigh * cosCache[i] + m_unk0x20.y,
								0.0 + m_unk0x20.z
							);
							manual->index(index++);
							positions.push_back(Vector3(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0f));
						}
					}
					else {
						positions.push_back(Vector3(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0f));
					}
				}
				else {
					if (textureCoords) {
						manual->textureCoord(texHigh * sinCache[i] + 0.5, texHigh * cosCache[i] + 0.5);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->position(
								radiusHigh * sinCache[i] + m_unk0x20.x,
								radiusHigh * cosCache[i] + m_unk0x20.y,
								0.0 + m_unk0x20.z
							);
							manual->index(index++);
							positions.push_back(Vector3(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0f));
						}
						else {
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->position(
								radiusHigh * sinCache[i] + m_unk0x20.x,
								radiusHigh * cosCache[i] + m_unk0x20.y,
								0.0 + m_unk0x20.z
							);
							manual->index(index++);
							positions.push_back(Vector3(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0f));
						}
					}
					else {
						positions.push_back(Vector3(radiusHigh * sinCache[i], radiusHigh * cosCache[i], 0.0f));
					}

					if (textureCoords) {
						manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->position(
								radiusLow * sinCache[i] + m_unk0x20.x,
								radiusLow * cosCache[i] + m_unk0x20.y,
								0.0 + m_unk0x20.z
							);
							manual->index(index++);
							positions.push_back(Vector3(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0f));
						}
						else {
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->position(
								radiusLow * sinCache[i] + m_unk0x20.x,
								radiusLow * cosCache[i] + m_unk0x20.y,
								0.0 + m_unk0x20.z
							);
							manual->index(index++);
							positions.push_back(Vector3(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0f));
						}
					}
					else {
						positions.push_back(Vector3(radiusLow * sinCache[i], radiusLow * cosCache[i], 0.0f));
					}
				}
			}
			manual->end();
		}
		break;
	case 0:
		manual->begin("", RenderOperation::OT_POINT_LIST);
		index = 0;
		if (m_unk0x10.r >= 0.0f) {
			manual->colour(m_unk0x10);
		}
		switch (normals) {
		case 1:
		case 0:
			if (orientation == 0) {
				manual->normal(0.0f, 0.0f, 1.0f);
			}
			else {
				manual->normal(0.0f, 0.0f, -1.0f);
			}
			break;
		default:
			break;
		}
		for (i = 0; i < slices2; i++) {
			sintemp = sinCache[i];
			costemp = cosCache[i];
			for (j = 0; j <= loops; j++) {
				radiusLow = outerRadius - deltaRadius * ((float) j / loops);
				manual->position(
					radiusLow * sintemp + m_unk0x20.x, radiusLow * costemp + m_unk0x20.y, 0.0 + m_unk0x20.z
				);
				manual->index(index++);
				if (textureCoords) {
					texLow = radiusLow / outerRadius / 2;
					manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
				}
			}
		}
		manual->end();
		break;
	case 1:
		if (innerRadius == outerRadius) {
			manual->begin("", RenderOperation::OT_LINE_STRIP);
			index = 0;
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			switch (normals) {
			case 1:
			case 0:
				if (orientation == 0) {
					manual->normal(0.0f, 0.0f, 1.0f);
				}
				else {
					manual->normal(0.0f, 0.0f, -1.0f);
				}
				break;
			default:
				break;
			}
			for (i = 0; i <= slices; i++) {
				manual->position(
					innerRadius * sinCache[i] + m_unk0x20.x, innerRadius * cosCache[i] + m_unk0x20.y, 0.0 + m_unk0x20.z
				);
				manual->index(index++);
				if (textureCoords) {
					manual->textureCoord(sinCache[i] / 2 + 0.5, cosCache[i] / 2 + 0.5);
				}
			}
			manual->end();
			break;
		}
		for (j = 0; j <= loops; j++) {
			radiusLow = outerRadius - deltaRadius * ((float) j / loops);
			if (textureCoords) {
				texLow = radiusLow / outerRadius / 2;
			}

			manual->begin("", RenderOperation::OT_LINE_STRIP);
			index = 0;
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (i = 0; i <= slices; i++) {
				manual->position(
					radiusLow * sinCache[i] + m_unk0x20.x, radiusLow * cosCache[i] + m_unk0x20.y, 0.0 + m_unk0x20.z
				);
				manual->index(index++);
				if (textureCoords) {
					manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
				}
			}
			manual->end();
		}
		for (i = 0; i < slices2; i++) {
			sintemp = sinCache[i];
			costemp = cosCache[i];
			manual->begin("", RenderOperation::OT_LINE_STRIP);
			index = 0;
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (j = 0; j <= loops; j++) {
				radiusLow = outerRadius - deltaRadius * ((float) j / loops);
				manual->position(
					radiusLow * sintemp + m_unk0x20.x, radiusLow * costemp + m_unk0x20.y, 0.0 + m_unk0x20.z
				);
				manual->index(index++);
				if (textureCoords) {
					texLow = radiusLow / outerRadius / 2;
					manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
				}
			}
			manual->end();
		}
		break;
	case 3:
		if (sweepAngle < 360.0) {
			for (i = 0; i <= slices; i += slices) {
				sintemp = sinCache[i];
				costemp = cosCache[i];
				manual->begin("", RenderOperation::OT_LINE_STRIP);
				index = 0;
				if (m_unk0x10.r >= 0.0f) {
					manual->colour(m_unk0x10);
				}
				switch (normals) {
				case 1:
				case 0:
					if (orientation == 0) {
						manual->normal(0.0f, 0.0f, 1.0f);
					}
					else {
						manual->normal(0.0f, 0.0f, -1.0f);
					}
					break;
				default:
					break;
				}
				for (j = 0; j <= loops; j++) {
					radiusLow = outerRadius - deltaRadius * ((float) j / loops);
					manual->position(
						radiusLow * sintemp + m_unk0x20.x, radiusLow * costemp + m_unk0x20.y, 0.0 + m_unk0x20.z
					);
					manual->index(index++);
					if (textureCoords) {
						texLow = radiusLow / outerRadius / 2;
						manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
					}
				}
				manual->end();
			}
		}
		for (j = 0; j <= loops; j += loops) {
			radiusLow = outerRadius - deltaRadius * ((float) j / loops);
			if (textureCoords) {
				texLow = radiusLow / outerRadius / 2;
			}

			manual->begin("", RenderOperation::OT_LINE_STRIP);
			index = 0;
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (i = 0; i <= slices; i++) {
				manual->position(
					radiusLow * sinCache[i] + m_unk0x20.x, radiusLow * cosCache[i] + m_unk0x20.y, 0.0 + m_unk0x20.z
				);
				manual->index(index++);
				if (textureCoords) {
					manual->textureCoord(texLow * sinCache[i] + 0.5, texLow * cosCache[i] + 0.5);
				}
			}
			manual->end();
			if (innerRadius == outerRadius) {
				break;
			}
		}
		break;
	default:
		break;
	}

	return manual;
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

	if (normals == 0) {
		needCache2 = true;
	}

	if (normals == 1) {
		if (drawStyle != 0) {
			needCache3 = true;
		}
		if (drawStyle == 1) {
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
			if (orientation == 0) {
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
			if (orientation == 0) {
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

	switch (drawStyle) {
	case 2:
		if (!textureCoords) {
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
			switch (normals) {
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
			if (orientation == 0) {
				for (i = slices; i >= 0; i--) {
					manual->position(
						sintemp2 * sinCache1a[i] + m_unk0x20.x,
						sintemp2 * cosCache1a[i] + m_unk0x20.y,
						zHigh + m_unk0x20.z
					);
					manual->index(index++);
					switch (normals) {
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
					switch (normals) {
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
			switch (normals) {
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
			if (orientation == 0) {
				for (i = 0; i <= slices; i++) {
					manual->position(
						sintemp2 * sinCache1a[i] + m_unk0x20.x,
						sintemp2 * cosCache1a[i] + m_unk0x20.y,
						zHigh + m_unk0x20.z
					);
					manual->index(index++);
					switch (normals) {
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
					switch (normals) {
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
			switch (normals) {
			case 1:
				sintemp4 = sinCache3b[j + 1];
				costemp4 = cosCache3b[j + 1];
				break;
			case 0:
				if (orientation == 0) {
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
			std::vector<Vector3> vertexNormals;
			if (j == start) {
				manual->begin("", RenderOperation::OT_TRIANGLE_LIST);
				index = 0;
			}
			if (m_unk0x10.r >= 0.0f) {
				manual->colour(m_unk0x10);
			}
			for (i = 0; i <= slices; i++) {
				Vector3 normal;
				if (normals == 0) {
					normal = Vector3(sinCache2a[i] * sintemp3, cosCache2a[i] * sintemp3, costemp3);
				}
				if (orientation == 0) {
					if (textureCoords) {
						manual->textureCoord(1 - (float) i / slices, 1 - (float) (j + 1) / stacks);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
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
							manual->normal(vertexNormals[size - 2]);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
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
					}
					else {
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp2 * sinCache1a[i],
							m_unk0x20.y + sintemp2 * cosCache1a[i],
							m_unk0x20.z + zHigh
						));
					}
					vertexNormals.push_back(normal);
				}
				else {
					if (textureCoords) {
						manual->textureCoord(1 - (float) i / slices, 1 - (float) j / stacks);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
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
							manual->normal(vertexNormals[size - 2]);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
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
					}
					else {
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp1 * sinCache1a[i],
							m_unk0x20.y + sintemp1 * cosCache1a[i],
							m_unk0x20.z + zLow
						));
					}
					vertexNormals.push_back(normal);
				}
				switch (normals) {
				case 0:
					normal = Vector3(sinCache2a[i] * sintemp4, cosCache2a[i] * sintemp4, costemp4);
					break;
				case 1:
					normal = Vector3(sinCache3a[i] * sintemp4, cosCache3a[i] * sintemp4, costemp4);
					break;
				default:
					break;
				}
				if (orientation == 0) {
					if (textureCoords) {
						manual->textureCoord(1 - (float) i / slices, 1 - (float) j / stacks);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
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
							manual->normal(vertexNormals[size - 2]);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
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
					}
					else {
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp1 * sinCache1a[i],
							m_unk0x20.y + sintemp1 * cosCache1a[i],
							m_unk0x20.z + zLow
						));
					}
					vertexNormals.push_back(normal);
				}
				else {
					if (textureCoords) {
						manual->textureCoord(1 - (float) i / slices, 1 - (float) (j + 1) / stacks);
					}
					size = positions.size();
					if (positions.size() >= 2) {
						if (size % 2 != 0) {
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
							manual->position(positions[size - 2]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 2]);
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
							manual->normal(vertexNormals[size - 2]);
							manual->position(positions[size - 1]);
							manual->index(index++);
							manual->normal(vertexNormals[size - 1]);
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
					}
					else {
						positions.push_back(Vector3(
							m_unk0x20.x + sintemp2 * sinCache1a[i],
							m_unk0x20.y + sintemp2 * cosCache1a[i],
							m_unk0x20.z + zHigh
						));
					}
					vertexNormals.push_back(normal);
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
			switch (normals) {
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
				switch (normals) {
				case 1:
				case 0:
					manual->normal(sinCache2a[i] * sintemp2, cosCache2a[i] * sintemp2, costemp2);
					break;
				default:
					break;
				}

				if (textureCoords) {
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
			switch (normals) {
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
				switch (normals) {
				case 1:
					manual->normal(sinCache3a[i] * sintemp2, cosCache3a[i] * sintemp2, costemp2);
					break;
				case 0:
					manual->normal(sinCache2a[i] * sintemp2, cosCache2a[i] * sintemp2, costemp2);
					break;
				default:
					break;
				}
				if (textureCoords) {
					manual->textureCoord(1 - (float) i / slices, 1 - (float) j / stacks);
				}
			}
			manual->end();
		}
		for (i = 0; i < slices; i++) {
			sintemp1 = sinCache1a[i];
			costemp1 = cosCache1a[i];
			switch (normals) {
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
				switch (normals) {
				case 1:
					manual->normal(sintemp2 * sinCache3b[j], costemp2 * sinCache3b[j], cosCache3b[j]);
					break;
				case 0:
					manual->normal(sintemp2 * sinCache2b[j], costemp2 * sinCache2b[j], cosCache2b[j]);
					break;
				default:
					break;
				}

				if (textureCoords) {
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

// FUNCTION: ROBLOX 0x00685580
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
