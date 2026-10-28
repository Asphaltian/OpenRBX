#ifndef V8DATAMODEL_SURFACES_H
#define V8DATAMODEL_SURFACES_H

#include "decomp.h"
#include "util/NormalId.h"
#include "util/Vector6.h"
#include "v8datamodel/Surface.h"

#include <boost/noncopyable.hpp>

namespace RBX {

namespace Reflection {
class PropertyDescriptor;
}

// SIZE 0x30
class Surfaces : public boost::noncopyable
{
private:
	Surface Top;    // 0x00
	Surface Bottom; // 0x08
	Surface Left;   // 0x10
	Surface Right;  // 0x18
	Surface Front;  // 0x20
	Surface Back;   // 0x28

public:
	static const bool isSurfaceDescriptor(const Reflection::PropertyDescriptor& desc);

	Surfaces(PartInstance* partInstance);

	Surface& operator[](NormalId normalId);
	const Surface& operator[](NormalId normalId) const;

	// FUNCTION: WEBSERVICE 0x1009ace0
	Vector6<SurfaceType> surf6() const
	{
		Vector6<SurfaceType> answer;

		for (int i = 0; i < 6; i++) {
			answer[i] = (*this)[static_cast<NormalId>(i)].getSurfaceType();
		}

		return answer;
	}

	const Reflection::PropertyDescriptor& getSurfaceType(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getSurfaceInput(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getParamA(NormalId normalId) const;
	const Reflection::PropertyDescriptor& getParamB(NormalId normalId) const;

	const bool isStandardPart() const;
};

DECOMP_SIZE_ASSERT(Surfaces, 0x30)

// clang-format off
// TEMPLATE: WEBSERVICE 0x101018b0
// RBX::SurfaceDescriptor<1>::readValue
// TEMPLATE: WEBSERVICE 0x101018e0
// RBX::SurfaceDescriptor<4>::readValue
// TEMPLATE: WEBSERVICE 0x10101930
// RBX::SurfaceDescriptor<3>::readValue
// SYNTHETIC: WEBSERVICE 0x10101960
// RBX::SurfaceDescriptor<1>::`scalar deleting destructor'
// TEMPLATE: WEBSERVICE 0x10101980
// RBX::SurfaceDescriptor<0>::readValue
// TEMPLATE: WEBSERVICE 0x101019c0
// RBX::SurfaceDescriptor<5>::readValue
// TEMPLATE: WEBSERVICE 0x10101a00
// RBX::SurfaceDescriptor<2>::readValue
// clang-format on

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021dde0
// RBX::`dynamic initializer for 'desc_LegacyTop''
// SYNTHETIC: WEBSERVICE 0x1021de20
// RBX::`dynamic initializer for 'desc_LegacyBottom''
// SYNTHETIC: WEBSERVICE 0x1021de60
// RBX::`dynamic initializer for 'desc_LegacyLeft''
// SYNTHETIC: WEBSERVICE 0x1021dea0
// RBX::`dynamic initializer for 'desc_LegacyRight''
// SYNTHETIC: WEBSERVICE 0x1021dee0
// RBX::`dynamic initializer for 'desc_LegacyFront''
// SYNTHETIC: WEBSERVICE 0x1021df20
// RBX::`dynamic initializer for 'desc_LegacyBack''
// SYNTHETIC: WEBSERVICE 0x10224da0
// `RBX::Reflection::Type::singleton<RBX::Surface>'::`2'::`dynamic atexit destructor for 'type''
// SYNTHETIC: WEBSERVICE 0x10224db0
// RBX::`dynamic atexit destructor for 'desc_LegacyTop''
// SYNTHETIC: WEBSERVICE 0x10224dc0
// RBX::`dynamic atexit destructor for 'desc_LegacyBottom''
// SYNTHETIC: WEBSERVICE 0x10224dd0
// RBX::`dynamic atexit destructor for 'desc_LegacyLeft''
// SYNTHETIC: WEBSERVICE 0x10224de0
// RBX::`dynamic atexit destructor for 'desc_LegacyRight''
// SYNTHETIC: WEBSERVICE 0x10224df0
// RBX::`dynamic atexit destructor for 'desc_LegacyFront''
// SYNTHETIC: WEBSERVICE 0x10224e00
// RBX::`dynamic atexit destructor for 'desc_LegacyBack''
// clang-format on

// clang-format off
// TEMPLATE: WEBSERVICE 0x101022b0
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::Controller::InputType>::setEnumValue
// TEMPLATE: WEBSERVICE 0x10102440
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::SurfaceType>::getIndexValue
// TEMPLATE: WEBSERVICE 0x101024a0
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::Controller::InputType>::getIndexValue
// TEMPLATE: WEBSERVICE 0x10102740
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::SurfaceType>::setEnumValue
// clang-format on

// clang-format off
// TEMPLATE: WEBSERVICE 0x101022f0
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::SurfaceType>::readValue
// TEMPLATE: WEBSERVICE 0x10102500
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::Controller::InputType>::readValue
// TEMPLATE: WEBSERVICE 0x10102690
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::SurfaceType>::setIndexValue
// TEMPLATE: WEBSERVICE 0x10102780
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::Controller::InputType>::setIndexValue
// clang-format on

// clang-format off
// SYNTHETIC: WEBSERVICE 0x1021dfc0
// RBX::`dynamic initializer for 'desc_TopParamA''
// SYNTHETIC: WEBSERVICE 0x1021dff0
// RBX::`dynamic initializer for 'desc_TopParamB''
// SYNTHETIC: WEBSERVICE 0x1021e080
// RBX::`dynamic initializer for 'desc_BottomParamA''
// SYNTHETIC: WEBSERVICE 0x1021e0b0
// RBX::`dynamic initializer for 'desc_BottomParamB''
// SYNTHETIC: WEBSERVICE 0x1021e140
// RBX::`dynamic initializer for 'desc_LeftParamA''
// SYNTHETIC: WEBSERVICE 0x1021e170
// RBX::`dynamic initializer for 'desc_LeftParamB''
// SYNTHETIC: WEBSERVICE 0x1021e200
// RBX::`dynamic initializer for 'desc_RightParamA''
// SYNTHETIC: WEBSERVICE 0x1021e230
// RBX::`dynamic initializer for 'desc_RightParamB''
// SYNTHETIC: WEBSERVICE 0x1021e2c0
// RBX::`dynamic initializer for 'desc_FrontParamA''
// SYNTHETIC: WEBSERVICE 0x1021e2f0
// RBX::`dynamic initializer for 'desc_FrontParamB''
// SYNTHETIC: WEBSERVICE 0x1021e380
// RBX::`dynamic initializer for 'desc_BackParamA''
// SYNTHETIC: WEBSERVICE 0x1021e3b0
// RBX::`dynamic initializer for 'desc_BackParamB''
// SYNTHETIC: WEBSERVICE 0x1021df60
// RBX::`dynamic initializer for 'desc_TopType''
// SYNTHETIC: WEBSERVICE 0x1021df90
// RBX::`dynamic initializer for 'desc_TopSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x1021e020
// RBX::`dynamic initializer for 'desc_BottomType''
// SYNTHETIC: WEBSERVICE 0x1021e050
// RBX::`dynamic initializer for 'desc_BottomSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x1021e0e0
// RBX::`dynamic initializer for 'desc_LeftType''
// SYNTHETIC: WEBSERVICE 0x1021e110
// RBX::`dynamic initializer for 'desc_LeftSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x1021e1a0
// RBX::`dynamic initializer for 'desc_RightType''
// SYNTHETIC: WEBSERVICE 0x1021e1d0
// RBX::`dynamic initializer for 'desc_RightSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x1021e260
// RBX::`dynamic initializer for 'desc_FrontType''
// SYNTHETIC: WEBSERVICE 0x1021e290
// RBX::`dynamic initializer for 'desc_FrontSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x1021e320
// RBX::`dynamic initializer for 'desc_BackType''
// SYNTHETIC: WEBSERVICE 0x1021e350
// RBX::`dynamic initializer for 'desc_BackSurfaceInput''
// clang-format on

// clang-format off
// TEMPLATE: WEBSERVICE 0x10100f10
// RBX::SurfaceGetSet<1,enum RBX::Controller::InputType,enum RBX::Controller::InputType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::Controller::InputType)>::setValue
// TEMPLATE: WEBSERVICE 0x10100f40
// RBX::SurfaceGetSet<1,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::setValue
// TEMPLATE: WEBSERVICE 0x10100f70
// RBX::SurfaceGetSet<1,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::getValue
// TEMPLATE: WEBSERVICE 0x10100fa0
// RBX::SurfaceGetSet<4,enum RBX::Controller::InputType,enum RBX::Controller::InputType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::Controller::InputType)>::setValue
// TEMPLATE: WEBSERVICE 0x10100fd0
// RBX::SurfaceGetSet<4,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::getValue
// TEMPLATE: WEBSERVICE 0x10101010
// RBX::SurfaceGetSet<4,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::setValue
// TEMPLATE: WEBSERVICE 0x10101040
// RBX::SurfaceGetSet<3,enum RBX::Controller::InputType,enum RBX::Controller::InputType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::Controller::InputType)>::setValue
// TEMPLATE: WEBSERVICE 0x10101070
// RBX::SurfaceGetSet<3,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::getValue
// TEMPLATE: WEBSERVICE 0x101010b0
// RBX::SurfaceGetSet<3,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::setValue
// TEMPLATE: WEBSERVICE 0x101010e0
// RBX::SurfaceGetSet<0,enum RBX::Controller::InputType,enum RBX::Controller::InputType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::Controller::InputType)>::setValue
// TEMPLATE: WEBSERVICE 0x10101110
// RBX::SurfaceGetSet<0,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::getValue
// TEMPLATE: WEBSERVICE 0x10101150
// RBX::SurfaceGetSet<0,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::setValue
// TEMPLATE: WEBSERVICE 0x10101180
// RBX::SurfaceGetSet<5,enum RBX::Controller::InputType,enum RBX::Controller::InputType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::Controller::InputType)>::setValue
// TEMPLATE: WEBSERVICE 0x101011b0
// RBX::SurfaceGetSet<5,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::getValue
// TEMPLATE: WEBSERVICE 0x101011f0
// RBX::SurfaceGetSet<5,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::setValue
// TEMPLATE: WEBSERVICE 0x10101220
// RBX::SurfaceGetSet<2,enum RBX::Controller::InputType,enum RBX::Controller::InputType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::Controller::InputType)>::setValue
// TEMPLATE: WEBSERVICE 0x10101250
// RBX::SurfaceGetSet<2,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::setValue
// TEMPLATE: WEBSERVICE 0x10101280
// RBX::SurfaceGetSet<2,float,float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>::getValue
// TEMPLATE: WEBSERVICE 0x10101490
// RBX::SurfacePropDescriptor<1,float>::SurfacePropDescriptor<1,float><float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>
// TEMPLATE: WEBSERVICE 0x10101540
// RBX::SurfacePropDescriptor<4,float>::SurfacePropDescriptor<4,float><float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>
// TEMPLATE: WEBSERVICE 0x101015f0
// RBX::SurfacePropDescriptor<3,float>::SurfacePropDescriptor<3,float><float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>
// TEMPLATE: WEBSERVICE 0x101016a0
// RBX::SurfacePropDescriptor<0,float>::SurfacePropDescriptor<0,float><float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>
// TEMPLATE: WEBSERVICE 0x10101750
// RBX::SurfacePropDescriptor<5,float>::SurfacePropDescriptor<5,float><float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>
// TEMPLATE: WEBSERVICE 0x10101800
// RBX::SurfacePropDescriptor<2,float>::SurfacePropDescriptor<2,float><float (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(float)>
// TEMPLATE: WEBSERVICE 0x10101af0
// RBX::SurfaceEnumPropDescriptor<1,enum RBX::SurfaceType>::SurfaceEnumPropDescriptor<1,enum RBX::SurfaceType><enum RBX::SurfaceType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::SurfaceType)>
// TEMPLATE: WEBSERVICE 0x10101c30
// RBX::SurfaceEnumPropDescriptor<4,enum RBX::SurfaceType>::SurfaceEnumPropDescriptor<4,enum RBX::SurfaceType><enum RBX::SurfaceType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::SurfaceType)>
// TEMPLATE: WEBSERVICE 0x10101d70
// RBX::SurfaceEnumPropDescriptor<3,enum RBX::SurfaceType>::SurfaceEnumPropDescriptor<3,enum RBX::SurfaceType><enum RBX::SurfaceType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::SurfaceType)>
// TEMPLATE: WEBSERVICE 0x10101eb0
// RBX::SurfaceEnumPropDescriptor<0,enum RBX::SurfaceType>::SurfaceEnumPropDescriptor<0,enum RBX::SurfaceType><enum RBX::SurfaceType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::SurfaceType)>
// TEMPLATE: WEBSERVICE 0x10101ff0
// RBX::SurfaceEnumPropDescriptor<5,enum RBX::SurfaceType>::SurfaceEnumPropDescriptor<5,enum RBX::SurfaceType><enum RBX::SurfaceType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::SurfaceType)>
// TEMPLATE: WEBSERVICE 0x10102160
// RBX::SurfaceEnumPropDescriptor<2,enum RBX::SurfaceType>::SurfaceEnumPropDescriptor<2,enum RBX::SurfaceType><enum RBX::SurfaceType (__thiscall RBX::Surface::*)(void)const ,void (__thiscall RBX::Surface::*)(enum RBX::SurfaceType)>
// clang-format on

// clang-format off
// SYNTHETIC: WEBSERVICE 0x10224e10
// RBX::`dynamic atexit destructor for 'desc_TopSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10224e30
// RBX::`dynamic atexit destructor for 'desc_TopType''
// SYNTHETIC: WEBSERVICE 0x10224e50
// RBX::`dynamic atexit destructor for 'desc_BottomParamB''
// SYNTHETIC: WEBSERVICE 0x10224e70
// RBX::`dynamic atexit destructor for 'desc_BottomParamA''
// SYNTHETIC: WEBSERVICE 0x10224e90
// RBX::`dynamic atexit destructor for 'desc_BottomSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10224eb0
// RBX::`dynamic atexit destructor for 'desc_BottomType''
// SYNTHETIC: WEBSERVICE 0x10224ed0
// RBX::`dynamic atexit destructor for 'desc_LeftParamB''
// SYNTHETIC: WEBSERVICE 0x10224ef0
// RBX::`dynamic atexit destructor for 'desc_LeftParamA''
// SYNTHETIC: WEBSERVICE 0x10224f10
// RBX::`dynamic atexit destructor for 'desc_LeftSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10224f30
// RBX::`dynamic atexit destructor for 'desc_LeftType''
// SYNTHETIC: WEBSERVICE 0x10224f50
// RBX::`dynamic atexit destructor for 'desc_RightParamB''
// SYNTHETIC: WEBSERVICE 0x10224f70
// RBX::`dynamic atexit destructor for 'desc_RightParamA''
// SYNTHETIC: WEBSERVICE 0x10224f90
// RBX::`dynamic atexit destructor for 'desc_RightSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10224fb0
// RBX::`dynamic atexit destructor for 'desc_RightType''
// SYNTHETIC: WEBSERVICE 0x10224fd0
// RBX::`dynamic atexit destructor for 'desc_FrontParamB''
// SYNTHETIC: WEBSERVICE 0x10224ff0
// RBX::`dynamic atexit destructor for 'desc_FrontParamA''
// SYNTHETIC: WEBSERVICE 0x10225010
// RBX::`dynamic atexit destructor for 'desc_FrontSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x10225030
// RBX::`dynamic atexit destructor for 'desc_FrontType''
// SYNTHETIC: WEBSERVICE 0x10225050
// RBX::`dynamic atexit destructor for 'desc_BackParamB''
// SYNTHETIC: WEBSERVICE 0x10225070
// RBX::`dynamic atexit destructor for 'desc_BackParamA''
// SYNTHETIC: WEBSERVICE 0x10225090
// RBX::`dynamic atexit destructor for 'desc_BackSurfaceInput''
// SYNTHETIC: WEBSERVICE 0x102250b0
// RBX::`dynamic atexit destructor for 'desc_BackType''
// SYNTHETIC: WEBSERVICE 0x102250d0
// RBX::`dynamic atexit destructor for 'desc_TopParamB''
// SYNTHETIC: WEBSERVICE 0x102250f0
// RBX::`dynamic atexit destructor for 'desc_TopParamA''
// clang-format on

} // namespace RBX

#endif // V8DATAMODEL_SURFACES_H
