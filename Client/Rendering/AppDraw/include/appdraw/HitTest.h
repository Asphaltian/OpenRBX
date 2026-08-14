#ifndef APPDRAW_HITTEST_H
#define APPDRAW_HITTEST_H

#include <G3D/Ray.h>
#include <G3D/Vector3.h>

namespace RBX {

class Part;

class HitTest
{
	static bool hitTestBox(
		const Part& part,
		G3D::Ray& rayInPartCoords,
		G3D::Vector3& hitPointInPartCoords,
		float gridToReal
	);

	static bool hitTestBall(
		const Part& part,
		G3D::Ray& rayInPartCoords,
		G3D::Vector3& hitPointInPartCoords,
		float gridToReal
	);

	static bool hitTestCylinder(
		const Part& part,
		G3D::Ray& rayInPartCoords,
		G3D::Vector3& hitPointInPartCoords,
		float gridToReal
	);

public:
	static bool hitTest(
		const Part& part,
		G3D::Ray& rayInPartCoords,
		G3D::Vector3& hitPointInPartCoords,
		float gridToReal
	);
};

} // namespace RBX

#endif // APPDRAW_HITTEST_H
