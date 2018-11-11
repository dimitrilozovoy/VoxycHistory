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

import java.io.File;
import java.io.FileInputStream;
import java.nio.*;
import java.io.InputStream;
import java.io.*;
import java.util.*;

/**
 *
 * @author dimitri
 */
public class TileMap3D {

	public boolean fixGap = true;
	public boolean optimize = true;
    // Tiles
    public final int MAX_TILES = 524288;//65536; // 110592 524288
    public final int BYTEFLOAT_PRECISION = 32;
    public int BYTEFLOAT_MIN = -128 / BYTEFLOAT_PRECISION + 1;
    public int BYTEFLOAT_MAX = 128 / BYTEFLOAT_PRECISION - 1;
	public int OFFSET_HEIGHT_PAD = 2;
//	public int DEFAULT_TEXTURE_SPAN = 4;
    public byte[] mTiles = null;
    public byte[] mOffsetsX = null;
    public byte[] mOffsetsY = null;
    public byte[] mOffsetsZ = null;
    public int mSizeX = 0;
    public int mSizeY = 0;
    public int mSizeZ = 0;
    public Palette mPalette = null;
    public int mNumTiles = 0;
    public byte[] mTexRotations = null;
    // Entities
    public final int MAX_ENTITIES = 1024;
    public int numEntities = 0;
    // Locs
    public final int MAX_LOCS = 65536;
    public int mNumLocs = 0;
    // Skybox
    public final int NUM_SKYBOX_PARTS = 6;
    public int[] mSkyBox;
    // Merged surfaces
    public BitSet mergedBits = null;
    public int numMergedSurfs = 0;
//    public ArrayList<MergedSurface> mergedSurfs = null;
	// Optimized data structure
	public byte[] heightMap;
	public byte[][] optTiles;
	public byte[][] optOffsetsX;
	public byte[][] optOffsetsY;
	public byte[][] optOffsetsZ;
	public int tileSize;
	public boolean autoCropped = false;
	//public int textureSpan = EngineState.DEFAULT_TEXTURE_SPAN;
	//public String id = DDLUtils.getRandStr(5);
	int textureSpan = 1;

    public TileMap3D() {
//        init(16, 16, 16);
        init(64, 64, 8, 64);
    }
	
	public TileMap3D(boolean opt) {
//        init(16, 16, 16);
		optimize = opt;
        init(64, 64, 8, 64);
    }
	
	
	public TileMap3D(int sizex, int sizey, int sizez) {
		init(sizex, sizey, sizez, 64);
	}
	
	public TileMap3D(int sizex, int sizey, int sizez, boolean optimize) {
		this.optimize = optimize;
		init(sizex, sizey, sizez, 64);
	}

    public TileMap3D(int sizex, int sizey, int sizez, int tilesize) {
        init(sizex, sizey, sizez, tilesize);
    }
	
	public void init(int sizex, int sizey, int sizez) {
		init(sizex, sizey, sizez, 64);
	}

    public void init(int sizex, int sizey, int sizez, int tileSize) {
		
	//	try
	//	{
		System.gc();
		
		if (fixGap)
			OFFSET_HEIGHT_PAD = 0;

        // Init tiles
        mSizeX = sizex;
        mSizeY = sizey;
        mSizeZ = sizez;
		
		this.tileSize = tileSize;

        mNumTiles = sizex * sizey * sizez;
		
		// Init optimized memory structure
		if (optimize)
		{
			heightMap = new byte[mSizeX * mSizeY];

			for (int i = 0; i < mSizeX * mSizeY; i++)
			{
				heightMap[i] = 0;
			}

			optTiles = new byte[mSizeX * mSizeY][];

			for (int i = 0; i < (mSizeX * mSizeY); i++)
			{
				optTiles[i] = new byte[0];
			}

			optOffsetsX = new byte[(mSizeX + 1) * (mSizeY + 1)][];

			for (int i = 0; i < ((mSizeX + 1) * (mSizeY + 1)); i++)
			{
				optOffsetsX[i] = new byte[0];
			}

			optOffsetsY = new byte[(mSizeX + 1) * (mSizeY + 1)][];

			for (int i = 0; i < ((mSizeX + 1) * (mSizeY + 1)); i++)
			{
				optOffsetsY[i] = new byte[0];
			}

			optOffsetsZ = new byte[(mSizeX + 1) * (mSizeY + 1)][];

			for (int i = 0; i < ((mSizeX + 1) * (mSizeY + 1)); i++)
			{
				optOffsetsZ[i] = new byte[0];
			}
		}
		else
		{
        mTiles = new byte[mNumTiles];

        int offsetLen = (mSizeX + 1) * (mSizeY + 1) * (mSizeZ + 1);
        mOffsetsX = new byte[offsetLen];
        mOffsetsY = new byte[offsetLen];
        mOffsetsZ = new byte[offsetLen];

        for (int i = 0; i < mNumTiles; i++) {
            mTiles[i] = 0;
        }
        for (int i = 0; i < offsetLen; i++) {
            mOffsetsX[i] = 0;
        }
        for (int i = 0; i < offsetLen; i++) {
            mOffsetsY[i] = 0;
        }
        for (int i = 0; i < offsetLen; i++) {
            mOffsetsZ[i] = 0;
        }
		}

//        mTexRotations = new byte[mNumTiles * 6];
        mPalette = new Palette();		

        // Merged surfs
//		recalcMergedSurfs();
/*		}
		catch (Exception e)
		{
			Lozoware.exception(e);
			Lozoware.getUI().consoleInUIThread(e.getMessage());
		}*/
    }

    public int coordsToIdx(int x, int y, int z) {
        return (mSizeX * mSizeY * z) + (y * mSizeX) + x;
    }

    public void set(int x, int y, int z, byte value) {
		
		if (optimize && x >= 0 && y >= 0 && z >= 0 && x < mSizeX && y < mSizeY && z < mSizeZ)
		{
			if (getHeight(x, y) <= z)
			{
			    setHeight(x, y, z + 1);
			}

//			Lozoware.getUI().consoleInUIThread("" + y * mSizeX + x + " z = " + z);
			if (y * mSizeX + x < optTiles.length && z < optTiles[y * mSizeX + x].length)
 	            optTiles[y * mSizeX + x][z] = value;
		}
		
        if (x >= 0 && y >= 0 && z >= 0 && x < mSizeX && y < mSizeY && z < mSizeZ) {
			if (mTiles == null)
				return;
			
            int idx = coordsToIdx(x, y, z);
			
			if (idx < mTiles.length)
                mTiles[idx] = value;
        }		
    }

    public void set(int x, int y, int z, int value) {
		set(x, y, z, (byte)value);
    }

    public byte get(int x, int y, int z) {
		
		if (optimize)
    	{
//			if (getHeight(x, y) > z && z >= 0)
			if (getHeight(x, y) > z && z >= 0 && y * mSizeX + x < optTiles.length && z < optTiles[y * mSizeX + x].length)
			{
				return optTiles[y * mSizeX + x][z];
			}
			else
			    return 0;
		}
		
        if (x >= 0 && x < mSizeX && y >= 0 && y < mSizeY && z >= 0 && z < mSizeZ) {
            return mTiles[coordsToIdx(x, y, z)];
        } else {
            return 0;
        }
    }

    public void setSkyBox(int which, int tileId) {
        if (which < NUM_SKYBOX_PARTS) {
            mSkyBox[which] = tileId;
        }
    }

    public void loadPXE(String fname) {
        int sizex = 0;
        int sizey = 0;
        int sizez = 0;

        File queryImg = new File(fname);
        int imageLen = (int) queryImg.length();

        FileInputStream fis = null;

        try {
            fis = new FileInputStream(queryImg);
            if (fis != null) {

                sizex = fis.read();
                sizey = fis.read();
                sizez = fis.read();

                byte[] byteTiles = new byte[MAX_TILES];
                fis.read(byteTiles);
                for (int i = 0; i < MAX_TILES; i++) {
                    mTiles[i] = byteTiles[i];
                }

//                    fis.read(mTiles);
//                Roamer.showMsg("File " + fname + " read");
            }
        } catch (Exception e) {
	//		Lozoware.exception(e);
  //          Lozoware.log(Lozoware.logtype.nonfatalerror, e.getMessage());
        }
    }

    public void loadPXEAsset(String fname) {
    }

    public void fillIsland(int islandSize) {
        // Put some test data into the tilemap
        for (int z = 0; z < 1; z++) {
            for (int y = (mSizeY / 2) - (islandSize / 2); y < (mSizeY / 2) + (islandSize / 2); y++) {
                for (int x = (mSizeX / 2) - (islandSize / 2); x < (mSizeX / 2) + (islandSize / 2); x++) {
                    set(x, y, z, 5);
                }
            }
        }

//		recalcMergedSurfs();
    }

    public void setOffsetX(int x, int y, int z, float value) {
		
		if (optimize && x >= 0 && y >= 0 && z >= 0 && x < mSizeX && y < mSizeY && z < mSizeZ)
		{
			if (getHeight(x, y) <= z)
			{
			    setHeight(x, y, z + 1);
			}

			if (value < BYTEFLOAT_MIN) {
                value = BYTEFLOAT_MIN;
            }
            if (value > BYTEFLOAT_MAX) {
                value = BYTEFLOAT_MAX;
            }

//			Lozoware.getUI().consoleInUIThread("x=" + x + " y=" + y + " z=" + z + " stack.length=" + optOffsetsX[y * (mSizeX + 1) + x].length);			
//			Lozoware.getUI().consoleInUIThread("index=" + (y * (mSizeX + 1) + x) + " in size=" + optOffsetsX.length + " @xyz " + x + " " + y + " " + z);
//			Lozoware.getUI().consoleInUIThread("z=" + z + " stacksize=" + optOffsetsX[y * (mSizeX + 1) + x].length);
			
			if ((y * (mSizeX + 1) + x) < optOffsetsX.length && z < optOffsetsX[y * (mSizeX + 1) + x].length)
			{
//				Lozoware.getUI().consoleInUIThread("setting offset x");
   	            optOffsetsX[y * (mSizeX + 1) + x][z] = (byte) (value * BYTEFLOAT_PRECISION);
//   	            optOffsetsX[y * (mSizeX + 1) + x][z] = (byte) (100 * BYTEFLOAT_PRECISION);
			}
		}
		
		if (!optimize && x >= 0 && y >= 0 && z >= 0 && x <= mSizeX && y <= mSizeY && z <= mSizeZ) {
            int idx = offsetCoordsToIdx(x, y, z);

            if (value < BYTEFLOAT_MIN) {
                value = BYTEFLOAT_MIN;
            }
            if (value > BYTEFLOAT_MAX) {
                value = BYTEFLOAT_MAX;
            }

            mOffsetsX[idx] = (byte) (value * BYTEFLOAT_PRECISION);
        }
		
    }

    public float getOffsetX(int x, int y, int z) {
		
		if (optimize)
    	{
			if (getHeight(x, y) > z && z >= 0 && x >= 0 && y >= 0 && z >= 0
				&& x < mSizeX && y < mSizeY && z < mSizeZ
				&& (y * (mSizeX + 1) + x) < optOffsetsX.length
				&& z < optOffsetsX[y * (mSizeX + 1) + x].length)
			{
//				Lozoware.getUI().consoleInUIThread("returning x offset");
//				if (x == 16 && y == 18 && z == 2)
//					Lozoware.getUI().consoleInUIThread("returning x offset " + (float)optOffsetsX[y * mSizeX + x][z] / BYTEFLOAT_PRECISION);
				
				return (float)optOffsetsX[y * (mSizeX + 1) + x][z] / BYTEFLOAT_PRECISION;
			}
			else
			    return 0;
		}
		
		if (!optimize)
		{
            if (x >= 0 && x <= mSizeX && y >= 0 && y <= mSizeY && z >= 0 && z <= mSizeZ) {
                return (float) (mOffsetsX[offsetCoordsToIdx(x, y, z)]) / BYTEFLOAT_PRECISION;
            } else {
                return 0;
            }
		}
		
		return 0;
    }

    public void setOffsetY(int x, int y, int z, float value) {
		
		if (optimize && x >= 0 && y >= 0 && z >= 0 && x < mSizeX && y < mSizeY && z < mSizeZ)
		{
			if (getHeight(x, y) <= z)
			{
			    setHeight(x, y, z + 1);
			}

			if (value < BYTEFLOAT_MIN) {
                value = BYTEFLOAT_MIN;
            }
            if (value > BYTEFLOAT_MAX) {
                value = BYTEFLOAT_MAX;
            }

			if ((y * (mSizeX + 1) + x) < optOffsetsY.length && z < optOffsetsY[y * (mSizeX + 1) + x].length)
   	            optOffsetsY[y * (mSizeX + 1) + x][z] = (byte) (value * BYTEFLOAT_PRECISION);
		}
		
		if (!optimize && x >= 0 && y >= 0 && z >= 0 && x <= mSizeX && y <= mSizeY && z <= mSizeZ) {
            int idx = offsetCoordsToIdx(x, y, z);

            if (value < BYTEFLOAT_MIN) {
                value = BYTEFLOAT_MIN;
            }
            if (value > BYTEFLOAT_MAX) {
                value = BYTEFLOAT_MAX;
            }

            mOffsetsY[idx] = (byte) (value * BYTEFLOAT_PRECISION);
        }
		
    }

    public float getOffsetY(int x, int y, int z) {
		
		if (optimize)
    	{
			if (getHeight(x, y) > z && z >= 0 && x >= 0 && y >= 0 && z >= 0
			&& x < mSizeX && y < mSizeY && z < mSizeZ
			&& (y * (mSizeX + 1) + x) < optOffsetsY.length
			&& z < optOffsetsY[y * (mSizeX + 1) + x].length)
			{
				return (float)optOffsetsY[y * (mSizeX + 1) + x][z] / BYTEFLOAT_PRECISION;
			}
			else
			    return 0;
		}
		
		if (!optimize)
		{
        if (x >= 0 && x <= mSizeX && y >= 0 && y <= mSizeY && z >= 0 && z <= mSizeZ) {
            return (float) (mOffsetsY[offsetCoordsToIdx(x, y, z)]) / BYTEFLOAT_PRECISION;
        } else {
            return 0;
        }
		}
		
		return 0;
    }

    public void setOffsetZ(int x, int y, int z, float value) {
		
		if (optimize && x >= 0 && y >= 0 && z >= 0 && x < mSizeX && y < mSizeY && z < mSizeZ)
		{
			if (getHeight(x, y) <= z)
			{
			    setHeight(x, y, z + 1);
			}

			if (value < BYTEFLOAT_MIN) {
                value = BYTEFLOAT_MIN;
            }
            if (value > BYTEFLOAT_MAX) {
                value = BYTEFLOAT_MAX;
            }

			if ((y * (mSizeX + 1) + x) < optOffsetsZ.length && z < optOffsetsZ[y * (mSizeX + 1) + x].length)
   	            optOffsetsZ[y * (mSizeX + 1) + x][z] = (byte) (value * BYTEFLOAT_PRECISION);
		}
		
		if (!optimize && x >= 0 && y >= 0 && z >= 0 && x <= mSizeX && y <= mSizeY && z <= mSizeZ) {
            int idx = offsetCoordsToIdx(x, y, z);

            if (value < BYTEFLOAT_MIN) {
                value = BYTEFLOAT_MIN;
            }
            if (value > BYTEFLOAT_MAX) {
                value = BYTEFLOAT_MAX;
            }

            mOffsetsZ[idx] = (byte) (value * BYTEFLOAT_PRECISION);
        }
		
    }

    public float getOffsetZ(int x, int y, int z) {
		
		if (optimize)
    	{
			if (getHeight(x, y) > z && z >= 0 && x >= 0 && y >= 0 && z >= 0
				&& x < mSizeX && y < mSizeY && z < mSizeZ
				&& (y * (mSizeX + 1) + x) < optOffsetsZ.length
				&& z < optOffsetsZ[y * (mSizeX + 1) + x].length)
			{
				return (float)optOffsetsZ[y * (mSizeX + 1) + x][z] / BYTEFLOAT_PRECISION;
			}
			else
			    return 0;
		}
		
		if (!optimize)
		{
        if (x >= 0 && x <= mSizeX && y >= 0 && y <= mSizeY && z >= 0 && z <= mSizeZ) {
            return (float) (mOffsetsZ[offsetCoordsToIdx(x, y, z)]) / BYTEFLOAT_PRECISION;
        } else {
            return 0;
        }
		}
		
		return 0;
    }

    public int offsetCoordsToIdx(int x, int y, int z) {
        return ((mSizeX + 1) * (mSizeY + 1) * z) + (y * (mSizeX + 1)) + x;
    }
	
	public int getHeight(int x, int y)
	{
		if (x < 0 || y < 0 || x >= mSizeX || y >= mSizeY)
			return 0;

		return heightMap[y * mSizeX + x];
	}
	
	
	public void setHeight(int x, int y, int height)
	{
		setHeight(x, y, height, true);
	}
	
	public void setHeight(int x, int y, int height, boolean adjacentToo)
	{
		if (x < 0 || y < 0 || height < 0 || x >= mSizeX || y >= mSizeY || height >= mSizeZ)
		    return;

		// Tile stack

		byte[] newStack = new byte[height];

		for (int i = 0; i < height; i++)
		{
			if (i < getHeight(x, y) && i < newStack.length && y * mSizeX + x < optTiles.length && i < optTiles[y * mSizeX + x].length)
			{
			    newStack[i] = optTiles[y * mSizeX + x][i];
		    }
			else if (i < newStack.length)
				newStack[i] = 0;
		}

		optTiles[y * mSizeX + x] = newStack;

		// Offset X stack

		byte[] newOffsetsXStack = new byte[height + OFFSET_HEIGHT_PAD];

		for (int i = 0; i <= height; i++)
		{
			if (i < getHeight(x, y) && i < optOffsetsX[y * (mSizeX + 1) + x].length)
			{
//				Lozoware.getUI().consoleInUIThread("getheight = " + optOffsetsX[y * (mSizeX + 1) + x].length);
			    newOffsetsXStack[i] = optOffsetsX[y * (mSizeX + 1) + x][i];
		    }
			else if (i < newOffsetsXStack.length)
				newOffsetsXStack[i] = 0;
		}

		optOffsetsX[y * (mSizeX + 1) + x] = newOffsetsXStack;

		// Offset Y stack

		byte[] newOffsetsYStack = new byte[height + OFFSET_HEIGHT_PAD];

		for (int i = 0; i <= height; i++)
		{
			if (i < getHeight(x, y) && i < optOffsetsY[y * (mSizeX + 1) + x].length)
			{
			    newOffsetsYStack[i] = optOffsetsY[y * (mSizeX + 1) + x][i];
		    }
			else if (i < newOffsetsYStack.length)
				newOffsetsYStack[i] = 0;
		}

		optOffsetsY[y * (mSizeX + 1) + x] = newOffsetsYStack;

		// Offset Z stack

		byte[] newOffsetsZStack = new byte[height + OFFSET_HEIGHT_PAD];

		for (int i = 0; i <= height; i++)
		{
			if (i < getHeight(x, y) && i < optOffsetsZ[y * (mSizeX + 1) + x].length)
			{
			    newOffsetsZStack[i] = optOffsetsZ[y * (mSizeX + 1) + x][i];
		    }
			else if (i < newOffsetsZStack.length)
				newOffsetsZStack[i] = 0;
		}

		optOffsetsZ[y * (mSizeX + 1) + x] = newOffsetsZStack;

		// Height map

		heightMap[y * mSizeX + x] = (byte)height;
		
		// Set adjacent
		
/*		if (adjacentToo)
		{
			setHeight(x - 1, y + 1, height, false);
			setHeight(x, y + 1, height, false);
			setHeight(x + 1, y + 1, height, false);
			setHeight(x - 1, y, height, false);
			setHeight(x + 1, y, height, false);
			setHeight(x - 1, y - 1, height, false);
			setHeight(x, y - 1, height, false);
			setHeight(x + 1, y - 1, height, false);
		}*/
	}

    public byte getMaxHeight()
	{
		byte height = 0;

		for (int i = 0; i < mSizeX * mSizeY; i++)
		{
			if (heightMap[i] > height)
				height = heightMap[i];
		}

		return height;
	}

    public boolean loadTLM(String filename) {
        return loadTLM(filename, false);
    }
    
    public boolean loadTLM(String filename, boolean fromAsset) {
       try {
        	
        	//
        	// TODO: Set fixGap to false when loading TLM's 1.0.3 or lower
        	//
			
//			if (filename.equals(ui.getStateTLMFilename()))
//				return false;
			
/*			try 
			{
				throw new Exception();
			}
			catch (Exception e)
			{
				Lozoware.exception(e);
			}*/
			

            int sizex = 0;
            int sizey = 0;
            int sizez = 0;

            if (filename.equals("")) {
                return false;
            }

            File queryImg = new File(filename);
            int imageLen = (int) queryImg.length();

            InputStream is = null;

            if (!fromAsset) {
				if (!queryImg.exists())
					return false;
					
                FileInputStream fis = new FileInputStream(queryImg);
                is = (InputStream) fis;
            } else {
//                is = DDLUtils.getAssetInputStream(filename);
            }

            if (is == null) {
                return false;
            }

            sizex = is.read();
            sizey = is.read();
            sizez = is.read();

            if (sizex == 84 && sizey == 76 && sizez == 77) {

                // READ 1.0.3 FORMAT
                int vera = is.read();
                int verb = is.read();
                int verc = is.read();
				
                if (vera == 1 && verb == 0 && verc == 3) {

                    // READ TILES
                    sizex = is.read();
                    sizey = is.read();
                    sizez = is.read();
                    
                    // Bytes can only have values up to 255, but it's super-convenent for us to have
                    // tilemaps up to 256 in size, and tilemaps never have size zero, so let's 
                    // assume that 0 means 256
                    if (sizex == 0)
                    	sizex = 256;
                    if (sizey == 0)
                    	sizey = 256;
                    if (sizez == 0)
                    	sizez = 256;

					mSizeX = sizex;
					mSizeY = sizey;
					mSizeZ = sizez;
					
					if (mSizeX == 0 || mSizeY == 0 || mSizeZ == 0)
						return false;

                    byte[] bt = null;
                    ByteBuffer bb = null;
                    ByteBuffer ib = null;

					tileSize = is.read();
					
					if (tileSize == 0)
						tileSize = 256;
					
					textureSpan = is.read();

					if (textureSpan == 0)
						textureSpan = 256;
					
					init(sizex, sizey, sizez, tileSize);

//					Lozoware.print("read size=(" + sizex + ", " + sizey + ", " + sizez + ") tilesize=" + tileSize);

					is.read(heightMap);

					int idx = 0;

					for (int y = 0; y < sizey; y++)
					{
						for (int x = 0; x < sizex; x++)
						{
						    setHeight(x, y, heightMap[idx]);
							idx++;
						}
					}

					for (int i = 0; i < optTiles.length; i++)
					{						
						is.read(optTiles[i]);

					}


					for (int i = 0; i < optOffsetsX.length; i++)
					{
						if (optOffsetsX[i].length > OFFSET_HEIGHT_PAD)
						{
						    is.read(optOffsetsX[i]);

						}

					}


					for (int i = 0; i < optOffsetsY.length; i++)
					{
						if (optOffsetsY[i].length > OFFSET_HEIGHT_PAD)
					        is.read(optOffsetsY[i]);
					}


					for (int i = 0; i < optOffsetsZ.length; i++)
					{
						if (optOffsetsZ[i].length > OFFSET_HEIGHT_PAD)
						    is.read(optOffsetsZ[i]);

					}


                    // READ SKYBOX
                    int siga = is.read();
                    int sigb = is.read();
                    int sigc = is.read();
                    int sigd = is.read();
                    int sige = is.read();
                    int sigf = is.read();

                    // RECOGNIZE "SKYBOX"
                    if (siga == 83 && sigb == 75 && sigc == 89
						&& sigd == 66 && sige == 79 && sigf == 88) {

                        byte[] bytes;

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[0] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[1] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[2] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[3] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[4] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[5] = ByteBuffer.wrap(bytes).getInt();

                    }

                    // READ ENTITIES
                    siga = is.read();
                    sigb = is.read();
                    sigc = is.read();
                    sigd = is.read();
                    sige = is.read();
                    sigf = is.read();

                    // RECOGNIZE "ENTBLK"
                    if (siga == 69 && sigb == 78 && sigc == 84
						&& sigd == 66 && sige == 76 && sigf == 75) {

                        // Read size X integer form 4 bytes - number of locs
                        byte[] numEntitiesBytes = new byte[4];
                        is.read(numEntitiesBytes);
                        int num = ByteBuffer.wrap(numEntitiesBytes).getInt();
						
//						Lozoware.print("Reading " + num + " entities");

                        // Allocate array for entities
/*                        entities = new Entity[MAX_ENTITIES];

                        // Read the entities
                        for (int i = 0; i < num; i++) {

                            Entity e = new Entity();

                            e.load(is);
							
							addEntity(e);

//                            entities[i] = e;

                        }*/
                    }
                }
                else if (vera == 1 && verb == 0 && verc == 2) {

                    // READ TILES
                    sizex = is.read();
                    sizey = is.read();
                    sizez = is.read();
					
					mSizeX = sizex;
					mSizeY = sizey;
					mSizeZ = sizez;

                    byte[] bt = null;
                    ByteBuffer bb = null;
                    ByteBuffer ib = null;

					tileSize = is.read();
					textureSpan = 1;
					
					init(sizex, sizey, sizez, tileSize);
					
	//				Lozoware.print("read size=(" + sizex + ", " + sizey + ", " + sizez + ") tilesize=" + tileSize);

					is.read(heightMap);
									
					int idx = 0;
					
					for (int y = 0; y < sizey; y++)
					{
						for (int x = 0; x < sizex; x++)
						{
						    setHeight(x, y, heightMap[idx]);
							idx++;
						}
					}

					for (int i = 0; i < optTiles.length; i++)
					{						
						is.read(optTiles[i]);
						
					}
					
					
					for (int i = 0; i < optOffsetsX.length; i++)
					{
						if (optOffsetsX[i].length > OFFSET_HEIGHT_PAD)
						{
						    is.read(optOffsetsX[i]);
							
						}
						
					}
					

					for (int i = 0; i < optOffsetsY.length; i++)
					{
						if (optOffsetsY[i].length > OFFSET_HEIGHT_PAD)
					        is.read(optOffsetsY[i]);
					}

					
					for (int i = 0; i < optOffsetsZ.length; i++)
					{
						if (optOffsetsZ[i].length > OFFSET_HEIGHT_PAD)
						    is.read(optOffsetsZ[i]);
						
					}
					

                    // READ SKYBOX
                    int siga = is.read();
                    int sigb = is.read();
                    int sigc = is.read();
                    int sigd = is.read();
                    int sige = is.read();
                    int sigf = is.read();

                    // RECOGNIZE "SKYBOX"
                    if (siga == 83 && sigb == 75 && sigc == 89
						&& sigd == 66 && sige == 79 && sigf == 88) {

                        byte[] bytes;

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[0] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[1] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[2] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[3] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[4] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[5] = ByteBuffer.wrap(bytes).getInt();
						
                    }
					
                    // READ ENTITIES
                    siga = is.read();
                    sigb = is.read();
                    sigc = is.read();
                    sigd = is.read();
                    sige = is.read();
                    sigf = is.read();

                    // RECOGNIZE "ENTBLK"
                    if (siga == 69 && sigb == 78 && sigc == 84
						&& sigd == 66 && sige == 76 && sigf == 75) {

                        // Read size X integer form 4 bytes - number of locs
                        byte[] numEntitiesBytes = new byte[4];
                        is.read(numEntitiesBytes);
                        numEntities = ByteBuffer.wrap(numEntitiesBytes).getInt();

                        // Allocate array for entities
/*                        entities = new Entity[MAX_ENTITIES];

                        // Read the entities
                        for (int i = 0; i < numEntities; i++) {

                            Entity e = new Entity();

                            e.load(is);

                            entities[i] = e;

                        }*/
                    }
                }
                else if (vera == 1 && verb == 0 && verc == 1) {

                    // READ TILES
                    sizex = is.read();
                    sizey = is.read();
                    sizez = is.read();
					
					tileSize = 64;
					textureSpan = 1;

                    init(sizex, sizey, sizez);

                    byte[] bt = null;
                    ByteBuffer bb = null;
                    ByteBuffer ib = null;

                    mTiles = new byte[(sizex) * (sizey) * (sizez)];
                    is.read(mTiles);

                    mOffsetsX = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1)];
                    is.read(mOffsetsX);

                    mOffsetsY = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1)];
                    is.read(mOffsetsY);
					
                    mOffsetsZ = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1)];
                    is.read(mOffsetsZ);
									
					if (optimize)
					{
					    // Convert old unoptimized mTiles and mOffsets arrays
					    // to new jagged array format
					    for (int z = 0; z < sizez; z++)
					    {
						    for (int y = 0; y < sizez; y++)
						    {
							    for (int x = 0; x < sizez; x++)
							    {
                                    set(x, y, z, mTiles[coordsToIdx(x, y, z)]);
							    }
						    }
					    }
					
					    mTiles = null;
					
						for (int z = 0; z < sizez + 1; z++)
						{
							for (int y = 0; y < sizey + 1; y++)
							{
								for (int x = 0; x < sizex + 1; x++)
								{
									setOffsetX(x, y, z, (float)mOffsetsX[offsetCoordsToIdx(x, y, z)] / (float)BYTEFLOAT_PRECISION);
									setOffsetY(x, y, z, (float)mOffsetsY[offsetCoordsToIdx(x, y, z)] / (float)BYTEFLOAT_PRECISION);
									setOffsetZ(x, y, z, (float)mOffsetsZ[offsetCoordsToIdx(x, y, z)] / (float)BYTEFLOAT_PRECISION);
								}
							}
						}
						
						mOffsetsX = null;
						mOffsetsY = null;
						mOffsetsZ = null;
					}

                    // READ SKYBOX
                    int siga = is.read();
                    int sigb = is.read();
                    int sigc = is.read();
                    int sigd = is.read();
                    int sige = is.read();
                    int sigf = is.read();

                    // RECOGNIZE "SKYBOX"
                    if (siga == 83 && sigb == 75 && sigc == 89
                            && sigd == 66 && sige == 79 && sigf == 88) {

                        byte[] bytes;

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[0] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[1] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[2] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[3] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[4] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[5] = ByteBuffer.wrap(bytes).getInt();
                    }

                    // READ ENTITIES
                    siga = is.read();
                    sigb = is.read();
                    sigc = is.read();
                    sigd = is.read();
                    sige = is.read();
                    sigf = is.read();

                    // RECOGNIZE "ENTBLK"
                    if (siga == 69 && sigb == 78 && sigc == 84
                            && sigd == 66 && sige == 76 && sigf == 75) {
							
                        // Read size X integer form 4 bytes - number of locs
                        byte[] numEntitiesBytes = new byte[4];
                        is.read(numEntitiesBytes);
                        numEntities = ByteBuffer.wrap(numEntitiesBytes).getInt();

                        // Allocate array for entities
/*                        entities = new Entity[MAX_ENTITIES];

                        // Read the entities
                        for (int i = 0; i < numEntities; i++) {
								
							
                            Entity e = new Entity();

                            e.load(is);

                            entities[i] = e;
								
                        }*/
                    }

                    // READ TEXROT

                    /*                    siga = is.read();
                     sigb = is.read();
                     sigc = is.read();
                     sigd = is.read();
                     sige = is.read();
                     sigf = is.read();

                     if (siga == 84 && sigb == 69 && sigc == 88
                     && sigd == 82 && sige == 79 && sigf == 84) {
                     is.read(mTexRotations);
                     }*/
                } else if (vera == 1 && verb == 0 && verc == 0) {
					
					
                    // READ TILES
                    sizex = is.read();
                    sizey = is.read();
                    sizez = is.read();

                    init(sizex, sizey, sizez);

                    byte[] bt = null;
                    ByteBuffer bb = null;
                    ByteBuffer ib = null;

                    bt = new byte[(sizex) * (sizey) * (sizez) * 4];
                    is.read(bt);
                    bb = ByteBuffer.wrap(bt);
                    bb.rewind();
//                    bb.asIntBuffer().get(mTiles);

                    bt = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1) * 4];
                    is.read(bt);
                    bb = ByteBuffer.wrap(bt);
                    bb.rewind();
//                    bb.asFloatBuffer().get(mOffsetsX);

                    bt = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1) * 4];
                    is.read(bt);
                    bb = ByteBuffer.wrap(bt);
                    bb.rewind();
//                    bb.asFloatBuffer().get(mOffsetsY);

                    bt = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1) * 4];
                    is.read(bt);
                    bb = ByteBuffer.wrap(bt);
                    bb.rewind();
//                    bb.asFloatBuffer().get(mOffsetsZ);

                    // READ SKYBOX
                    int siga = is.read();
                    int sigb = is.read();
                    int sigc = is.read();
                    int sigd = is.read();
                    int sige = is.read();
                    int sigf = is.read();

                    // RECOGNIZE "SKYBOX"
                    if (siga == 83 && sigb == 75 && sigc == 89
                            && sigd == 66 && sige == 79 && sigf == 88) {

                        byte[] bytes;

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[0] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[1] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[2] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[3] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[4] = ByteBuffer.wrap(bytes).getInt();

                        bytes = new byte[4];
                        is.read(bytes);
                        mSkyBox[5] = ByteBuffer.wrap(bytes).getInt();
                    }

                    // READ LOCS
                    siga = is.read();
                    sigb = is.read();
                    sigc = is.read();
                    sigd = is.read();
                    sige = is.read();
                    sigf = is.read();
					
                    // RECOGNIZE "ENTBLK"
                    if (siga == 69 && sigb == 78 && sigc == 84
                            && sigd == 66 && sige == 76 && sigf == 75) {

                        // Read size X integer form 4 bytes - number of locs
                        byte[] numEntitiesBytes = new byte[4];
                        is.read(numEntitiesBytes);
                        numEntities = ByteBuffer.wrap(numEntitiesBytes).getInt();

/*                        Lozoware.getUI().consoleInUIThread("numEntities = " + numEntities);

                        // Read the locs
                        for (int i = 0; i < numEntities; i++) {
                            Loc3D t = new Loc3D();

                            t.load(is);

                            mLocs[i] = t;
                            //					Lozoware.showMsg("read loc x = " + t.mX);
                        }*/
                    }

                    // READ TEXROT

                    /*                    siga = is.read();
                     sigb = is.read();
                     sigc = is.read();
                     sigd = is.read();
                     sige = is.read();
                     sigf = is.read();

                     if (siga == 84 && sigb == 69 && sigc == 88
                     && sigd == 82 && sige == 79 && sigf == 84) {
                     is.read(mTexRotations);
                     }*/
                }
            } else {

                // READ OLD FORMAT
                init(sizex, sizey, sizez);

                byte[] byteTiles = new byte[MAX_TILES];
                is.read(byteTiles);

                for (int i = 0; i < MAX_TILES; i++) {
                    mTiles[i] = byteTiles[i];
                }

                byte[] bt = null;
                ByteBuffer bb = null;

                bt = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1) * 4];
                is.read(bt);
                bb = ByteBuffer.wrap(bt);
                bb.rewind();
//                bb.asFloatBuffer().get(mOffsetsX);

                bt = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1) * 4];
                is.read(bt);
                bb = ByteBuffer.wrap(bt);
                bb.rewind();
//                bb.asFloatBuffer().get(mOffsetsY);

                bt = new byte[(sizex + 1) * (sizey + 1) * (sizez + 1) * 4];
                is.read(bt);
                bb = ByteBuffer.wrap(bt);
                bb.rewind();
//                bb.asFloatBuffer().get(mOffsetsZ);

            }
        } catch (Exception e) {
			
  //          Lozoware.exception(e);	
//			return false;
//            Lozoware.showMsg("tilemap3d 586 " + e.toString());
        }

		return true;
    }

    public void saveTLM(String filename) {
		
        try {
			
		//	Lozoware.print("saveTLM() " + filename);
			
			if (!optimize)
				return;
			
            FileOutputStream f = new FileOutputStream(new File(filename));

            f.write((int) 'T');
            f.write((int) 'L');
            f.write((int) 'M');
            f.write(1);
            f.write(0);
            f.write(3);

            // SAVE TILES
            f.write(mSizeX);
            f.write(mSizeY);
            f.write(mSizeZ);
	        f.write(tileSize);
			f.write(textureSpan);
			
			int a = 0;
			int b = 0;
			
		//	Lozoware.print("wrote size=(" + mSizeX + ", " + mSizeY + ", " + mSizeZ + ") tilesize=" + tileSize);
			
			f.write(heightMap);
			
//			Lozoware.print("wrote heightMap " + heightMap.length);
			
			for (int i = 0; i < optTiles.length; i++)
			{
				f.write(optTiles[i]);
				
				if (optTiles[i].length == 0)
					a++;
				else
					b++;
			}
			
//			Lozoware.print("wrote optTiles " + optTiles.length + " zerolen " + a + " nonzerolen  " + b);
			a = 0;
			b = 0;
			int count = 0;
			
			for (int i = 0; i < optOffsetsX.length; i++)
			{
				if (optOffsetsX[i].length > OFFSET_HEIGHT_PAD)
				{
				    f.write(optOffsetsX[i]);
					
					if (count < 10)
					{
//						Lozoware.print("wrote optOffsetsX value " + optOffsetsX[i][0]);
						count++;
					}
				}
				
				if (optOffsetsX[i].length == 0)
					a++;
				else
					b++;
			}
			
//			Lozoware.print("wrote optOffsetsX " + optOffsetsX.length + " zerolen " + a + " nonzerolen  " + b);

 			for (int i = 0; i < optOffsetsY.length; i++)
			{
				if (optOffsetsY[i].length > OFFSET_HEIGHT_PAD)
				    f.write(optOffsetsY[i]);
			}
			
//			Lozoware.print("wrote optOffsetsY " + optOffsetsY.length);
			
			for (int i = 0; i < optOffsetsZ.length; i++)
			{
				if (optOffsetsZ[i].length > OFFSET_HEIGHT_PAD)
				    f.write(optOffsetsZ[i]);
			}
			
//			Lozoware.print("wrote optOffsetsZ " + optOffsetsZ.length);
			
            // WRITE SKYBOX
            f.write((int) 'S');
            f.write((int) 'K');
            f.write((int) 'Y');
            f.write((int) 'B');
            f.write((int) 'O');
            f.write((int) 'X');

            // Write skybox
            byte[] bytes = null;

            bytes = ByteBuffer.allocate(4).putInt(mSkyBox[0]).array();
            f.write(bytes[0]);
            f.write(bytes[1]);
            f.write(bytes[2]);
            f.write(bytes[3]);

            bytes = ByteBuffer.allocate(4).putInt(mSkyBox[1]).array();
            f.write(bytes[0]);
            f.write(bytes[1]);
            f.write(bytes[2]);
            f.write(bytes[3]);

            bytes = ByteBuffer.allocate(4).putInt(mSkyBox[2]).array();
            f.write(bytes[0]);
            f.write(bytes[1]);
            f.write(bytes[2]);
            f.write(bytes[3]);

            bytes = ByteBuffer.allocate(4).putInt(mSkyBox[3]).array();
            f.write(bytes[0]);
            f.write(bytes[1]);
            f.write(bytes[2]);
            f.write(bytes[3]);

            bytes = ByteBuffer.allocate(4).putInt(mSkyBox[4]).array();
            f.write(bytes[0]);
            f.write(bytes[1]);
            f.write(bytes[2]);
            f.write(bytes[3]);

            bytes = ByteBuffer.allocate(4).putInt(mSkyBox[5]).array();
            f.write(bytes[0]);
            f.write(bytes[1]);
            f.write(bytes[2]);
            f.write(bytes[3]);
			
//			Lozoware.print("wrote skybox " + mSkyBox[0] + " " + mSkyBox[1] + " " + mSkyBox[2] + " " + mSkyBox[3]);
		
            // WRITE ENTITIES
            f.write((int) 'E');
            f.write((int) 'N');
            f.write((int) 'T');
            f.write((int) 'B');
            f.write((int) 'L');
            f.write((int) 'K');

            // Write number of entities
            byte[] numEntitiesBytes = ByteBuffer.allocate(4).putInt(numEntities).array();
            f.write(numEntitiesBytes[0]);
            f.write(numEntitiesBytes[1]);
            f.write(numEntitiesBytes[2]);
            f.write(numEntitiesBytes[3]);

            // Write entities
/*            for (int i = 0; i < numEntities; i++) {
                Entity ent = entities[i];

                if (ent != null) {
                    ent.save(f);
                }
            }*/

            // WRITE END
            f.write((int) 'E');
            f.write((int) 'N');
            f.write((int) 'D');
            f.write((int) 'T');
            f.write((int) 'L');
            f.write((int) 'M');

            // CLOSE
            f.flush();
            f.close();

        } catch (Exception e) {
	//		Lozoware.exception(e);
     //       Lozoware.showMsg(e.toString());
        }

    }

    public void recalcMergedSurfs() {
     /*   mergedBits = new BitSet(mSizeX * mSizeY * mSizeZ * 6);
        mergedSurfs = new ArrayList<MergedSurface>();

        MergedSurface newSurf = null;

        for (int z = 0; z < mSizeZ; z++) {
            for (int y = 0; y < mSizeY; y++) {
                for (int x = 0; x < mSizeX; x++) {

                    newSurf = new MergedSurface();

                    //
                    // Look for surfaces in Z plane
                    //
                    newSurf.startX = x;
                    newSurf.startY = y;
                    newSurf.startZ = z;
                    newSurf.endX = x;
                    newSurf.endY = y;
                    newSurf.endZ = z;
                    byte tile = get(x, y, z);

                    if (tile != 0) {

                        // Look for surfaces looking into positive Z
                        // Expand like a square
                        boolean done = false;
                        int size = 2;

                        while (!done) {
                            for (int xx = newSurf.startX; xx < newSurf.startX + size; xx++) {
                                for (int yy = newSurf.startY; yy < newSurf.startY + size; yy++) {
                                    if (get(xx, yy, z) != tile
                                            || get(xx, yy, z + 1) != 0
                                            || getOffsetX(xx, yy, z) != 0
                                            || getOffsetY(xx, yy, z) != 0
                                            || getOffsetZ(xx, yy, z) != 0
                                            || getMergedBit(xx, yy, z, 5)
                                            || size > mSizeX || size > mSizeY) {
//									Lozoware.getUI().console("size " + size);
                                        done = true;
                                    }
                                }
                            }

                            if (!done) {
                                size++;
                            } else {
                                size--;
                            }
                        }

//				    Lozoware.getUI().console("size " + size);
                        // Expand along x	 
                        // Expand along y
                        if (size > 1) {
                            newSurf.endX += size;
                            newSurf.endY += size;

                            // This surf is on top of the cubes in question
                            newSurf.startZ += 1;
                            newSurf.endZ += 1;

                            mergedSurfs.add(newSurf);

                            // Mark the bits
                            for (int xx = newSurf.startX; xx < newSurf.endX; xx++) {
                                for (int yy = newSurf.startY; yy < newSurf.endY; yy++) {
                                    setMergedBit(xx, yy, z, 5, true);
                                }
                            }

//						Lozoware.getUI().console("mergedSurf [" + newSurf.startX + ", " + newSurf.startY + ", " + newSurf.startZ + "] [" + newSurf.endX + ", " + newSurf.endY + ", " + newSurf.endZ + "]");
                        }

                        // Look for surfaces looking into negative Z
                        // Expand like a square
                        // Expand along x
                        // Expand along y
                        //
                        // Look for surfaces in Y plane
                        //
                        // Look for surfaces looking into positive Y
                        // Expand like a square
                        // Expand along x
                        // Expand along y
                        // Look for surfaces looking into negative Y
                        // Expand like a square
                        // Expand along x
                        // Expand along y
                        //
                        // Look for surfaces in X plane
                        //
                        // Look for surfaces looking into positive X
                        // Expand like a square
                        // Expand along x
                        // Expand along y
                        // Look for surfaces looking into negative X
                        // Expand like a square
                        // Expand along x
                        // Expand along y	
                    }
                }
            }
        }*/
    }

    public void setMergedBit(int x, int y, int z, int side, boolean bit) {
        if (mergedBits == null) {
            return;
        }

        int p = (z * mSizeX * mSizeY + (y * mSizeX) + x) * 6 + side;

        if (bit) {
            mergedBits.set(p);
        } else {
            mergedBits.clear(p);
        }
    }

    public boolean getMergedBit(int x, int y, int z, int side) {
        int p = (z * mSizeX * mSizeY + (y * mSizeX) + x) * 6 + side;

        if (mergedBits == null) {
            return false;
        } else {
            return mergedBits.get(p);
        }
    }

   	
	@Override
	public String toString()
	{
		return "sizex: " + mSizeX + " sizey: " + mSizeY + " sizez: " + mSizeZ + " tileSize: " + tileSize + " numEntities: " + numEntities;
	}
	
	public void autoCrop()
	{
		if (!autoCropped)
		{
		    // TODO
			autoCropped = true;
		}
	}
	
	public void setR(int x, int y, int z, int r)
	{
		setOffsetX(x, y, z, (float)r / (float)BYTEFLOAT_PRECISION);
	}
	
	public void setG(int x, int y, int z, int g)
	{
		setOffsetY(x, y, z, (float)g / (float)BYTEFLOAT_PRECISION);
	}
	
	public void setB(int x, int y, int z, int b)
	{
		setOffsetZ(x, y, z, (float)b / (float)BYTEFLOAT_PRECISION);
	}
	
	public void setA(int x, int y, int z, int a)
	{
		set(x, y, z, a);
	}
	
	public int getR(int x, int y, int z)
	{
		return (int)BYTEFLOAT_PRECISION * (int)getOffsetX(x, y, z);
	}
	
	public int getG(int x, int y, int z)
	{
		return (int)BYTEFLOAT_PRECISION * (int)getOffsetY(x, y, z);
	}
	
	public int getB(int x, int y, int z)
	{
		return (int)BYTEFLOAT_PRECISION * (int)getOffsetZ(x, y, z);
	}
	
	public int getA(int x, int y, int z)
	{
		return (int)get(x, y, z);
	}
}
