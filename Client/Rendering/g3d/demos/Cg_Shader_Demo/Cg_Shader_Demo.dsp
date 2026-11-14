# Microsoft Developer Studio Project File - Name="Cg_Shader_Demo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Cg_Shader_Demo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Cg_Shader_Demo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Cg_Shader_Demo.mak" CFG="Cg_Shader_Demo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Cg_Shader_Demo - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Cg_Shader_Demo - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Cg_Shader_Demo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../temp/Cg_Shader_Demo/Release"
# PROP Intermediate_Dir "../../../temp//Cg_Shader_Demo/Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /out:"../Cg_Shader_Demo.exe" /libpath:"../../win32-lib" /libpath:"../../../temp/win32-lib"

!ELSEIF  "$(CFG)" == "Cg_Shader_Demo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Cg_Shader_Demo.exe" /pdbtype:sept /libpath:"../../win32-lib" /libpath:"../../../temp/win32-lib"

!ENDIF 

# Begin Target

# Name "Cg_Shader_Demo - Win32 Release"
# Name "Cg_Shader_Demo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Mesh.h
# End Source File
# End Group
# Begin Group "Cg Shaders"

# PROP Default_Filter ".cg"
# Begin Source File

SOURCE=.\parallaxPP.cg

!IF  "$(CFG)" == "Cg_Shader_Demo - Win32 Release"

# Begin Custom Build - Compiling $(InputPath)
InputPath=.\parallaxPP.cg
InputName=parallaxPP

"$(InputName).pp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"C:\Program Files\NVIDIA Corporation\Cg\bin\cgc" -profile arbfp1 -o $(InputName).pp $(InputName).cg

# End Custom Build

!ELSEIF  "$(CFG)" == "Cg_Shader_Demo - Win32 Debug"

# Begin Custom Build - Compiling $(InputPath)
InputPath=.\parallaxPP.cg
InputName=parallaxPP

"$(InputName).pp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"C:\Program Files\NVIDIA Corporation\Cg\bin\cgc" -profile arbfp1 -o $(InputName).pp $(InputName).cg

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\parallaxVP.cg

!IF  "$(CFG)" == "Cg_Shader_Demo - Win32 Release"

# Begin Custom Build - Compiling $(InputPath)
InputPath=.\parallaxVP.cg
InputName=parallaxVP

"$(InputName).vp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"C:\Program Files\NVIDIA Corporation\Cg\bin\cgc" -profile arbvp1 -o $(InputName).vp $(InputName).cg

# End Custom Build

!ELSEIF  "$(CFG)" == "Cg_Shader_Demo - Win32 Debug"

# Begin Custom Build - Compiling $(InputPath)
InputPath=.\parallaxVP.cg
InputName=parallaxVP

"$(InputName).vp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"C:\Program Files\NVIDIA Corporation\Cg\bin\cgc" -profile arbvp1 -o $(InputName).vp $(InputName).cg

# End Custom Build

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
