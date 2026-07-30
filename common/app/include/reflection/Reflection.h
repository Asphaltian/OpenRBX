#ifndef REFLECTION_REFLECTION_H
#define REFLECTION_REFLECTION_H

namespace RBX {
namespace Reflection {

class DescribedBase
{
public:
	virtual ~DescribedBase() {}
};

template <class T, const char* const& name, class Base>
class Described : public Base
{
};

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_REFLECTION_H
