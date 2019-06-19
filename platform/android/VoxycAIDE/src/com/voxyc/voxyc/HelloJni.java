/*      c
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

import android.Manifest;
import android.app.*;
//import android.support.v4.app.ActivityCompat;
//import android.support.v4.content.ContextCompat;
import android.widget.TextView;
import android.os.Bundle;

import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.media.AudioManager;
import android.os.Bundle;
import android.util.Log;
import android.content.Intent;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import android.media.SoundPool;
import android.media.MediaPlayer;
import android.view.Surface;
import android.opengl.*;
import android.opengl.GLSurfaceView.*;
import android.widget.*;

import android.view.ViewGroup.*;
import android.view.*;

import android.graphics.*;
import android.os.*;
import android.content.pm.*;
import android.content.*;
import android.net.Uri;

import android.view.View.*;

import java.io.*;
import android.graphics.Matrix;
import android.location.*;

public class HelloJni extends Activity
{
	public static final Boolean SOFTWARE_RENDER = false;
	
	CustomSurfaceView surfaceView;
	android.opengl.GLSurfaceView.Renderer renderer;
	Bitmap screen;
	public static native void SetUpBridge(HelloJni activity);
	public static native void SetScreen(Bitmap bitmap);
	public static native void StartApp(int step);
	public static native void LoadApp(int step);
	public static native void AppTick();
	public static native void Draw();
	public static native void AppFree();
	public static native void touchEvent(int count, int action1, float x1, float y1, int action2, float x2, float y2, int actionIndex);
	public static native void enterCommand(String str);
	public static native void setFilesDir(String str, String str2, String str3);
	public static native void setExtraInt(String name, int value);
	public static native void setExtraStr(String name, String value);
	
	private static AssetManager amgr;
	InputStream is;
	SoundPool soundPool;
	MediaPlayer player;
	String curTrackFilename = "";
    float curTrackVolume = 1.0f;
    String curAssetsDir = "";
	RelativeLayout rl = null;
	TextView console;
	LinearLayout llConsoleInput;
	EditText consoleInput;
	Button consoleEnter;
	public static final boolean DEBUG_BUILD = true;
	int mConsoleMaxTotalLines = 25;
	int mConsoleTotalLines = 0;
	int bytesRead = 0;
	
	public static Handler onConsoleEnterCommand;
	public static Handler touchHandler;
	public static Handler onShowFileSelector;
	public static Handler onClearListMenu;
	public static Handler onAddListMenuOption;
	public static Handler onShowListMenuInDialog;
	public static Handler onShowText;
	public static Handler onShowLongText;
	public static Handler onGetCameraPic;
	public static Handler onShowDialog;
	public static Handler onStartTrackLocation;
	public static Handler onStopTrackLocation;

	// =======================================================================
    //
    //   App lifecycle
    //
    // =======================================================================

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
		
		SetUpBridge(this);
		
		amgr = getAssets();
		
		copyAssets(getFilesDir().getAbsolutePath());
		setFilesDir(getFilesDir().getAbsolutePath(), getExternalFilesDir(null).getAbsolutePath(), Environment.getExternalStorageDirectory().getAbsolutePath());
		
		// Sound pool
		soundPool = new SoundPool(32, AudioManager.STREAM_MUSIC, 0);

		// Media player
		try
		{
			player = new MediaPlayer();
		}
		catch (Exception e)
		{
			Log.e("onCreate", e.toString());
		}
		
		    surfaceView = new CustomSurfaceView(this);
		    surfaceView.setEGLContextClientVersion(2);
		
//		    renderer = new CustomRendererES20();

			// Detect OpenGL ES version
			final ActivityManager activityManager = 
				(ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
			final ConfigurationInfo configurationInfo = 
				activityManager.getDeviceConfigurationInfo();
			boolean supportsES3 = configurationInfo.reqGlEsVersion >= 0x30000;
			
		    supportsES3 = false;

			if (supportsES3)
			{
				surfaceView.setEGLContextClientVersion(3);
				renderer = new CustomRenderer();
			}
			else
			{
				surfaceView.setEGLContextClientVersion(2);
				surfaceView.setEGLConfigChooser(true);
		//		surfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
				renderer = new CustomRendererES20();
			}
		
		    surfaceView.setRenderer(renderer);

			if (DEBUG_BUILD)
			{
                rl = new RelativeLayout(this);
                rl.setLayoutParams(new LayoutParams(ViewGroup.LayoutParams.FILL_PARENT, ViewGroup.LayoutParams.FILL_PARENT));

                rl.addView(surfaceView);

                console = new TextView(this);
                console.setTextColor(Color.rgb(255, 0, 0));
                console.setShadowLayer(1, 0, 0, Color.BLACK);
                rl.addView(console);
			
/*			    llConsoleInput = new LinearLayout(this);
				llConsoleInput.setLayoutParams(new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
				llConsoleInput.setOrientation(LinearLayout.HORIZONTAL);
				
				consoleInput = new EditText(this);
				consoleInput.setLayoutParams(new LayoutParams(512, LayoutParams.WRAP_CONTENT));
			    consoleEnter = new Button(this);
				consoleEnter.setText("Enter");
				consoleEnter.setOnClickListener(new OnClickListener() {
						public void onClick(View v) {
							Message msg = new Message();
							Bundle bundle = new Bundle();
							
						    bundle.putString("cmd", consoleInput.getText().toString());
							msg.setData(bundle);
							
							onConsoleEnterCommand.sendMessage(msg);
							
							consoleInput.setText("");
						}
					});
				
				llConsoleInput.addView(consoleInput);
				llConsoleInput.addView(consoleEnter);
				
				rl.addView(llConsoleInput);*/

                setContentView(rl);
		    }
		    else
		    {
			    setContentView(surfaceView);
		    }
			
		HelloJni.onShowFileSelector = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				showFileSelector2(msg.getData().getString("ext"), msg.getData().getString("dir"));
			}
		};
		
		HelloJni.onClearListMenu = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				clearListMenu2();
			}
		};
		
		HelloJni.onAddListMenuOption = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				Bundle bun = msg.getData();
				addListMenuOption2(bun.getString("title"), bun.getString("desc"));
			}
		};
		
		HelloJni.onShowListMenuInDialog = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				showListMenuInDialog2("", "");
			}
		};
		
		HelloJni.onShowText = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				Bundle bun = msg.getData();
				showText2(bun.getString("text"));
			}
		};
		
		HelloJni.onShowLongText = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				Bundle bun = msg.getData();
				showLongText2(bun.getString("text"));
			}
		};

		HelloJni.onGetCameraPic = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				Bundle bun = msg.getData();
				getCameraPic2(bun.getString("longfname"));
			}
		};
		
		HelloJni.onShowDialog = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				Bundle bun = msg.getData();
				showDialog2(bun.getString("title"), bun.getString("okText"), bun.getString("cancelText"), bun.getString("okExtra"));
			}
		};

		HelloJni.onStartTrackLocation = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				Bundle bun = msg.getData();
				startTrackLocation2();
			}
		};
		
		HelloJni.onStopTrackLocation = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				Bundle bun = msg.getData();
				stopTrackLocation2();
			}
		};

		// Permissions
/*		if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
				!= PackageManager.PERMISSION_GRANTED) {
			ActivityCompat.requestPermissions(this,
					new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE},
					1000);
		}*/	
    }
		
	@Override
    public void onStart()
    {
		super.onStart();
	}

	@Override
	public void onResume()
	{
		super.onResume();

		playTrack(curTrackFilename, curAssetsDir, true);
        player.setVolume(curTrackVolume, curTrackVolume);
	}

	@Override
	public void onPause()
	{
		super.onPause();

		player.stop();
	}

	@Override
	public void onStop()
	{
		super.onStop();
	}
    /* A native method that is implemented by the
     * 'hello-jni' native library, which is packaged
     * with this application.
     */
//    public native String  stringFromJNI();

    /* This is another native method declaration that is *not*
     * implemented by 'hello-jni'. This is simply to show that
     * you can declare as many native methods in your Java code
     * as you want, their implementation is searched in the
     * currently loaded native libraries only the first time
     * you call them.
     *
     * Trying to call this function will result in a
     * java.lang.UnsatisfiedLinkError exception !
     */
//    public native String  unimplementedStringFromJNI();

    /* this is used to load the 'hello-jni' library on application
     * startup. The library has already been unpacked into
     * /data/data/com.example.hellojni/lib/libhello-jni.so at
     * installation time by the package manager.
     */
    static {
        System.loadLibrary("hello-jni");
//		System.loadLibrary("android");
//		System.loadLibrary("assimp");
    }
	
	// =======================================================================
    //
    //   Asset loading
    //
    // =======================================================================
	
	public Bitmap open(String path)
	{
		try
		{
			return BitmapFactory.decodeStream(amgr.open(path));
		}
		catch (Exception e) {
			Log.e("open", "Error opening " + path);
		}
		return null;
	}

	public int getWidth(Bitmap bmp) {
		if (bmp != null)
			return bmp.getWidth();
		else
			return 0;
	}

	public int getHeight(Bitmap bmp) {
		if (bmp != null)
			return bmp.getHeight();
		else
			return 0;
	}

	public void getPixels(Bitmap bmp, int[] pixels)
	{
		int w = bmp.getWidth();
		int h = bmp.getHeight();
		bmp.getPixels(pixels, 0, w, 0, 0, w, h);
	}

	public void close(Bitmap bmp)
	{
		bmp.recycle();
	}

	public void assetOpen(String filename)
	{
		try {
			is = getResources().getAssets().open(filename);
			bytesRead = 0;
		}
		catch (IOException e)
		{
			Log.e("openAsset", e.toString());
		}
	}

	public char assetReadByte()
	{
		try {
			int b = is.read();
			
			if (b == -1)
			{
				Log.e("assetReadByte", "ERROR: byte not read");
				return 0;
			}
			
			bytesRead++;
			
			return (char)b;
		}
		catch (IOException e)
		{
			Log.e("assetReadByte", e.toString());
		}

		return 0;
	}

	public int assetReadInt()
	{
		byte[] bytes;

		bytes = new byte[4];

		try {
			int r = is.read(bytes);
			
			if (r == -1)
			{
				Log.e("assetReadInt", "ERROR: int not read");
				return 0;
			}
			else
			{
				bytesRead += r;
			}
		}
		catch (IOException e)
		{
			Log.e("assetReadInt", e.toString());
		}

		byte b0 = bytes[3];
		byte b1 = bytes[2];
		byte b2 = bytes[1];
		byte b3 = bytes[0];

		bytes[0] = b0;
		bytes[1] = b1;
		bytes[2] = b2;
		bytes[3] = b3;

		return ByteBuffer.wrap(bytes).getInt();
		// TODO: Fix byte ordering
//		return (int)bytes[0];
	}

	public float assetReadFloat()
	{
		byte[] bytes;

		bytes = new byte[4];

		try {
			int r = is.read(bytes);

			if (r == -1)
			{
				Log.e("assetReadFloat", "ERROR: float not read");
				return 0;
			}
			else
			{
				bytesRead += r;
			}
		}
		catch (IOException e)
		{
			Log.e("assetReadInt", e.toString());
		}

		byte b0 = bytes[3];
		byte b1 = bytes[2];
		byte b2 = bytes[1];
		byte b3 = bytes[0];

		bytes[0] = b0;
		bytes[1] = b1;
		bytes[2] = b2;
		bytes[3] = b3;

		return ByteBuffer.wrap(bytes).getFloat();
	}
	
	public int getBytesRead()
	{
		return bytesRead;
	}

	public AssetManager getAssetManager()
	{
		return amgr;
	}

	// =======================================================================
    //
    //   Sound
    //
    // =======================================================================
	
	public int loadSound(String filename, String assetsDir)
	{
		int id = -1;

		// Assets dir
		try
		{
			id = soundPool.load(assetsDir + "/" + filename, 0);
		}
		catch (Exception e)
		{
			// Fall back to build-in assets
			try {
				AssetFileDescriptor afd = amgr.openFd(filename);
				id = soundPool.load(afd, 0);
			}
			catch (Exception e2)
			{
				Log.e("loadSound", "Error loading sound " + filename + ": " + e2.toString());
			}
		}
		
		return id;
	}

	public int playSound(int id)
	{
		return soundPool.play(id, 1.0f, 1.0f, 0, 0, 1.0f);
	}

	public void stopSound(int stream)
	{
		soundPool.stop(stream);
	}


    public void playTrack(String filename, String assetsDir, boolean force) {

        if (filename.equals("") || (filename.equals(curTrackFilename) && !force))
            return;

        curTrackFilename = filename;
        curAssetsDir = assetsDir;

        if (player.isPlaying()) {
            player.stop();
        }

        // Media player
        try {
            player = new MediaPlayer();
        } catch (Exception e) {
            Log.e("onCreate", e.toString());
        }

        // Assets dir
        try {
            player.setDataSource(assetsDir + "/" + filename);
        } catch (Exception e) {
            try {
                // Fall back to build-in assets
                AssetFileDescriptor afd = amgr.openFd(filename);
                player.setDataSource(afd.getFileDescriptor(), afd.getStartOffset(), afd.getLength());
            } catch (IOException e2) {
                Log.e("playTrack", e2.toString());
            }
        }

        try {
            player.setLooping(true);
            player.prepare();
            player.start();
        } catch (Exception e) {
            Log.e("playTrack", e.toString());
        }
    }
	
	public void stopTrack()
	{
		player.stop();
	}
	
        public void setTrackVolume(float vol) {
                curTrackVolume = vol;
                player.setVolume(vol, vol);
        }

	public void log(final String str)
	{
		runOnUiThread (new Thread(new Runnable()
	    { 
			public void run() {
				Log.i("log", str);
			}
		}));

		if (DEBUG_BUILD)
		{
			runOnUiThread (new Thread(new Runnable()
			{ 
			    public void run() {
					console(str);
                }
			}));
		}
			
	}
	
	// =======================================================================
    //
    //   Misc
    //
    // =======================================================================

	enum FLIP{HORIZONTAL,VERTICAL};

	public GLSurfaceView getSurfaceView()
	{
		return surfaceView;
	}
	
	public Surface getSurface()
	{
		return surfaceView.getHolder().getSurface();
	}
	
	public void loadTextureInJava(int tex, String filename, String assetsDir) {

		try
		{
			InputStream inputStream = null;
			Bitmap bitmap = null;


			// Assets dir
			try
			{
				File f = new File(filename);
				FileInputStream fin = new FileInputStream(assetsDir + "/" + filename);
				bitmap = android.graphics.BitmapFactory.decodeStream(fin);
			}
			catch (IOException e)
			{
				// Fall back to build-in assets
				inputStream = amgr.open(filename);
				bitmap = android.graphics.BitmapFactory.decodeStream(inputStream);
				inputStream.close();
			}

			// Flip texture upside down
			bitmap = flip(bitmap, FLIP.VERTICAL);

			// Load texture into GPU memory
            GLES20.glBindTexture(GLES30.GL_TEXTURE_2D, tex);

            GLES20.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_NEAREST);
            GLES20.glTexParameterf(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_NEAREST);

            GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, bitmap, 0);
		}
		catch (Exception e)
		{
			log("loadTexture " + e.toString());
		}
    }

	public static Bitmap flip(Bitmap bitmap, FLIP type)
	{
		Matrix matrix =new Matrix();

		if(type==FLIP.HORIZONTAL)
		{
			matrix.preScale(-1.0f, 1.0f);
		}
		else if(type==FLIP.VERTICAL)
		{
			matrix.preScale(1.0f, -1.0f);
		}

		return Bitmap.createBitmap(bitmap, 0, 0, bitmap.getWidth(), bitmap.getHeight(), matrix, true);
	}

	public int getWindowWidth()
	{
/*		Display display = getWindowManager().getDefaultDisplay();
		Point size = new Point();
		display.getSize(size);
		return size.x;*/
		return surfaceView.getWidth();
	}
	
	public int getWindowHeight()
	{
/*		Display display = getWindowManager().getDefaultDisplay();
		Point size = new Point();
		display.getSize(size);
		return size.y;*/
		return surfaceView.getHeight();
	}
	
	public long getTime()
	{
		return android.os.SystemClock.uptimeMillis();
//		return System.nanoTime();
//	    return System.currentTimeMillis();
	}
	
	public int stoi(String str, int fallback)
	{
		try
		{
		    return Integer.parseInt(str);
		}
		catch (Exception e)
		{
			return fallback;
		}
	}

	public float stof(String str, float fallback)
	{
		try
		{
			return Float.parseFloat(str);
		}
		catch (Exception e)
		{
			return fallback;
		}
	}

	// =======================================================================
    //
    //   Console
    //
    // =======================================================================
   
	String m_fullConsole = "";
    String m_visibleConsole = "";
    int m_visConsoleLen = 200;
	
    public void console(String str) {
		if (!DEBUG_BUILD)
			return;
			
		m_visibleConsole += "\n" + str;
		
		if (m_visibleConsole.length() > 500)
		{
			m_visibleConsole = "";
		}
		
		console.setText(m_visibleConsole);
    }
		
	// =======================================================================
    //
    //   Java OBJ loader
    //
    // =======================================================================
	
	void oobjloaderLoad(String filename)
	{
/*		 try {
//	         builder = new FatelessObjBuild();
//		     obj = new Parse(builder, filename, this);
		 } catch (java.io.FileNotFoundException e) {
//		 log.log(INFO, "FileNotFoundException loading file "+filename+", e=" + e);
		     e.printStackTrace();
		 } catch (java.io.IOException e) {
//		 log.log(INFO, "IOException loading file "+filename+", e=" + e);
		     e.printStackTrace();
		 }*/
	}
	
/*	int oobjloaderGetNumVertices()
	{
		return builder.verticesG.size();
	}
	
	float oobjloaderGetVertexX(int index)
	{
		return builder.verticesG.get(index).x;
	}
	
	float oobjloaderGetVertexY(int index)
	{
		return builder.verticesG.get(index).y;
	}
	
	float oobjloaderGetVertexZ(int index)
	{
		return builder.verticesG.get(index).z;
	}
	
	int oobjloaderGetNumTexCoords()
	{
		return builder.verticesT.size();
	}

	float oobjloaderGetTexCoordX(int index)
	{
		return builder.verticesT.get(index).u;
	}

	float oobjloaderGetTexCoordY(int index)
	{
		return builder.verticesT.get(index).v;
	}

	int oobjloaderGetNumNormals()
	{
		return builder.verticesN.size();
	}

	float oobjloaderGetNormalX(int index)
	{
		return builder.verticesN.get(index).x;
	}

	float oobjloaderGetNormalY(int index)
	{
		return builder.verticesN.get(index).y;
	}

	float oobjloaderGetNormalZ(int index)
	{
		return builder.verticesN.get(index).z;
	}
	
	int oobjloaderGetNumFaces()
	{
		return builder.faces2.size();
	}

	int oobjloaderGetFaceElement(int face, int element)
	{
		return builder.faces2.get(face).vertexIndices[element];
	}
*/
	// =======================================================================
    //
    //   Asset copy
    //
    // =======================================================================
	
	private void copyAssets(String dest) {
		AssetManager assetManager = getAssets();
		String[] files = null;
		try {
			files = assetManager.list("");
		} catch (IOException e) {
			Log.e("tag", "Failed to get asset file list.", e);
		}
		if (files != null) for (String filename : files) {
				InputStream in = null;
				OutputStream out = null;
				try {
					in = assetManager.open(filename);
					File outFile = new File(dest, filename);
					out = new FileOutputStream(outFile);
					copyFile(in, out);
				} catch(IOException e) {
					Log.e("tag", "Failed to copy asset file: " + filename, e);
				}     
				finally {
					if (in != null) {
						try {
							in.close();
						} catch (IOException e) {
							// NOOP
						}
					}
					if (out != null) {
						try {
							out.close();
						} catch (IOException e) {
							// NOOP
						}
					}
				}  
			}
	}
	
	private void copyFile(InputStream in, OutputStream out) throws IOException {
		byte[] buffer = new byte[1024];
		int read;
		while((read = in.read(buffer)) != -1){
			out.write(buffer, 0, read);
		}
	}
	
	// =======================================================================
    //
    //   GUI
    //
    // =======================================================================

	//
	// File selector
	//
	
	public void showFileSelector(String ext, String dir)
	{
		Message msg = new Message();

		msg.getData().putString("ext", ext);
		msg.getData().putString("dir", dir);

		onShowFileSelector.sendMessage(msg);
	}
	
	public void showFileSelector2(String ext, String sdir)
	{
		File dir;
		dir = getExternalFilesDir(null);

		if (sdir == null || sdir.equals(""))
		{
			sdir = dir.getAbsolutePath();
		}

		DDLUtils.mainActivity = this;
        DDLUtils.showFileSelectorWithPrompt("Enter filename", sdir,
			new RespondToNode() {
				@Override
				public void respondToNode() {
					try
					{
						String fname = m_input.get("filename");

						File f = new File(fname);
						String shortfname = f.getName();
						
						setExtraStr("fileselected", fname);
					}
					catch (Exception e)
					{
					}
				}
			}, true);
	}
	
	//
	// ListMenu
	//

    public void clearListMenu()
	{
		Message msg = new Message();
		onClearListMenu.sendMessage(msg);
	}
	
	public void clearListMenu2()
	{
		DDLUtils.mainActivity = this;
		DDLUtils.clearListMenu();
	}
	
    public void addListMenuOption(String caption, String description)
	{
		Message msg = new Message();
		Bundle data = new Bundle();
		data.putString("title", caption);
		data.putString("desc", description);
		msg.setData(data);
		onAddListMenuOption.sendMessage(msg);
	}
		
    public void addListMenuOption2(final String caption, String description)
	{
		DDLUtils.mainActivity = this;
		DDLUtils.addListMenuOption(caption, description, new RespondToNode() {
				@Override
				public void respondToNode() {
					setExtraStr("listmenuoptionclicked", caption);
				    }
			}, new RespondToNode() {
				@Override
				public void respondToNode() {
				}
			});
	}
	
	public void showListMenuInDialog(String title, String options)
	{
		Message msg = new Message();
		onShowListMenuInDialog.sendMessage(msg);
	}
	
	public void showListMenuInDialog2(String title, String options)
	{
		DDLUtils.mainActivity = this;
		DDLUtils.showListMenuInDialog(title, options);
	}
	
	//
	// Text
	//
	
	public void showText(String text)
	{
		Message msg = new Message();
		Bundle data = new Bundle();
		data.putString("text", text);
		msg.setData(data);
		onShowText.sendMessage(msg);
	}

	public void showText2(String text)
	{
		DDLUtils.mainActivity = this;
		DDLUtils.showText(text);
	}
	
	public void showLongText(String text)
	{
		Message msg = new Message();
		Bundle data = new Bundle();
		data.putString("text", text);
		msg.setData(data);
		onShowLongText.sendMessage(msg);
	}
	
	public void showLongText2(String text)
	{
		TextView tv = new TextView(this);
		tv.setText(text);
		LayoutParams lp = new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		tv.setLayoutParams(lp);
		
		ScrollView sv = new ScrollView(this);
		sv.setLayoutParams(lp);
		
		sv.addView(tv);
		
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		builder.setView(sv);
		
		builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
				@Override
				public void onClick(DialogInterface dialog, int id) {
				}
			});
		Dialog dialog = builder.create();
		
		dialog.show();
	}
	
	//
	// Dialog
	//
	
	private class DialogPart
	{
		public String extraKey;
		public EditText etValue;
	}
	
	AlertDialog.Builder builder = null;
	LinearLayout llv;
	java.util.ArrayList<DialogPart> dialogParts;
	
	void clearDialog()
	{
		builder = new AlertDialog.Builder(this);
		llv = new LinearLayout(getApplication());
		llv.setOrientation(LinearLayout.VERTICAL);
		dialogParts = new java.util.ArrayList<DialogPart>();
	}
	
	void addDialogPart(String caption, String defaultValue, String extraKey)
	{
		if (builder == null)
			clearDialog();
			
		LinearLayout.LayoutParams lp = new LinearLayout.LayoutParams(200, LayoutParams.WRAP_CONTENT);
		lp.setMargins(10, 10, 10, 10);
		
		LinearLayout llh = new LinearLayout(getApplication());
		llh.setOrientation(LinearLayout.HORIZONTAL);
		
		TextView tvCaption = new TextView(this);
		tvCaption.setText(caption);
		tvCaption.setLayoutParams(lp);
		
		EditText etValue = new EditText(this);
		etValue.setText(defaultValue);
		etValue.setLayoutParams(lp);
		
		llh.addView(tvCaption);
		llh.addView(etValue);
		
		DialogPart dp = new DialogPart();
		dp.extraKey = extraKey;
		dp.etValue = etValue;
		dialogParts.add(dp);
		
		llv.addView(llh);
	}
	
	void showDialog(String title, String okText, String cancelText, final String okExtra)
	{
		Message msg = new Message();
		Bundle data = new Bundle();
		
		data.putString("title", title);
		data.putString("okText", okText);
		data.putString("cancelText", cancelText);
		data.putString("okExtra", okExtra);
		
		msg.setData(data);
		onShowDialog.sendMessage(msg);
	}
	
	void showDialog2(String title, String okText, String cancelText, final String okExtra)
	{
		builder.setPositiveButton(okText,
		    new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int whichButton)
				{
					for (int i = 0; i < dialogParts.size(); i++)
					{
						DialogPart dp = dialogParts.get(i);
						setExtraStr(dp.extraKey, dp.etValue.getText().toString());
					}
					
					setExtraInt(okExtra, 1);
				}
			});

		builder.setNegativeButton(cancelText,
		    new DialogInterface.OnClickListener() {
				public void onClick(DialogInterface dialog, int whichButton)
				{
				}
			});
		
		AlertDialog dialog = builder.create();
		
		dialog.setView(llv);
		dialog.setTitle(title);
		dialog.show();
	}
	
	// =======================================================================
    //
    //   Camera
    //
    // =======================================================================
	
	static final int REQUEST_IMAGE_CAPTURE = 1;
	
	String shortCameraFilename = "";
	public static final int REQUEST_CAMERA_PIC = 100;
	
	public String getCameraPic()
	{
		String timeStamp = new java.text.SimpleDateFormat("yyyyMMdd_HHmmss").format(new java.util.Date());
		
		shortCameraFilename = "IMG_" + timeStamp + ".png";
		
	    String longfname = getExternalFilesDir(null) + "/IMG_" + timeStamp + ".png";
		
		Message msg = new Message();
		Bundle data = new Bundle();
		data.putString("longfname", longfname);
		msg.setData(data);
		onGetCameraPic.sendMessage(msg);
		
		return shortCameraFilename;
	}
	
	public String getCameraPic2(String longfname)
	{
	    String fname = "";
		
	    Intent intent = new Intent(android.provider.MediaStore.ACTION_IMAGE_CAPTURE);

	    if (intent.resolveActivity(getPackageManager()) != null)
	    {
	        Uri fileUri = Uri.fromFile(new File(longfname));

	        if (fileUri == null)
	            return "";

	        intent.putExtra(android.provider.MediaStore.EXTRA_OUTPUT, fileUri);

	        startActivityForResult(intent, REQUEST_CAMERA_PIC);

	        fname = fileUri.toString();            
	        fname = fname.substring(7);
	    }

	    return shortCameraFilename;
	}

/*	private Uri getOutputMediaFileUri(int type)
	{
   	    File f = getOutputMediaFile();
		 
	    if (f != null)
	        return Uri.fromFile(f);
	    else
	         return null;
	}

	private File getOutputMediaFile()
	{
	 //		ui.getMP().set("camerapicfilename", fname);
	     
	    return new File(fname);
	}*/

	private static File getOutputMediaFile(int type) {
/*	 // To be safe, you should check that the SDCard is mounted

	 if (Environment.getExternalStorageState() != null) {
	 // this works for Android 2.2 and above
	 File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), "3DEdit");

	 // This location works best if you want the created images to be shared
	 // between applications and persist after your app has been uninstalled.

	 // Create the storage directory if it does not exist
	 if (!mediaStorageDir.exists()) {
	 if (!mediaStorageDir.mkdirs()) {
	 Lozoware.getUI().showMsgTS("Failed to create directory " + mediaStorageDir.getAbsolutePath());
	 return null;
	 }
	 }

	 // Create a media file name
	 String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
	 File mediaFile;
	 if (type == 1) {
	 mediaFile = new File(mediaStorageDir.getPath() + File.separator
	 + "IMG_" + timeStamp + ".jpg");
	 } else if (type == 1) {
	 mediaFile = new File(mediaStorageDir.getPath() + File.separator
	 + "VID_" + timeStamp + ".mp4");
	 } else {
	 return null;
	 }

	 return mediaFile;
	 }

	 Lozoware.showMsgTS("ExternalStorageState is null");
*/
	    return null;
	} 
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data)
	{
	    // Check which request we're responding to
		if (requestCode == REQUEST_CAMERA_PIC)
		{
			// Make sure the request was successful
			if (resultCode == RESULT_OK)
			{
				setExtraInt("camerapicready", 1);
			}
		}
	}
	
	// =======================================================================
    //
    //   Prefs
    //
    // =======================================================================
	
	public void savePref(String section, String key, String value)
	{
		SharedPreferences prefs = getApplicationContext().getSharedPreferences(section, 0);
		SharedPreferences.Editor editor = prefs.edit();
		
		editor.putString(key, value);
		editor.commit();
	}
	
	public String loadPref(String section, String key, String def)
	{
		SharedPreferences prefs = getApplicationContext().getSharedPreferences(section, 0);
		
		String val = prefs.getString(key, def);
		
		return prefs.getString(key, def);
	}
	
	// =======================================================================
    //
    //   Location
    //
    // =======================================================================
	
	LocationManager locationManager;
	LocationListener locationListener;
	
	public void startTrackLocation()
	{
		Message msg = new Message();
		Bundle data = new Bundle();
		msg.setData(data);
		onStartTrackLocation.sendMessage(msg);
	}
	
	public void startTrackLocation2()
	{
		// Acquire a reference to the system Location Manager
		locationManager = (LocationManager)this.getSystemService(Context.LOCATION_SERVICE);

        // Define a listener that responds to location updates
		locationListener = new LocationListener() {
			public void onLocationChanged(Location location) {
				// Called when a new location is found by the network location provider.
//				makeUseOfNewLocation(location);
//				showText(location.getLatitude() + " " + location.getLongitude());
			}

			public void onStatusChanged(String provider, int status, Bundle extras) {}

			public void onProviderEnabled(String provider) {}

			public void onProviderDisabled(String provider) {}
		};

        // Register the listener with the Location Manager to receive location updates
		locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, locationListener);
	}
	
	public void stopTrackLocation()
	{
		Message msg = new Message();
		Bundle data = new Bundle();
		msg.setData(data);
		onStopTrackLocation.sendMessage(msg);
	}
	
	public void stopTrackLocation2()
	{
        locationManager.removeUpdates(locationListener);
	}
}
