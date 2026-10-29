#include "ThumbnailGenerator.h"

#include "appdraw/AdornG3D.h"
#include "gui/GuiDraw.h"
#include "rbxgraphics/TextureProxyBase.h"
#include "rbxview/View.h"
#include "renderlib/Mesh.h"
#include "script/Script.h"
#include "script/ScriptContext.h"
#include "util/ContentProvider.h"
#include "util/Name.h"
#include "util/RunStateOwner.h"
#include "util/base64.hpp"
#include "util/standardout.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/Lighting.h"
#include "v8datamodel/ModelInstance.h"
#include "v8datamodel/Selection.h"
#include "v8datamodel/Workspace.h"

#include <G3D/GImage.h>
#include <GL/osmesa.h>
#include <GLG3D/GWindow.h>
#include <GLG3D/RenderDevice.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/once.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <memory>
#include <sstream>
#include <stdexcept>

const char sThumbnailGenerator[] = "ThumbnailGenerator";

template <class T>
class StaticMutex
{
	static boost::once_flag initFlag;
	static boost::mutex* m;

	static void init()
	{
		static boost::mutex _m;
		m = &_m;
	}

public:
	static boost::mutex& get()
	{
		boost::call_once(init, initFlag);
		return *m;
	}
};

template <class T>
boost::once_flag StaticMutex<T>::initFlag = BOOST_ONCE_INIT;
template <class T>
boost::mutex* StaticMutex<T>::m;

static RBX::Reflection::BoundFuncDesc<ThumbnailGenerator, std::string(std::string, int, int, bool), 4> clickFunction(
	&ThumbnailGenerator::click,
	"Click",
	"fileType",
	"width",
	"height",
	"hideSky",
	RBX::Reflection::FunctionDescriptor::NeedTrustedCaller
);

static RBX::Reflection::BoundFuncDesc<ThumbnailGenerator, std::string(std::string, std::string, int, int), 4>
	clickTextureFunction(
		&ThumbnailGenerator::clickTexture,
		"ClickTexture",
		"textureId",
		"fileType",
		"width",
		"height",
		RBX::Reflection::FunctionDescriptor::NeedTrustedCaller
	);

// FUNCTION: WEBSERVICE 0x10006470
ThumbnailGenerator::ThumbnailGenerator()
{
}

// SIZE 0x34
class OSContext : public G3D::GWindow
{
private:
	OSMesaContext ctx; // 0x1c

	// FUNCTION: WEBSERVICE 0x100064d0
	OSContext(bool alphaChannel)
	{
		ctx = OSMesaCreateContext(alphaChannel ? OSMESA_RGBA : OSMESA_RGB, NULL);
		if (ctx == NULL) {
			throw std::runtime_error("shared OSMesaCreateContext failed");
		}
	}

	// STUB: WEBSERVICE 0x100065a0
	static OSMesaContext shared(bool alphaChannel)
	{
		if (alphaChannel) {
			static OSContext shared(true);
			return shared.ctx;
		}
		else {
			static OSContext shared(false);
			return shared.ctx;
		}
	}

public:
	G3D::GImage image; // 0x20

	// FUNCTION: WEBSERVICE 0x10006660
	OSContext(int cx, int cy, bool alphaChannel) : image(cx, cy, alphaChannel ? 4 : 3)
	{
		ctx = OSMesaCreateContext(alphaChannel ? OSMESA_RGBA : OSMESA_RGB, shared(alphaChannel));
		if (ctx == NULL) {
			throw std::runtime_error("OSMesaCreateContext failed");
		}
	}

	// STUB: WEBSERVICE 0x10006740
	virtual ~OSContext() { OSMesaDestroyContext(ctx); }

	// FUNCTION: WEBSERVICE 0x100067d0
	virtual void reallyMakeCurrent() const
	{
		if (!OSMesaMakeCurrent(
				ctx, const_cast<G3D::uint8*>(image.byte()), GL_UNSIGNED_BYTE, image.width, image.height
			)) {
			throw std::runtime_error("OSMesaMakeCurrent failed");
		}
	}

	// FUNCTION: WEBSERVICE 0x10006860
	virtual void getSettings(G3D::GWindow::Settings& settings) const { settings = G3D::GWindow::Settings(); }

	// FUNCTION: WEBSERVICE 0x100068d0
	virtual int width() const { return image.width; }

	// FUNCTION: WEBSERVICE 0x100068e0
	virtual int height() const { return image.height; }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual G3D::Rect2D dimensions() const { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void setDimensions(const G3D::Rect2D& dims) { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void setPosition(int x, int y) { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual bool hasFocus() const { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual std::string getAPIVersion() const { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual std::string getAPIName() const { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x100068f0 FOLDED
	virtual void setGammaRamp(const G3D::Array<G3D::uint16>& gammaRamp) {}

	// FUNCTION: WEBSERVICE 0x100068f0 FOLDED
	virtual void setCaption(const std::string& caption) {}

	// FUNCTION: WEBSERVICE 0x1002b5f0 FOLDED
	virtual int numJoysticks() const { return 0; }

	// FUNCTION: WEBSERVICE 0x10006900
	virtual std::string joystickName(unsigned int) { return ""; }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual std::string caption() { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void swapGLBuffers() { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void notifyResize(int w, int h) { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void setRelativeMousePosition(double x, double y)
	{
		throw std::runtime_error("OSContext unimplemented method");
	}

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void setRelativeMousePosition(const G3D::Vector2& p)
	{
		throw std::runtime_error("OSContext unimplemented method");
	}

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void getRelativeMouseState(G3D::Vector2& position, G3D::uint8& mouseButtons) const
	{
		throw std::runtime_error("OSContext unimplemented method");
	}

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void getRelativeMouseState(int& x, int& y, G3D::uint8& mouseButtons) const
	{
		throw std::runtime_error("OSContext unimplemented method");
	}

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void getRelativeMouseState(double& x, double& y, G3D::uint8& mouseButtons) const
	{
		throw std::runtime_error("OSContext unimplemented method");
	}

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void getJoystickState(unsigned int stickNum, G3D::Array<float>& axis, G3D::Array<bool>& button)
	{
		throw std::runtime_error("OSContext unimplemented method");
	}

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void setInputCapture(bool c) { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10001cc0 FOLDED
	virtual bool inputCapture() const { return false; }

	// FUNCTION: WEBSERVICE 0x10006930 FOLDED
	virtual void setMouseVisible(bool b) { throw std::runtime_error("OSContext unimplemented method"); }

	// FUNCTION: WEBSERVICE 0x10001cc0 FOLDED
	virtual bool mouseVisible() const { return false; }
};

DECOMP_SIZE_ASSERT(OSContext, 0x34)

// FUNCTION: WEBSERVICE 0x10006a50
static void waitForContent()
{
	while (!RBX::ContentProvider::singleton().isRequestQueueEmpty()) {
		boost::xtime xt;
		boost::xtime_get(&xt, boost::TIME_UTC);
		boost::thread::sleep(xt);
	}
}

// STUB: WEBSERVICE 0x10006af0
std::string ThumbnailGenerator::clickTexture(std::string textureId, std::string fileType, int cx, int cy)
{
	try {
		RBX::StandardOut::singleton()->print(
			RBX::MESSAGE_INFO,
			"ThumbnailGenerator::clickTexture(%s, %s, %d, %d)",
			textureId.c_str(),
			fileType.c_str(),
			cx,
			cy
		);

		RBX::ContentProvider::singleton().getContent(textureId);

		G3D::BinaryOutput binaryOutput;
		binaryOutput.setEndian(G3D::G3D_LITTLE_ENDIAN);

		G3D::GImage::Format format = G3D::GImage::stringToFormat(fileType);

		OSContext context(cx, cy, true);

		boost::mutex::scoped_lock lock(StaticMutex<OSContext>::get());

		context.makeCurrent();

		RBX::Render::Mesh::makeVAR();

		{
			RBX::Render::Mesh::varDirty = true;
			G3D::RenderDevice renderDevice;
			renderDevice.init(&context);
			renderDevice.pushState();
			renderDevice.push2D();

			renderDevice.setProjectionMatrix(G3D::Matrix4::orthogonalProjection(0, cx, 0, cy, -1, 1));

			{
				RBX::AdornG3D adorn(&renderDevice);
				RBX::GuiDrawImage image;
				if (image.setImage(&adorn, RBX::TextureId(textureId))) {
					RBX::Rect rect(0, 0, cx, cy);
					image.render2d(&adorn, true, rect, RBX::Widget::NOTHING);
				}
			}

			context.image.encode(format, binaryOutput);

			renderDevice.pop2D();
			renderDevice.popState();
			renderDevice.cleanup();
		}

		int size = binaryOutput.size();
		std::string data;
		data.resize(size);
		memcpy(const_cast<char*>(data.c_str()), binaryOutput.getCArray(), size);

		std::stringstream stream;

		std::ostreambuf_iterator<char> out(stream);
		int state = 0;
		base64<char>().put(data.begin(), data.end(), out, state, base64<char>::noline());

		RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "ThumbnailGenerator::click() success");

		return stream.str();
	}
	catch (std::exception& e) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, e.what());
		throw;
	}
}

// STUB: WEBSERVICE 0x10007170
std::string ThumbnailGenerator::click(std::string fileType, int cx, int cy, bool hideSky)
{
	try {
		RBX::StandardOut::singleton()->print(
			RBX::MESSAGE_INFO,
			"ThumbnailGenerator::click(%s, %d, %d, %s)",
			fileType.c_str(),
			cx,
			cy,
			hideSky ? "true" : "false"
		);

		G3D::BinaryOutput binaryOutput;
		binaryOutput.setEndian(G3D::G3D_LITTLE_ENDIAN);

		doClick(fileType, cx, cy, hideSky, NULL);

		waitForContent();

		doClick(fileType, cx, cy, hideSky, &binaryOutput);

		int size = binaryOutput.size();
		std::string data;
		data.resize(size);
		memcpy(const_cast<char*>(data.c_str()), binaryOutput.getCArray(), size);

		std::stringstream stream;

		std::ostreambuf_iterator<char> out(stream);
		int state = 0;
		base64<char>().put(data.begin(), data.end(), out, state, base64<char>::noline());

		RBX::StandardOut::singleton()->print(RBX::MESSAGE_INFO, "ThumbnailGenerator::click() success");

		return stream.str();
	}
	catch (std::exception& e) {
		RBX::StandardOut::singleton()->print(RBX::MESSAGE_ERROR, e.what());
		throw;
	}
}

// STUB: WEBSERVICE 0x10007470
void ThumbnailGenerator::doClick(std::string fileType, int cx, int cy, bool hideSky, G3D::BinaryOutput* binaryOutput)
{
	G3D::GImage::Format format = G3D::GImage::stringToFormat(fileType);
	bool alphaChannel = false;

	if (hideSky) {
		RBX::Lighting* lighting = RBX::ServiceProvider::create<RBX::Lighting>(this);
		lighting->suppressSky(true);
		if (format == G3D::GImage::PNG) {
			alphaChannel = true;
			lighting->setClearColor(G3D::Color4(0, 0, 0, 0));
		}
		else {
			lighting->setClearColor(G3D::Color4(1, 1, 1, 1));
		}

		RBX::ServiceProvider::find<RBX::Workspace>(this)->setImageServerView(G3D::Rect2D::xywh(0, 0, cx, cy));
	}

	OSContext context(cx, cy, alphaChannel);

	boost::mutex::scoped_lock lock(StaticMutex<OSContext>::get());

	context.makeCurrent();

	RBX::Render::Mesh::makeVAR();

	RBX::Render::Mesh::varDirty = true;
	G3D::RenderDevice renderDevice;
	renderDevice.init(&context);

	std::auto_ptr<RBX::ViewBase> rbxView(
		new RBX::View::View(RBX::shared_from(static_cast<RBX::DataModel*>(getParent())))
	);

	rbxView->updateSettings(100, 100, false, 10);

	glMatrixMode(GL_MODELVIEW);
	rbxView->render(&renderDevice);
	glMatrixMode(GL_MODELVIEW);

	if (alphaChannel) {
		G3D::GImage::flipRGBAVertical(
			context.image.byte(), context.image.byte(), context.image.width, context.image.height
		);
	}
	else {
		G3D::GImage::flipRGBVertical(
			context.image.byte(), context.image.byte(), context.image.width, context.image.height
		);
	}

	double s = renderDevice.getLightSaturation();
	if (s != 1.0) {
		G3D::uint8 L[256];
		for (int i = 255; i >= 0; --i) {
			L[i] = G3D::iMin(G3D::iRound(i * s), 255);
		}

		int N = context.image.width * context.image.height * context.image.channels;
		int stride = alphaChannel ? 4 : 3;
		for (int i = 0; i < N; i += stride) {
			for (int c = 0; c < 3; ++c) {
				context.image.byte()[i + c] = L[context.image.byte()[i + c]];
			}
		}
	}

	if (binaryOutput) {
		context.image.encode(format, *binaryOutput);
	}
	else if (hideSky) {
		RBX::ServiceProvider::find<RBX::Workspace>(this)->setImageServerView(G3D::Rect2D::xywh(0, 0, cx, cy));
	}

	renderDevice.cleanup();
}

template void RBX::Name::callDoDeclare<sThumbnailGenerator>();
template const RBX::Name& RBX::Name::doDeclare<sThumbnailGenerator>();
template void RBX::Name::callDoDeclare<RBX::sLighting>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLighting>();
template void RBX::Name::callDoDeclare<RBX::sRunService>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sRunService>();
template void RBX::Name::callDoDeclare<RBX::sModel>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sModel>();
template void RBX::Name::callDoDeclare<RBX::sSelection>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sSelection>();
template void RBX::Name::callDoDeclare<RBX::sScriptContext>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sScriptContext>();
template void RBX::Name::callDoDeclare<RBX::sScript>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sScript>();
template void RBX::Name::callDoDeclare<RBX::sLocalScript>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sLocalScript>();
template void RBX::Name::callDoDeclare<RBX::sWorkspace>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sWorkspace>();

template const RBX::FactoryProduct<ThumbnailGenerator, RBX::Instance, sThumbnailGenerator>::Creator
	RBX::FactoryProduct<ThumbnailGenerator, RBX::Instance, sThumbnailGenerator>::creator;
template const RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting>::Creator
	RBX::FactoryProduct<RBX::Lighting, RBX::Instance, RBX::sLighting>::creator;
template const RBX::FactoryProduct<RBX::RunService, RBX::Instance, RBX::sRunService>::Creator
	RBX::FactoryProduct<RBX::RunService, RBX::Instance, RBX::sRunService>::creator;
template const RBX::FactoryProduct<RBX::ModelInstance, RBX::PVInstance, RBX::sModel>::Creator
	RBX::FactoryProduct<RBX::ModelInstance, RBX::PVInstance, RBX::sModel>::creator;
template const RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>::Creator
	RBX::FactoryProduct<RBX::Selection, RBX::Instance, RBX::sSelection>::creator;
template const RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>::Creator
	RBX::FactoryProduct<RBX::ScriptContext, RBX::Instance, RBX::sScriptContext>::creator;
template const RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript>::Creator
	RBX::FactoryProduct<RBX::Script, RBX::Instance, RBX::sScript>::creator;
template const RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript>::Creator
	RBX::FactoryProduct<RBX::LocalScript, RBX::Script, RBX::sLocalScript>::creator;

typedef std::vector<boost::shared_ptr<RBX::Instance> > InstanceVector;
template InstanceVector::size_type InstanceVector::size() const;
template InstanceVector::iterator InstanceVector::begin();
template InstanceVector::iterator InstanceVector::end();
template InstanceVector::iterator InstanceVector::erase(InstanceVector::iterator);
template void InstanceVector::resize(InstanceVector::size_type);

template RBX::Lighting* RBX::ServiceProvider::find<RBX::Lighting>() const;

template boost::shared_ptr<RBX::Instance>::~shared_ptr();
template boost::shared_ptr<RBX::ModelInstance>::~shared_ptr();
template boost::shared_ptr<RBX::Selection>::~shared_ptr();
template boost::shared_ptr<RBX::RunService>::~shared_ptr();
template boost::shared_ptr<RBX::Script>::~shared_ptr();
template boost::shared_ptr<RBX::ScriptContext>::~shared_ptr();
template boost::shared_ptr<RBX::Lighting>::~shared_ptr();

template G3D::ReferenceCountedPointer<G3D::Texture>::~ReferenceCountedPointer();
template G3D::ReferenceCountedPointer<RBX::TextureProxyBase>::~ReferenceCountedPointer();
