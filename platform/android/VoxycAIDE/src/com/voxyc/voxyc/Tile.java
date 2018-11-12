/*
Copyright (C) 2018 Dimitri Lozovoy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

package com.voxyc.voxyc;

import java.io.*;
import java.nio.*;

/**
 *
 * @author dimitri
 */
public class Tile {

    public final int MAX_DOTS = 65536;
//    public TileType mType = TileType.NONE;
    public String mFilename = "";
    public TileMap3D mTileMap = null;
//    public RoamerBitmap mBackground = null;
    public boolean mBlank = false;
	public boolean changed = false;
//	public TileSet tileSet = null;

    public Tile() {

    }	
	
    public void save(OutputStream os) {
/*        if (mTileMap == null) {
            mTileMap = new TileMap3D(32, 32, 8, false);
        }*/

//        LozowareUI ui = Lozoware.getUI();

        try {
            os.write('P');
            os.write('X');
            os.write('E');
            os.write(1);
            os.write(0);
            os.write(0);

            // Write X size as integer

            byte[] sizeXBytes = ByteBuffer.allocate(4).putInt(mTileMap.mSizeX).array();
            os.write(sizeXBytes[0]);
            os.write(sizeXBytes[1]);
            os.write(sizeXBytes[2]);
            os.write(sizeXBytes[3]);

            // Write Y size as integer

            byte[] sizeYBytes = ByteBuffer.allocate(4).putInt(mTileMap.mSizeY).array();
            os.write(sizeYBytes[0]);
            os.write(sizeYBytes[1]);
            os.write(sizeYBytes[2]);
            os.write(sizeYBytes[3]);

            // Write Z size as integer

            byte[] sizeZBytes = ByteBuffer.allocate(4).putInt(mTileMap.mSizeZ).array();
            os.write(sizeZBytes[0]);
            os.write(sizeZBytes[1]);
            os.write(sizeZBytes[2]);
            os.write(sizeZBytes[3]);

            // Write the dots as byte array
			
//			if (mTileMap.mTiles == null)
//				Lozoware.print("mTileMap.mTiles is null");

            int numDotBytes = mTileMap.mSizeX * mTileMap.mSizeY * mTileMap.mSizeZ;

            byte[] byteTiles = new byte[numDotBytes];
            for (int i = 0; i < numDotBytes; i++) {
				
				// TODO: fix bug
//				if (byteTiles != null && mTileMap != null && mTileMap != null && mTileMap.mTiles != null && i < mTileMap.mTiles.length && i < byteTiles.length)
                    byteTiles[i] = (byte) mTileMap.mTiles[i];
            }
            os.write(byteTiles);

            // Write background image as PNG chunk if we have it

/*            if (mBackground != null) {
                os.write('P');
                os.write('N');
                os.write('G');
                os.write('I');
                os.write('M');
                os.write('G');

                ui.writePNGChunk(mBackground, os);
            }*/

            // Write end of PXE file marker

            os.write('E');
            os.write('N');
            os.write('D');
            os.write('P');
            os.write('X');
            os.write('E');
        } catch (IOException e) {
//			Lozoware.exception(e);
//            Lozoware.log(Lozoware.logtype.nonfatalerror, e.toString());
        }
    }

    public void load(InputStream is) {
//        LozowareUI ui = Lozoware.getUI();

        int sizex = 0;
        int sizey = 0;
        int sizez = 0;

        int siga = 0;
        int sigb = 0;
        int sigc = 0;

        try {
            siga = is.read();

            if (siga == -1) {
                mBlank = true;
                return;
            }

            sigb = is.read();
            sigc = is.read();

            if (siga == 80 && sigb == 88 && sigc == 69) {

                int vera = is.read();
                int verb = is.read();
                int verc = is.read();

                if (vera == 1 && verb == 0 && verc == 0) { 

                    mTileMap = new TileMap3D(false);

                    // Read size X integer form 4 bytes
                    byte[] sizeXBytes = new byte[4];
                    is.read(sizeXBytes);
                    sizex = ByteBuffer.wrap(sizeXBytes).getInt();

                    // Read size Y integer form 4 bytes
                    byte[] sizeYBytes = new byte[4];
                    is.read(sizeYBytes);
                    sizey = ByteBuffer.wrap(sizeYBytes).getInt();

                    // Read size Z integer form 4 bytes
                    byte[] sizeZBytes = new byte[4];
                    is.read(sizeZBytes);
                    sizez = ByteBuffer.wrap(sizeZBytes).getInt();

                    // Read the dots byte array
					mTileMap.optimize = false;
                    mTileMap.init(sizex, sizey, sizez);

                    int numDotBytes = sizex * sizey * sizez;

                    byte[] byteTiles = new byte[numDotBytes];
                    is.read(byteTiles);
                    for (int i = 0; i < numDotBytes; i++) {
                        mTileMap.mTiles[i] = byteTiles[i];
                    }

                    // Read the background image PNG chunk

                    int a = is.read();
                    int b = is.read();
                    int c = is.read();
                    int d = is.read();
                    int e = is.read();
                    int f = is.read();

/*                    if (a == 80
                            && b == 78
                            && c == 71
                            && d == 73
                            && e == 77
                            && f == 71) {
//						Lozoware.getUI().consoleInUIThread("reading png chunk " + mFilename);
                        mBackground = ui.readPNGChunk(is);
                    }*/
                }
            } else {
                sizex = siga;
                sizey = sigb;
                sizez = sigc;

                mTileMap = new TileMap3D(false);
                mTileMap.init(sizex, sizey, sizez);

                byte[] byteTiles = new byte[sizex * sizey * sizez];
                is.read(byteTiles);
                for (int i = 0; i < sizex * sizey * sizez; i++) {
                    mTileMap.mTiles[i] = byteTiles[i];
                }

            }
        } catch (IOException e) {
//			Lozoware.exception(e);
        } catch (NullPointerException e) {
//			Lozoware.exception(e);
        }
    }

    public void loadPXEFile(String fname) {
        loadPXEFile(fname, false);
    }

    public void loadPXEFile(String fname, boolean fromAsset) {
//        LozowareUI ui = Lozoware.getUI();

        File f = new File(fname);
        if (!fromAsset && !f.exists()) {
            return;
        }

        try {
            if (fromAsset) {
//                InputStream is = ui.getAssetInputStream(fname);
//                load(is);
//                is.close();
/*                InputStream is = ui.getAssetInputStream(fname);
                BufferedInputStream bis = new BufferedInputStream(is);
                load(bis);
                bis.close();
                is.close();*/
            } else {
                FileInputStream fin = new FileInputStream(new File(fname));
                load(fin);
                fin.close();
            }
        } catch (Exception e) {
//            Lozoware.exception(e);			
//            Lozoware.getUI().consoleInUIThread("Error loading PXE file " + fname);
//            Lozoware.log(Lozoware.logtype.nonfatalerror, e.toString());
        }

//        mTileMap = new TileMap3D();
//        mTileMap.loadPXE(fname);
    }

    public void writePXEFile(String fname) {
		
        // Make sure target exists
        File f = new File(fname);
        if (!f.exists()) {
            // Make sure dir exists
            File pd = new File(f.getParent());
            if (!pd.exists()) {
                pd.mkdirs();
            }
        }

        try {
            FileOutputStream fout = new FileOutputStream(new File(fname));
			
            save(fout);

            fout.close();
        } catch (Exception e) {
//			Lozoware.exception(e);
//            Lozoware.log(Lozoware.logtype.nonfatalerror, e.toString());
        }
    }

//    public void setBackgroundImage(RoamerBitmap bitmap) {
//        mBackground = bitmap;
//    }
	
	public String getDebugReport()
	{
		String s = "";
		
/*	s += "[MAX_DOTS=" + MAX_DOTS + ",";
    s += "mType=" + mType.toString() + ",";
    s += "mFilename=" + mFilename + ", ";
    s += "mTileMap=" + mTileMap + ", ";
    s += "mBackground=" + mBackground + ", ";
    s += "mBlank=" + mBlank + "]";*/
	
	return s;
	}
	
    public void loadTLMFile(String fname) {
        loadTLMFile(fname, false);
    }
	
    public void writeTLMFile(String fname) {

        // Make sure target exists
        File f = new File(fname);
        if (!f.exists()) {
            // Make sure dir exists
            File pd = new File(f.getParent());
            if (!pd.exists()) {
                pd.mkdirs();
            }
        }

        try {
			mTileMap.saveTLM(fname);
        } catch (Exception e) {
//			Lozoware.exception(e);
//            Lozoware.log(Lozoware.logtype.nonfatalerror, e.toString());
        }
    }
	
    public void loadTLMFile(String fname, boolean fromAsset) {
//        LozowareUI ui = Lozoware.getUI();

        File f = new File(fname);
        if (!fromAsset && !f.exists()) {
            return;
        }

        try {
            mTileMap = new TileMap3D();
            mTileMap.loadTLM(fname, fromAsset);
        } catch (Exception e) {
//            Lozoware.exception(e);			
//            Lozoware.getUI().consoleInUIThread("Error loading TLM file " + fname);
//            Lozoware.log(Lozoware.logtype.nonfatalerror, e.toString());
        }
    }
	
	public void backgroundToTilemap()
	{
/*		mTileMap = Lozoware.getUI().newTilemap3D(EngineState.MAX_BITMAP_SIZE, EngineState.MAX_BITMAP_SIZE, EngineState.MAX_BITMAP_SIZE, mFilename);

		mBackground.mScaleToNearestPowerOf2 = true;
		mBackground.getBitmap();
		
		for (int x = 0; x < EngineState.MAX_BITMAP_SIZE; x++)
		{
			for (int y = 0; y < EngineState.MAX_BITMAP_SIZE; y++)
			{
				mTileMap.setR(x, y, 0, mBackground.getPixelR(x, y));
				mTileMap.setG(x, y, 0, mBackground.getPixelG(x, y));
				mTileMap.setB(x, y, 0, mBackground.getPixelB(x, y));
				mTileMap.setA(x, y, 0, 255);
			}
		}*/
	}
}
