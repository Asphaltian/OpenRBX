#include "v8datamodel/Surface.h"

#include "v8datamodel/PartInstance.h"
#include "v8world/Primitive.h"

namespace RBX {

// FUNCTION: WEBSERVICE 0x10104260
Surface::Surface(PartInstance* partInstance, NormalId surfId) : partInstance(partInstance), surfId(surfId)
{
}

// FUNCTION: WEBSERVICE 0x10104280
SurfaceType Surface::getSurfaceType() const
{
	return partInstance->getPrimitive()->getSurfaceType(surfId);
}

// FUNCTION: WEBSERVICE 0x101042a0
Controller::InputType Surface::getInput() const
{
	return partInstance->getPrimitive()->getSurfaceData(surfId).inputType;
}

// FUNCTION: WEBSERVICE 0x10104300
float Surface::getParamA() const
{
	return partInstance->getPrimitive()->getSurfaceData(surfId).paramA;
}

// FUNCTION: WEBSERVICE 0x10104360
float Surface::getParamB() const
{
	return partInstance->getPrimitive()->getSurfaceData(surfId).paramB;
}

// FUNCTION: WEBSERVICE 0x101043c0
bool Surface::isControllable() const
{
	Controller::InputType input = getInput();

	return input != Controller::NO_INPUT && input != Controller::CONSTANT_INPUT && input != Controller::SIN_INPUT;
}

// FUNCTION: WEBSERVICE 0x101043e0
void Surface::setSurfaceType(SurfaceType type)
{
	if (type != getSurfaceType()) {
		partInstance->getPrimitive()->setSurfaceType(surfId, type);
		partInstance->onSurfaceChanged(surfId);
		partInstance->raisePropertyChanged(partInstance->getSurfaces().getSurfaceType(surfId));
	}
}

// FUNCTION: WEBSERVICE 0x10104430
void Surface::setSurfaceInput(Controller::InputType value)
{
	SurfaceData current = partInstance->getPrimitive()->getSurfaceData(surfId);

	if (value != current.inputType) {
		current.inputType = value;
		partInstance->getPrimitive()->setSurfaceData(surfId, current);
		partInstance->onSurfaceChanged(surfId);
		partInstance->raisePropertyChanged(partInstance->getSurfaces().getSurfaceInput(surfId));
	}
}

// FUNCTION: WEBSERVICE 0x101044f0
void Surface::setParamA(float value)
{
	SurfaceData current = partInstance->getPrimitive()->getSurfaceData(surfId);

	if (current.paramA != value) {
		current.paramA = value;
		partInstance->getPrimitive()->setSurfaceData(surfId, current);
		partInstance->onSurfaceChanged(surfId);
		partInstance->raisePropertyChanged(partInstance->getSurfaces().getParamA(surfId));
	}
}

// FUNCTION: WEBSERVICE 0x101045c0
void Surface::setParamB(float value)
{
	SurfaceData current = partInstance->getPrimitive()->getSurfaceData(surfId);

	if (current.paramB != value) {
		current.paramB = value;
		partInstance->getPrimitive()->setSurfaceData(surfId, current);
		partInstance->onSurfaceChanged(surfId);
		partInstance->raisePropertyChanged(partInstance->getSurfaces().getParamB(surfId));
	}
}

// FUNCTION: WEBSERVICE 0x10104690
void Surface::readValue(const XmlElement* element, IReferenceBinder& binder)
{
	static const Name& desc_LegacyType = Name::declare("Type", -1);
	static const Name& desc_LegacySurfaceInput = Name::declare("SurfaceInput", -1);
	static const Name& desc_LegacyParamA = Name::declare("ParamA", -1);
	static const Name& desc_LegacyParamB = Name::declare("ParamB", -1);
	static const Name& desc_LegacyConstraint = Name::declare("Constraint", -1);

	const XmlElement* child = element->firstChild();
	while (child) {
		const Name* name = NULL;
		if (!child->isXsiNil() && child->findAttributeValue(name_name, name)) {
			if (name == &desc_LegacyType) {
				std::string sValue;
				if (child->getValue(sValue)) {
					SurfaceType e;
					if (Reflection::EnumDesc<SurfaceType>::singleton().convertToValue(sValue, e)) {
						setSurfaceType(e);
					}
					else {
						setSurfaceType(NO_SURFACE);
					}
				}
			}
			else if (name == &desc_LegacySurfaceInput) {
				std::string sValue;
				if (child->getValue(sValue)) {
					Controller::InputType e;
					if (Reflection::EnumDesc<Controller::InputType>::singleton().convertToValue(sValue, e)) {
						setSurfaceInput(e);
					}
				}
			}
			else if (name == &desc_LegacyParamA) {
				float value;
				if (child->getValue(value)) {
					setParamA(value);
				}
			}
			else if (name == &desc_LegacyParamB) {
				float value;
				if (child->getValue(value)) {
					setParamB(value);
				}
			}
			else if (name == &desc_LegacyConstraint) {
				std::string sValue;
				if (child->getValue(sValue)) {
					SurfaceType e;
					if (Reflection::EnumDesc<SurfaceType>::singleton().convertToValue(sValue, e) && e != NO_SURFACE) {
						setSurfaceType(e);
					}
				}
			}
		}
		child = child->nextSibling();
	}
}

} // namespace RBX
