#ifndef V8DATAMODEL_COMMANDS_H
#define V8DATAMODEL_COMMANDS_H

#include "placeholder.h"
#include "v8datamodel/MouseCommand.h"
#include "v8datamodel/Workspace.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"
#include "v8world/Controller.h"

#include <boost/shared_ptr.hpp>
#include <string>
#include <typeinfo>

class XmlElement;

namespace RBX {

class Camera;
class DataModel;
class RunService;
class Selection;

void FUN_00597490(XmlElement* container, boost::shared_ptr<Instance> instance);

class FirstPersonCommand : public Verb
{
private:
	DataModel* dataModel;

public:
	FirstPersonCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class StatsCommand : public Verb
{
private:
	DataModel* dataModel;

public:
	StatsCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

class ForceAssertionCommand : public Verb
{
public:
	ForceAssertionCommand(DataModel* dataModel);

	virtual void doIt(IDataState* dataState);
};

class EngineStatsCommand : public Verb
{
private:
	DataModel* dataModel;

public:
	EngineStatsCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class JoinCommand : public Verb
{
private:
	DataModel* dataModel;

public:
	JoinCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class ClearStarterpack : public Verb
{
private:
	DataModel* dataModel;

public:
	ClearStarterpack(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class ClearBackpack : public Verb
{
private:
	DataModel* dataModel;

public:
	ClearBackpack(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class RunStateVerb : public Verb
{
protected:
	DataModel* dataModel;
	ServiceClient<RunService> runService;

	RunStateVerb(std::string name, DataModel* dataModel);

	void playActionSound();

public:
	virtual ~RunStateVerb();
};

class SelectAllCommand : public RunStateVerb
{
public:
	SelectAllCommand(DataModel* dataModel) : RunStateVerb("SelectAll", dataModel) {}

	virtual void doIt(IDataState* dataState);
};

class AllCanSelectCommand : public RunStateVerb
{
public:
	AllCanSelectCommand(DataModel* dataModel) : RunStateVerb("AllCanSelect", dataModel) {}

	virtual void doIt(IDataState* dataState);
};

class RunCommand : public RunStateVerb
{
public:
	RunCommand(DataModel* dataModel) : RunStateVerb("Run", dataModel) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class StopCommand : public RunStateVerb
{
public:
	StopCommand(DataModel* dataModel) : RunStateVerb("Stop", dataModel) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class ResetCommand : public RunStateVerb
{
public:
	ResetCommand(DataModel* dataModel) : RunStateVerb("Reset", dataModel) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

template <class T, class Base>
class TToolVerb : public Base
{
public:
	TToolVerb(DataModel* dataModel) : Base(T::name().toString() + "Tool", dataModel) {}

	virtual bool isChecked() const
	{
		MouseCommand* command = this->dataModel->getWorkspace()->getCurrentMouseCommand();
		if (command) {
			return typeid(T) == typeid(*command);
		}
		return false;
	}

	virtual void doIt(IDataState* dataState) { this->dataModel->getWorkspace()->setMouseCommand(VTable0x14()); }

	virtual MouseCommand* VTable0x14() { return new T(this->dataModel->getWorkspace()); }
};

class EditSelectionVerb : public Verb
{
protected:
	boost::shared_ptr<Workspace> workspace;
	ServiceClient<Selection> selection;
	DataModel* dataModel;

	EditSelectionVerb(VerbContainer* container, std::string name, DataModel* dataModel);
	EditSelectionVerb(std::string name, DataModel* dataModel);

public:
	virtual ~EditSelectionVerb();

	virtual bool isEnabled() const;
};

class DeleteBase : public EditSelectionVerb
{
protected:
	bool m_unk0x24;

	DeleteBase(DataModel* dataModel, std::string name, bool p_unk0x24);
	DeleteBase(VerbContainer* container, DataModel* dataModel, std::string name);

public:
	virtual void doIt(IDataState* dataState);
};

class PlayDeleteSelectionVerb : public DeleteBase
{
public:
	PlayDeleteSelectionVerb(DataModel* dataModel) : DeleteBase(dataModel, "PlayDelete", true) {}
};

class DeleteSelectionVerb : public DeleteBase
{
public:
	DeleteSelectionVerb(DataModel* dataModel) : DeleteBase(dataModel, "Delete", false) {}

	DeleteSelectionVerb(VerbContainer* container, DataModel* dataModel, std::string name)
		: DeleteBase(container, dataModel, name)
	{
	}
};

class CanNotSelectCommand : public EditSelectionVerb
{
public:
	CanNotSelectCommand(DataModel* dataModel) : EditSelectionVerb("CanNotSelect", dataModel) {}

	virtual void doIt(IDataState* dataState);
};

class RotateAxisCommand : public EditSelectionVerb
{
protected:
	RotateAxisCommand(std::string name, DataModel* dataModel);

	virtual G3D::Matrix3 getRotationAxis() = 0;

private:
	void FUN_00598da0(const G3D::Matrix3& rotation, const std::vector<PVInstance*>& pvInstances);

public:
	virtual void doIt(IDataState* dataState);
};

class ControllerCommand : public EditSelectionVerb
{
protected:
	ControllerCommand(std::string name, DataModel* dataModel);

	void setController(Controller::ControllerType controllerType);

public:
	virtual bool isEnabled() const;
};

class BoolPropertyVerb : public EditSelectionVerb
{
private:
	const Name& propertyName;

public:
	BoolPropertyVerb(const std::string& name, DataModel* dataModel, const char* property);

protected:
	virtual void doIt(IDataState* dataState);
	virtual bool isChecked() const;
};

class AnchorVerb : public BoolPropertyVerb
{
public:
	AnchorVerb(DataModel* dataModel) : BoolPropertyVerb("AnchorVerb", dataModel, "Anchored") {}
};

class TranslucentVerb : public BoolPropertyVerb
{
public:
	TranslucentVerb(DataModel* dataModel) : BoolPropertyVerb("TranslucentVerb", dataModel, "Transparent") {}
};

class CanCollideVerb : public BoolPropertyVerb
{
public:
	CanCollideVerb(DataModel* dataModel) : BoolPropertyVerb("CanCollideVerb", dataModel, "CanCollide") {}
};

class MoveUpSelectionVerb : public EditSelectionVerb
{
private:
	float moveUpHeight;

public:
	MoveUpSelectionVerb(DataModel* dataModel, const std::string& name, float moveUpHeight)
		: EditSelectionVerb(name, dataModel), moveUpHeight(moveUpHeight)
	{
	}

	virtual void doIt(IDataState* dataState);
};

class MoveUpPlateVerb : public MoveUpSelectionVerb
{
public:
	MoveUpPlateVerb(DataModel* dataModel) : MoveUpSelectionVerb(dataModel, "SelectionUpPlate", 0.4f) {}
};

class MoveUpBrickVerb : public MoveUpSelectionVerb
{
public:
	MoveUpBrickVerb(DataModel* dataModel) : MoveUpSelectionVerb(dataModel, "SelectionUpBrick", 1.2f) {}
};

class RotateSelectionVerb : public RotateAxisCommand
{
public:
	RotateSelectionVerb(DataModel* dataModel);

protected:
	virtual G3D::Matrix3 getRotationAxis();
};

class TiltSelectionVerb : public RotateAxisCommand
{
public:
	TiltSelectionVerb(DataModel* dataModel);

protected:
	virtual G3D::Matrix3 getRotationAxis();
};

class PrimaryControllerCommand : public ControllerCommand
{
public:
	PrimaryControllerCommand(DataModel* dataModel);

	virtual void doIt(IDataState* dataState);
};

class SecondaryControllerCommand : public ControllerCommand
{
public:
	SecondaryControllerCommand(DataModel* dataModel);

	virtual void doIt(IDataState* dataState);
};

class GroupSelectionVerb : public EditSelectionVerb
{
public:
	GroupSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class WeldSelectionVerb : public EditSelectionVerb
{
public:
	WeldSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class SnapSelectionVerb : public EditSelectionVerb
{
public:
	SnapSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class UngroupSelectionVerb : public EditSelectionVerb
{
public:
	UngroupSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class DuplicateSelectionVerb : public EditSelectionVerb
{
public:
	DuplicateSelectionVerb(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class AttachCameraCommand : public EditSelectionVerb
{
public:
	AttachCameraCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

class TrackCameraCommand : public EditSelectionVerb
{
public:
	TrackCameraCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

class FollowCameraCommand : public EditSelectionVerb
{
public:
	FollowCameraCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

class WatchCameraCommand : public EditSelectionVerb
{
public:
	WatchCameraCommand(DataModel* dataModel);

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

class MoveDownSelectionVerb : public EditSelectionVerb
{
public:
	MoveDownSelectionVerb(DataModel* dataModel);

	virtual void doIt(IDataState* dataState);
};

class CameraVerb : public Verb
{
protected:
	Workspace* workspace;

	Camera* getCamera() const { return workspace->getCamera(); }

	ServiceClient<Selection> selection;

public:
	CameraVerb(std::string name, Workspace* workspace);

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class CameraPanLeftCommand : public CameraVerb
{
public:
	CameraPanLeftCommand(Workspace* workspace) : CameraVerb("CameraPanLeft", workspace) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class CameraPanRightCommand : public CameraVerb
{
public:
	CameraPanRightCommand(Workspace* workspace) : CameraVerb("CameraPanRight", workspace) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class CameraTiltUpCommand : public CameraVerb
{
public:
	CameraTiltUpCommand(Workspace* workspace) : CameraVerb("CameraTiltUp", workspace) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class CameraTiltDownCommand : public CameraVerb
{
public:
	CameraTiltDownCommand(Workspace* workspace) : CameraVerb("CameraTiltDown", workspace) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class CameraZoomInCommand : public CameraVerb
{
public:
	CameraZoomInCommand(Workspace* workspace) : CameraVerb("CameraZoomIn", workspace) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class CameraZoomOutCommand : public CameraVerb
{
public:
	CameraZoomOutCommand(Workspace* workspace) : CameraVerb("CameraZoomOut", workspace) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class CameraModelViewCommand : public CameraVerb
{
public:
	CameraModelViewCommand(Workspace* workspace) : CameraVerb("CameraModelView", workspace) {}

	virtual void doIt(IDataState* dataState);
};

class FixedCameraCommand : public CameraVerb
{
public:
	FixedCameraCommand(Workspace* workspace) : CameraVerb("FixedCamera", workspace) {}

	virtual bool isEnabled() const;
	virtual bool isChecked() const;
	virtual void doIt(IDataState* dataState);
};

class CameraCenterCommand : public CameraVerb
{
public:
	CameraCenterCommand(Workspace* workspace) : CameraVerb("CameraCenter", workspace) {}

	virtual bool isEnabled() const;
	virtual void doIt(IDataState* dataState);
};

class CameraZoomExtentsCommand : public CameraVerb
{
public:
	CameraZoomExtentsCommand(Workspace* workspace) : CameraVerb("CameraZoomExtents", workspace) {}

	virtual void doIt(IDataState* dataState);
};

} // namespace RBX

#endif // V8DATAMODEL_COMMANDS_H
