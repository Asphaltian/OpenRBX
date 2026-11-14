CSharp contrib project for G3D
author: sascha becker shine sbshine@gmail.com with endless help from corey taylor and morgan mcguire
created: 6.21.05

This project demonstrates one technique for integrating G3D into a C#/Microsoft .
NET Windows Forms application. This particular application is for displaying 
chemical molecules in 3D,but the general architecture can be applied to whatever 
you want to render. 

TestApp is a C# application. It instantiates a Viewer, which is a C# .NET control. 
Viewer calls G3D via the RendererWrapper, which is a C# wrapper around ChemCPP::Renderer. 
ChemCPP::Renderer is an unmanaged C++ class which calls G3D. G3D uses a Win32Window, 
created with the HWND (window handle) for the Viewer control. Giving G3D the HWND 
means that G3D can draw directly to the screen. 

Besides the rendering architecture, this project also demonstrates passing simple 
data structures from C# to C++. Viewer.MolecularModel defines an AtomStruct and a 
BondStruct; MolecularModel.h defines the same structs in C++. Careful use of 
System.Runtime.InteropServices attributes allows the C# AtomStruct to be passed to C++, 
and treated as a C++ AtomStruct. 

The Viewer assembly can be dropped into any C# .NET application. It searches for the 
ChemCPP DLL at runtime; your job when deploying this project is to make sure ChemCPP.dll 
is in the location Viewer expects it to be. For instance, the current setup has the 
following code in RendererWrapper.cs: 

	[DllImport("../../../ChemCPP/Debug/ChemCPP.dll")]
	public static extern IntPtr CreateCppInternals();	
 
That path to the dll must be the path to the dll. There are probably other ways 
to do this involving system library paths and environment variables. The technique I'm 
using is very explicit. 
 
The current setup of the solution allows you to change any of the code in the solution, 
then hit F5 (build and run), and immediately see that new code operating. 

A few problems:
* The display flickers. 
* We're using p/invoke, which isn't the ideal way to do this. Find a better way then post it! 
* Two pointer reference errors on shutdown

Setup Instructions:
Get the G3D data package, and put it in C:/libraries/g3d/data. In particular, this application
uses data/font/arial.fnt and data/sky/plainsky.



 
 
 
 