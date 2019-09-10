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

public class CustomRendererES20 implements android.opengl.GLSurfaceView.Renderer
{	
    boolean started = false;
    boolean loaded = false;
	int step = 0;

	public static native void SetUpGLBridge(CustomRendererES20 renderer);
	
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

        HelloJni.touchHandler = new Handler(Looper.getMainLooper())
        {
            @Override
            public void handleMessage(Message msg)
            {
                Bundle b = msg.getData();
                HelloJni.touchEvent(b.getInt("count"), b.getInt("action1"), b.getFloat("x1"), b.getFloat("y1"), b.getInt("action2"), b.getFloat("x2"), b.getFloat("y2"), b.getInt("actionIndex"));
            }
        };
	}
	
	public void onDrawFrame(GL10 gl)
	{
		if (!started)
		{
		    HelloJni.StartApp(step);
			started = true;
		}
		else if (!loaded)
		{
			HelloJni.LoadApp(step);
			loaded = true;
		}
		else
		{
		    HelloJni.AppTick();
		    HelloJni.Draw();
		}
	}
		
	public void onSurfaceChanged(GL10 gl, int a, int b)
	{
		HelloJni.surfaceChanged();
    }
	
	public void glActiveTexture(int texture)
	{
		GLES20.glActiveTexture(texture);
	}

	public void glAttachShader(int program, int shader)
	{
		GLES20.glAttachShader(program, shader);
	}

	public void glBindBuffer(int target, int buffer)
	{
		GLES20.glBindBuffer(target, buffer);
	}
	
	public void glBindFramebuffer(int target, int buffer)
	{
		GLES20.glBindFramebuffer(target, buffer);
	}

	public void glBindRenderbuffer(int target, int buffer)
	{
		GLES20.glBindRenderbuffer(target, buffer);
	}

	public void glBindTexture(int target, int texture)
	{
		GLES20.glBindTexture(target, texture);
	}

	public void glBindVertexArray(int array)
	{
		//GLES20.glBindVertexArray(array);
	}
	
	public void glBlendFunc(int dfactor, int sfactor)
	{
		GLES20.glBlendFunc(dfactor, sfactor);
	}

	public void glBufferData(int target, int size, float data[], int usage)
	{
		FloatBuffer buf = FloatBuffer.allocate(size / 4);
		buf.put(data);
		buf.position(0);
		
	    GLES20.glBufferData(target, size, buf, usage);
	}
	
	public int glCheckFramebufferStatus(int fb)
	{
		return GLES20.glCheckFramebufferStatus(fb);
	}

	public void glClear(int mask)
	{
		GLES20.glClear(mask);
	}

	public void glClearColor(float red, float green, float blue, float alpha)
	{
		GLES20.glClearColor(red, green, blue, alpha);
	}
	
	public void glClearDepth(float depth)
	{
		GLES20.glClearDepthf(depth);
	}

	public void glCompileShader(int shader)
	{
		GLES20.glCompileShader(shader);
	}

	public int glCreateProgram()
	{
		return GLES20.glCreateProgram();
	}

	public int glCreateShader(int shaderType)
	{
		int shader = GLES20.glCreateShader(shaderType);
		return shader;
	}

	public void glCullFace(int mode)
	{
		GLES20.glCullFace(mode);
	}

	public void glDeleteBuffers(int n, int arrays[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(arrays);
		buf.position(0);
		
		GLES20.glDeleteBuffers(n, buf);
	}

	public void glDeleteProgram(int program)
	{
		GLES20.glDeleteProgram(program);
	}
	
	public void glDeleteTextures(int n, int textures[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(textures);
		buf.position(0);

		GLES20.glDeleteTextures(n, buf);
	}

	public void glDeleteVertexArrays(int n, int arrays[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(arrays);
		buf.position(0);
		
		//GLES20.glDeleteVertexArrays(n, buf);
	}

	public void glDepthFunc(int cap)
	{
		GLES20.glDepthFunc(cap);
	}

	public void glDepthMask(boolean val)
	{
		GLES20.glDepthMask(val);
	}
	
	public void glDepthRange(float near, float far)
	{
		GLES20.glDepthRangef(near, far);
	}

	public void glDisable(int cap)
	{
		GLES20.glDisable(cap);
	}

	public void glDrawArrays(int mode, int first, int count)
	{
		GLES20.glDrawArrays(mode, first, count);
	}
	
	public void glDrawElements(int mode, int count, int type, int indices)
	{
		GLES20.glDrawElements(mode, count, type, indices);
	}

	public void glEnable(int cap)
	{
		GLES20.glEnable(cap);
	}

	public void glEnableVertexAttribArray(int index)
	{
		GLES20.glEnableVertexAttribArray(index);
	}

	public void glFlush()
	{
		GLES20.glFlush();
	}

	public void glFramebufferRenderbuffer(int target, int attachment, int renderbuffertarget, int renderbuffer)
	{
		GLES20.glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
	}

	public void glFramebufferTexture2D(int target, int attachment, int texTarget, int texture, int level)
	{
		GLES20.glFramebufferTexture2D(target, attachment, texTarget, texture, level);
	}

	public void glFrontFace(int mode)
	{
		GLES20.glFrontFace(mode);
	}

	public void glGenBuffers(int n, int buffers[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(buffers);
		buf.position(0);
		
		GLES20.glGenBuffers(n, buf);
		
		buf.position(0);
		buf.get(buffers, 0, n);
	}
	
	public void glGenFramebuffers(int n, int buffers[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(buffers);
		buf.position(0);

		GLES20.glGenFramebuffers(n, buf);

		buf.position(0);
		buf.get(buffers, 0, n);
	}
	
	public void glGenRenderbuffers(int n, int buffers[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(buffers);
		buf.position(0);

		GLES20.glGenRenderbuffers(n, buf);

		buf.position(0);
		buf.get(buffers, 0, n);
	}
	
	public void glGenTextures(int n, int textures[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(textures);
		buf.position(0);

		GLES20.glGenTextures(n, buf);

		buf.position(0);
		buf.get(textures, 0, n);
	}

	public void glGenVertexArrays(int n, int arrays[])
	{
		IntBuffer buf = IntBuffer.allocate(n);
		buf.put(arrays);
		buf.position(0);
		
		//GLES20.glGenVertexArrays(n, buf);
		
		buf.position(0);
		buf.get(arrays, 0, n);
	}

	public int glGetAttribLocation(int program, String name)
	{
		int loc = GLES20.glGetAttribLocation(program, name);
		return loc;
	}
	
	public int glGetError()
	{
		return GLES20.glGetError();
	}

	public String glGetProgramInfoLog(int program)
	{
		return GLES20.glGetProgramInfoLog(program);
	}

	public void glGetProgramiv(int program, int pname, int params[])
	{
		IntBuffer buf = IntBuffer.allocate(1);
		buf.put(params);
		buf.position(0);
		
		GLES20.glGetProgramiv(program, pname, buf);
		
		buf.position(0);
		buf.get(params, 0, 1);
	}

	public String glGetShaderInfoLog(int shader)
	{
		String str = GLES20.glGetShaderInfoLog(shader);
		Log.i("glGetShaderInfoLog()", GLES30.glGetShaderInfoLog(shader));
		return GLES20.glGetShaderInfoLog(shader);
	}

	public void glGetShaderiv(int shader, int pname, int params[])
	{
		IntBuffer buf = IntBuffer.allocate(1);
		buf.put(params);
		buf.position(0);
		
		GLES20.glGetShaderiv(shader, pname, buf);
		
		buf.position(0);
		buf.get(params, 0, 1);
	}

	public int glGetUniformLocation(int program, String name)
	{
		return GLES20.glGetUniformLocation(program, name);
	}

	public void glLinkProgram(int program)
	{
		GLES20.glLinkProgram(program);
	}

	public void glRenderbufferStorage(int target, int internalformat, int width, int height)
	{
		GLES20.glRenderbufferStorage(target, internalformat, width, height);
	}

	public void glShaderSource(int shader, String str)
	{
		GLES20.glShaderSource(shader, str);
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
		if (data == null)
		{
			GLES20.glTexImage2D(target, level, internalformat, width, height, border, format, type, null);
		}
		else
		{
			IntBuffer buf = IntBuffer.allocate(height * width);
			buf.put(data);
			buf.position(0);

			GLES20.glTexImage2D(target, level, internalformat, width, height, border, format, type, buf);
		}
	}
	
	public void glTexImage2Db(int target,
							 int level,
							 int internalformat,
							 int width,
							 int height,
							 int border,
							 int format,
							 int type,
							 byte data[])
	{
		if (data == null)
		{
			GLES20.glTexImage2D(target, level, internalformat, width, height, border, format, type, null);
		}
		else
		{
			ByteBuffer buf = ByteBuffer.allocate(height * width * 4);
			buf.put(data);
			buf.position(0);

			GLES20.glTexImage2D(target, level, internalformat, width, height, border, format, type, buf);
		}
	}

	public void glTexParameterf(int target, int pname, float param)
	{
		GLES20.glTexParameterf(target, pname, param);
	}

	public void glTexParameteri(int target, int pname, int param)
	{
		GLES20.glTexParameteri(target, pname, param);
	}

	public void glUniform1f(int location, float v0)
	{
		GLES20.glUniform1f(location, v0);
	}
	
	public void glUniform1i(int location, int v0)
	{
		GLES20.glUniform1i(location, v0);
	}
	
	public void glUniform2f(int location, float v0, float v1)
	{
		GLES20.glUniform2f(location, v0, v1);
	}
	
	public void glUniform2fv(int location, int count, float data[])
	{
		GLES20.glUniform2fv(location, count, data, 0);
	}
	
	public void glUniform4f(int location, float v0, float v1, float v2, float v3)
	{
		GLES20.glUniform4f(location, v0, v1, v2, v3);
	}
	
	public void glUniform1fv(int location, int count, float value[])
	{
		FloatBuffer buf = FloatBuffer.allocate(count);
		buf.put(value);
		buf.position(0);

		GLES20.glUniform1fv(location, count, buf);
	}
	
	public void glUniform4fv(int location, int count, float value[])
	{
		FloatBuffer buf = FloatBuffer.allocate(count * 4);
		buf.put(value);
		buf.position(0);

		GLES20.glUniform4fv(location, count, buf);
	}

	public void glUniformMatrix4fv(int location, int count, boolean transpose, float value[])
	{
		FloatBuffer buf = FloatBuffer.allocate(16 * count);
		buf.put(value);
		buf.position(0);
		
		GLES20.glUniformMatrix4fv(location, count, transpose, buf);
	}

	public void glUseProgram(int program)
	{
		GLES20.glUseProgram(program);
	}

	public void glVertexAttribPointer( int index, int size, int type, boolean normalized, int stride, int pointe)
	{	
		GLES20.glVertexAttribPointer(index, size, type, normalized, stride, pointe);
	}
	
	public void glViewport( int x, int y, int width, int height)
	{	
		GLES20.glViewport(x, y, width, height);
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
