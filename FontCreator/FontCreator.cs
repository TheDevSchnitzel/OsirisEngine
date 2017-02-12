using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FontCreator {
    public partial class FontCreator : Form {
        public FontCreator() {
            InitializeComponent();

            this.Height = Convert.ToInt32(16 * NUD_SpaceY.Value) + 167;
            this.Width = Convert.ToInt32(15 * NUD_SpaceX.Value) + 40;
        }


        public class UV_Coordinates {
            public float LeftUpU, RightDownU, LeftUpV, RightDownV, TexLength, TexHeight;

            public UV_Coordinates() {
                LeftUpU = 0;
                LeftUpV = 0;
                RightDownU = 0;
                RightDownV = 0;
                TexLength = 0;
                TexHeight = 0;
            }
            public UV_Coordinates(float LeftUpU, float RightDownU, float LeftUpV, float RightDownV, float TexLength, float TexHeight) {
                this.LeftUpU = LeftUpU;
                this.LeftUpV = LeftUpV;
                this.RightDownU = RightDownU;
                this.RightDownV = RightDownV;
                this.TexLength = TexLength;
                this.TexHeight = TexHeight;
            }
        };

        private FontFamily fontFamily;

        private void button1_Click(object sender, EventArgs e) {
            Bitmap bmp2 = new Bitmap(100, 100);
            StringFormat sf = new StringFormat();
            sf.FormatFlags = StringFormatFlags.DirectionRightToLeft;
            Font font = new Font(fontFamily, 25, FontStyle.Regular, GraphicsUnit.Pixel);
            Dictionary<int, UV_Coordinates> mappingUV = new Dictionary<int, UV_Coordinates>();
            Hashtable bitmapMapping = new Hashtable();
            using ( Graphics g = Graphics.FromImage(bmp2) ) {
                int currX = 0;
                for ( int i = 0; i < 256; i++ ) {
                    g.Clear(Color.FromArgb(255,0,0,0));
                    using ( SolidBrush brush = new SolidBrush(Color.Red) ) {
                        g.DrawString(Char.ConvertFromUtf32(i), font, brush, new Point(50, 50), sf);
                    }
                    int firstX = 99999999, lastX = 0, firstY = 99999999, lastY = 0;
                    for ( int x = 0; x < bmp2.Width; x++ ) {
                        for ( int y = 0; y < bmp2.Height; y++ ) {
                            //if ( bmp.GetPixel(x, y) != Color.FromArgb(0, 0, 0, 0) && bmp.GetPixel(x, y) != Color.FromArgb(255, 0, 0, 0) ) {
                            //    //Console.WriteLine(bmp.GetPixel(x, y) + " _ " + x + ", " + y);
                            //}
                            if ( bmp2.GetPixel(x, y).R > 0 ) {
                                if ( x < firstX )
                                    firstX = x;
                                if ( x > lastX )
                                    lastX = x;
                                if ( y < firstY )
                                    firstY = y;
                                if ( y > lastY )
                                    lastY = y;
                            }
                        }
                    }
                    if ( firstX != 99999999 && lastX != 0 && firstY != 99999999 && lastY != 0 ) {
                        mappingUV.Add(i, new UV_Coordinates(firstX - 1, lastX + 1, firstY - 1, lastY + 1, ( lastX + 1 ) - ( firstX - 1 ), ( lastY + 1 ) - ( firstY - 1 )));
                        bitmapMapping.Add(i, (Bitmap)Deserialisierer.deserialize(Serialisierer.serialize(bmp2)));
                    }
                    else {
                        mappingUV.Add(i, new UV_Coordinates(0,0,0,0,0,0));
                    }
                }

                int widthMax = 0, heightMax = 0;
                foreach ( int key in mappingUV.Keys ) {
                    UV_Coordinates coord = mappingUV[key];
                    widthMax += Convert.ToInt32(coord.RightDownU - coord.LeftUpU);
                    if ( heightMax < ( coord.RightDownV - coord.LeftUpV ) ) {
                        heightMax = Convert.ToInt32(coord.RightDownV - coord.LeftUpV);
                    }
                }
                
                Bitmap tex = new Bitmap(widthMax+1, heightMax+1);
                Graphics g_tex = Graphics.FromImage(tex);
                g_tex.Clear(Color.FromArgb(255,0,0,0));
                foreach ( KeyValuePair<int, UV_Coordinates> pair in mappingUV ) {
                    int xCounter = 0, ytmpCache = 0;
                    for ( int x = Convert.ToInt32(pair.Value.LeftUpU); x < pair.Value.RightDownU + 1; x++ ) {
                        int yCounter = 0;
                        for ( int y = Convert.ToInt32(pair.Value.LeftUpV); y < pair.Value.RightDownV; y++ ) {
                            tex.SetPixel(currX + xCounter, yCounter, ((Bitmap)bitmapMapping[pair.Key]).GetPixel(x, y));
                            yCounter++;
                        }
                        ytmpCache = yCounter;
                        xCounter++;
                    }

                    int currXtmp = Convert.ToInt32(( pair.Value.RightDownU - pair.Value.LeftUpU ));
                    pair.Value.LeftUpV = 0; // V immer 0, da immer oben in der Textur begonnen wird  -> V ist vertikal
                    pair.Value.LeftUpU = currX;
                    pair.Value.RightDownV = ytmpCache;

                    currX += currXtmp;
                    pair.Value.RightDownU = currX;
                }

                FileStream f = new FileStream(fontFamily.Name + "_" + NUD_FontSize.Value + ".OIFont", FileMode.CreateNew);
                BinaryWriter binWrit = new BinaryWriter(f);
                foreach ( int i in mappingUV.Keys ) {
                    binWrit.Write(( (float)mappingUV[i].LeftUpU ) / tex.Width);
                    binWrit.Write(( (float)mappingUV[i].RightDownU ) / tex.Width);
                    binWrit.Write(( (float)mappingUV[i].LeftUpV ) / tex.Height);
                    binWrit.Write(( (float)mappingUV[i].RightDownV ) / tex.Height);
                    binWrit.Write((float)mappingUV[i].TexLength);
                    binWrit.Write((float)mappingUV[i].TexHeight);
                }
                tex.Save(f, ImageFormat.Png);
                f.Close();
            }
        }

        private void panel1_Paint(object sender, PaintEventArgs e) {
            using ( SolidBrush br = new SolidBrush(Color.Red) ) {
                StringFormat sf = new StringFormat();
                sf.FormatFlags = StringFormatFlags.DirectionRightToLeft;
                Font font = new Font(fontFamily, Convert.ToInt32(NUD_FontSize.Value), FontStyle.Regular, GraphicsUnit.Pixel);
                for ( int i = 0; i < 256; i++ ) {
                    Console.WriteLine(Convert.ToInt32(NUD_SpaceX.Value) * ( i % 16 ) + " _ " + Convert.ToInt32(NUD_SpaceY.Value) * ( i / 16 ));
                    e.Graphics.DrawString(Char.ConvertFromUtf32(i), font, br, new Point(Convert.ToInt32(NUD_SpaceX.Value) * ( i % 16 ), ( Convert.ToInt32(NUD_SpaceY.Value) * ( i / 16 ) )), sf);
                    //if ( i % 16 == 0 ) {
                    //    e.Graphics.DrawLine(new Pen(Color.Blue, 1), new Point(Convert.ToInt32(NUD_SpaceX.Value) * ( i / 16 ), 0), new Point(Convert.ToInt32(NUD_SpaceX.Value) * ( i / 16 ), PNL_FontTexture.Height));
                    //    e.Graphics.DrawLine(new Pen(Color.Yellow, 1), new Point(0, Convert.ToInt32(NUD_SpaceY.Value) * ( i / 16 )), new Point(PNL_FontTexture.Width, Convert.ToInt32(NUD_SpaceY.Value) * ( i / 16 )));
                    //}
                }
            }
        }

        private void FontCreator_Load(object sender, EventArgs e) {
            InstalledFontCollection installedFontCollection = new InstalledFontCollection();

            bool t = true;
            foreach ( FontFamily family in installedFontCollection.Families ) {
                if ( t ) {
                    fontFamily = family;
                    t = false;
                }
                CB_Fonts.Items.Add(family.Name);
            }
            CB_Fonts.SelectedIndex = 0;
        }

        private int NUD_SpaceX_tmp = 0;
        private void NUD_SpaceX_ValueChanged(object sender, EventArgs e) {
            if ( NUD_SpaceX_tmp > 0 ) {
                if ( NUD_SpaceX.Value > NUD_SpaceX_tmp ) {
                    this.Width += 16 * Convert.ToInt32(NUD_SpaceX.Value - NUD_SpaceX_tmp);
                    //PNL_FontTexture.Width += 16 * Convert.ToInt32(NUD_SpaceX.Value - NUD_SpaceX_tmp);
                }
                else {
                    this.Width -= 16 * Convert.ToInt32(NUD_SpaceX_tmp - NUD_SpaceX.Value);
                    //PNL_FontTexture.Width -= 16 * Convert.ToInt32(NUD_SpaceX_tmp - NUD_SpaceX.Value);
                }
            }
            NUD_SpaceX_tmp = Convert.ToInt32(NUD_SpaceX.Value);
            PNL_FontTexture.Refresh();
        }

        private int NUD_SpaceY_tmp = 0;
        private void NUD_SpaceY_ValueChanged(object sender, EventArgs e) {
            if ( NUD_SpaceY_tmp > 0 ) {
                if ( NUD_SpaceY.Value > NUD_SpaceY_tmp ) {
                    this.Height += 16 * Convert.ToInt32(NUD_SpaceY.Value - NUD_SpaceY_tmp);
                    //PNL_FontTexture.Height += 16*Convert.ToInt32(NUD_SpaceY.Value - NUD_SpaceY_tmp);
                }
                else {
                    this.Height -= 16 * Convert.ToInt32(NUD_SpaceY_tmp - NUD_SpaceY.Value);
                    //PNL_FontTexture.Height -= 16 * Convert.ToInt32(NUD_SpaceY_tmp - NUD_SpaceY.Value);
                }
            }
            NUD_SpaceY_tmp = Convert.ToInt32(NUD_SpaceY.Value);
            PNL_FontTexture.Refresh();
        }

        private void NUD_FontSize_ValueChanged(object sender, EventArgs e) {
            PNL_FontTexture.Refresh();
        }

        private void CB_Fonts_SelectedIndexChanged(object sender, EventArgs e) {
            InstalledFontCollection installedFontCollection = new InstalledFontCollection();
            foreach ( FontFamily family in installedFontCollection.Families ) {
                if ( family.Name == CB_Fonts.Text ) {
                    fontFamily = family;
                    break;
                }
            }

            PNL_FontTexture.Refresh();
        }     
    }
}
