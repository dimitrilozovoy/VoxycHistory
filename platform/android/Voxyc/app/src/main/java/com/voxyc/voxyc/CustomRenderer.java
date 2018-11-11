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

import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.opengles.GL10;
import android.opengl.*;

import android.graphics.PointF;
import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;
import android.opengl.GLU;

import java.nio.*;
import android.opengl.*;
import java.util.*;
import android.os.*;

public class CustomRenderer implements android.opengl.GLSurfaceView.Renderer
{	
    boolean started = false;
	int step = 0;

	public static native void SetUpGLBridge(CustomRenderer renderer);
	
	public void onSurfaceCreated(GL10 gl, EGLConfig el)
	{
		SetUpGLBridge(this);
		
		HelloJni.onConsoleEnterCommand = new Handler(Looper.getMainLooper()) {
			@Override
			public void handleMessage(Message msg) {
				Bundle b = msg.getData();
				
				HelloJni.enterCommand(b.getString("cmd"));
			}
		};

/*        HelloJni.touchHandler = new Handler(Looper.getMainLooper())
		{
			@Override
			public void handleMessage(Message msg)
			{
				Bundle b = msg.getData();

				HelloJni.touchEvent(b.getInt("action"), b.getFloat("x"), b.getFloat("y"));
			}
		};*/
	}
	
	public void onDrawFrame(GL10 gl)
	{
		if (!started)
		{
			HelloJni.StartApp(step);
			started = true;
		}
		else
		{
		    HelloJni.AppTick();
		    HelloJni.Draw();
		}
	}
		
	public void onSurfaceChanged(GL10 gl, int a, int b)
	{	
    }
	
	public void glActiveTexture(int texture)
	{
		GLES30.glActiveTexture(texture);
	}

	public void glAttachShader(int program, int shader)
	{
		GLES30.glAttachShader(program, shader);
	}

	public void glBindBuffer(int target, int buffer)
	{
		GLES30.glBindBuffer(target, buffer);
	}
	
	public void glBindTexture(int target, int texture)
	{
		GLES30.glBindTexture(target, texture);
	}

	public void glBindVertexArray(int array)
	{
		GLES30.glBindVertexArray(array);
	}
	
	public void glBlendFunc(int sfactor, int dfactor)
	{
		GLES30.glBlendFunc(sfactor, dfactor);
	}

	public void glBufferData(int target, int size, float data[], int usage)
	{
		FloatBuffer buf = FloatBuffer.allocate(size / 4);
		buf.put(data);
		buf.position(0);
		
	    GLES30.glBufferData(target, size, buf, usage);
	}

	public void glClear(int mask)
	{
		GLES30.glClear(mask);
	}

	public void glClearColor(float red, float green, float blue, float alpha)
	{
		GLES30.glClearColor(red, green, blue, alpha);
	}
	
	public void glClearDepth(float depth)
	{
		GLES30.glClearDepthf(depth);
	}

	public void glCompileShader(int shader)
	{
		GLES30.glCompileShader(shader);
	}

	public int glCreateProgram()
	{
		return GLES30.glCreateProgram();
	}

	public int glCreateShader(int shaderType)
	{
		int shader = GLES30.glCreateShader(shaderType);
		return shader;
	}

	public void glCullFace(int mode)
	{
		GLES30.glCullFace(mode);
	}

	public void glDeleteBuffers(int n, int arrays[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(arrays);
		buf.position(0);
		
		GLES30.glDeleteBuffers(n, buf);
	}

	public void glDeleteProgram(int program)
	{
		GLES30.glDeleteProgram(program);
	}
	
	public void glDeleteTextures(int n, int textures[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(textures);
		buf.position(0);

		GLES30.glDeleteTextures(n, buf);
	}

	public void glDeleteVertexArrays(int n, int arrays[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(arrays);
		buf.position(0);
		
		GLES30.glDeleteVertexArrays(n, buf);
	}

	public void glDisable(int cap)
	{
		GLES30.glDisable(cap);
	}

	public void glDrawArrays(int mode, int first, int count)
	{
		GLES30.glDrawArrays(mode, first, count);
	}
	
	public void glDrawElements(int mode, int count, int type, int indices)
	{
		GLES30.glDrawElements(mode, count, type, indices);
	}

	public void glEnable(int cap)
	{
		GLES30.glEnable(cap);
	}

	public void glEnableVertexAttribArray(int index)
	{
		GLES30.glEnableVertexAttribArray(index);
	}

	public void glFlush()
	{
		GLES30.glFlush();
	}

	public void glFrontFace(int mode)
	{
		GLES30.glFrontFace(mode);
	}

	public void glGenBuffers(int n, int buffers[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(buffers);
		buf.position(0);
		
		GLES30.glGenBuffers(n, buf);
		
		buf.position(0);
		buf.get(buffers, 0, n);
	}
	
	public void glGenTextures(int n, int textures[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(textures);
		buf.position(0);

		GLES30.glGenTextures(n, buf);

		buf.position(0);
		buf.get(textures, 0, n);
	}

	public void glGenVertexArrays(int n, int arrays[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(arrays);
		buf.position(0);
		
		GLES30.glGenVertexArrays(n, buf);
		
		buf.position(0);
		buf.get(arrays, 0, n);
	}

	public int glGetAttribLocation(int program, String name)
	{
		int loc = GLES30.glGetAttribLocation(program, name);
		return loc;
	}
	
	public int glGetError()
	{
		return GLES30.glGetError();
	}

	public String glGetProgramInfoLog(int program)
	{
		return GLES30.glGetProgramInfoLog(program);
	}

	public void glGetProgramiv(int program, int pname, int params[])
	{
		IntBuffer buf = IntBuffer.allocate(1);
		buf.put(params);
		buf.position(0);
		
		GLES30.glGetProgramiv(program, pname, buf);
		
		buf.position(0);
		buf.get(params, 0, 1);
	}

	public String glGetShaderInfoLog(int shader)
	{
		String str = GLES30.glGetShaderInfoLog(shader);
		Log.i("glGetShaderInfoLog()", GLES30.glGetShaderInfoLog(shader));
		return GLES30.glGetShaderInfoLog(shader);
	}

	public void glGetShaderiv(int shader, int pname, int params[])
	{
		IntBuffer buf = IntBuffer.allocate(1);
		buf.put(params);
		buf.position(0);
		
		GLES30.glGetShaderiv(shader, pname, buf);
		
		buf.position(0);
		buf.get(params, 0, 1);
	}

	public int glGetUniformLocation(int program, String name)
	{
		return GLES30.glGetUniformLocation(program, name);
	}

	public void glLinkProgram(int program)
	{
		GLES30.glLinkProgram(program);
	}

	public void glShaderSource(int shader, String str)
	{
		GLES30.glShaderSource(shader, str);
	}
	
	public void glTexImage2D(int target,
 	int level,
 	int internalformat,
 	int width,
 	int height,
 	int border,
 	int format,
 	int type,
 	int data[])
	{
		IntBuffer buf = IntBuffer.allocate(1);
		buf.put(data);
		buf.position(0);
		
		GLES30.glTexImage2D(target, level, internalformat, width, height, border, format, type, buf);
	}
	
	public void glTexParameterf(int target, int pname, float param)
	{
		GLES30.glTexParameterf(target, pname, param);
	}

	public void glUniform1f(int location, float v0)
	{
		GLES30.glUniform1f(location, v0);
	}

	public void glUniform1i(int location, int v0)
	{
		GLES30.glUniform1i(location, v0);
	}
	
	public void glUniform4f(int location, float v0, float v1, float v2, float v3)
	{
		GLES30.glUniform4f(location, v0, v1, v2, v3);
	}

	public void glUniformMatrix4fv(int location, int count, boolean transpose, float value[])
	{
		FloatBuffer buf = FloatBuffer.allocate(16);
		buf.put(value);
		buf.position(0);
		
		GLES30.glUniformMatrix4fv(location, count, transpose, buf);
	}

	public void glUseProgram(int program)
	{
		GLES30.glUseProgram(program);
	}

	public void glVertexAttribPointer( int index, int size, int type, boolean normalized, int stride, int pointe)
	{	
		GLES30.glVertexAttribPointer(index, size, type, normalized, stride, pointe);
	}
	
/*	public void compileShaders()
	{
		int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexShaderCode);
		int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentShaderCode);

		mProgram = GLES20.glCreateProgram();             // Create empty OpenGL ES Program
		GLES20.glAttachShader(mProgram, vertexShader);   // Add the vertex shader to program
		GLES20.glAttachShader(mProgram, fragmentShader); // Add the fragment shader to program
		
		GLES20.glBindAttribLocation(mProgram, VERTEX_ARRAY_ID, "vPosition");
		GLES20.glBindAttribLocation(mProgram, TEXTURECOORD_ARRAY_ID, "vTexCoord");
		
		GLES20.glLinkProgram(mProgram);                  // Creates OpenGL ES program executables
	}
	
	public static int loadShader(int type, String shaderCode){

		int shader = GLES20.glCreateShader(type);

		// Add the source code to the shader and compile it
		GLES20.glShaderSource(shader, shaderCode);
		GLES20.glCompileShader(shader);
		
		int[] res = new int[2];
		
		GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, res, 1);
		
		if (res[1] == 0)
		{
//		    Lozoware.getUI().consoleInUIThread("Shader compile error");
//			Lozoware.getUI().consoleInUIThread(GLES20.glGetShaderInfoLog(shader));
		}

		return shader;
	}*/
}
