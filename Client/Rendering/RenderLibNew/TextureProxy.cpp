#include "renderlibnew/TextureProxy.h"

#include "appdraw/VisualEngine.h"
#include "decomp.h"
#include "util/standardout.h"

#include <G3D/GImage.h>
#include <G3D/format.h>
#include <OgreDataStream.h>
#include <OgreImage.h>
#include <OgreLogManager.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>
#include <OgreTextureManager.h>
#include <algorithm>
#include <fstream>

namespace RBX {
namespace RenderNew {

void TextureProxy::shadePixel(const G3D::Color3uint8& base, G3D::Color3uint8& lighting)
{
	for (int i = 0; i < 3; i++) {
		const int shade = lighting[i];
		float color = base[i];

		if (shade > 130) {
			float a = std::min(float((shade - 128.0) / 128.0), 1.0f);
			color = (1.0 - a) * color + 255.0 * a;
		}
		else if (shade < 126) {
			float a = shade / 128.0;
			a = 0.5 + a * 0.5;
			color = color * a;
		}

		lighting[i] = std::min(255, std::max(0, G3D::iRound(255.0 * pow(color / 255.0, 1.0))));
	}
}

// FUNCTION: ROBLOX 0x005345a0
TextureProxyRef TextureProxy::shade(const G3D::Color3& baseColor, const TextureProxyRef& baseTex)
{
	TextureProxy* answer = new TextureProxy(baseTex);

	answer->textureManager = textureManager;
	answer->root = baseTex;
	answer->shadeColor = baseColor;
	answer->shadeTexture = this;

	return answer;
}

G3D::ReferenceCountedPointer<G3D::Texture> TextureProxy::getIfResolved() const
{
	return texture;
}

// FUNCTION: ROBLOX 0x00534170
static int FUN_00534170(int value)
{
	if (value < 2) {
		return 2;
	}

	int result = 1;
	while (result < value) {
		result += result;
	}
	return result;
}

// STUB: ROBLOX 0x00534190
static void FUN_00534190(
	int components,
	unsigned int width,
	unsigned int height,
	const unsigned char* in,
	unsigned char* out,
	int elementSize,
	int rowSize,
	int groupSize
)
{
	int halfWidth = width / 2;
	int halfHeight = height / 2;
	const unsigned char* src = in;
	unsigned char* dest = out;

	if (height == 1) {
		for (int jj = 0; jj < halfWidth; jj++) {
			for (int kk = 0; kk < components; kk++) {
				*dest = (src[0] + src[groupSize]) / 2;
				src += elementSize;
				dest++;
			}
			src += groupSize;
		}
	}
	else if (width == 1) {
		int padBytes = rowSize - width * groupSize;
		for (int jj = 0; jj < halfHeight; jj++) {
			for (int kk = 0; kk < components; kk++) {
				*dest = (src[0] + src[rowSize]) / 2;
				src += elementSize;
				dest++;
			}
			src += padBytes;
			src += rowSize;
		}
	}
}

// STUB: ROBLOX 0x00534250
static void FUN_00534250(
	unsigned int width,
	unsigned int height,
	int components,
	const unsigned char* in,
	unsigned char* out,
	int elementSize,
	int rowSize,
	int groupSize
)
{
	if (width == 1 || height == 1) {
		FUN_00534190(components, width, height, in, out, elementSize, rowSize, groupSize);
		return;
	}

	int newWidth = width / 2;
	int newHeight = height / 2;
	int padBytes = rowSize - width * groupSize;
	unsigned char* s = out;
	const unsigned char* t = in;

	for (int i = 0; i < newHeight; i++) {
		for (int j = 0; j < newWidth; j++) {
			for (int k = 0; k < components; k++) {
				s[0] = (t[0] + t[groupSize] + t[rowSize] + t[rowSize + groupSize] + 2) / 4;
				s++;
				t += elementSize;
			}
			t += groupSize;
		}
		t += padBytes;
		t += rowSize;
	}
}

static bool g_unk0x009587d0 = false;
static std::string g_unk0x009587d4 = "shadetex";

// STUB: ROBLOX 0x00534340
G3D::Vector2 TextureProxy::getSize()
{
	G3D::Vector2 size(0.0f, 0.0f);

	if (!m_unk0x4c.isNull()) {
		size.x = m_unk0x4c->getWidth();
		size.y = m_unk0x4c->getHeight();
	}

	return size;
}

// FUNCTION: ROBLOX 0x00534740
TextureProxy::~TextureProxy()
{
	if (!m_unk0x4c.isNull() && m_unk0x4c->getName() == g_unk0x009587d4) {
		m_unk0x4c.setNull();
		Ogre::ResourceGroupManager::getSingleton().unloadUnreferencedResourcesInGroup(g_unk0x009587d4, true);
		Ogre::StringVectorPtr names =
			Ogre::ResourceGroupManager::getSingleton().listResourceNames(g_unk0x009587d4, false);
		Ogre::LogManager::getSingleton().logMessage(
			"RBX shadetex remaining: " + Ogre::StringConverter::toString(names->size())
		);
	}
}

// STUB: ROBLOX 0x00534d20
G3D::ReferenceCountedPointer<G3D::Texture> TextureProxy::resolve(G3D::RenderDevice* rd)
{
	Ogre::VisualEngine* engine = reinterpret_cast<Ogre::VisualEngine*>(rd);

	if (!loaded) {
		if (!shadeTexture.isNull()) {
			if (!g_unk0x009587d0) {
				Ogre::ResourceGroupManager::getSingleton().createResourceGroup(g_unk0x009587d4);
				Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(g_unk0x009587d4);
				g_unk0x009587d0 = true;
			}

			if (!m_unk0x4c.isNull()) {
				Ogre::LogManager::getSingleton().logMessage("RBX Texture " + m_unk0x4c->getName() + " overwritten");
			}

			char encodedFileName[1024];
			sprintf(
				encodedFileName, "%g%g%g%s", shadeColor.r, shadeColor.g, shadeColor.b, shadeTexture->filename.c_str()
			);

			m_unk0x4c = Ogre::TextureManager::getSingleton().getByName(std::string(encodedFileName));
			if (!m_unk0x4c.isNull()) {
				root = NULL;
				shadeTexture = NULL;
				loaded = true;
				return texture;
			}

			static std::string lastFilename = shadeTexture->filename;
			static G3D::GImage cachedBumpImage(lastFilename, G3D::GImage::AUTODETECT);

			Ogre::Image bumpImage;
			bumpImage.load(shadeTexture->filename, "General");

			G3D::Color3uint8 colorTable[256];
			for (int i = 0; i < 256; i++) {
				colorTable[i].r = colorTable[i].g = colorTable[i].b = i;
				shadePixel(G3D::Color3uint8(shadeColor), colorTable[i]);
			}

			int numPixels = bumpImage.getHeight() * bumpImage.getWidth();
			for (int i = 0; i < numPixels; i++) {
				unsigned char* pixel = bumpImage.getData() + i * 3;
				const G3D::Color3uint8& shaded = colorTable[pixel[0]];
				pixel[0] = shaded.r;
				pixel[1] = shaded.g;
				pixel[2] = shaded.b;
			}

			Ogre::Image shadedImage;
			std::vector<unsigned char> mipData;
			if (!engine->m_unk0x805) {
				int width = bumpImage.getWidth();
				int height = bumpImage.getHeight();

				unsigned int size = (width > height ? width : height) - 1;
				size |= size >> 16;
				size |= size >> 8;
				size |= size >> 4;
				size |= size >> 2;
				size |= size >> 1;
				size++;

				int numMipmaps = 0;
				if (size != 0) {
					while (!(size & 1)) {
						size >>= 1;
						numMipmaps++;
					}
				}

				mipData.resize(Ogre::Image::calculateSize(numMipmaps, 1, width, height, 1, Ogre::PF_BYTE_RGB), 0);

				unsigned char* dst = &mipData[0];
				unsigned char* prev = NULL;
				int prevWidth = -1;
				int prevHeight = -1;
				for (int level = 0; level <= numMipmaps; level++) {
					if (level == 0) {
						memcpy(&mipData[0], bumpImage.getData(), width * height * 3);
					}
					else {
						FUN_00534250(prevWidth, prevHeight, 3, prev, dst, 1, prevWidth * 3, 3);
					}

					prev = dst;
					dst += width * height * 3;
					prevWidth = width;
					prevHeight = height;

					if (width > 1) {
						width /= 2;
					}
					if (height > 1) {
						height /= 2;
					}
				}

				shadedImage.loadDynamicImage(
					&mipData[0], bumpImage.getWidth(), bumpImage.getHeight(), 1, Ogre::PF_BYTE_RGB, false, 1, numMipmaps
				);
			}
			else {
				shadedImage.loadDynamicImage(
					bumpImage.getData(), bumpImage.getWidth(), bumpImage.getHeight(), 1, Ogre::PF_BYTE_RGB, false, 1, 0
				);
			}

			std::string name(encodedFileName);
			m_unk0x4c = Ogre::TextureManager::getSingleton().create(name, g_unk0x009587d4);
			m_unk0x4c->loadImage(shadedImage);

			root = NULL;
			loaded = true;

			Ogre::LogManager::getSingleton().logMessage("create shadetex=" + m_unk0x4c->getName() + "\n");
			Ogre::ResourceGroupManager::getSingleton().unloadUnreferencedResourcesInGroup(g_unk0x009587d4, true);
		}
		else if (root.isNull()) {
			if (filename == "") {
				Ogre::LogManager::getSingleton().logMessage("filename is empty in resolve()\n");
			}
			else {
				Ogre::Image image;
				try {
					m_unk0x4c = Ogre::TextureManager::getSingleton().getByName(filename);
					if (m_unk0x4c.isNull()) {
						const char* extensions[] = {
							"png", "jpg", "bmp", "gif", "tif", "tiff", "pbm", "pgm", "tga", "dds", "xpm", "jig", "ico"
						};
						int count = 13;

						bool found = false;
						for (int i = 0; i < 13; i++) {
							size_t length = strlen(extensions[i]);
							std::string extension = std::string(".") + extensions[i];
							found = filename.substr(filename.size() - length - 1).find(extension) != std::string::npos;
							if (found) {
								break;
							}
						}

						if (found) {
							image.load(filename, "General");
						}
						else {
							for (int i = 0; i < count; i++) {
								try {
									std::ifstream stream;
									stream.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
									Ogre::DataStreamPtr dataStream(new Ogre::FileStreamDataStream(&stream, false));
									image.load(dataStream, extensions[i]);
									found = true;
									break;
								}
								catch (...) {
									if (i == count - 1) {
										StandardOut::singleton()->print(
											MESSAGE_ERROR, "Failed to load texture '%s'", filename.c_str()
										);
										m_unk0x4c.setNull();
									}
								}
							}
						}

						if (found && image.getFormat() != Ogre::PF_UNKNOWN && image.getWidth() > 0 &&
							image.getHeight() > 0) {
							if (engine->m_unk0x800 == 1) {
								int width = FUN_00534170(image.getWidth());
								int height = FUN_00534170(image.getHeight());

								if (width != image.getWidth() || height != image.getHeight()) {
									Ogre::LogManager::getSingleton().logMessage(
										"Texture resized: (" + Ogre::StringConverter::toString(image.getWidth()) + "," +
										Ogre::StringConverter::toString(image.getHeight()) + ") -> (" +
										Ogre::StringConverter::toString(width) + "," +
										Ogre::StringConverter::toString(height) + ")"
									);
									image.resize(width, height, Ogre::Image::FILTER_BILINEAR);
								}
							}

							m_unk0x4c = Ogre::TextureManager::getSingleton().create(filename, "General");
							m_unk0x4c->loadImage(image);
							Ogre::LogManager::getSingleton().logMessage("create tex=" + m_unk0x4c->getName());
						}
					}
				}
				catch (std::exception& e) {
					m_unk0x4c.setNull();
					Ogre::LogManager::getSingleton().logMessage(
						"RBX failed to load " + filename + " what(): " + e.what()
					);
				}
			}
		}
		else {
			G3D::TextureRef t = root->resolve(rd);

			if (!t.isNull() && !t->opaque()) {
				texture = t->alphaOnlyVersion();
			}
		}

		if (m_unk0x4c.isNull()) {
			Ogre::LogManager::getSingleton().logMessage("RBX texture not loaded in resolve()\n");
		}

		root = NULL;
		shadeTexture = NULL;
		loaded = true;
	}

	return texture;
}

// FUNCTION: ROBLOX 0x005346a0
TextureProxyRef TextureProxy::alphaOnlyVersion()
{
	return new TextureProxy(this);
}

} // namespace RenderNew
} // namespace RBX
