#include "v8kernel/Cofm.h"

#include "decomp.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x101216f0
Cofm::Cofm(Body* body) : body(body), dirty(true), cofmInBody(0, 0, 0)
{
}

// STUB: WEBSERVICE 0x10121710
float Cofm::getMass()
{
	updateIfDirty();

	return mass;
}

// STUB: WEBSERVICE 0x10121720
void Cofm::updateIfDirty()
{
	STUB(0x10121720);
}

// STUB: WEBSERVICE 0x10121970
const Vector3& Cofm::getCofmInBody()
{
	updateIfDirty();

	return cofmInBody;
}

// STUB: WEBSERVICE 0x10121980
const Matrix3& Cofm::getMoment()
{
	updateIfDirty();

	return moment;
}

} // namespace RBX
