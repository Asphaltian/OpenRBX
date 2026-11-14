#ifndef V8DATAMODEL_FACEINSTANCE_H
#define V8DATAMODEL_FACEINSTANCE_H

#include "placeholder.h"
#include "reflection/property.h"
#include "util/NormalId.h"
#include "v8tree/Instance.h"

namespace RBX {

extern const char sFaceInstance[];

class FaceInstance : public Reflection::Described<FaceInstance, sFaceInstance, Instance>
{
public:
	FaceInstance();

	static const Reflection::EnumPropDescriptor<FaceInstance, NormalId> prop_Face;

	NormalId getFace() const { return face; }

	void setFace(NormalId value);

protected:
	virtual bool askSetParent(const Instance* instance) const;

private:
	NormalId face;
};

} // namespace RBX

#endif // V8DATAMODEL_FACEINSTANCE_H
