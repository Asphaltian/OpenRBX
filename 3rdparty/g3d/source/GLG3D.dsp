# Microsoft Developer Studio Project File - Name="GLG3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GLG3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GLG3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GLG3D.mak" CFG="GLG3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GLG3D - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GLG3D - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GLG3D - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\temp\win32-lib"
# PROP Intermediate_Dir "../temp/release/glg3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /I "include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\temp/GLG3D.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "GLG3D - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\temp\win32-lib"
# PROP Intermediate_Dir "../temp/debug/glg3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /ZI /Od /I "include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /FD /GZ /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"..\temp/GLG3D-debug.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\temp\win32-lib\GLG3D-debug.lib"

!ENDIF 

# Begin Target

# Name "GLG3D - Win32 Release"
# Name "GLG3D - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\GLG3Dcpp\DDSTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Draw.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\DXCaps.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\edgeFeatures.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Framebuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\G3DGameUnits.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\GApp.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\getOpenGLState.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\GFont.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\glcalls.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\GLCaps.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\glenumtostring.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\GModule.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\GPUProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\GWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\IFSModel.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\LightingParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\ManualCameraController.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\MD2Model.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\MD2Model_load.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Milestone.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\PixelProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\PosedModel.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Renderbuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\RenderDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\SDLWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Shader.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\shadowVolume.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Shape.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Sky.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\tesselate.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Texture.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\TextureFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\TextureManager.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\UserInput.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\VAR.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\VARArea.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\VertexProgram.cpp
# End Source File
# Begin Source File

SOURCE=.\GLG3Dcpp\Win32Window.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\GLG3Dcpp\directinput8.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Draw.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\DXCaps.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\edgeFeatures.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Framebuffer.h
# End Source File
# Begin Source File

SOURCE=.\include\G3DAll.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\GApp.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\getOpenGLState.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\GFont.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\glcalls.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\GLCaps.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\glFormat.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\glheaders.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\GModule.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\GPUProgram.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\GWindow.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\GWindowSettings.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\IFSModel.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\LightingParameters.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\ManualCameraController.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\MD2Model.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Milestone.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\PixelProgram.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\PosedModel.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Renderbuffer.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\RenderDevice.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\SDLWindow.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Shader.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\shadowVolume.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Shape.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Sky.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\tesselate.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Texture.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\TextureFormat.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\TextureManager.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\UserInput.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\VAR.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\VARArea.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\VertexProgram.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\Win32Window.h
# End Source File
# Begin Source File

SOURCE=.\include\GLG3D\X11Window.h
# End Source File
# End Group
# Begin Group "GL Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\include\GL\gl.h
# End Source File
# Begin Source File

SOURCE=.\include\GL\glext.h
# End Source File
# End Group
# End Target
# End Project
