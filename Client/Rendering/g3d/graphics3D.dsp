# Microsoft Developer Studio Project File - Name="graphics3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=graphics3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "graphics3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "graphics3D.mak" CFG="graphics3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "graphics3D - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "graphics3D - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "graphics3D - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\temp\win32-lib"
# PROP Intermediate_Dir "../temp/release/g3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /I "include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /FD /c
# SUBTRACT CPP /Fr /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"../temp/graphics3D.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\temp\win32-lib\G3D.lib"

!ELSEIF  "$(CFG)" == "graphics3D - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\temp\win32-lib"
# PROP Intermediate_Dir "../temp/debug/g3d"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /Od /I "include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_CRT_SECURE_NO_DEPRECATE" /FD /GZ /c
# SUBTRACT CPP /Z<none> /Fr /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"../temp/graphics3D-debug.bsc"
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\temp\win32-lib\G3D-debug.lib"

!ENDIF 

# Begin Target

# Name "graphics3D - Win32 Release"
# Name "graphics3D - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\G3Dcpp\AABox.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\BinaryFormat.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\BinaryInput.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\BinaryOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Box.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Capsule.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\CollisionDetection.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Color3.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Color3uint8.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Color4.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Color4uint8.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Cone.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\ConvexPolyhedron.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\CoordinateFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Crypto.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Crypto_md5.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Cylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\debugAssert.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Discovery.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\fileutils.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\format.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\g3derror.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\g3dmath.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GImage.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GImage_bayer.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GImage_bmp.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GImage_jpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GImage_png.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GImage_ppm.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GImage_tga.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GLight.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\GThread.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\license.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Line.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\LineSegment.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Matrix3.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Matrix4.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\MeshAlg.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\MeshAlgAdjacency.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\MeshAlgWeld.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\MeshBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\NetAddress.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\NetworkDevice.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\PhysicsFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Plane.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\prompt.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Quat.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Ray.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\RegistryUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Stopwatch.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\stringutils.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\System.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\TextInput.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\TextOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Triangle.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Vector2.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Vector2int16.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Vector3.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Vector3int16.cpp
# End Source File
# Begin Source File

SOURCE=.\G3Dcpp\Vector4.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\include\G3D\AABox.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\AABSPTree.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Array.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\AtomicInt32.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\BinaryFormat.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\BinaryInput.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\BinaryOutput.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Box.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Capsule.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\CollisionDetection.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Color3.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Color3.inl
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Color3uint8.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Color4.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Color4.inl
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Color4uint8.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Cone.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\ConvexPolyhedron.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\CoordinateFrame.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Crypto.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Cylinder.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\debug.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\debugAssert.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\debugPrintf.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Discovery.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\fileutils.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\format.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\g3derror.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\G3DGameUnits.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\g3dmath.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\g3dmath.inl
# End Source File
# Begin Source File

SOURCE=.\include\G3D\GCamera.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\GImage.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\GLight.h
# End Source File
# Begin Source File

SOURCE=.\include\graphics3D.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\GThread.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Line.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\LineSegment.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Log.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Matrix3.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Matrix4.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\MeshAlg.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\MeshBuilder.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\NetAddress.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\NetworkDevice.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\PhysicsFrame.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Plane.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\platform.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\prompt.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Quat.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Quat.inl
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Queue.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Ray.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Rect2D.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\ReferenceCount.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\RegistryUtil.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Set.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Sphere.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\spline.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Stopwatch.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\stringutils.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\System.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Table.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\TextInput.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\TextOutput.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Triangle.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Vector2.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Vector2.inl
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Vector2int16.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Vector3.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Vector3.inl
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Vector3int16.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Vector4.h
# End Source File
# Begin Source File

SOURCE=.\include\G3D\Vector4.inl
# End Source File
# Begin Source File

SOURCE=.\include\G3D\vectorMath.h
# End Source File
# End Group
# Begin Group "IJG"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\IJG\cderror.h
# End Source File
# Begin Source File

SOURCE=.\IJG\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jcapimin.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcapistd.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jccoefct.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jccolor.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcdctmgr.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jchuff.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jcinit.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcmainct.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcmarker.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcmaster.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcomapi.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jcparam.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcphuff.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcprepct.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jcsample.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jctrans.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdapimin.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdapistd.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdatadst.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdatasrc.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdcoefct.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdcolor.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdct.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jddctmgr.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdhuff.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jdinput.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdmainct.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdmarker.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdmaster.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdmerge.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdphuff.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdpostct.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdsample.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jdtrans.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jerror.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jerror.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jfdctflt.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jfdctfst.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jfdctint.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jidctflt.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jidctfst.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jidctint.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jidctred.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jmemansi.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jmemmgr.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\IJG\jquant1.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jquant2.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jutils.c
# End Source File
# Begin Source File

SOURCE=.\IJG\jversion.h
# End Source File
# Begin Source File

SOURCE=.\IJG\transupp.h
# End Source File
# End Group
# Begin Group "png"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\png\png.c
# End Source File
# Begin Source File

SOURCE=.\png\png.h
# End Source File
# Begin Source File

SOURCE=.\png\pngconf.h
# End Source File
# Begin Source File

SOURCE=.\png\pngerror.c
# End Source File
# Begin Source File

SOURCE=.\png\pnggccrd.c
# End Source File
# Begin Source File

SOURCE=.\png\pngget.c
# End Source File
# Begin Source File

SOURCE=.\png\pngmem.c
# End Source File
# Begin Source File

SOURCE=.\png\pngpread.c
# End Source File
# Begin Source File

SOURCE=.\png\pngread.c
# End Source File
# Begin Source File

SOURCE=.\png\pngrio.c
# End Source File
# Begin Source File

SOURCE=.\png\pngrtran.c
# End Source File
# Begin Source File

SOURCE=.\png\pngrutil.c
# End Source File
# Begin Source File

SOURCE=.\png\pngset.c
# End Source File
# Begin Source File

SOURCE=.\png\pngtrans.c
# End Source File
# Begin Source File

SOURCE=.\png\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=.\png\pngwio.c
# End Source File
# Begin Source File

SOURCE=.\png\pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\png\pngwtran.c
# End Source File
# Begin Source File

SOURCE=.\png\pngwutil.c
# End Source File
# End Group
# End Target
# End Project
