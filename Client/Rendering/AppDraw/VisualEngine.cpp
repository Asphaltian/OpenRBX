#include "appdraw/VisualEngine.h"

#include "appdraw/RbxSceneManager.h"

#include <G3D/Color4.h>
#include <OgreCamera.h>
#include <OgreException.h>
#include <OgreHardwareBufferManager.h>
#include <OgreLogManager.h>
#include <OgreMeshManager.h>
#include <OgreRenderSystem.h>
#include <OgreRenderTarget.h>
#include <OgreRenderWindow.h>
#include <OgreResourceGroupManager.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreTextureManager.h>
#include <OgreVertexIndexData.h>
#include <OgreViewport.h>
#include <algorithm>
#include <d3d9.h>
#include <ddraw.h>
#include <stdexcept>
#include <stdio.h>
#include <vector>

namespace Ogre {

// GLOBAL: ROBLOX 0x00949dd0
bool g_unk0x00949dd0;

// GLOBAL: ROBLOX 0x0096134c
Root* g_unk0x0096134c;

// GLOBAL: ROBLOX 0x00961350
DWORD g_unk0x00961350;

// GLOBAL: ROBLOX 0x00961354
bool g_unk0x00961354;

// GLOBAL: ROBLOX 0x00961358
int g_unk0x00961358;

// GLOBAL: ROBLOX 0x0096135c
int g_unk0x0096135c;

// FUNCTION: ROBLOX 0x00671c70
Unknown00671c70::Unknown00671c70() : m_unk0x84(false), m_unk0x88(0)
{
}

// FUNCTION: ROBLOX 0x00671cd0
ColourValue FUN_00671cd0(const G3D::Color4& color)
{
	return ColourValue(color.r, color.g, color.b, color.a);
}

// FUNCTION: ROBLOX 0x00671cf0
unsigned int FUN_00671cf0()
{
	if (!g_unk0x00961354) {
		CoInitialize(NULL);
		IDirectDraw2* directDraw = NULL;
		DWORD freeMemory = 0;
		if (SUCCEEDED(
				CoCreateInstance(CLSID_DirectDraw, NULL, CLSCTX_INPROC_SERVER, IID_IDirectDraw2, (void**) &directDraw)
			)) {
			directDraw->Initialize(NULL);
			DDSCAPS caps;
			caps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_LOCALVIDMEM;
			if (FAILED(directDraw->GetAvailableVidMem(&caps, &g_unk0x00961350, &freeMemory))) {
				g_unk0x00961350 = 0;
				freeMemory = 0;
			}
			directDraw->Release();
		}
		CoUninitialize();
		g_unk0x00961354 = true;
	}
	return g_unk0x00961350;
}

// FUNCTION: ROBLOX 0x00671d90
SubMesh* VisualEngine::FUN_00671d90(
	MeshPtr& mesh, const unsigned short* indices16, const unsigned int* indices32, size_t triangleCount
)
{
	SubMesh* subMesh = mesh->createSubMesh();
	size_t indexCount = triangleCount * 3;
	HardwareIndexBufferSharedPtr indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
		indices16 ? HardwareIndexBuffer::IT_16BIT : HardwareIndexBuffer::IT_32BIT,
		indexCount,
		HardwareBuffer::HBU_STATIC_WRITE_ONLY
	);
	indexBuffer->writeData(
		0, indexBuffer->getSizeInBytes(), indices16 ? (const void*) indices16 : (const void*) indices32, true
	);
	subMesh->useSharedVertices = true;
	subMesh->indexData->indexBuffer = indexBuffer;
	subMesh->indexData->indexCount = indexCount;
	subMesh->indexData->indexStart = 0;
	return subMesh;
}

// STUB: ROBLOX 0x00671e60
VisualEngine::Unk0x18::Unk0x18() : m_unk0x00(0)
{
}

// FUNCTION: ROBLOX 0x00671ea0 FOLDED
int VisualEngine::Unk0x18::FUN_00671ea0() const
{
	return m_unk0x00;
}

// FUNCTION: ROBLOX 0x00671eb0
void VisualEngine::Unk0x18::FUN_00671eb0(const Unk0x08& entry)
{
	m_unk0x08[m_unk0x00 % 100] = entry;
	m_unk0x00++;
}

// STUB: ROBLOX 0x00671ef0
VisualEngine::Unk0x18::Unk0x08& VisualEngine::Unk0x18::FUN_00671ef0(int back)
{
	return m_unk0x08[(m_unk0x00 - back - 1) % 100];
}

// FUNCTION: ROBLOX 0x00671f80
VisualEngine::VisualEngine()
	: m_unk0x04(NULL), m_unk0x08(NULL), m_unk0x0c(NULL), m_unk0x10(NULL), m_unk0x14(NULL), m_unk0x7f8(-1)
{
}

VisualEngine::~VisualEngine()
{
	if (g_unk0x0096134c) {
		if (m_unk0x04) {
			g_unk0x0096134c->destroySceneManager(m_unk0x04);
		}
		if (m_unk0x0c) {
			g_unk0x0096134c->detachRenderTarget(m_unk0x0c);
		}
		delete m_unk0x14;
	}
}

// FUNCTION: ROBLOX 0x00672010
void FUN_00672010()
{
	if (g_unk0x0096134c) {
		delete g_unk0x0096134c;
		g_unk0x0096134c = NULL;
	}
}

// FUNCTION: ROBLOX 0x00672980
float VisualEngine::Unk0x18::FUN_00672980(int count)
{
	if (count <= 99 && m_unk0x00 >= count) {
		std::vector<float> values(count, 0.0f);
		values.clear();
		for (int i = 1; i <= count; i++) {
			values.push_back(m_unk0x08[(m_unk0x00 - i) % 100].m_unk0x00);
		}
		std::sort(values.begin(), values.end());
		return values[count / 2];
	}
	return -1.0f;
}

// STUB: ROBLOX 0x00672ab0
VertexData* VisualEngine::FUN_00672ab0(
	const float* positions,
	size_t vertexCount,
	const float* normals,
	const ColourValue* colours,
	const float* textureCoords
)
{
	RenderSystem* renderSystem = Root::getSingleton().getRenderSystem();
	VertexData* vertexData = new VertexData();
	vertexData->vertexCount = vertexCount;
	VertexDeclaration* declaration = vertexData->vertexDeclaration;

	declaration->addElement(0, 0, VET_FLOAT3, VES_POSITION);
	size_t vertexSize = VertexElement::getTypeSize(VET_FLOAT3);
	HardwareVertexBufferSharedPtr buffer = HardwareBufferManager::getSingleton().createVertexBuffer(
		vertexSize, vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY
	);
	buffer->writeData(0, buffer->getSizeInBytes(), positions, true);
	VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, buffer);

	unsigned short source = 1;
	if (colours) {
		std::vector<RGBA> converted;
		converted.resize(vertexCount);
		for (size_t i = 0; i < vertexCount; i++, colours++) {
			ColourValue colour(colours->r, colours->r, colours->r, colours->r);
			renderSystem->convertColourValue(colour, &converted[i]);
		}

		declaration->addElement(1, 0, VET_COLOUR, VES_DIFFUSE);
		vertexSize = VertexElement::getTypeSize(VET_COLOUR);
		buffer = HardwareBufferManager::getSingleton().createVertexBuffer(
			vertexSize, vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY
		);
		buffer->writeData(0, buffer->getSizeInBytes(), &converted[0], true);
		binding->setBinding(1, buffer);
		source = 2;
	}

	if (normals) {
		declaration->addElement(source, 0, VET_FLOAT3, VES_NORMAL);
		vertexSize = VertexElement::getTypeSize(VET_FLOAT3);
		buffer = HardwareBufferManager::getSingleton().createVertexBuffer(
			vertexSize, vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY
		);
		buffer->writeData(0, buffer->getSizeInBytes(), normals, true);
		binding->setBinding(source, buffer);
		source++;
	}

	if (textureCoords) {
		declaration->addElement(source, 0, VET_FLOAT2, VES_TEXTURE_COORDINATES);
		vertexSize = VertexElement::getTypeSize(VET_FLOAT2);
		buffer = HardwareBufferManager::getSingleton().createVertexBuffer(
			vertexSize, vertexData->vertexCount, HardwareBuffer::HBU_STATIC_WRITE_ONLY
		);
		buffer->writeData(0, buffer->getSizeInBytes(), textureCoords, true);
		binding->setBinding(source, buffer);
	}

	return vertexData;
}

// STUB: ROBLOX 0x00672e60
void VisualEngine::FUN_00672e60(
	MeshPtr& mesh,
	const float* positions,
	size_t vertexCount,
	const float* normals,
	const ColourValue* colours,
	const float* textureCoords,
	const Vector3* minimum,
	const Vector3* maximum
)
{
	if (mesh->sharedVertexData) {
		delete mesh->sharedVertexData;
		mesh->sharedVertexData = NULL;
	}
	mesh->sharedVertexData = FUN_00672ab0(positions, vertexCount, normals, colours, textureCoords);

	if (minimum && maximum) {
		mesh->_setBounds(AxisAlignedBox(minimum->x, minimum->y, minimum->z, maximum->x, maximum->y, maximum->z));
		mesh->_setBoundingSphereRadius((*maximum - *minimum).length() * 0.5);
	}
	else {
		Vector3 lo(1e9f, 1e9f, 1e9f);
		Vector3 hi(-1e9f, -1e9f, -1e9f);
		for (size_t i = 0; i < vertexCount * 3; i += 3) {
			if (lo.x > positions[i]) {
				lo.x = positions[i];
			}
			if (lo.y > positions[i + 1]) {
				lo.y = positions[i + 1];
			}
			if (lo.z > positions[i + 2]) {
				lo.z = positions[i + 2];
			}
			if (hi.x < positions[i]) {
				hi.x = positions[i];
			}
			if (hi.y < positions[i + 1]) {
				hi.y = positions[i + 1];
			}
			if (hi.z < positions[i + 2]) {
				hi.z = positions[i + 2];
			}
		}

		mesh->_setBounds(AxisAlignedBox(lo, hi));
		mesh->_setBoundingSphereRadius((hi - lo).length() * 0.5);
	}
}

// STUB: ROBLOX 0x006731e0
MeshPtr VisualEngine::FUN_006731e0(
	const char* name,
	const char* group,
	const float* positions,
	size_t vertexCount,
	const float* normals,
	const ColourValue* colours,
	const float* textureCoords,
	const Vector3* minimum,
	const Vector3* maximum
)
{
	MeshPtr mesh = MeshManager::getSingleton().createManual(name, group);
	FUN_00672e60(mesh, positions, vertexCount, normals, colours, textureCoords, minimum, maximum);

	if (minimum && maximum) {
		mesh->_setBounds(AxisAlignedBox(minimum->x, minimum->y, minimum->z, maximum->x, maximum->y, maximum->z));
		mesh->_setBoundingSphereRadius((*maximum - *minimum).length() * 0.5);
	}
	else {
		Vector3 lo(1e9f, 1e9f, 1e9f);
		Vector3 hi(-1e9f, -1e9f, -1e9f);
		for (size_t i = 0; i < vertexCount * 3; i += 3) {
			if (lo.x > positions[i]) {
				lo.x = positions[i];
			}
			if (lo.y > positions[i + 1]) {
				lo.y = positions[i + 1];
			}
			if (lo.z > positions[i + 2]) {
				lo.z = positions[i + 2];
			}
			if (hi.x < positions[i]) {
				hi.x = positions[i];
			}
			if (hi.y < positions[i + 1]) {
				hi.y = positions[i + 1];
			}
			if (hi.z < positions[i + 2]) {
				hi.z = positions[i + 2];
			}
		}

		mesh->_setBounds(AxisAlignedBox(lo, hi));
		mesh->_setBoundingSphereRadius((hi - lo).length() * 0.5);
	}

	mesh->load();
	return mesh;
}

// STUB: ROBLOX 0x006735b0
Unknown00671c70* VisualEngine::FUN_006735b0(
	const float* positions,
	size_t vertexCount,
	const float* normals,
	const ColourValue* colours,
	const float* textureCoords,
	const unsigned short* indices16,
	const unsigned int* indices32,
	size_t triangleCount,
	Vector3* minimumOut,
	Vector3* maximumOut
)
{
	Unknown00671c70* subMesh = new Unknown00671c70();
	subMesh->parent = NULL;
	subMesh->vertexData = FUN_00672ab0(positions, vertexCount, normals, colours, textureCoords);
	subMesh->useSharedVertices = false;

	size_t indexCount = triangleCount * 3;
	HardwareIndexBufferSharedPtr indexBuffer = HardwareBufferManager::getSingleton().createIndexBuffer(
		indices16 ? HardwareIndexBuffer::IT_16BIT : HardwareIndexBuffer::IT_32BIT,
		indexCount,
		HardwareBuffer::HBU_STATIC_WRITE_ONLY
	);
	if (indices16) {
		indexBuffer->writeData(0, indexBuffer->getSizeInBytes(), indices16, true);
	}
	else {
		indexBuffer->writeData(0, indexBuffer->getSizeInBytes(), indices32, true);
	}
	subMesh->indexData->indexBuffer = indexBuffer;
	subMesh->indexData->indexCount = indexCount;
	subMesh->indexData->indexStart = 0;

	Vector3 minimum(1e9f, 1e9f, 1e9f);
	Vector3 maximum(-1e9f, -1e9f, -1e9f);
	for (size_t i = 0; i < vertexCount * 3; i += 3) {
		if (minimum.x > positions[i]) {
			minimum.x = positions[i];
		}
		if (minimum.y > positions[i + 1]) {
			minimum.y = positions[i + 1];
		}
		if (minimum.z > positions[i + 2]) {
			minimum.z = positions[i + 2];
		}
		if (maximum.x < positions[i]) {
			maximum.x = positions[i];
		}
		if (maximum.y < positions[i + 1]) {
			maximum.y = positions[i + 1];
		}
		if (maximum.z < positions[i + 2]) {
			maximum.z = positions[i + 2];
		}
	}

	*minimumOut = minimum;
	*maximumOut = maximum;

	AxisAlignedBox box(minimum, maximum);
	subMesh->m_unk0x8c = box;
	subMesh->m_unk0xac = box.getHalfSize().length();
	return subMesh;
}

// STUB: ROBLOX 0x00673860
void VisualEngine::FUN_00673860(
	int renderSystem, unsigned long window, int width, int height, int antialiasing, std::string logFileName
)
{
	try {
		if (g_unk0x0096134c == NULL) {
			g_unk0x0096134c = new Root("", "", logFileName);

			static RbxSceneManagerFactory factory;
			g_unk0x0096134c->addSceneManagerFactory(&factory);

			ResourceGroupManager::getSingleton().initialiseResourceGroup("General");
			ResourceGroupManager::getSingleton().addResourceLocation(".", "FileSystem", "General", false);

			if (renderSystem == 0) {
				try {
					g_unk0x0096134c->loadPlugin("rgdx.dll");
				}
				catch (Exception& e) {
					LogManager::getSingleton().logMessage("Error loading DX9 render system.\n");
					FUN_00672010();
					throw std::runtime_error(
						std::string("Cannot initialize graphics; installation error likely. Error: ") + e.what()
					);
				}
			}
			else if (renderSystem == 1) {
				try {
					g_unk0x0096134c->loadPlugin("rggl.dll");
				}
				catch (Exception&) {
					LogManager::getSingleton().logMessage("Error loading OGL render system.\n");
					FUN_00672010();
					throw std::runtime_error("Cannot initialize graphics; installation error likely.");
				}
			}

			try {
				g_unk0x0096134c->loadPlugin("rgpar.dll");
			}
			catch (Exception&) {
				LogManager::getSingleton().logMessage("Error loading particle system.\n");
				FUN_00672010();
				throw std::runtime_error("Cannot initialize gfx component; installation error likely.");
			}

			RenderSystemList* renderers = g_unk0x0096134c->getAvailableRenderers();
			g_unk0x0096134c->setRenderSystem(renderers->front());

			ConfigOptionMap& options = Root::getSingleton().getRenderSystem()->getConfigOptions();
			ConfigOptionMap::iterator floatingPoint = options.find("Floating-point mode");
			if (floatingPoint != options.end() && floatingPoint->second.currentValue != "Consistent") {
				floatingPoint->second.currentValue = "Consistent";
				LogManager::getSingleton().logMessage("NOTE forcing Floating-point mode to be Consistent");
			}

			g_unk0x0096134c->initialise(false, "OGRE Render Window");
		}

		m_unk0x7f0 = FUN_00671cf0();

		ConfigOptionMap& options = Root::getSingleton().getRenderSystem()->getConfigOptions();

		char name[8];
		_snprintf(name, sizeof(name), "win%d", g_unk0x0096135c++);

		NameValuePairList params;
		params["externalWindowHandle"] = StringConverter::toString(window);

		bool has32Bit = false;
		ConfigOptionMap::iterator videoMode = options.find("Video Mode");
		if (videoMode != options.end()) {
			for (int i = 0; i < (int) videoMode->second.possibleValues.size(); i++) {
				has32Bit = StringUtil::match(videoMode->second.possibleValues[i], "*32-bit*", false);
				if (has32Bit) {
					break;
				}
			}
		}
		if (!has32Bit) {
			params["colourDepth"] = 16;
			LogManager::getSingleton().logMessage("RBX warning 16-bit color");
		}

		if (antialiasing == 0 || antialiasing == 1) {
			ConfigOptionMap::iterator antialias = options.find("Anti aliasing");
			int best = -1;
			if (antialias != options.end()) {
				StringVector& values = antialias->second.possibleValues;
				for (int i = 0; i < (int) values.size(); i++) {
					if (values[i] == "NonMaskable 1") {
						if (best < 1) {
							best = 1;
						}
					}
					else if (values[i] == "NonMaskable 2") {
						if (best < 2) {
							best = 2;
						}
					}
					else if (values[i] == "NonMaskable 3") {
						if (best < 3) {
							best = 3;
						}
					}
					else if (values[i] == "NonMaskable 4") {
						if (best < 4) {
							best = 4;
						}
					}
				}
			}
			if (best > 0) {
				params["FSAA"] = "1";
				params["FSAAQuality"] = "0";
				LogManager::getSingleton().logMessage("RBX antiliasing is on\n");
				m_unk0x800 = 1;
			}
			else {
				LogManager::getSingleton().logMessage("RBX antiliasing is off\n");
				m_unk0x800 = 2;
			}
		}
		else if (antialiasing == 2) {
			m_unk0x800 = antialiasing;
		}

		try {
			m_unk0x0c = g_unk0x0096134c->createRenderWindow(name, width, height, false, &params);
		}
		catch (Exception&) {
			LogManager::getSingleton().logMessage("Error createRenderWindow.\n");
			throw std::runtime_error("Cannot open window.");
		}

		if (renderSystem == 0) {
			try {
				IDirect3DSurface9* depthBuffer;
				m_unk0x0c->getCustomAttribute("D3DZBUFFER", &depthBuffer);
				D3DSURFACE_DESC desc;
				if (FAILED(depthBuffer->GetDesc(&desc))) {
					m_unk0x7fc = 24;
				}
				else {
					switch (desc.Format) {
					case D3DFMT_D16_LOCKABLE:
					case D3DFMT_D16:
						m_unk0x7fc = 16;
						break;
					case D3DFMT_D32:
					case D3DFMT_D32F_LOCKABLE:
						m_unk0x7fc = 32;
						break;
					case D3DFMT_D15S1:
						m_unk0x7fc = 15;
						break;
					case D3DFMT_D24S8:
					case D3DFMT_D24X8:
					case D3DFMT_D24X4S4:
					case D3DFMT_D24FS8:
						m_unk0x7fc = 24;
						break;
					default:
						m_unk0x7fc = 32;
						LogManager::getSingleton().logMessage("RBX unknown zbuffer format");
						break;
					}
				}
			}
			catch (...) {
				m_unk0x7fc = 24;
			}
		}
		else {
			m_unk0x7fc = -1;
		}
		LogManager::getSingleton().logMessage("RBX zbuffer bits " + StringConverter::toString(m_unk0x7fc));

		unsigned int maxVertexIndex = 0xffffffff;
		m_unk0x804 = false;
		m_unk0x805 = false;
		m_unk0x806 = true;
		if (renderSystem == 0) {
			try {
				IDirect3DDevice9* device;
				m_unk0x0c->getCustomAttribute("D3DDEVICE", &device);
				D3DCAPS9 caps;
				device->GetDeviceCaps(&caps);
				maxVertexIndex = caps.MaxVertexIndex;
				LogManager::getSingleton().logMessage(
					"RBX maxVertexIndex=" + StringConverter::toString(maxVertexIndex)
				);
				m_unk0x804 = device->GetSoftwareVertexProcessing() != FALSE;
				LogManager::getSingleton().logMessage(
					"RBX softwareVertexProcessing=" + StringConverter::toString(m_unk0x804)
				);
				m_unk0x805 = (caps.TextureCaps & D3DPTEXTURECAPS_SQUAREONLY) != 0;
				LogManager::getSingleton().logMessage("RBX textureSquareOnly=" + StringConverter::toString(m_unk0x805));
				m_unk0x806 = (caps.RasterCaps & D3DPRASTERCAPS_DEPTHBIAS) != 0;
				LogManager::getSingleton().logMessage("RBX supportDepthBias=" + StringConverter::toString(m_unk0x806));
			}
			catch (...) {
			}
		}

		ConfigOptionMap::iterator renderingDevice = options.find("Rendering Device");
		std::string deviceName;
		if (renderingDevice == options.end()) {
			deviceName = "Unknown";
		}
		else {
			deviceName = renderingDevice->second.currentValue;
		}
		LogManager::getSingleton().logMessage("RBX device=" + deviceName);
		LogManager::getSingleton().logMessage("RBX vidMemSize=" + StringConverter::toString(m_unk0x7f0));

		bool smallVertexBuffer = maxVertexIndex < 0x10000 || StringUtil::match(deviceName, "*Intel*", false) ||
								 StringUtil::match(deviceName, "*GeForce*MX*", false) ||
								 (m_unk0x7f0 & 0xfff00000) < 0x2000000;
		if (smallVertexBuffer) {
			LogManager::getSingleton().logMessage("RBX useLargeVertexBuffer=false");
			m_unk0x7f5 = false;
			m_unk0x7f4 = false;
		}
		else {
			LogManager::getSingleton().logMessage("RBX useLargeVertexBuffer=true");
			m_unk0x7f5 = true;
			m_unk0x7f4 = true;
		}
		if (m_unk0x7f8 == 0) {
			m_unk0x7f5 = false;
			m_unk0x7f4 = false;
		}
		else if (m_unk0x7f8 == 1) {
			m_unk0x7f5 = true;
			m_unk0x7f4 = true;
		}

		bool savage = StringUtil::match(deviceName, "*Savage*", true) && !m_unk0x805;
		if (savage) {
			LogManager::getSingleton().logMessage("RBX Savage lies");
			m_unk0x805 = true;
		}

		String sceneManagerName = String("sm") + StringConverter::toString(g_unk0x00961358++);
		m_unk0x04 =
			dynamic_cast<RbxSceneManager*>(g_unk0x0096134c->createSceneManager("RbxSceneManager", sceneManagerName));
		m_unk0x08 = m_unk0x04->createCamera("Camera");
		m_unk0x08->setAutoAspectRatio(true);
		m_unk0x08->setNearClipDistance(5.0f);
		m_unk0x10 = m_unk0x0c->addViewport(m_unk0x08, 0, 0.0f, 0.0f, 1.0f, 1.0f);
		m_unk0x10->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f));
		m_unk0x08->setAspectRatio(Real(width) / height);
		ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		if (m_unk0x7f0 > 0) {
			TextureManager::getSingleton().setMemoryBudget(m_unk0x7f0);
		}
		m_unk0x14 = new TwoDManager();
		m_unk0x14->FUN_0067c6c0(m_unk0x04, m_unk0x0c, RENDER_QUEUE_OVERLAY, true, false);
	}
	catch (Exception& e) {
		throw std::runtime_error(e.getFullDescription().c_str());
	}
}

} // namespace Ogre
