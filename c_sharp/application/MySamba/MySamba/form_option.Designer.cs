namespace MySamba
{
	partial class form_option
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.button1 = new System.Windows.Forms.Button();
			this.btnOK = new System.Windows.Forms.Button();
			this.textBoxPassword = new System.Windows.Forms.TextBox();
			this.textBoxId = new System.Windows.Forms.TextBox();
			this.textBoxPort = new System.Windows.Forms.TextBox();
			this.textBoxIp = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.label_id = new System.Windows.Forms.Label();
			this.address = new System.Windows.Forms.Label();
			this.textBoxRemotePath = new System.Windows.Forms.TextBox();
			this.textBoxLocalPath = new System.Windows.Forms.TextBox();
			this.btnDelDir = new System.Windows.Forms.Button();
			this.textBoxDir = new System.Windows.Forms.TextBox();
			this.btnAddDir = new System.Windows.Forms.Button();
			this.listBoxExclusiveDir = new System.Windows.Forms.ListBox();
			this.btnDelExt = new System.Windows.Forms.Button();
			this.btnAddExt = new System.Windows.Forms.Button();
			this.listBoxExclusiveExt = new System.Windows.Forms.ListBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.textBoxExt = new System.Windows.Forms.TextBox();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(660, 391);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 48;
			this.button1.Text = "Cancel";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// btnOK
			// 
			this.btnOK.Location = new System.Drawing.Point(561, 391);
			this.btnOK.Name = "btnOK";
			this.btnOK.Size = new System.Drawing.Size(75, 23);
			this.btnOK.TabIndex = 47;
			this.btnOK.Text = "OK";
			this.btnOK.UseVisualStyleBackColor = true;
			this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
			// 
			// textBoxPassword
			// 
			this.textBoxPassword.Location = new System.Drawing.Point(147, 74);
			this.textBoxPassword.Name = "textBoxPassword";
			this.textBoxPassword.Size = new System.Drawing.Size(198, 21);
			this.textBoxPassword.TabIndex = 46;
			// 
			// textBoxId
			// 
			this.textBoxId.Location = new System.Drawing.Point(147, 47);
			this.textBoxId.Name = "textBoxId";
			this.textBoxId.Size = new System.Drawing.Size(198, 21);
			this.textBoxId.TabIndex = 45;
			// 
			// textBoxPort
			// 
			this.textBoxPort.Location = new System.Drawing.Point(287, 14);
			this.textBoxPort.Name = "textBoxPort";
			this.textBoxPort.Size = new System.Drawing.Size(58, 21);
			this.textBoxPort.TabIndex = 44;
			// 
			// textBoxIp
			// 
			this.textBoxIp.Location = new System.Drawing.Point(147, 14);
			this.textBoxIp.Name = "textBoxIp";
			this.textBoxIp.Size = new System.Drawing.Size(134, 21);
			this.textBoxIp.TabIndex = 43;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(14, 132);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(97, 12);
			this.label3.TabIndex = 42;
			this.label3.Text = "remote directory";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(14, 107);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(85, 12);
			this.label2.TabIndex = 41;
			this.label2.Text = "local directory";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(14, 76);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(61, 12);
			this.label1.TabIndex = 40;
			this.label1.Text = "password";
			// 
			// label_id
			// 
			this.label_id.AutoSize = true;
			this.label_id.Location = new System.Drawing.Point(14, 50);
			this.label_id.Name = "label_id";
			this.label_id.Size = new System.Drawing.Size(15, 12);
			this.label_id.TabIndex = 39;
			this.label_id.Text = "id";
			// 
			// address
			// 
			this.address.AutoSize = true;
			this.address.Location = new System.Drawing.Point(14, 17);
			this.address.Name = "address";
			this.address.Size = new System.Drawing.Size(42, 12);
			this.address.TabIndex = 38;
			this.address.Text = "ip/port";
			// 
			// textBoxRemotePath
			// 
			this.textBoxRemotePath.Location = new System.Drawing.Point(147, 129);
			this.textBoxRemotePath.Name = "textBoxRemotePath";
			this.textBoxRemotePath.Size = new System.Drawing.Size(364, 21);
			this.textBoxRemotePath.TabIndex = 37;
			// 
			// textBoxLocalPath
			// 
			this.textBoxLocalPath.Location = new System.Drawing.Point(147, 104);
			this.textBoxLocalPath.Name = "textBoxLocalPath";
			this.textBoxLocalPath.Size = new System.Drawing.Size(364, 21);
			this.textBoxLocalPath.TabIndex = 36;
			// 
			// btnDelDir
			// 
			this.btnDelDir.Location = new System.Drawing.Point(634, 217);
			this.btnDelDir.Name = "btnDelDir";
			this.btnDelDir.Size = new System.Drawing.Size(101, 23);
			this.btnDelDir.TabIndex = 35;
			this.btnDelDir.Text = "del_directory";
			this.btnDelDir.UseVisualStyleBackColor = true;
			this.btnDelDir.Click += new System.EventHandler(this.btnDelDir_Click);
			// 
			// textBoxDir
			// 
			this.textBoxDir.Location = new System.Drawing.Point(336, 188);
			this.textBoxDir.Name = "textBoxDir";
			this.textBoxDir.Size = new System.Drawing.Size(244, 21);
			this.textBoxDir.TabIndex = 34;
			// 
			// btnAddDir
			// 
			this.btnAddDir.Location = new System.Drawing.Point(634, 186);
			this.btnAddDir.Name = "btnAddDir";
			this.btnAddDir.Size = new System.Drawing.Size(101, 23);
			this.btnAddDir.TabIndex = 33;
			this.btnAddDir.Text = "add_directory";
			this.btnAddDir.UseVisualStyleBackColor = true;
			this.btnAddDir.Click += new System.EventHandler(this.btnAddDir_Click);
			// 
			// listBoxExclusiveDir
			// 
			this.listBoxExclusiveDir.FormattingEnabled = true;
			this.listBoxExclusiveDir.ItemHeight = 12;
			this.listBoxExclusiveDir.Location = new System.Drawing.Point(336, 246);
			this.listBoxExclusiveDir.Name = "listBoxExclusiveDir";
			this.listBoxExclusiveDir.Size = new System.Drawing.Size(399, 124);
			this.listBoxExclusiveDir.TabIndex = 32;
			// 
			// btnDelExt
			// 
			this.btnDelExt.Location = new System.Drawing.Point(175, 215);
			this.btnDelExt.Name = "btnDelExt";
			this.btnDelExt.Size = new System.Drawing.Size(101, 23);
			this.btnDelExt.TabIndex = 31;
			this.btnDelExt.Text = "del_extention";
			this.btnDelExt.UseVisualStyleBackColor = true;
			this.btnDelExt.Click += new System.EventHandler(this.btnDelExt_Click);
			// 
			// btnAddExt
			// 
			this.btnAddExt.Location = new System.Drawing.Point(175, 186);
			this.btnAddExt.Name = "btnAddExt";
			this.btnAddExt.Size = new System.Drawing.Size(101, 23);
			this.btnAddExt.TabIndex = 30;
			this.btnAddExt.Text = "add_extention";
			this.btnAddExt.UseVisualStyleBackColor = true;
			this.btnAddExt.Click += new System.EventHandler(this.btnAddExt_Click);
			// 
			// listBoxExclusiveExt
			// 
			this.listBoxExclusiveExt.FormattingEnabled = true;
			this.listBoxExclusiveExt.ItemHeight = 12;
			this.listBoxExclusiveExt.Location = new System.Drawing.Point(32, 244);
			this.listBoxExclusiveExt.Name = "listBoxExclusiveExt";
			this.listBoxExclusiveExt.Size = new System.Drawing.Size(244, 124);
			this.listBoxExclusiveExt.TabIndex = 29;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.textBoxExt);
			this.groupBox1.Location = new System.Drawing.Point(16, 169);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(279, 214);
			this.groupBox1.TabIndex = 49;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "exclusive extension";
			// 
			// textBoxExt
			// 
			this.textBoxExt.Location = new System.Drawing.Point(16, 17);
			this.textBoxExt.Name = "textBoxExt";
			this.textBoxExt.Size = new System.Drawing.Size(100, 21);
			this.textBoxExt.TabIndex = 2;
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.textBox1);
			this.groupBox2.Location = new System.Drawing.Point(311, 171);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(437, 214);
			this.groupBox2.TabIndex = 50;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "exclusive directory";
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(25, 17);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(91, 21);
			this.textBox1.TabIndex = 2;
			// 
			// form_option
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(760, 425);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.btnOK);
			this.Controls.Add(this.textBoxPassword);
			this.Controls.Add(this.textBoxId);
			this.Controls.Add(this.textBoxPort);
			this.Controls.Add(this.textBoxIp);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.label_id);
			this.Controls.Add(this.address);
			this.Controls.Add(this.textBoxRemotePath);
			this.Controls.Add(this.textBoxLocalPath);
			this.Controls.Add(this.btnDelDir);
			this.Controls.Add(this.textBoxDir);
			this.Controls.Add(this.btnAddDir);
			this.Controls.Add(this.listBoxExclusiveDir);
			this.Controls.Add(this.btnDelExt);
			this.Controls.Add(this.btnAddExt);
			this.Controls.Add(this.listBoxExclusiveExt);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.groupBox2);
			this.Name = "form_option";
			this.Text = "MySamba option";
			this.Load += new System.EventHandler(this.form_option_Load);
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.Button btnOK;
		private System.Windows.Forms.TextBox textBoxPassword;
		private System.Windows.Forms.TextBox textBoxId;
		private System.Windows.Forms.TextBox textBoxPort;
		private System.Windows.Forms.TextBox textBoxIp;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label_id;
		private System.Windows.Forms.Label address;
		private System.Windows.Forms.TextBox textBoxRemotePath;
		private System.Windows.Forms.TextBox textBoxLocalPath;
		private System.Windows.Forms.Button btnDelDir;
		private System.Windows.Forms.TextBox textBoxDir;
		private System.Windows.Forms.Button btnAddDir;
		private System.Windows.Forms.ListBox listBoxExclusiveDir;
		private System.Windows.Forms.Button btnDelExt;
		private System.Windows.Forms.Button btnAddExt;
		private System.Windows.Forms.ListBox listBoxExclusiveExt;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TextBox textBoxExt;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.TextBox textBox1;
	}
}