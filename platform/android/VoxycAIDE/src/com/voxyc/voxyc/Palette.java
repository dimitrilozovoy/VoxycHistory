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

/**
 *
 * @author dimitri
 */
public class Palette {
    
    final int MAX_COLORS = 256;
    
    public float[] mRed = null;
    public float[] mGreen = null;
    public float[] mBlue = null;
            
    Palette()
    {
        init();
    }
    
    public void init()
    {
        mRed = new float[MAX_COLORS];
        mGreen = new float[MAX_COLORS];
        mBlue = new float[MAX_COLORS];
        
        for (int i = 0; i < mRed.length; i++)
            mRed[i] = 0.0f;
        for (int i = 0; i < mGreen.length; i++)
            mGreen[i] = 0.0f;
        for (int i = 0; i < mBlue.length; i++)
            mBlue[i] = 0.0f;

        setDefaults();
		setColorWheel();
    }
    
    public void setDefaults()
    {
        mRed[0] = 0.0f;
        mGreen[0] = 0.0f;
        mBlue[0] = 0.0f;

        mRed[1] = 1.0f;
        mGreen[1] = 0.0f;
        mBlue[1] = 0.0f;

        mRed[2] = 0.0f;
        mGreen[2] = 1.0f;
        mBlue[2] = 0.0f;

        mRed[3] = 0.0f;
        mGreen[3] = 0.0f;
        mBlue[3] = 1.0f;

        mRed[4] = 1.0f;
        mGreen[4] = 1.0f;
        mBlue[4] = 0.0f;

        mRed[5] = 0.0f;
        mGreen[5] = 1.0f;
        mBlue[5] = 1.0f;

        mRed[6] = 1.0f;
        mGreen[6] = 0.0f;
        mBlue[6] = 1.0f;

        mRed[7] = 0.0f;
        mGreen[7] = 0.0f;
        mBlue[7] = 0.0f;

        mRed[8] = 1.0f;
        mGreen[8] = 1.0f;
        mBlue[8] = 1.0f;

        mRed[9] = 0.5f;
        mGreen[9] = 0.0f;
        mBlue[9] = 0.0f;

        mRed[10] = 0.0f;
        mGreen[10] = 0.5f;
        mBlue[10] = 0.0f;

        mRed[11] = 0.0f;
        mGreen[11] = 0.0f;
        mBlue[11] = 0.5f;

        mRed[12] = 0.5f;
        mGreen[12] = 0.5f;
        mBlue[12] = 0.0f;

        mRed[13] = 0.0f;
        mGreen[13] = 0.5f;
        mBlue[13] = 0.5f;

        mRed[14] = 0.5f;
        mGreen[14] = 0.0f;
        mBlue[14] = 0.5f;

        mRed[15] = 0.0f;
        mGreen[15] = 0.0f;
        mBlue[15] = 0.0f;
        
        setRGBRange(16, 0.0f, 0.0f, 0.0f, 32, 1.0f, 0.0f, 0.0f);
        setRGBRange(32, 0.0f, 0.0f, 0.0f, 48, 0.0f, 1.0f, 0.0f);
        setRGBRange(48, 0.0f, 0.0f, 0.0f, 64, 0.0f, 0.0f, 1.0f);
        setRGBRange(64, 0.0f, 0.0f, 0.0f, 80, 1.0f, 1.0f, 0.0f);
        setRGBRange(80, 0.0f, 0.0f, 0.0f, 96, 0.0f, 1.0f, 1.0f);
        setRGBRange(96, 0.0f, 0.0f, 0.0f, 112, 1.0f, 0.0f, 1.0f);
        setRGBRange(112, 0.0f, 0.0f, 0.0f, 128, 1.0f, 1.0f, 1.0f);

		// Sunset red = 214 100 88
		// Sunset sky = 224 168 148
		// Late morning sky = 224 195 175
		
		// Greenish yellow = 230 205 142
		// Light yellowgreen = 217 214 171
		// Brown = 150 101 84 ---> 117 72 47
		
		// Evening blue = 34 40 56 ---> 212 217 204
		
/*        setRGBRange(128, 1.0f, 0.0f, 0.0f, 144, 0.0f, 0.0f, 0.0f);
        setRGBRange(144, 0.0f, 1.0f, 0.0f, 160, 0.0f, 0.0f, 0.0f);
        setRGBRange(160, 0.0f, 0.0f, 1.0f, 176, 0.0f, 0.0f, 0.0f);
        setRGBRange(176, 0.0f, 0.0f, 0.0f, 192, 1.0f, 0.0f, 0.0f);
        setRGBRange(192, 0.0f, 0.0f, 0.0f, 208, 0.0f, 1.0f, 0.0f);
        setRGBRange(208, 0.0f, 0.0f, 0.0f, 224, 0.0f, 0.0f, 1.0f);
        setRGBRange(224, 0.0f, 0.0f, 0.0f, 240, 0.0f, 0.0f, 0.0f);*/
    }
    
    public void setRGBRange(int start, float startR, float startG, float startB,
            int end, float endR, float endG, float endB)
    {
        int gradations = end - start;

        float r = startR;
        float g = startG;
        float b = startB;
        float deltaR = (endR - startR) / gradations;
        float deltaG = (endG - startG) / gradations;
        float deltaB = (endB - startB) / gradations;

        for (int i = start; i < end; i++)
        {
            mRed[i] = r;
            mGreen[i] = g;
            mBlue[i] = b;
            
            r += deltaR;
            g += deltaG;
            b += deltaB;
        }
    }
	
	public void setColorWheel()
	{
		int interval = 16;
		
		// Red to yellow
		setRGBRange(1, 1.0f, 0.0f, 0.0f, interval, 1.0f, 1.0f, 0.0f);
		// Yellow to green
		setRGBRange(interval, 1.0f, 1.0f, 0.0f, interval * 2, 0.0f, 1.0f, 0.0f);
		// Green to cyan
		setRGBRange(interval * 2, 0.0f, 1.0f, 0.0f, interval * 3, 0.0f, 1.0f, 1.0f);
		// Cyan to blue
		setRGBRange(interval * 3, 0.0f, 1.0f, 1.0f, interval * 4, 0.0f, 0.0f, 1.0f);
		// Blue to magenta
		setRGBRange(interval * 4, 0.0f, 0.0f, 1.0f, interval * 5, 1.0f, 0.0f, 1.0f);
		// Magenta to red
		setRGBRange(interval * 5, 1.0f, 0.0f, 1.0f, interval * 6, 1.0f, 0.0f, 0.0f);
		// Red to black
		setRGBRange(interval * 6, 1.0f, 0.0f, 0.0f, interval * 7, 0.0f, 0.0f, 0.0f);
		// Black to white
		setRGBRange(interval * 7, 0.0f, 0.0f, 0.0f, interval * 8, 1.0f, 1.0f, 1.0f);
	}
}
