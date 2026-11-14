using System;
using System.Collections;
using System.Runtime.InteropServices;


namespace Viewer
{
	[StructLayout( LayoutKind.Sequential)]
	public struct TestDataStruct
	{
		public float x;
		public float y;
		public float z;
		public int elementNumber;
		public int id;
	};

	[StructLayout( LayoutKind.Sequential)]
	public struct AtomStruct
	{
		public int elementNumber;
		public int ID;
		public float x;
		public float y;
		public float z;
	};

	[StructLayout( LayoutKind.Sequential)]
	public struct BondStruct 
	{
		public int atomIndex1;
		public int atomIndex2;
		public int order;
	};

	public class Atom 
	{
		private double x;
		public double X 
		{
			get { return x; }
			set { x = value; }
		}
		private double y;
		public double Y 
		{
			get { return y; }
			set { y = value; }
		}

		private double z;
		public double Z 
		{
			get { return z; }
			set { z = value; }
		}
		private int elementNumber; 
		public int ElementNumber 
		{
			get { return elementNumber; }
			set { elementNumber = value; }
		}
		private int id;
		public int ID 
		{ 
			get { return id; }
			set { id = value; }
		}

		public Atom(int elementNum, double xd, double yd, double zd, int idi) 
		{
			elementNumber = elementNum;
			x = xd;
			y = yd;
			z = zd;
			id = idi;
		}
	}

	public class Bond 
	{
		private int atomIndex1;
		public int AtomIndex1 
		{
			get { return atomIndex1; }
		}

		private int atomIndex2;
		public int AtomIndex2 
		{
			get { return atomIndex2; }
		}

		private int order;
		public int Order
		{
			get { return order; }
			set { order = value; }
		}

		
		public Bond()
		{
			atomIndex1 = atomIndex2 = - 1;
			order = 1;
		}
		
		public Bond(int atomIndex1, int atomIndex2, int order)
		{
			this.atomIndex1 = atomIndex1;
			this.atomIndex2 = atomIndex2;
			this.order = order;
		}
		
	}
	/// <summary>
	/// Summary description for MolecularModel.
	/// </summary>
	public class MolecularModel
	{

		private System.Collections.ArrayList _atoms;
		public ArrayList Atoms 
		{
			get { return _atoms; }
		}
		private System.Collections.ArrayList _bonds;
		public ArrayList Bonds 
		{
			get { return _bonds; }
		}
 
		public MolecularModel(ArrayList atoms, ArrayList bonds)
		{
			_atoms = atoms;
			_bonds = bonds;
		}


		static public MolecularModel createEthanol() 
		{
			ArrayList atoms = new ArrayList();
			ArrayList bonds = new ArrayList();
			atoms.Add( new Atom(6, -0.4059, -0.0951, 0, 0));
			atoms.Add( new Atom(6, 0.2102, -0.0646, 0, 1));
			atoms.Add( new Atom(8, 0.3722, 0.4851, 0, 2));
			atoms.Add( new Atom(1, -0.5339, -0.5162, 0, 3));
			atoms.Add( new Atom(1, -0.5636, 0.1050, 0.3589, 4));
			atoms.Add( new Atom(1, -0.5636, 0.1050, -0.3589, 5));
			atoms.Add( new Atom(1, 0.3666, -0.2657, 0.3592, 6));
			atoms.Add( new Atom(1, 0.3666, -0.2657, -0.3592, 7));
			atoms.Add( new Atom(1, 0.7515, 0.5122, 0, 8));

			
			// Connect carbon skeleton
			bonds.Add(new Bond(0, 1, 1));
			
			// Connect carbon1 to oxygen
			bonds.Add(new Bond(1, 2, 1));

			//Connect carbon1 to two hydrogens
			bonds.Add(new Bond(1, 6, 1));
			bonds.Add(new Bond(1, 7, 1));

			// Connect carbon2 to three hydrogens
			bonds.Add(new Bond(0, 5, 1));
			bonds.Add(new Bond(0, 4, 1));
			bonds.Add(new Bond(0, 3, 1));


			// Connet oxygen to hydrogen
			bonds.Add(new Bond(2, 8, 1));

			


			return new MolecularModel(atoms, bonds);
		}

		static public MolecularModel createSimple() 
		{
			ArrayList atoms = new ArrayList();
			ArrayList bonds = new ArrayList();
			atoms.Add( new Atom(6, -1, 0, 0, 0));
			atoms.Add( new Atom(6, 1, 1, 0, 1));
			
			// Connect carbon skeleton
			bonds.Add(new Bond(0, 1, 1));
			


			return new MolecularModel(atoms, bonds);
		}

	
	}
}
