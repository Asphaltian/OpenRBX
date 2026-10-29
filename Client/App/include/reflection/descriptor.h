#ifndef REFLECTION_DESCRIPTOR_H
#define REFLECTION_DESCRIPTOR_H

#include "decomp.h"
#include "util/Name.h"

#include <boost/noncopyable.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace RBX {
namespace Reflection {

// FUNCTION: WEBSERVICE 0x10047470
// FUNCTION: ROBLOX 0x005a9440
inline boost::recursive_mutex& sync()
{
	static boost::recursive_mutex s;

	return s;
}

// SYNTHETIC: WEBSERVICE 0x10005990
// RBX::Reflection::Descriptor::`scalar deleting destructor'
// FUNCTION: WEBSERVICE 0x1004d7f0
// FUNCTION: ROBLOX 0x004b68f0
// RBX::Reflection::SignalDescImpl<1,void __cdecl(boost::shared_ptr<RBX::Instance>)>::fire
// FUNCTION: WEBSERVICE 0x100575e0
// FUNCTION: ROBLOX 0x004ad660
// RBX::Reflection::SignalDescImpl<1,void __cdecl(float)>::fire
// FUNCTION: WEBSERVICE 0x100e3560
// FUNCTION: ROBLOX 0x00626670
// RBX::Reflection::SignalDescImpl<2,void __cdecl(boost::shared_ptr<RBX::Instance>,float)>::fire
// clang-format off
// FUNCTION: WEBSERVICE 0x100f6aa0
// FUNCTION: ROBLOX 0x00430960
// RBX::Reflection::SignalDescImpl<1,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >)>::fire
// FUNCTION: WEBSERVICE 0x10198a10
// FUNCTION: ROBLOX 0x004bac90
// RBX::Reflection::SignalDescImpl<2,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,int)>::fire
// FUNCTION: WEBSERVICE 0x101a5f00
// FUNCTION: ROBLOX 0x004cee90
// RBX::Reflection::SignalDescImpl<2,void __cdecl(std::basic_string<char,std::char_traits<char>,std::allocator<char> >,bool)>::fire
// clang-format on

// VTABLE: WEBSERVICE 0x1022da44
// VTABLE: ROBLOX 0x008013ac
// SIZE 0x08
class Descriptor : public boost::noncopyable
{
public:
	virtual ~Descriptor() {} // vtable+0x00

	const Name& name; // 0x04

	Descriptor(const char* name) : name(Name::declare(name, -1)) {}
};

DECOMP_SIZE_ASSERT(Descriptor, 0x08)

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10221680
// SYNTHETIC: ROBLOX 0x007f27c0
// `RBX::Reflection::sync'::`2'::`dynamic atexit destructor for 's''
// clang-format on

} // namespace Reflection
} // namespace RBX

#endif // REFLECTION_DESCRIPTOR_H
