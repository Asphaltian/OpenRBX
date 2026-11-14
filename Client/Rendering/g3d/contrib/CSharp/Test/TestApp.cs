using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Test
{
	/// <summary>
	/// Summary description for TestApp.
	/// </summary>
	public class TestApp : System.Windows.Forms.Form
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.GroupBox groupBox2;

		private Viewer.Viewer viewer;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Button button4;
		private System.Windows.Forms.CheckBox checkBox1;

		Timer rotateTimer; 
		bool rotating = false; 

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public TestApp()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			addViewer();


			// Create a rotation timer to run in the background
			rotateTimer = new Timer();
			rotateTimer.Interval = 50;
			rotateTimer.Tick +=new EventHandler(rotateTimer_Tick);
			rotateTimer.Start();
			rotateTimer.Enabled = true;

		}

		public void addViewer() 
		{
			this.SuspendLayout();
			viewer = new Viewer.Viewer( groupBox2.Width, groupBox2.Height);
			viewer.Dock = DockStyle.Fill;
			groupBox2.Controls.Add(viewer);
			showSample();
			this.ResumeLayout();
			this.Invalidate(true);

		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.checkBox1 = new System.Windows.Forms.CheckBox();
			this.button4 = new System.Windows.Forms.Button();
			this.button3 = new System.Windows.Forms.Button();
			this.button2 = new System.Windows.Forms.Button();
			this.button1 = new System.Windows.Forms.Button();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.checkBox1);
			this.groupBox1.Controls.Add(this.button4);
			this.groupBox1.Controls.Add(this.button3);
			this.groupBox1.Controls.Add(this.button2);
			this.groupBox1.Controls.Add(this.button1);
			this.groupBox1.Location = new System.Drawing.Point(8, 8);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(632, 40);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Tests";
			// 
			// checkBox1
			// 
			this.checkBox1.Dock = System.Windows.Forms.DockStyle.Left;
			this.checkBox1.Location = new System.Drawing.Point(504, 16);
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Size = new System.Drawing.Size(104, 21);
			this.checkBox1.TabIndex = 4;
			this.checkBox1.Text = "autorotate";
			this.checkBox1.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
			// 
			// button4
			// 
			this.button4.Dock = System.Windows.Forms.DockStyle.Left;
			this.button4.Location = new System.Drawing.Point(352, 16);
			this.button4.Name = "button4";
			this.button4.Size = new System.Drawing.Size(152, 21);
			this.button4.TabIndex = 3;
			this.button4.Text = "Create Test Molecule";
			this.button4.Click += new System.EventHandler(this.button4_Click);
			// 
			// button3
			// 
			this.button3.Dock = System.Windows.Forms.DockStyle.Left;
			this.button3.Location = new System.Drawing.Point(232, 16);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(120, 21);
			this.button3.TabIndex = 2;
			this.button3.Text = "Make Ethanol";
			this.button3.Click += new System.EventHandler(this.button3_Click);
			// 
			// button2
			// 
			this.button2.Dock = System.Windows.Forms.DockStyle.Left;
			this.button2.Location = new System.Drawing.Point(112, 16);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(120, 21);
			this.button2.TabIndex = 1;
			this.button2.Text = "ViewerTests";
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// button1
			// 
			this.button1.Dock = System.Windows.Forms.DockStyle.Left;
			this.button1.Location = new System.Drawing.Point(3, 16);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(109, 21);
			this.button1.TabIndex = 0;
			this.button1.Text = "MoleculeTests";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// groupBox2
			// 
			this.groupBox2.Location = new System.Drawing.Point(8, 56);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(632, 552);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "Viewer";
			// 
			// TestApp
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(640, 613);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.Name = "TestApp";
			this.Text = "Test WinChemOpenView";
			this.groupBox1.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new TestApp());
		}

		private void button1_Click(object sender, System.EventArgs e)
		{
			bool success = MolecularModelTests.test1();
			if (success) 
				button1.BackColor = Color.PaleGreen;
			else 
				button1.BackColor = Color.Salmon;
		}

		private void button2_Click(object sender, System.EventArgs e)
		{
			bool success = ViewerTests.test1();
			if (success) 
				button2.BackColor = Color.PaleGreen;
			else 
				button2.BackColor = Color.Salmon;
		
		}

		private void button3_Click(object sender, System.EventArgs e)
		{
			showSample();
		}

		public void showSample() {
			Viewer.MolecularModel mm = Viewer.MolecularModel.createEthanol();
			viewer.setModel(mm);
		}

		private void button4_Click(object sender, System.EventArgs e)
		{
			ArrayList atoms = new ArrayList();
			ArrayList bonds = new ArrayList();

			int id=0; 

			Viewer.Atom foo = new Viewer.Atom(12, 0.1111, 0.2222, 0.3333, id++);
			Viewer.Atom bar = new Viewer.Atom(13, 0.4444, 0.5555, 0.6666, id++);
			Viewer.Atom baz = new Viewer.Atom(14, 777, 888, 999, id++);

			atoms.Add(foo);
			atoms.Add(bar);
			atoms.Add(baz);

			bonds.Add(new Viewer.Bond(0, 1, 1));
			bonds.Add(new Viewer.Bond(1, 2, 2));
			
			Viewer.MolecularModel mm = new Viewer.MolecularModel(atoms, bonds);
			
			viewer.setModel(mm);

		}

		private void checkBox1_CheckedChanged(object sender, System.EventArgs e)
		{
			rotating = !rotating;		
		}

		/// <summary>
		/// A tick of the autorotate timer.  Rotates the molecule just a little.
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void rotateTimer_Tick(object sender, EventArgs e)
		{
			float spinPerTickRadians = 0.01F;

			if(rotating)
			{				
				viewer.spinY(spinPerTickRadians);
			}
			if(!(this.Created))
			{
				// If the screen is gone, kill the rotating thread.
				rotateTimer.Stop();
				rotateTimer.Enabled = false;
			}
		}

		private void pictureBox1_Paint(object sender, System.Windows.Forms.PaintEventArgs e)
		{
			for (int i=0; i < 3; i++) 
			{
				e.Graphics.DrawString("I'm adorable!", new Font("Arial", 30), Brushes.Pink, 30+i, 30+i);

			}
		}
	
	}
}
