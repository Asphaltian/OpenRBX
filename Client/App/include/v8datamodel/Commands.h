#ifndef V8DATAMODEL_COMMANDS_H
#define V8DATAMODEL_COMMANDS_H

#include "decomp.h"
#include "v8datamodel/Workspace.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"

#include <string>

namespace RBX {

class Camera;
class DataModel;
class RunService;
class Selection;

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
