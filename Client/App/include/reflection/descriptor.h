#ifndef REFLECTION_DESCRIPTOR_H
#define REFLECTION_DESCRIPTOR_H

#include "placeholder.h"
#include "util/Name.h"

#include <boost/noncopyable.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace RBX {
namespace Reflection {

static boost::recursive_mutex& sync()
{
	static boost::recursive_mutex s;

	return s;
}

class Descriptor : public boost::noncopyable
{
public:
	virtual ~Descriptor() {}

	const Name& name;

	Descriptor(const char* name) : name(Name::declare(name, -1)) {}
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_DESCRIPTOR_H
