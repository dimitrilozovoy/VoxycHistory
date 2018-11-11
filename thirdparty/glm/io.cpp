/*
 * io.c
 *
 *  Created on: Aug 16, 2015
 *      Author: dimitri
 */

#include "platform.h"
#include "tilemap.h"
#include "io.h"

io::io()
{
}

io::~io()
{
}

void io::load_tlm(Tilemap *tm, char *filename, char fromAsset)
{
/*        try {

        	//
        	// TODO: Set fixGap to false when loading TLM's 1.0.3 or lower
        	//

            set("name", filename);

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
                is = Lozoware.getUI().getAssetInputStream(filename);
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

					Lozoware.print("read size=(" + sizex + ", " + sizey + ", " + sizez + ") tilesize=" + tileSize);

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

						Lozoware.print("Reading " + num + " entities");

                        // Allocate array for entities
                        entities = new Entity[MAX_ENTITIES];

                        // Read the entities
                        for (int i = 0; i < num; i++) {

                            Entity e = new Entity();

                            e.load(is);

							addEntity(e);

//                            entities[i] = e;

                        }
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

					Lozoware.print("read size=(" + sizex + ", " + sizey + ", " + sizez + ") tilesize=" + tileSize);

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
                        entities = new Entity[MAX_ENTITIES];

                        // Read the entities
                        for (int i = 0; i < numEntities; i++) {

                            Entity e = new Entity();

                            e.load(is);

                            entities[i] = e;

                        }
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
                        entities = new Entity[MAX_ENTITIES];

                        // Read the entities
                        for (int i = 0; i < numEntities; i++) {


                            Entity e = new Entity();

                            e.load(is);

                            entities[i] = e;

                        }
                    }

                    // READ TEXROT

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

                        Lozoware.getUI().consoleInUIThread("numEntities = " + numEntities);

                        // Read the locs
                        for (int i = 0; i < numEntities; i++) {
                            Loc3D t = new Loc3D();

                            t.load(is);

                            mLocs[i] = t;
                            //					Lozoware.showMsg("read loc x = " + t.mX);
                        }
                    }
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

            Lozoware.exception(e);
//			return false;
//            Lozoware.showMsg("tilemap3d 586 " + e.toString());
        }

		return true;*/
}

void io_save_tlm(char *filename) {

/*        try {

			Lozoware.print("saveTLM() " + filename);

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

			Lozoware.print("wrote size=(" + mSizeX + ", " + mSizeY + ", " + mSizeZ + ") tilesize=" + tileSize);

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
            for (int i = 0; i < numEntities; i++) {
                Entity ent = entities[i];

                if (ent != null) {
                    ent.save(f);
                }
            }

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
			Lozoware.exception(e);
            Lozoware.showMsg(e.toString());
        }
*/
}
