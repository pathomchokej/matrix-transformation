namespace Matrix_Transformation
{
    partial class FormMain
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            groupBox1 = new GroupBox();
            panelView = new Panel();
            SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Anchor = AnchorStyles.Top | AnchorStyles.Left | AnchorStyles.Right;
            groupBox1.Location = new Point(32, 33);
            groupBox1.Name = "groupBox1";
            groupBox1.Size = new Size(1304, 342);
            groupBox1.TabIndex = 0;
            groupBox1.TabStop = false;
            groupBox1.Text = "Control";
            // 
            // panelView
            // 
            panelView.Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right;
            panelView.BackColor = SystemColors.ControlLight;
            panelView.Location = new Point(32, 409);
            panelView.Name = "panelView";
            panelView.Size = new Size(1304, 538);
            panelView.TabIndex = 1;
            // 
            // FormMain
            // 
            AutoScaleDimensions = new SizeF(13F, 32F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1374, 979);
            Controls.Add(panelView);
            Controls.Add(groupBox1);
            Name = "FormMain";
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Maxtrix Transformation";
            WindowState = FormWindowState.Maximized;
            ResumeLayout(false);
        }

        #endregion

        private GroupBox groupBox1;
        private Panel panelView;
    }
}
