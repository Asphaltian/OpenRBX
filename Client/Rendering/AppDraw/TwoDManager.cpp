#include "appdraw/TwoDManager.h"

#include <OgreHardwareBufferManager.h>
#include <OgreLogManager.h>
#include <OgreRenderSystem.h>
#include <OgreRenderTarget.h>
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreTextureManager.h>

namespace Ogre {

// FUNCTION: ROBLOX 0x0067c6c0
void TwoDManager::FUN_0067c6c0(
	SceneManager* sceneManager, RenderTarget* target, uint8 queueGroupId, bool p_unk0x09, bool p_unk0x0a
)
{
	m_unk0x09 = p_unk0x09;
	m_unk0x08 = queueGroupId;
	m_unk0x04 = sceneManager;
	m_unk0x0c = target;
	m_unk0x0a = p_unk0x0a;
	m_unk0x24.setNull();
	sceneManager->addRenderQueueListener(this);
}

// STUB: ROBLOX 0x0067c710
void TwoDManager::FUN_0067c710()
{
	LayerBlendModeEx colourBlend;
	LayerBlendModeEx alphaBlend;
	RenderSystem* renderSystem = Root::getSingleton().getRenderSystem();

	colourBlend.blendType = LBT_COLOUR;
	colourBlend.operation = LBX_SOURCE1;
	colourBlend.source1 = LBS_TEXTURE;
	alphaBlend.blendType = LBT_ALPHA;
	alphaBlend.operation = LBX_SOURCE1;
	alphaBlend.source1 = LBS_TEXTURE;

	TextureUnitState::UVWAddressingMode addressingMode;
	addressingMode.u = TextureUnitState::TAM_CLAMP;
	addressingMode.v = TextureUnitState::TAM_CLAMP;
	addressingMode.w = TextureUnitState::TAM_CLAMP;

	renderSystem->_setWorldMatrix(Matrix4::IDENTITY);

	Matrix4 projection;
	if (m_unk0x0a) {
		Matrix4 view = Matrix4::IDENTITY;
		view[0][3] = -0.5f;
		view[1][3] = -0.5f;
		view[2][3] = 0.0f;
		view[0][0] = 0.5f;
		view[1][1] = 0.5f;
		view[2][2] = 0.0f;
		renderSystem->_setViewMatrix(view);

		Radian fovy(Math::PI * 0.5);
		Real width = m_unk0x0c->getWidth();
		renderSystem->_makeOrthoMatrix(fovy, width / m_unk0x0c->getHeight(), 1.0f, 0.0f, projection, false);
	}
	else {
		Real nearPlane = m_unk0x0c->getHeight() * 0.5;
		m_unk0x0c->getHeight();

		Matrix4 view = Matrix4::IDENTITY;
		view[0][3] = m_unk0x0c->getWidth() * -0.5;
		view[1][3] = m_unk0x0c->getHeight() * -0.5;
		view[2][3] = 0.0f;
		renderSystem->_setViewMatrix(view);

		Radian fovy(Math::PI * 0.5);
		Real width = m_unk0x0c->getWidth();
		renderSystem->_makeOrthoMatrix(fovy, width / m_unk0x0c->getHeight(), nearPlane, 0.0f, projection, false);
	}

	renderSystem->_setProjectionMatrix(projection);
	renderSystem->_setTextureMatrix(0, Matrix4::IDENTITY);
	renderSystem->_setTextureCoordSet(0, 0);
	renderSystem->_setTextureCoordCalculation(0, TEXCALC_NONE, NULL);
	renderSystem->_setTextureUnitFiltering(0, FO_LINEAR, FO_LINEAR, FO_LINEAR);
	renderSystem->_setTextureBlendMode(0, colourBlend);
	renderSystem->_setTextureBlendMode(0, alphaBlend);
	renderSystem->_setTextureAddressingMode(0, addressingMode);
	renderSystem->_disableTextureUnitsFrom(1);
	renderSystem->setLightingEnabled(false);
	renderSystem->_setFog(FOG_NONE, ColourValue::White, 1.0f, 0.0f, 1.0f);
	renderSystem->_setCullingMode(CULL_NONE);
	renderSystem->_setDepthBufferParams(false, false, CMPF_LESS_EQUAL);
	renderSystem->_setColourBufferWriteEnabled(true, true, true, false);
	renderSystem->setShadingType(SO_GOURAUD);
	renderSystem->_setPolygonMode(PM_SOLID);
	renderSystem->unbindGpuProgram(GPT_FRAGMENT_PROGRAM);
	renderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);
	renderSystem->_setSceneBlending(SBF_SOURCE_ALPHA, SBF_ONE_MINUS_SOURCE_ALPHA);
	renderSystem->_setAlphaRejectSettings(CMPF_ALWAYS_PASS, 0);
}

// FUNCTION: ROBLOX 0x0067cb20
void TwoDManager::FUN_0067cb20(size_t numVertices)
{
	m_unk0x10.vertexData = new VertexData();
	m_unk0x10.vertexData->vertexStart = 0;

	VertexDeclaration* declaration = m_unk0x10.vertexData->vertexDeclaration;
	size_t offset = 0;
	declaration->addElement(0, offset, VET_FLOAT3, VES_POSITION);
	offset += VertexElement::getTypeSize(VET_FLOAT3);
	declaration->addElement(0, offset, VET_COLOUR, VES_DIFFUSE);
	offset += VertexElement::getTypeSize(VET_COLOUR);
	declaration->addElement(0, offset, VET_FLOAT2, VES_TEXTURE_COORDINATES, 0);

	m_unk0x24 = HardwareBufferManager::getSingleton().createVertexBuffer(
		declaration->getVertexSize(0), numVertices, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, false
	);
	m_unk0x10.vertexData->vertexBufferBinding->setBinding(0, m_unk0x24);

	m_unk0x10.useIndexes = false;
	m_unk0x10.operationType = RenderOperation::OT_TRIANGLE_LIST;
}

// FUNCTION: ROBLOX 0x0067cda0
TwoDManager::~TwoDManager()
{
}

// FUNCTION: ROBLOX 0x0067ce10
TwoDManager::TwoDManager()
{
}

// STUB: ROBLOX 0x0067cec0
void TwoDManager::FUN_0067cec0()
{
	RenderSystem* renderSystem = Root::getSingleton().getRenderSystem();
	std::list<Unk0x10> batches;

	if (m_unk0x3c.size() == 0) {
		return;
	}

	size_t numVertices = 0;
	for (std::list<Unk0x30>::iterator it = m_unk0x3c.begin(); it != m_unk0x3c.end(); ++it) {
		if (it->m_unk0x28 == RenderOperation::OT_TRIANGLE_LIST) {
			numVertices += 6;
		}
		else if (it->m_unk0x2c == 0) {
			numVertices += 8;
		}
		else {
			numVertices += 2;
		}
	}

	if (numVertices < 120) {
		numVertices = 120;
	}

	if (m_unk0x24.isNull() || m_unk0x24->getNumVertices() < numVertices) {
		if (!m_unk0x24.isNull()) {
			delete m_unk0x10.vertexData;
			m_unk0x10.vertexData = NULL;
			m_unk0x24.setNull();
		}

		FUN_0067cb20(numVertices);

		if (m_unk0x24.isNull()) {
			LogManager::getSingleton().logMessage("TwoDManager: failed to create hardware buffer\n");
			return;
		}
	}

	float* vertex = static_cast<float*>(m_unk0x24->lock(HardwareBuffer::HBL_DISCARD));

	std::list<Unk0x30>::iterator it = m_unk0x3c.begin();
	Unk0x10 batch;
	batch.m_unk0x00 = it->m_unk0x24;
	batch.m_unk0x04 = it->m_unk0x28;
	batch.m_unk0x08 = it->m_unk0x2c;
	batch.m_unk0x0c = 0;

	while (it != m_unk0x3c.end()) {
		if (batch.m_unk0x04 == RenderOperation::OT_TRIANGLE_LIST) {
			*vertex++ = it->m_unk0x00;
			*vertex++ = it->m_unk0x0c;
			*vertex++ = -1.0f;
			*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
			*vertex++ = it->m_unk0x10;
			*vertex++ = it->m_unk0x1c;

			*vertex++ = it->m_unk0x08;
			*vertex++ = it->m_unk0x04;
			*vertex++ = -1.0f;
			*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
			*vertex++ = it->m_unk0x18;
			*vertex++ = it->m_unk0x14;

			*vertex++ = it->m_unk0x00;
			*vertex++ = it->m_unk0x04;
			*vertex++ = -1.0f;
			*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
			*vertex++ = it->m_unk0x10;
			*vertex++ = it->m_unk0x14;

			*vertex++ = it->m_unk0x00;
			*vertex++ = it->m_unk0x0c;
			*vertex++ = -1.0f;
			*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
			*vertex++ = it->m_unk0x10;
			*vertex++ = it->m_unk0x1c;

			*vertex++ = it->m_unk0x08;
			*vertex++ = it->m_unk0x04;
			*vertex++ = -1.0f;
			*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
			*vertex++ = it->m_unk0x18;
			*vertex++ = it->m_unk0x14;

			*vertex++ = it->m_unk0x08;
			*vertex++ = it->m_unk0x0c;
			*vertex++ = -1.0f;
			*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
			*vertex++ = it->m_unk0x18;
			*vertex++ = it->m_unk0x1c;

			batch.m_unk0x0c += 6;
		}
		else {
			*vertex++ = it->m_unk0x00;
			*vertex++ = it->m_unk0x04;
			*vertex++ = -1.0f;
			*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
			*vertex++ = it->m_unk0x10;
			*vertex++ = it->m_unk0x14;

			if (batch.m_unk0x08 == 0) {
				*vertex++ = it->m_unk0x08;
				*vertex++ = it->m_unk0x04;
				*vertex++ = -1.0f;
				*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
				*vertex++ = it->m_unk0x10;
				*vertex++ = it->m_unk0x14;

				*vertex++ = it->m_unk0x08;
				*vertex++ = it->m_unk0x04;
				*vertex++ = -1.0f;
				*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
				*vertex++ = it->m_unk0x10;
				*vertex++ = it->m_unk0x14;

				*vertex++ = it->m_unk0x08;
				*vertex++ = it->m_unk0x0c;
				*vertex++ = -1.0f;
				*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
				*vertex++ = it->m_unk0x10;
				*vertex++ = it->m_unk0x14;

				*vertex++ = it->m_unk0x08;
				*vertex++ = it->m_unk0x0c;
				*vertex++ = -1.0f;
				*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
				*vertex++ = it->m_unk0x10;
				*vertex++ = it->m_unk0x14;

				*vertex++ = it->m_unk0x00;
				*vertex++ = it->m_unk0x0c;
				*vertex++ = -1.0f;
				*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
				*vertex++ = it->m_unk0x10;
				*vertex++ = it->m_unk0x14;

				*vertex++ = it->m_unk0x00;
				*vertex++ = it->m_unk0x0c;
				*vertex++ = -1.0f;
				*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
				*vertex++ = it->m_unk0x10;
				*vertex++ = it->m_unk0x14;

				*vertex++ = it->m_unk0x00;
				*vertex++ = it->m_unk0x04;
				*vertex++ = -1.0f;
				*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
				*vertex++ = it->m_unk0x10;
				*vertex++ = it->m_unk0x14;

				batch.m_unk0x0c += 8;
			}
			else {
				*vertex++ = it->m_unk0x08;
				*vertex++ = it->m_unk0x0c;
				*vertex++ = -1.0f;
				*reinterpret_cast<uint32*>(vertex++) = it->m_unk0x20;
				*vertex++ = it->m_unk0x10;
				*vertex++ = it->m_unk0x14;

				batch.m_unk0x0c += 2;
			}
		}

		++it;

		if (it == m_unk0x3c.end() || batch.m_unk0x00 != it->m_unk0x24 || batch.m_unk0x04 != it->m_unk0x28) {
			batches.push_back(batch);

			if (it != m_unk0x3c.end()) {
				batch.m_unk0x00 = it->m_unk0x24;
				batch.m_unk0x04 = it->m_unk0x28;
				batch.m_unk0x08 = it->m_unk0x2c;
				batch.m_unk0x0c = 0;
			}
		}
	}

	m_unk0x24->unlock();
	FUN_0067c710();

	TexturePtr texture;
	m_unk0x10.vertexData->vertexStart = 0;
	TexturePtr current;

	for (std::list<Unk0x10>::iterator batchIt = batches.begin(); batchIt != batches.end(); ++batchIt) {
		if (batchIt->m_unk0x00 == 0) {
			renderSystem->_disableTextureUnit(0);
			current.setNull();
		}
		else {
			texture = TextureManager::getSingleton().getByHandle(batchIt->m_unk0x00);

			if (current.get() != texture.get()) {
				renderSystem->_setTexture(0, true, texture);
				renderSystem->_setTextureMatrix(0, Matrix4::IDENTITY);

				LayerBlendModeEx colourBlend;
				LayerBlendModeEx alphaBlend;
				colourBlend.blendType = LBT_COLOUR;
				if (texture->getName() == "ft0" || texture->getFormat() == PF_A8) {
					colourBlend.source2 = LBS_DIFFUSE;
					colourBlend.operation = LBX_SOURCE2;
				}
				else {
					colourBlend.source2 = LBS_CURRENT;
					colourBlend.operation = LBX_SOURCE1;
				}
				colourBlend.source1 = LBS_TEXTURE;
				current = texture;

				alphaBlend.blendType = LBT_ALPHA;
				alphaBlend.operation = LBX_SOURCE1;
				alphaBlend.source1 = LBS_TEXTURE;
				alphaBlend.source2 = LBS_DIFFUSE;

				renderSystem->_setTextureBlendMode(0, colourBlend);
				renderSystem->_setTextureBlendMode(0, alphaBlend);
				renderSystem->_setTextureUnitFiltering(0, FO_LINEAR, FO_LINEAR, FO_LINEAR);
			}
		}

		m_unk0x10.vertexData->vertexCount = batchIt->m_unk0x0c;
		m_unk0x10.operationType = static_cast<RenderOperation::OperationType>(batchIt->m_unk0x04);
		renderSystem->_render(m_unk0x10);
		m_unk0x10.vertexData->vertexStart += batchIt->m_unk0x0c;
	}

	m_unk0x3c.clear();
}

// FUNCTION: ROBLOX 0x0067d9e0
void TwoDManager::FUN_0067d9e0(
	std::string texture,
	const G3D::Color4& color,
	float x0,
	float y0,
	float x1,
	float y1,
	float u0,
	float v0,
	float u1,
	float v1
)
{
	TexturePtr tex;
	Unk0x30 item;
	item.m_unk0x00 = x0;
	item.m_unk0x04 = y0;
	item.m_unk0x08 = x1;
	item.m_unk0x0c = y1;
	item.m_unk0x10 = u0;
	item.m_unk0x14 = v0;
	item.m_unk0x18 = u1;
	item.m_unk0x1c = v1;
	RenderSystem* renderSystem = Root::getSingleton().getRenderSystem();
	renderSystem->convertColourValue(ColourValue(color.r, color.g, color.b, color.a), &item.m_unk0x20);
	item.m_unk0x28 = 4;
	item.m_unk0x2c = 0;
	if (texture != "") {
		tex = TextureManager::getSingleton().getByName(texture);
		if (tex.isNull()) {
			tex = TextureManager::getSingleton().load(texture, "General");
		}
		if (tex.isNull()) {
			item.m_unk0x24 = 0;
		}
		else {
			item.m_unk0x24 = tex->getHandle();
			if (x0 == x1 && y0 == y1) {
				x1 = x0 + tex->getWidth();
				y1 = y0 + tex->getHeight();
			}
		}
	}
	else {
		item.m_unk0x24 = 0;
	}
	m_unk0x3c.push_back(item);
}

// FUNCTION: ROBLOX 0x0067dcc0
void TwoDManager::FUN_0067dcc0(int type, float x0, float y0, float x1, float y1, const G3D::Color4& color)
{
	Unk0x30 item;
	item.m_unk0x00 = x0;
	item.m_unk0x04 = y0;
	item.m_unk0x08 = x1;
	item.m_unk0x0c = y1;
	Root::getSingleton().getRenderSystem()->convertColourValue(
		ColourValue(color.r, color.g, color.b, color.a), &item.m_unk0x20
	);
	item.m_unk0x24 = 0;
	item.m_unk0x28 = type;
	item.m_unk0x2c = 0;
	m_unk0x3c.push_back(item);
}

// FUNCTION: ROBLOX 0x0067dd70
void TwoDManager::FUN_0067dd70(float x0, float y0, float x1, float y1, const G3D::Color4& color)
{
	Unk0x30 item;
	item.m_unk0x00 = x0;
	item.m_unk0x04 = y0;
	item.m_unk0x08 = x1;
	item.m_unk0x0c = y1;
	Root::getSingleton().getRenderSystem()->convertColourValue(
		ColourValue(color.r, color.g, color.b, color.a), &item.m_unk0x20
	);
	item.m_unk0x24 = 0;
	item.m_unk0x28 = 2;
	item.m_unk0x2c = 1;
	m_unk0x3c.push_back(item);
}

// FUNCTION: ROBLOX 0x0067de30
void TwoDManager::renderQueueStarted(uint8 queueGroupId, const String& invocation, bool& skipThisInvocation)
{
	if (!m_unk0x09 && queueGroupId == m_unk0x08) {
		FUN_0067cec0();
	}
}

// FUNCTION: ROBLOX 0x0067de50
void TwoDManager::renderQueueEnded(uint8 queueGroupId, const String& invocation, bool& repeatThisInvocation)
{
	if (m_unk0x09 && queueGroupId == m_unk0x08) {
		FUN_0067cec0();
	}
}

} // namespace Ogre
