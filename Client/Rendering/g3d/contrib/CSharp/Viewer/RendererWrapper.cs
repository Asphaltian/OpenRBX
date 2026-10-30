using System;
using System.Runtime.InteropServices;


namespace Viewer
{
	/// <summary>
	/// Summary description for RendererWrapper.
	/// </summary>
	public class RendererWrapper
	{
		IntPtr _renderer; 

// In the CSharp development project, it's set up to work with 
// a relative path:
//		[DllImport("../../../ChemCPP/Debug/ChemCPP.dll")]
// But for deployment it's going to be easier to just put the dll 
// right into the runtime path
// 		[ DllImport("ChemCPP.dll") ]

		[ DllImport("ChemCPP.dll") ]
		public static extern IntPtr CreateCppInternals();	

		[ DllImport("ChemCPP.dll") ]
		public static extern void DeleteCppInternals( IntPtr instance );	

		[ DllImport("ChemCPP.dll") ]
		public static extern void InitHWND( IntPtr instance, IntPtr hwnd, int width, int height );	

		[ DllImport("ChemCPP.dll") ]
		public static extern void RenderScene( IntPtr instance );	

		[ DllImport("ChemCPP.dll") ]
		public static extern void ClearModel( IntPtr instance );	

		[ DllImport("ChemCPP.dll") ]
		public static extern void AddAnAtom( IntPtr instance, AtomStruct atom);	

		[ DllImport("ChemCPP.dll") ]
		public static extern void AddBond( IntPtr instance, BondStruct bond);	

		[ DllImport("ChemCPP.dll") ]
		public static extern void SpinY( IntPtr instance, float radians);	

		[ DllImport("ChemCPP.dll") ]
		public static extern void TestStructExtreme(TestDataStruct
			foo);


		public RendererWrapper()
		{
			_renderer = CreateCppInternals();
		}

		public void initHWND(IntPtr hwnd, int width, int height) 
		{
			InitHWND(_renderer, hwnd, width, height);
		}

		public void renderScene() 
		{
			RenderScene(_renderer);
		}

		public void clearModel() 
		{
			ClearModel(_renderer);
		}


		public void spinY(float spinRadians) 
		{
			SpinY(_renderer, spinRadians);
		}

		public void setModel(MolecularModel mm) 
		{

			clearModel();

			for (int a = 0; a < mm.Atoms.Count; a++) 
			{
				Atom atom = (Atom) mm.Atoms[a];
				AtomStruct anAtom = new AtomStruct();
				anAtom.x = (float) atom.X;
				anAtom.y = (float) atom.Y;
				anAtom.z = (float) atom.Z;
				anAtom.ID = atom.ID;
				anAtom.elementNumber = atom.ElementNumber;
				
				AddAnAtom(_renderer, anAtom);
			}

			for (int b = 0; b < mm.Bonds.Count; b++) 
			{
				Bond bond = (Bond) mm.Bonds[b];
				BondStruct aBond = new BondStruct();
				aBond.atomIndex1 = bond.AtomIndex1;
				aBond.atomIndex2 = bond.AtomIndex2;
				AddBond(_renderer, aBond);
			}

		}
	

		public static bool test2() 
		{
			TestDataStruct myTest= new TestDataStruct();
			myTest.x = 0.1f;
			myTest.y = 0.2f;
			myTest.z = 0.3f;
			myTest.id = 5;
			myTest.elementNumber = 7;
			TestStructExtreme(myTest);

			return true; 
		}
	}
}
