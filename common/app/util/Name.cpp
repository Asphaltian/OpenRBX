#include "util/Name.h"

#include "decomp.h"

namespace RBX {

DECOMP_SIZE_ASSERT(Name, 0x20)

// STUB: WEBSERVICE 0x10058840
const Name& Name::declare(const char* name, int dictionaryIndex)
{
	STUB(0x10058840);
	return *(const Name*) NULL;
}

// STUB: WEBSERVICE 0x10058a30
const Name& Name::getNullName()
{
	STUB(0x10058a30);
	return *(const Name*) NULL;
}

// STUB: WEBSERVICE 0x10058ad0
const Name& Name::lookup(const std::string& name)
{
	STUB(0x10058ad0);
	return *(const Name*) NULL;
}

// STUB: WEBSERVICE 0x10058ba0
const Name& Name::lookup(const char* name)
{
	STUB(0x10058ba0);
	return *(const Name*) NULL;
}

// STUB: WEBSERVICE 0x10058c70
bool Name::empty() const
{
	STUB(0x10058c70);
	return false;
}

} // namespace RBX
