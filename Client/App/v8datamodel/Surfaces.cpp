#include "v8datamodel/Surfaces.h"

#include "reflection/property.h"
#include "v8datamodel/PartInstance.h"
#include "v8world/Controller.h"

#include <memory>

namespace RBX {

// SIZE 0x20
template <int normalId, class T>
class SurfaceEnumPropDescriptor : public Reflection::EnumPropertyDescriptor
{
public:
	// SIZE 0x0c
	template <class Getter, class Setter>
	class GetSetImpl : public Reflection::TypedPropertyDescriptor<T>::GetSet
	{
	public:
		GetSetImpl(Getter getter, Setter setter) : getter(getter), setter(setter) {}

		virtual bool isReadOnly() const { return false; }

		virtual T getValue(const Reflection::DescribedBase* instance) const
		{
			const Surfaces& surfaces = static_cast<const PartInstance*>(instance)->getSurfaces();

			return (surfaces[static_cast<NormalId>(normalId)].*getter)();
		}

		virtual void setValue(Reflection::DescribedBase* instance, const T& value) const
		{
			Surfaces& surfaces = static_cast<PartInstance*>(instance)->getSurfaces();

			(surfaces[static_cast<NormalId>(normalId)].*setter)(value);
		}

	private:
		Getter getter; // 0x04
		Setter setter; // 0x08
	};

	template <class Getter, class Setter>
	SurfaceEnumPropDescriptor(
		const char* name,
		const char* category,
		Getter getter,
		Setter setter,
		Reflection::PropertyDescriptor::Functionality functionality = Reflection::PropertyDescriptor::STANDARD
	)
		: Reflection::EnumPropertyDescriptor(
			  PartInstance::classDescriptor(),
			  Reflection::EnumDesc<T>::singleton(),
			  name,
			  category,
			  functionality
		  ),
		  getset(new GetSetImpl<Getter, Setter>(getter, setter))
	{
	}

	virtual bool isReadOnly() const { return getset->isReadOnly(); }

	T getValue(const Reflection::DescribedBase* instance) const { return getset->getValue(instance); }

	void setValue(Reflection::DescribedBase* instance, const T& value) const { getset->setValue(instance, value); }

	virtual bool equalValues(const Reflection::DescribedBase* a, const Reflection::DescribedBase* b) const
	{
		return getValue(a) == getValue(b);
	}

	virtual bool hasStringValue() const { return true; }

	virtual std::string getStringValue(const Reflection::DescribedBase* instance) const
	{
		STUB(0x10102460);
		return std::string();
	}

	virtual bool setStringValue(Reflection::DescribedBase* instance, const std::string& value) const
	{
		STUB(0x10102610);
		return false;
	}

	virtual void readValue(
		Reflection::DescribedBase* instance,
		const XmlElement* element,
		IReferenceBinder& binder
	) const
	{
		STUB(0x101022f0);
	}

	virtual void writeValue(const Reflection::DescribedBase* instance, XmlElement* element) const { STUB(0x10102130); }

private:
	std::auto_ptr<typename Reflection::TypedPropertyDescriptor<T>::GetSet> getset; // 0x1c
};

static SurfaceEnumPropDescriptor<NORM_Y, SurfaceType> desc_TopType(
	"TopSurface",
	"Surface",
	&Surface::getSurfaceType,
	&Surface::setSurfaceType
);

static SurfaceEnumPropDescriptor<NORM_Y, Controller::InputType> desc_TopSurfaceInput(
	"TopSurfaceInput",
	"Surface Inputs",
	&Surface::getInput,
	&Surface::setSurfaceInput
);

static SurfaceEnumPropDescriptor<NORM_Y_NEG, SurfaceType> desc_BottomType(
	"BottomSurface",
	"Surface",
	&Surface::getSurfaceType,
	&Surface::setSurfaceType
);

static SurfaceEnumPropDescriptor<NORM_Y_NEG, Controller::InputType> desc_BottomSurfaceInput(
	"BottomSurfaceInput",
	"Surface Inputs",
	&Surface::getInput,
	&Surface::setSurfaceInput
);

static SurfaceEnumPropDescriptor<NORM_X_NEG, SurfaceType> desc_LeftType(
	"LeftSurface",
	"Surface",
	&Surface::getSurfaceType,
	&Surface::setSurfaceType
);

static SurfaceEnumPropDescriptor<NORM_X_NEG, Controller::InputType> desc_LeftSurfaceInput(
	"LeftSurfaceInput",
	"Surface Inputs",
	&Surface::getInput,
	&Surface::setSurfaceInput
);

static SurfaceEnumPropDescriptor<NORM_X, SurfaceType> desc_RightType(
	"RightSurface",
	"Surface",
	&Surface::getSurfaceType,
	&Surface::setSurfaceType
);

static SurfaceEnumPropDescriptor<NORM_X, Controller::InputType> desc_RightSurfaceInput(
	"RightSurfaceInput",
	"Surface Inputs",
	&Surface::getInput,
	&Surface::setSurfaceInput
);

static SurfaceEnumPropDescriptor<NORM_Z_NEG, SurfaceType> desc_FrontType(
	"FrontSurface",
	"Surface",
	&Surface::getSurfaceType,
	&Surface::setSurfaceType
);

static SurfaceEnumPropDescriptor<NORM_Z_NEG, Controller::InputType> desc_FrontSurfaceInput(
	"FrontSurfaceInput",
	"Surface Inputs",
	&Surface::getInput,
	&Surface::setSurfaceInput
);

static SurfaceEnumPropDescriptor<NORM_Z, SurfaceType> desc_BackType(
	"BackSurface",
	"Surface",
	&Surface::getSurfaceType,
	&Surface::setSurfaceType
);

static SurfaceEnumPropDescriptor<NORM_Z, Controller::InputType> desc_BackSurfaceInput(
	"BackSurfaceInput",
	"Surface Inputs",
	&Surface::getInput,
	&Surface::setSurfaceInput
);

// FUNCTION: WEBSERVICE 0x10100bd0
const bool Surfaces::isSurfaceDescriptor(const Reflection::PropertyDescriptor& desc)
{
	if (&desc == &desc_TopType) {
		return true;
	}

	if (&desc == &desc_BottomType) {
		return true;
	}

	if (&desc == &desc_LeftType) {
		return true;
	}

	if (&desc == &desc_RightType) {
		return true;
	}

	if (&desc == &desc_FrontType) {
		return true;
	}

	return &desc == &desc_BackType;
}

// FUNCTION: WEBSERVICE 0x10100c10
const Reflection::PropertyDescriptor& Surfaces::getSurfaceType(NormalId normalId) const
{
	switch (normalId) {
	default:
		return desc_TopType;
	case NORM_Y_NEG:
		return desc_BottomType;
	case NORM_Z:
		return desc_BackType;
	case NORM_Z_NEG:
		return desc_FrontType;
	case NORM_X:
		return desc_RightType;
	case NORM_X_NEG:
		return desc_LeftType;
	}
}

// FUNCTION: WEBSERVICE 0x10100c70
const Reflection::PropertyDescriptor& Surfaces::getSurfaceInput(NormalId normalId) const
{
	switch (normalId) {
	default:
		return desc_TopSurfaceInput;
	case NORM_Y_NEG:
		return desc_BottomSurfaceInput;
	case NORM_Z:
		return desc_BackSurfaceInput;
	case NORM_Z_NEG:
		return desc_FrontSurfaceInput;
	case NORM_X:
		return desc_RightSurfaceInput;
	case NORM_X_NEG:
		return desc_LeftSurfaceInput;
	}
}

// FUNCTION: WEBSERVICE 0x10100d90
Surfaces::Surfaces(PartInstance* partInstance)
	: Top(partInstance, NORM_Y), Bottom(partInstance, NORM_Y_NEG), Left(partInstance, NORM_X_NEG),
	  Right(partInstance, NORM_X), Front(partInstance, NORM_Z_NEG), Back(partInstance, NORM_Z)
{
}

// STUB: WEBSERVICE 0x10100de0
const Surface& Surfaces::operator[](NormalId normalId) const
{
	switch (normalId) {
	case NORM_Y_NEG:
		return Bottom;
	case NORM_Z:
		return Back;
	case NORM_Z_NEG:
		return Front;
	case NORM_X:
		return Right;
	case NORM_X_NEG:
		return Left;
	default:
		return Top;
	}
}

Surface& Surfaces::operator[](NormalId normalId)
{
	switch (normalId) {
	case NORM_Y_NEG:
		return Bottom;
	case NORM_Z:
		return Back;
	case NORM_Z_NEG:
		return Front;
	case NORM_X:
		return Right;
	case NORM_X_NEG:
		return Left;
	default:
		return Top;
	}
}

// FUNCTION: WEBSERVICE 0x10100e30
const bool Surfaces::isStandardPart() const
{
	return Bottom.getSurfaceType() == INLET && Front.getSurfaceType() == NO_SURFACE &&
		   Back.getSurfaceType() == NO_SURFACE && Left.getSurfaceType() == NO_SURFACE &&
		   Right.getSurfaceType() == NO_SURFACE;
}

} // namespace RBX
