namespace Localizer {
    partial class Localizer {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.resetSettings = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.resetSettings.Location = new System.Drawing.Point(0, 0);
            this.resetSettings.Name = "resetStation";
            this.resetSettings.Size = new System.Drawing.Size(297, 23);
            this.resetSettings.TabIndex = 0;
            this.resetSettings.Text = "Reset Station Settings";
            this.resetSettings.UseVisualStyleBackColor = true;
            this.resetSettings.Click += new System.EventHandler(this.resetSettings_Click);
            // 
            // Localizer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.resetSettings);
            this.Name = "Localizer";
            this.Text = "Localizer";
            this.ResumeLayout(false);

        }

        #endregion
        
        private System.Windows.Forms.Button resetSettings;
    }
}