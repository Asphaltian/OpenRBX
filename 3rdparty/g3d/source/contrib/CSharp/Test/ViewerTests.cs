using System;
using System.Runtime.InteropServices;
using Viewer;


namespace Test
{
	/// <summary>
	/// Summary description for ViewerTests.
	/// </summary>
	public class ViewerTests
	{


		[DllImport("../../../ChemCPP/Debug/ChemCPP.dll")]
		public static extern IntPtr CreateCppInternals();

		[DllImport("../../../ChemCPP/Debug/ChemCPP.dll")]
		public static extern void DeleteCppInternals(IntPtr instance);
		
		[DllImport("../../../ChemCPP/Debug/ChemCPP.dll")]
		public static extern void InitHWND(IntPtr instance, IntPtr hwnd, int width, int height); 
		
		[DllImport("../../../ChemCPP/Debug/ChemCPP.dll")]
		public static extern void RenderScene(IntPtr instance); 

		public ViewerTests()
		{
			//
			// TODO: Add constructor logic here
			//
		}

		public static bool test1() 
		{
			IntPtr internals = CreateCppInternals();
			return true;
		}
	}
}
