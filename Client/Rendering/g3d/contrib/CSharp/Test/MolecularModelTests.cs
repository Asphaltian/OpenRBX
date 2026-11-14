using System;
using System.Runtime.InteropServices;
using Viewer;

namespace Test
{



	
	/// <summary>
	/// Summary description for MolecularModelTests.
	/// </summary>
	public class MolecularModelTests
	{


		[DllImport("../../../ChemCPP/Debug/ChemCPP.dll")]
		public static extern void TestStructExtreme(Viewer.TestDataStruct
			foo);

		[DllImport("../../../ChemCPP/Debug/ChemCPP.dll")]
		public static extern int TestAtomStruct(AtomStruct atom);


		public MolecularModelTests()
		{
		}

		public static bool test1() 
		{
			AtomStruct test;
			test.x = 3.5F;
			test.y = 1.0F;
			test.z = 2992.3F;
			test.elementNumber = 12;
			test.ID = 2; 

			int answer = 0; 
			answer = TestAtomStruct(test);

			if (answer == 14) 
				return true;
			else
				return false;
				

		}
	}
}
