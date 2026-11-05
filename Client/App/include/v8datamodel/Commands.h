#ifndef V8DATAMODEL_COMMANDS_H
#define V8DATAMODEL_COMMANDS_H

#include "decomp.h"
#include "v8datamodel/Workspace.h"
#include "v8tree/Service.h"
#include "v8tree/Verb.h"

#include <string>

namespace RBX {

class Camera;
class Selection;

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
