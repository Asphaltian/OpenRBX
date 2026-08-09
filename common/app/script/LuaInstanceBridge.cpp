#include "reflection/property.h"
#include "util/Name.h"
#include "v8datamodel/BrickColor.h"
#include "v8datamodel/Decal.h"

#include <G3D/Color3.h>
#include <G3D/CoordinateFrame.h>
#include <G3D/Vector3.h>
#include <lua.h>

namespace RBX {
namespace Lua {

// FUNCTION: WEBSERVICE 0x100aaf70
void newweaktable(lua_State* L, const char* mode)
{
	lua_newtable(L);
	lua_pushvalue(L, -1);
	lua_setmetatable(L, -2);
	lua_pushliteral(L, "__mode");
	lua_pushstring(L, mode);
	lua_settable(L, -3);
}

} // namespace Lua
} // namespace RBX

template const RBX::Name& RBX::Name::doDeclare<RBX::sDecal>();
template const RBX::Name& RBX::Name::doDeclare<RBX::sTexture>();
template void RBX::Name::callDoDeclare<RBX::sDecal>();
template void RBX::Name::callDoDeclare<RBX::sTexture>();

template bool RBX::Reflection::ConstProperty::getValue<bool>() const;
template float RBX::Reflection::ConstProperty::getValue<float>() const;
template int RBX::Reflection::ConstProperty::getValue<int>() const;
template double RBX::Reflection::ConstProperty::getValue<double>() const;
template G3D::Vector3 RBX::Reflection::ConstProperty::getValue<G3D::Vector3>() const;
template G3D::CoordinateFrame RBX::Reflection::ConstProperty::getValue<G3D::CoordinateFrame>() const;
template G3D::Color3 RBX::Reflection::ConstProperty::getValue<G3D::Color3>() const;
template RBX::BrickColor RBX::Reflection::ConstProperty::getValue<RBX::BrickColor>() const;

template void RBX::Reflection::Property::setValue<bool>(const bool& value);
template void RBX::Reflection::Property::setValue<float>(const float& value);
template void RBX::Reflection::Property::setValue<int>(const int& value);
template void RBX::Reflection::Property::setValue<G3D::Vector3>(const G3D::Vector3& value);
template void RBX::Reflection::Property::setValue<G3D::CoordinateFrame>(const G3D::CoordinateFrame& value);
template void RBX::Reflection::Property::setValue<G3D::Color3>(const G3D::Color3& value);
template void RBX::Reflection::Property::setValue<RBX::BrickColor>(const RBX::BrickColor& value);
