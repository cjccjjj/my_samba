namespace MySamba
{
	partial class Form1
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
			this.btnConfig = new System.Windows.Forms.Button();
			this.textBoxMonitor = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// btnConfig
			// 
			this.btnConfig.Location = new System.Drawing.Point(484, 6);
			this.btnConfig.Name = "btnConfig";
			this.btnConfig.Size = new System.Drawing.Size(75, 23);
			this.btnConfig.TabIndex = 16;
			this.btnConfig.Text = "config";
			this.btnConfig.UseVisualStyleBackColor = true;
			this.btnConfig.Click += new System.EventHandler(this.btnConfig_Click);
			// 
			// textBoxMonitor
			// 
			this.textBoxMonitor.Location = new System.Drawing.Point(12, 35);
			this.textBoxMonitor.Multiline = true;
			this.textBoxMonitor.Name = "textBoxMonitor";
			this.textBoxMonitor.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.textBoxMonitor.Size = new System.Drawing.Size(547, 342);
			this.textBoxMonitor.TabIndex = 15;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(566, 383);
			this.Controls.Add(this.btnConfig);
			this.Controls.Add(this.textBoxMonitor);
			this.Name = "Form1";
			this.Text = "MySamba";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button btnConfig;
		private System.Windows.Forms.TextBox textBoxMonitor;
	}
}

