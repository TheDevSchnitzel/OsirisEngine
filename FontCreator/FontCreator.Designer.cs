namespace FontCreator {
    partial class FontCreator {
        /// <summary>
        /// Erforderliche Designervariable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Verwendete Ressourcen bereinigen.
        /// </summary>
        /// <param name="disposing">True, wenn verwaltete Ressourcen gelöscht werden sollen; andernfalls False.</param>
        protected override void Dispose(bool disposing) {
            if (disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Vom Windows Form-Designer generierter Code

        /// <summary>
        /// Erforderliche Methode für die Designerunterstützung.
        /// Der Inhalt der Methode darf nicht mit dem Code-Editor geändert werden.
        /// </summary>
        private void InitializeComponent( ) {
            this.PNL_FontTexture = new System.Windows.Forms.Panel();
            this.BTN_CreateTexture = new System.Windows.Forms.Button();
            this.CB_Fonts = new System.Windows.Forms.ComboBox();
            this.L_Fonts = new System.Windows.Forms.Label();
            this.NUD_SpaceX = new System.Windows.Forms.NumericUpDown();
            this.NUD_SpaceY = new System.Windows.Forms.NumericUpDown();
            this.L_SpaceX = new System.Windows.Forms.Label();
            this.L_SpaceY = new System.Windows.Forms.Label();
            this.L_FontSize = new System.Windows.Forms.Label();
            this.NUD_FontSize = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.NUD_SpaceX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUD_SpaceY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUD_FontSize)).BeginInit();
            this.SuspendLayout();
            // 
            // PNL_FontTexture
            // 
            this.PNL_FontTexture.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.PNL_FontTexture.BackColor = System.Drawing.Color.Black;
            this.PNL_FontTexture.Location = new System.Drawing.Point(12, 117);
            this.PNL_FontTexture.Name = "PNL_FontTexture";
            this.PNL_FontTexture.Size = new System.Drawing.Size(265, 262);
            this.PNL_FontTexture.TabIndex = 0;
            this.PNL_FontTexture.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // BTN_CreateTexture
            // 
            this.BTN_CreateTexture.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.BTN_CreateTexture.Font = new System.Drawing.Font("Microsoft Sans Serif", 20F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.BTN_CreateTexture.Location = new System.Drawing.Point(134, 39);
            this.BTN_CreateTexture.Name = "BTN_CreateTexture";
            this.BTN_CreateTexture.Size = new System.Drawing.Size(144, 72);
            this.BTN_CreateTexture.TabIndex = 1;
            this.BTN_CreateTexture.Text = "Export";
            this.BTN_CreateTexture.UseVisualStyleBackColor = true;
            this.BTN_CreateTexture.Click += new System.EventHandler(this.button1_Click);
            // 
            // CB_Fonts
            // 
            this.CB_Fonts.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.CB_Fonts.FormattingEnabled = true;
            this.CB_Fonts.Location = new System.Drawing.Point(49, 12);
            this.CB_Fonts.Name = "CB_Fonts";
            this.CB_Fonts.Size = new System.Drawing.Size(229, 21);
            this.CB_Fonts.TabIndex = 2;
            this.CB_Fonts.SelectedIndexChanged += new System.EventHandler(this.CB_Fonts_SelectedIndexChanged);
            // 
            // L_Fonts
            // 
            this.L_Fonts.AutoSize = true;
            this.L_Fonts.Location = new System.Drawing.Point(7, 15);
            this.L_Fonts.Name = "L_Fonts";
            this.L_Fonts.Size = new System.Drawing.Size(36, 13);
            this.L_Fonts.TabIndex = 3;
            this.L_Fonts.Text = "Fonts:";
            // 
            // NUD_SpaceX
            // 
            this.NUD_SpaceX.Location = new System.Drawing.Point(72, 39);
            this.NUD_SpaceX.Name = "NUD_SpaceX";
            this.NUD_SpaceX.Size = new System.Drawing.Size(56, 20);
            this.NUD_SpaceX.TabIndex = 4;
            this.NUD_SpaceX.Value = new decimal(new int[] {
            22,
            0,
            0,
            0});
            this.NUD_SpaceX.ValueChanged += new System.EventHandler(this.NUD_SpaceX_ValueChanged);
            // 
            // NUD_SpaceY
            // 
            this.NUD_SpaceY.Location = new System.Drawing.Point(72, 65);
            this.NUD_SpaceY.Name = "NUD_SpaceY";
            this.NUD_SpaceY.Size = new System.Drawing.Size(56, 20);
            this.NUD_SpaceY.TabIndex = 5;
            this.NUD_SpaceY.Value = new decimal(new int[] {
            22,
            0,
            0,
            0});
            this.NUD_SpaceY.ValueChanged += new System.EventHandler(this.NUD_SpaceY_ValueChanged);
            // 
            // L_SpaceX
            // 
            this.L_SpaceX.AutoSize = true;
            this.L_SpaceX.Location = new System.Drawing.Point(7, 41);
            this.L_SpaceX.Name = "L_SpaceX";
            this.L_SpaceX.Size = new System.Drawing.Size(59, 13);
            this.L_SpaceX.TabIndex = 6;
            this.L_SpaceX.Text = "Abstand X:";
            // 
            // L_SpaceY
            // 
            this.L_SpaceY.AutoSize = true;
            this.L_SpaceY.Location = new System.Drawing.Point(7, 67);
            this.L_SpaceY.Name = "L_SpaceY";
            this.L_SpaceY.Size = new System.Drawing.Size(59, 13);
            this.L_SpaceY.TabIndex = 7;
            this.L_SpaceY.Text = "Abstand Y:";
            // 
            // L_FontSize
            // 
            this.L_FontSize.AutoSize = true;
            this.L_FontSize.Location = new System.Drawing.Point(7, 93);
            this.L_FontSize.Name = "L_FontSize";
            this.L_FontSize.Size = new System.Drawing.Size(54, 13);
            this.L_FontSize.TabIndex = 9;
            this.L_FontSize.Text = "Font Size:";
            // 
            // NUD_FontSize
            // 
            this.NUD_FontSize.Location = new System.Drawing.Point(72, 91);
            this.NUD_FontSize.Name = "NUD_FontSize";
            this.NUD_FontSize.Size = new System.Drawing.Size(56, 20);
            this.NUD_FontSize.TabIndex = 8;
            this.NUD_FontSize.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            this.NUD_FontSize.ValueChanged += new System.EventHandler(this.NUD_FontSize_ValueChanged);
            // 
            // FontCreator
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(290, 391);
            this.Controls.Add(this.L_FontSize);
            this.Controls.Add(this.NUD_FontSize);
            this.Controls.Add(this.L_SpaceY);
            this.Controls.Add(this.L_SpaceX);
            this.Controls.Add(this.NUD_SpaceY);
            this.Controls.Add(this.NUD_SpaceX);
            this.Controls.Add(this.L_Fonts);
            this.Controls.Add(this.CB_Fonts);
            this.Controls.Add(this.BTN_CreateTexture);
            this.Controls.Add(this.PNL_FontTexture);
            this.Name = "FontCreator";
            this.Text = "OsirisI - FontCreator";
            this.Load += new System.EventHandler(this.FontCreator_Load);
            ((System.ComponentModel.ISupportInitialize)(this.NUD_SpaceX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUD_SpaceY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.NUD_FontSize)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel PNL_FontTexture;
        private System.Windows.Forms.Button BTN_CreateTexture;
        private System.Windows.Forms.ComboBox CB_Fonts;
        private System.Windows.Forms.Label L_Fonts;
        private System.Windows.Forms.NumericUpDown NUD_SpaceX;
        private System.Windows.Forms.NumericUpDown NUD_SpaceY;
        private System.Windows.Forms.Label L_SpaceX;
        private System.Windows.Forms.Label L_SpaceY;
        private System.Windows.Forms.Label L_FontSize;
        private System.Windows.Forms.NumericUpDown NUD_FontSize;
    }
}

