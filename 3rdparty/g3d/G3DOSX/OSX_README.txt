How to Build a G3D application on OS X 

Caveats
-------

Setting up G3D projects for use with Apple Xcode is rather complicated, mostly due to the weirdnesses of Xcode and Cocoa development. G3D applications on OS X must be Cocoa applications due to the SDL implementation. This document will walk you through setting up a project. 

These instructions assume that you have some basic familiarity with Xcode. 
These instructions have been designed for OS X 10.4 (Tiger) and Xcode 2.1/2.2. Previous versions should of Xcode should work acceptably, though we do not provide any project files or support for them. Project Builder might work, but hasn't been tested. Previous versions of Xcode have been used with G3D, though we are primarily using as up-to-date as possible versions

If you just want to see G3D in action on the Mac, just open a demo project's *.xcodeproj file and hit, "build and run."

Prerequisites
-------------

You will need:

1.) OS X version 10.4 (Tiger)
2.) The Developer Tools from OS X 10.4, including Xcode

3.) SDL Development Libraries, version 1.2.9.  Available
    at http://www.libsdl.org/ 

    You will need the two files SDLMain.m and SDLMain.h
    from the SDL distribution. As a convenience, these files
    are also in the G3D distribution. You can use these
    files from either place since they're identical except
    for comments. 

4.) The G3D libraries, obviously.  You will need the files
    libG3D.a and libGLG3D.a

How to Build a G3D application on OS X version 10.4 Using Xcode
---------------------------------------------------------------

1.) Start Xcode.

2.) Create a New Project - Select File and New Project... In the dialog box that follows, select Cocoa Application under the heading Application. Pick a name and location for your new project and select finish. You should now have a window in Xcode with your new project in it. If you see MY_PROJ anywhere below, assume that it is actually the name of your new project.

3.) Delete Unused Files - Because Xcode created some files for you, which you do not need, you will now delete them from your system. Select main.m and MainMenu.nib (hold down command/apple key to multiple select, or do each one individually). Hit the delete button on your keyboard, or select Delete from the Edit menu. Xcode will ask you, "The references to be deleted refer to files on your disk. Do you wish to delete only the references and not the related files?" You can go ahead and select "Delete References & Files" because these will no longer be needed. If it makes you nervous, feel free to just select "Delete References".

4.) Remove Unused Referenced Libraries - Xcode also added some library references to your project, which are unnecessary. Select AppKit.framework, CoreData.framework, and Foundation.framework. Hit the delete button on your keyboard, or select Delete from the Edit menu. DO NOT GO ON AUTO PILOT HERE. Xcode will ask you, "The references to be deleted refer to files on your disk. Do you wish to delete only the references and not the related files?" Make sure you select "Delete References". That should be the default button.
Create a new main.cpp containing a main() routine, or copy the main.cpp from the source/demos/ directory to your project's directory, and add the new or existing file to your Xcode project.

5.) Add OpenGL.framework to your Xcode project. - OpenGL.framework is always located in /System/Library/Frameworks/. The dialog for adding the framework to the project has a checkbox labeled "Copy items into destination group's folder (if needed)". Make sure that this checkbox is unchecked.

6.) Add SDL.framework to your Xcode project. - SDL.framework should be located in /Library/Frameworks or ~/Library/Frameworks. The dialog for adding the framework to the project has a checkbox labeled "Copy items into destination group's folder (if needed)". Make sure that this checkbox is unchecked.

7.) Add the path to the %G3D header files to the include search path. - This setting is in the same location as Framework Search Paths. Select the project icon, press Command-I (or right-click/ctrl-click and "Get Info"), and then modify the "Header Search Path" entry to include the search path to the %G3D headers.

8.) Add the %G3D libraries to the project. - Using the Project/Add To Project menu (or right-click/ctrl-click on "Frameworks" and "Add" - "Existing Frameworks"), add libG3D.a and libGLG3D.a to the project. Recent versions of %G3D for OS X built the Independent JPEG Group (IJG) code into a separate library (libjpeg.a). The IJG code has only recently been folded into libG3D.a. If you're using %G3D 6.03 or earlier, you might have to add libjpeg.a to your project.

9.) Add "-lz" to "Other Linker Flags". - This tells the linker to use the system's version of zlib, which G3D requires.

10.) Very important: Set the link order of libraries. - This is non-obvious but very important. Failure to set the link order of libraries will give inexplicable link failures. In the left pane of the Xcode project, select the "Target" item. This will have at least one item under it corresponding to the name of the application. Select the application target (it will have a triangle next to it) and expand the triangle by clicking on it. Under the "Frameworks and Libraries" group make sure that libG3D.a and libGLG3D.a are listed before OpenGL.framework. Failure to do this will result in over 80 link errors related to OpenGL extensions.

11.) Add SDLMain.m and SDLMain.h. - SDL for OS X requires that we build SDLMain.m into the project. Failure to do this will result in the symbol _main being undefined when you try to link your %G3D application. SDLMain.m and SDLMain.h are included in %G3D for OS X for convenience. Note that they are not part of %G3D per se, and they are part of the SDL distribution.

12.) Write Your Code! - You can now write code for %G3D just like you would for Linux or Windows.

13.) Optional: Nullify Windows Cludges. - Define __stdcall and __cdecl to evaluate to nothing in the preprocessor: __stdcall and __cdecl are needed for Windows code, but they don't do anything on OS X. You can add __stdcall= and __cdecl= to the "Preprocessor Macros" section of the Project Info.

14.) Optional: Set Relative Paths. - In general when working in Xcode, make sure all source files have Path Type "Relative to Enclosing Group" or "Relative to Project" selected in file info (rather than "Absolute Path"). This is so that if the Xcode project is moved or a directory name is changed, the project file links will not break. If you highlight all source and header files and press Cmd-I, you can set Path Type easily.

If you have problems
--------------------

If you have a problem with G3D on OS X, feel free to post to the
G3D user's forum on Sourceforge. 


