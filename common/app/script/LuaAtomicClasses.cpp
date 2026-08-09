#include "lua/LuaBridge.h"

#include <G3D/Color3.h>

namespace RBX {
namespace Lua {

template void Bridge<G3D::Color3, 1>::on_newindex(G3D::Color3& value, const char* name, lua_State* L);

} // namespace Lua
} // namespace RBX
