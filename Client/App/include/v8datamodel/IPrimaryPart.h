#ifndef V8DATAMODEL_IPRIMARYPART_H
#define V8DATAMODEL_IPRIMARYPART_H

#include "placeholder.h"

namespace RBX {

class PartInstance;

class __declspec(novtable) IPrimaryPart
{
public:
	virtual PartInstance* getPrimaryPart() = 0;
	virtual const PartInstance* getPrimaryPartConst() const = 0;
};

} // namespace RBX

#endif // V8DATAMODEL_IPRIMARYPART_H
