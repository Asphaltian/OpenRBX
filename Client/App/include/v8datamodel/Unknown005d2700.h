#ifndef V8DATAMODEL_UNKNOWN005D2700_H
#define V8DATAMODEL_UNKNOWN005D2700_H

#include "decomp.h"
#include "tool/GrabTool.h"
#include "tool/NullTool.h"
#include "tool/ToolsArrow.h"
#include "tool/ToolsSurface.h"
#include "v8datamodel/Commands.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/ToolsPart.h"

namespace RBX {

// SIZE 0x724
class Unknown005d2700
{
public:
	Unknown005d2700(DataModel* dataModel);

	// SYNTHETIC: ROBLOX 0x00455440
	// RBX::Unknown005d2700::~Unknown005d2700

	PlayDeleteSelectionVerb m_unk0x0;                            // 0x0
	DeleteSelectionVerb m_unk0x28;                               // 0x28
	SelectAllCommand m_unk0x50;                                  // 0x50
	GroupSelectionVerb m_unk0x6c;                                // 0x6c
	UngroupSelectionVerb m_unk0x90;                              // 0x90
	WeldSelectionVerb m_unk0xb4;                                 // 0xb4
	SnapSelectionVerb m_unk0xd8;                                 // 0xd8
	DuplicateSelectionVerb m_unk0xfc;                            // 0xfc
	UndoVerb m_unk0x120;                                         // 0x120
	RedoVerb m_unk0x130;                                         // 0x130
	AttachCameraCommand m_unk0x140;                              // 0x140
	TrackCameraCommand m_unk0x164;                               // 0x164
	FollowCameraCommand m_unk0x188;                              // 0x188
	WatchCameraCommand m_unk0x1ac;                               // 0x1ac
	AnchorVerb m_unk0x1d0;                                       // 0x1d0
	TranslucentVerb m_unk0x1f8;                                  // 0x1f8
	CanCollideVerb m_unk0x220;                                   // 0x220
	CanNotSelectCommand m_unk0x248;                              // 0x248
	AllCanSelectCommand m_unk0x26c;                              // 0x26c
	MoveUpPlateVerb m_unk0x288;                                  // 0x288
	MoveUpBrickVerb m_unk0x2b0;                                  // 0x2b0
	MoveDownSelectionVerb m_unk0x2d8;                            // 0x2d8
	RotateSelectionVerb m_unk0x2fc;                              // 0x2fc
	TiltSelectionVerb m_unk0x320;                                // 0x320
	PrimaryControllerCommand m_unk0x344;                         // 0x344
	SecondaryControllerCommand m_unk0x368;                       // 0x368
	ClearStarterpack m_unk0x38c;                                 // 0x38c
	ClearBackpack m_unk0x39c;                                    // 0x39c
	RunCommand m_unk0x3ac;                                       // 0x3ac
	StopCommand m_unk0x3c8;                                      // 0x3c8
	ResetCommand m_unk0x3e4;                                     // 0x3e4
	FirstPersonCommand m_unk0x400;                               // 0x400
	StatsCommand m_unk0x410;                                     // 0x410
	ForceAssertionCommand m_unk0x420;                            // 0x420
	EngineStatsCommand m_unk0x42c;                               // 0x42c
	JoinCommand m_unk0x43c;                                      // 0x43c
	TToolVerb<ArrowTool, RunStateVerb> m_unk0x44c;               // 0x44c
	TToolVerb<ResizeTool, RunStateVerb> m_unk0x468;              // 0x468
	TToolVerb<AxisMoveTool, RunStateVerb> m_unk0x484;            // 0x484
	TToolVerb<FlatTool, RunStateVerb> m_unk0x4a0;                // 0x4a0
	TToolVerb<GlueTool, RunStateVerb> m_unk0x4bc;                // 0x4bc
	TToolVerb<WeldTool, RunStateVerb> m_unk0x4d8;                // 0x4d8
	TToolVerb<StudsTool, RunStateVerb> m_unk0x4f4;               // 0x4f4
	TToolVerb<InletTool, RunStateVerb> m_unk0x510;               // 0x510
	TToolVerb<HingeTool, RunStateVerb> m_unk0x52c;               // 0x52c
	TToolVerb<RightMotorTool, RunStateVerb> m_unk0x548;          // 0x548
	TToolVerb<LeftMotorTool, RunStateVerb> m_unk0x564;           // 0x564
	TToolVerb<OscillateMotorTool, RunStateVerb> m_unk0x580;      // 0x580
	TToolVerb<ModelSetFrontTool, RunStateVerb> m_unk0x59c;       // 0x59c
	TToolVerb<ModelSetPrimaryPartTool, RunStateVerb> m_unk0x5b8; // 0x5b8
	TToolVerb<AnchorTool, RunStateVerb> m_unk0x5d4;              // 0x5d4
	TToolVerb<LockTool, RunStateVerb> m_unk0x5f0;                // 0x5f0
	TToolVerb<FillTool, RunStateVerb> m_unk0x60c;                // 0x60c
	TToolVerb<DropperTool, RunStateVerb> m_unk0x628;             // 0x628
	TToolVerb<NullTool, RunStateVerb> m_unk0x644;                // 0x644
	TToolVerb<GameTool, RunStateVerb> m_unk0x660;                // 0x660
	TToolVerb<GrabTool, RunStateVerb> m_unk0x67c;                // 0x67c
	TToolVerb<CloneTool, RunStateVerb> m_unk0x698;               // 0x698
	TToolVerb<HammerTool, RunStateVerb> m_unk0x6b4;              // 0x6b4
	TToolVerb<SlingshotTool, RunStateVerb> m_unk0x6d0;           // 0x6d0
	TToolVerb<RocketTool, RunStateVerb> m_unk0x6ec;              // 0x6ec
	TToolVerb<LaserTool, RunStateVerb> m_unk0x708;               // 0x708
};

DECOMP_SIZE_ASSERT(Unknown005d2700, 0x724)

} // namespace RBX

#endif // V8DATAMODEL_UNKNOWN005D2700_H
