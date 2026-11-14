#ifndef UTIL_FRAMEINTERPOLATOR_H
#define UTIL_FRAMEINTERPOLATOR_H

#include <G3D/CoordinateFrame.h>

namespace RBX {

class FrameInterpolator
{
private:
	static int currentStep;
	static int currentFrame;
	static float alpha;
	static double nextStepTime;
	static unsigned long renderThread;
	static bool extraFrame;

	G3D::CoordinateFrame previous;
	G3D::CoordinateFrame latest;
	G3D::CoordinateFrame blended;
	int step;
	int frame;
	bool sampled;

	static bool isRenderThread();

public:
	FrameInterpolator();

	static void beginRender(int step, double stepTime, double period);
	static void endRender();

	static bool isRendering() { return renderThread != 0 && isRenderThread(); }

	static void setExtraFrame(bool value) { extraFrame = value; }
	static bool isExtraFrame() { return extraFrame; }

	static double getNextStepTime() { return nextStepTime; }

	const G3D::CoordinateFrame& sample(const G3D::CoordinateFrame& current);
};

} // namespace RBX

#endif // UTIL_FRAMEINTERPOLATOR_H
