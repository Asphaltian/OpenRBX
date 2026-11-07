#ifndef V8DATAMODEL_COMMANDS_H
#define V8DATAMODEL_COMMANDS_H

#include "decomp.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/Workspace.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"
#include "v8world/Controller.h"

#include <string>
#include <typeinfo>

namespace RBX {

class Camera;
class DataModel;
class RunService;
class Selection;

// SIZE 0x10
class FirstPersonCommand : public Verb
{
private:
	DataModel* dataModel; // 0x0c

public:
	FirstPersonCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(FirstPersonCommand, 0x10)

// SIZE 0x10
class StatsCommand : public Verb
{
private:
	DataModel* dataModel; // 0x0c

public:
	StatsCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(StatsCommand, 0x10)

// SIZE 0x0c
class ForceAssertionCommand : public Verb
{
public:
	ForceAssertionCommand(DataModel* dataModel);

	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(ForceAssertionCommand, 0x0c)

// SIZE 0x10
class EngineStatsCommand : public Verb
{
private:
	DataModel* dataModel; // 0x0c

public:
	EngineStatsCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(EngineStatsCommand, 0x10)

// SIZE 0x10
class JoinCommand : public Verb
{
private:
	DataModel* dataModel; // 0x0c

public:
	JoinCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(JoinCommand, 0x10)

// SIZE 0x10
class ClearStarterpack : public Verb
{
private:
	DataModel* dataModel; // 0x0c

public:
	ClearStarterpack(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(ClearStarterpack, 0x10)

// SIZE 0x10
class ClearBackpack : public Verb
{
private:
	DataModel* dataModel; // 0x0c

public:
	ClearBackpack(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(ClearBackpack, 0x10)

// SIZE 0x1c
class RunStateVerb : public Verb
{
protected:
	DataModel* dataModel;                 // 0x0c
	ServiceClient<RunService> runService; // 0x10

	RunStateVerb(std::string name, DataModel* dataModel);

	void playActionSound();

public:
	virtual ~RunStateVerb();
};

DECOMP_SIZE_ASSERT(RunStateVerb, 0x1c)

template <class T, class Base>
class TToolVerb : public Base
{
public:
	TToolVerb(DataModel* dataModel) : Base(T::name().toString() + "Tool", dataModel) {}

	virtual bool isChecked() const // vtable+0x0c
	{
		MouseCommand* command = this->dataModel->getWorkspace()->getCurrentMouseCommand();
		if (command) {
			return typeid(T) == typeid(*command);
		}
		return false;
	}

	virtual void doIt(IDataState* dataState) // vtable+0x10
	{
		this->dataModel->getWorkspace()->setMouseCommand(VTable0x14());
	}

	virtual MouseCommand* VTable0x14() { return new T(this->dataModel->getWorkspace()); } // vtable+0x14

	// clang-format off
	// TEMPLATE: ROBLOX 0x005d0a40
	// RBX::TToolVerb<RBX::ArrowTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d0a70
	// RBX::TToolVerb<RBX::ArrowTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d0e10
	// RBX::TToolVerb<RBX::FlatTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d0ed0
	// RBX::TToolVerb<RBX::FlatTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d0f60
	// RBX::TToolVerb<RBX::GlueTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d1020
	// RBX::TToolVerb<RBX::GlueTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d10b0
	// RBX::TToolVerb<RBX::WeldTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d1170
	// RBX::TToolVerb<RBX::WeldTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d1200
	// RBX::TToolVerb<RBX::OscillateMotorTool,RBX::RunStateVerb>::doIt
	// TEMPLATE: ROBLOX 0x005d1220
	// RBX::TToolVerb<RBX::StudsTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d12e0
	// RBX::TToolVerb<RBX::StudsTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d1370
	// RBX::TToolVerb<RBX::InletTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d1430
	// RBX::TToolVerb<RBX::InletTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d14c0
	// RBX::TToolVerb<RBX::HingeTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d15a0
	// RBX::TToolVerb<RBX::HingeTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d1630
	// RBX::TToolVerb<RBX::RightMotorTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d1660
	// RBX::TToolVerb<RBX::RightMotorTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d16f0
	// RBX::TToolVerb<RBX::LeftMotorTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d1720
	// RBX::TToolVerb<RBX::LeftMotorTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d17b0
	// RBX::TToolVerb<RBX::OscillateMotorTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d1810
	// RBX::TToolVerb<RBX::OscillateMotorTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d1990
	// RBX::TToolVerb<RBX::ModelSetPrimaryPartTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d19c0
	// RBX::TToolVerb<RBX::ModelSetPrimaryPartTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d1be0
	// RBX::TToolVerb<RBX::FillTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d1cc0
	// RBX::TToolVerb<RBX::FillTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d1d30
	// RBX::TToolVerb<RBX::DropperTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d1db0
	// RBX::TToolVerb<RBX::DropperTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d20c0
	// RBX::TToolVerb<RBX::HammerTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d2220
	// RBX::TToolVerb<RBX::HammerTool,RBX::RunStateVerb>::VTable0x14
	// TEMPLATE: ROBLOX 0x005d2610
	// RBX::TToolVerb<RBX::LaserTool,RBX::RunStateVerb>::isChecked
	// TEMPLATE: ROBLOX 0x005d2640
	// RBX::TToolVerb<RBX::LaserTool,RBX::RunStateVerb>::VTable0x14
	// clang-format on
};

// SIZE 0x24
class EditSelectionVerb : public Verb
{
protected:
	boost::shared_ptr<Workspace> workspace; // 0x0c
	ServiceClient<Selection> selection;     // 0x14
	DataModel* dataModel;                   // 0x20

	EditSelectionVerb(VerbContainer* container, std::string name, DataModel* dataModel);
	EditSelectionVerb(std::string name, DataModel* dataModel);

public:
	virtual ~EditSelectionVerb();

	virtual bool isEnabled() const;
};

DECOMP_SIZE_ASSERT(EditSelectionVerb, 0x24)

// SIZE 0x24
class RotateAxisCommand : public EditSelectionVerb
{
protected:
	RotateAxisCommand(std::string name, DataModel* dataModel);

	virtual G3D::Matrix3 getRotationAxis() = 0; // vtable+0x14

public:
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(RotateAxisCommand, 0x24)

// SIZE 0x24
class ControllerCommand : public EditSelectionVerb
{
protected:
	ControllerCommand(std::string name, DataModel* dataModel);

	void setController(Controller::ControllerType controllerType);

public:
	virtual bool isEnabled() const;
};

DECOMP_SIZE_ASSERT(ControllerCommand, 0x24)

// SIZE 0x28
class BoolPropertyVerb : public EditSelectionVerb
{
private:
	const Name& propertyName; // 0x24

public:
	BoolPropertyVerb(const std::string& name, DataModel* dataModel, const char* property);

protected:
	virtual void doIt(IDataState* dataState);
	virtual bool isChecked() const;
};

DECOMP_SIZE_ASSERT(BoolPropertyVerb, 0x28)

// SIZE 0x24
class RotateSelectionVerb : public RotateAxisCommand
{
public:
	RotateSelectionVerb(DataModel* dataModel);

protected:
	virtual G3D::Matrix3 getRotationAxis();
};

DECOMP_SIZE_ASSERT(RotateSelectionVerb, 0x24)

// SIZE 0x24
class TiltSelectionVerb : public RotateAxisCommand
{
public:
	TiltSelectionVerb(DataModel* dataModel);

protected:
	virtual G3D::Matrix3 getRotationAxis();
};

DECOMP_SIZE_ASSERT(TiltSelectionVerb, 0x24)

// SIZE 0x24
class PrimaryControllerCommand : public ControllerCommand
{
public:
	PrimaryControllerCommand(DataModel* dataModel);

	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(PrimaryControllerCommand, 0x24)

// SIZE 0x24
class SecondaryControllerCommand : public ControllerCommand
{
public:
	SecondaryControllerCommand(DataModel* dataModel);

	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(SecondaryControllerCommand, 0x24)

// SIZE 0x24
class GroupSelectionVerb : public EditSelectionVerb
{
public:
	GroupSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(GroupSelectionVerb, 0x24)

// SIZE 0x24
class WeldSelectionVerb : public EditSelectionVerb
{
public:
	WeldSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(WeldSelectionVerb, 0x24)

// SIZE 0x24
class SnapSelectionVerb : public EditSelectionVerb
{
public:
	SnapSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(SnapSelectionVerb, 0x24)

// SIZE 0x24
class UngroupSelectionVerb : public EditSelectionVerb
{
public:
	UngroupSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(UngroupSelectionVerb, 0x24)

// SIZE 0x24
class DuplicateSelectionVerb : public EditSelectionVerb
{
public:
	DuplicateSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(DuplicateSelectionVerb, 0x24)

// SIZE 0x24
class AttachCameraCommand : public EditSelectionVerb
{
public:
	AttachCameraCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(AttachCameraCommand, 0x24)

// SIZE 0x24
class TrackCameraCommand : public EditSelectionVerb
{
public:
	TrackCameraCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(TrackCameraCommand, 0x24)

// SIZE 0x24
class FollowCameraCommand : public EditSelectionVerb
{
public:
	FollowCameraCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(FollowCameraCommand, 0x24)

// SIZE 0x24
class WatchCameraCommand : public EditSelectionVerb
{
public:
	WatchCameraCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(WatchCameraCommand, 0x24)

// SIZE 0x24
class MoveDownSelectionVerb : public EditSelectionVerb
{
public:
	MoveDownSelectionVerb(DataModel* dataModel);

	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(MoveDownSelectionVerb, 0x24)

// VTABLE: ROBLOX 0x00801144 RBX::XmlState
// VTABLE: ROBLOX 0x0080113c RBX::Debugable
// SIZE 0x3c
class UndoState : public XmlState
{
public:
	virtual void addState(XmlElement* element, Instance& instance);
};

DECOMP_SIZE_ASSERT(UndoState, 0x3c)

// VTABLE: ROBLOX 0x0080115c RBX::XmlState
// VTABLE: ROBLOX 0x00801154 RBX::Debugable
// SIZE 0x3c
class RedoState : public XmlState
{
public:
	virtual void addState(XmlElement* element, Instance& instance);
};

DECOMP_SIZE_ASSERT(RedoState, 0x3c)

// SIZE 0x1c
class CameraVerb : public Verb
{
protected:
	Workspace* workspace; // 0x0c

	Camera* getCamera() const { return workspace->getCamera(); }

	ServiceClient<Selection> selection; // 0x10

public:
	CameraVerb(std::string name, Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraVerb, 0x1c)

// SIZE 0x1c
class CameraPanLeftCommand : public CameraVerb
{
public:
	CameraPanLeftCommand(Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraPanLeftCommand, 0x1c)

// SIZE 0x1c
class CameraPanRightCommand : public CameraVerb
{
public:
	CameraPanRightCommand(Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraPanRightCommand, 0x1c)

// SIZE 0x1c
class CameraTiltUpCommand : public CameraVerb
{
public:
	CameraTiltUpCommand(Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraTiltUpCommand, 0x1c)

// SIZE 0x1c
class CameraTiltDownCommand : public CameraVerb
{
public:
	CameraTiltDownCommand(Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraTiltDownCommand, 0x1c)

// SIZE 0x1c
class CameraZoomInCommand : public CameraVerb
{
public:
	CameraZoomInCommand(Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraZoomInCommand, 0x1c)

// SIZE 0x1c
class CameraZoomOutCommand : public CameraVerb
{
public:
	CameraZoomOutCommand(Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraZoomOutCommand, 0x1c)

// SIZE 0x1c
class CameraModelViewCommand : public CameraVerb
{
public:
	CameraModelViewCommand(Workspace* workspace);

	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraModelViewCommand, 0x1c)

// SIZE 0x1c
class FixedCameraCommand : public CameraVerb
{
public:
	FixedCameraCommand(Workspace* workspace);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(FixedCameraCommand, 0x1c)

// SIZE 0x1c
class CameraCenterCommand : public CameraVerb
{
public:
	CameraCenterCommand(Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraCenterCommand, 0x1c)

// SIZE 0x1c
class CameraZoomExtentsCommand : public CameraVerb
{
public:
	CameraZoomExtentsCommand(Workspace* workspace);

	virtual void doIt(IDataState* dataState);
};

DECOMP_SIZE_ASSERT(CameraZoomExtentsCommand, 0x1c)

} // namespace RBX

#endif // V8DATAMODEL_COMMANDS_H
