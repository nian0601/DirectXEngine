namespace LogReader
{
    partial class OpenFileWindow
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
            this.FolderPath_TxtBox = new System.Windows.Forms.TextBox();
            this.BrowseFolder_Btn = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.OpenFile_Btn = new System.Windows.Forms.Button();
            this.Cancel_Btn = new System.Windows.Forms.Button();
            this.FolderBrowserDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.LogList = new System.Windows.Forms.ListBox();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // FolderPath_TxtBox
            // 
            this.FolderPath_TxtBox.Location = new System.Drawing.Point(13, 13);
            this.FolderPath_TxtBox.Name = "FolderPath_TxtBox";
            this.FolderPath_TxtBox.Size = new System.Drawing.Size(182, 20);
            this.FolderPath_TxtBox.TabIndex = 0;
            // 
            // BrowseFolder_Btn
            // 
            this.BrowseFolder_Btn.Location = new System.Drawing.Point(197, 12);
            this.BrowseFolder_Btn.Name = "BrowseFolder_Btn";
            this.BrowseFolder_Btn.Size = new System.Drawing.Size(75, 20);
            this.BrowseFolder_Btn.TabIndex = 1;
            this.BrowseFolder_Btn.Text = "Browse";
            this.BrowseFolder_Btn.UseVisualStyleBackColor = true;
            this.BrowseFolder_Btn.Click += new System.EventHandler(this.BrowseFolder_Btn_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.LogList);
            this.panel1.Location = new System.Drawing.Point(13, 40);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(259, 127);
            this.panel1.TabIndex = 2;
            // 
            // OpenFile_Btn
            // 
            this.OpenFile_Btn.Location = new System.Drawing.Point(13, 174);
            this.OpenFile_Btn.Name = "OpenFile_Btn";
            this.OpenFile_Btn.Size = new System.Drawing.Size(125, 23);
            this.OpenFile_Btn.TabIndex = 3;
            this.OpenFile_Btn.Text = "Open File";
            this.OpenFile_Btn.UseVisualStyleBackColor = true;
            this.OpenFile_Btn.Click += new System.EventHandler(this.OpenFile_Btn_Click);
            // 
            // Cancel_Btn
            // 
            this.Cancel_Btn.Location = new System.Drawing.Point(147, 174);
            this.Cancel_Btn.Name = "Cancel_Btn";
            this.Cancel_Btn.Size = new System.Drawing.Size(125, 23);
            this.Cancel_Btn.TabIndex = 4;
            this.Cancel_Btn.Text = "Cancel";
            this.Cancel_Btn.UseVisualStyleBackColor = true;
            this.Cancel_Btn.Click += new System.EventHandler(this.Cancel_Btn_Click);
            // 
            // LogList
            // 
            this.LogList.FormattingEnabled = true;
            this.LogList.Location = new System.Drawing.Point(4, 4);
            this.LogList.Name = "LogList";
            this.LogList.Size = new System.Drawing.Size(252, 121);
            this.LogList.TabIndex = 0;
            this.LogList.SelectedIndexChanged += new System.EventHandler(this.LogList_SelectedIndexChanged);
            // 
            // OpenFileWindow
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 211);
            this.Controls.Add(this.Cancel_Btn);
            this.Controls.Add(this.OpenFile_Btn);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.BrowseFolder_Btn);
            this.Controls.Add(this.FolderPath_TxtBox);
            this.Name = "OpenFileWindow";
            this.Text = "Open Log";
            this.panel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox FolderPath_TxtBox;
        private System.Windows.Forms.Button BrowseFolder_Btn;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button OpenFile_Btn;
        private System.Windows.Forms.Button Cancel_Btn;
        private System.Windows.Forms.FolderBrowserDialog FolderBrowserDialog;
        private System.Windows.Forms.ListBox LogList;
    }
}