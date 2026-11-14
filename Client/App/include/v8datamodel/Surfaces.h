#ifndef V8DATAMODEL_SURFACES_H
#define V8DATAMODEL_SURFACES_H

#include "placeholder.h"
#include "util/NormalId.h"
#include "util/Vector6.h"
#include "v8datamodel/Surface.h"

#include <boost/noncopyable.hpp>

namespace RBX {

namespace Reflection {
class PropertyDescriptor;
}

class Surfaces : public boost::noncopyable
{
private:
	Surface Top;
	Surface Bottom;
	Surface Left;
	Surface Right;
	Surface Front;
	Surface Back;

public:
	static const bool isSurfaceDescriptor(const Reflection::PropertyDescriptor& desc);

	Surfaces(PartInstance* partInstance);

	Surface& operator[](NormalId normalId);
	const Surface& operator[](NormalId normalId) const;

	Vector6<SurfaceType> surf6() const
	{
		Vector6<SurfaceType> answer;

		for (int i = 0; i < 6; i++) {
			answer[i] = (*this)[static_cast<NormalId>(i)].getSurfaceType();
		}

		return answer;
	}

	const Reflection::PropertyDescriptor& getSurfaceType(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getSurfaceInput(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getParamA(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getParamB(NormalId normalId) const;

	const bool isStandardPart() const;
};

} // namespace RBX

#endif // V8DATAMODEL_SURFACES_H
