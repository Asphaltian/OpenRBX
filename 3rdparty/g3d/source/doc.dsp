# Microsoft Developer Studio Project File - Name="doc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=doc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "doc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "doc.mak" CFG="doc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "doc - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "doc - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "doc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "doc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "doc___Win32_Debug"
# PROP BASE Intermediate_Dir "doc___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "doc___Win32_Debug"
# PROP Intermediate_Dir "doc___Win32_Debug"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "doc - Win32 Release"
# Name "doc - Win32 Debug"
# Begin Group "Build Scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\build.py
# End Source File
# Begin Source File

SOURCE=..\buildlib.py
# End Source File
# Begin Source File

SOURCE=.\Doxyfile
# End Source File
# Begin Source File

SOURCE=..\g3dinstaller.nsi
# End Source File
# Begin Source File

SOURCE=.\OnlineDoxyfile
# End Source File
# End Group
# Begin Group "Guide"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\docsource\guide-app.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-compiling.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-concepts.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-debug.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-effects.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-files.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-install.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-intro.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-meshes.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-physics.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-shaders.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-tutorial.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-ui.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-var.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-video.h"
# End Source File
# Begin Source File

SOURCE=".\docsource\guide-welcome.h"
# End Source File
# Begin Source File

SOURCE=.\docsource\license.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\docsource\changelog.h
# End Source File
# Begin Source File

SOURCE=.\docsource\contributors.h
# End Source File
# Begin Source File

SOURCE=.\docsource\error.h
# End Source File
# Begin Source File

SOURCE=.\html\footer.html
# End Source File
# Begin Source File

SOURCE=.\docsource\frequent.h
# End Source File
# Begin Source File

SOURCE=.\html\gettingstarted.html
# End Source File
# Begin Source File

SOURCE=.\html\header.html
# End Source File
# Begin Source File

SOURCE=..\readme.html
# End Source File
# Begin Source File

SOURCE=".\docsource\topic-index.h"
# End Source File
# End Target
# End Project
