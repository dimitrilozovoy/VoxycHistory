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

import android.opengl.*;
import android.opengl.GLSurfaceView.*;
import android.opengl.GLSurfaceView.Renderer.*;

import android.os.Bundle;
import android.os.Looper;
import android.os.Message;
import android.util.*;
import android.content.*;
import android.graphics.Point;
import android.view.*;
import android.os.Handler;

/**
 *
 * @author dimitri
 */
public class CustomSurfaceView extends GLSurfaceView {

/*    pGL3DSurfaceView m_pxLastZoomX1 = 0;
    private int m_pxLastZoomY1 = 0;
    private int m_pxLastZoomX2 = 0;
    private int m_pxLastZoomY2 = 0;
    private int m_pxLastDeltaZoomX = 0;
    private int m_pxLastDeltaZoomY = 0;
    private double m_lastZoomH = 0.0;
    private int m_zoomLevel;
    private int m_zoomDelay = 4;
    private int m_pxLocTouchRadius = 20;
    RespondToNode m_onPan = null;
    private int m_afterZoomPanSkipTimer = 0;
    private int mPxLastDownX = -1;
    private int mPxLastDownY = -1;
    private int mPxLastUpX = -1;
    private int mPxLastUpY = -1;
    private int mPxLastMoveX = -1;
    private int mPxLastMoveY = -1;
	GestureRecognizer gestureRecognizer = null;
	int afterGestureTimer = 0;*/

    public CustomSurfaceView(Context context) {
        super(context);

//		setEGLConfigChooser(8, 8, 8, 0, 16, 0);
//		setEGLContextClientVersion(3);
		
/*		if (Lozoware.getUI().getUseES20())
		{
		    setEGLContextClientVersion(2);
		}
		
		gestureRecognizer = new GestureRecognizer();*/
    }

    @Override
    public synchronized boolean onTouchEvent(MotionEvent ev) {
		
	int count = ev.getPointerCount();
	int action1 = 0;
	float x1 = 0;
	float y1 = 0;
	int action2 = 0;
	float x2 = 0;
	float y2 = 0;
	
//	if (count >= 1)
//	{
		switch (ev.getAction())
		{
			case MotionEvent.ACTION_DOWN:
				action1 = 1;
				break;
			case MotionEvent.ACTION_MOVE:
				action1 = 2;
				break;
			case MotionEvent.ACTION_UP:
				action1 = 3;
				break;
		}
		
		x1 = ev.getX(0);
		y1 = ev.getY(0);
//	}
	
	if (count >= 2)
	{
		switch (ev.getAction())
		{
			case MotionEvent.ACTION_DOWN:
				action2 = 1;
				break;
			case MotionEvent.ACTION_MOVE:
				action2 = 2;
				break;
			case MotionEvent.ACTION_UP:
				action2 = 3;
				break;
		}

		x2 = ev.getX(1);
		y2 = ev.getY(1);
	}
	
	HelloJni.touchEvent(ev.getPointerCount(), action1, x1, y1, action2, x2, y2);
		
/*	for (int i = 0; i < ev.getPointerCount(); i++)
	{
		 int a = 0;

		 switch (ev.getAction())
		 {
		 case MotionEvent.ACTION_DOWN:
		 a = 0;
		 break;
		 case MotionEvent.ACTION_MOVE:
		 a = 1;
		 break;
		 case MotionEvent.ACTION_UP:
		 a = 2;
		 break;
		 }

//		 HelloJni.touchEvent(a, ev.getX(i), ev.getY(i), i, ev.getPointerCount());
    }*/
		
/*		for (int i = 0; i < ev.getPointerCount(); i++)
		{
		    int a = 0;
		
		    switch (ev.getAction())
		    {
			    case MotionEvent.ACTION_DOWN:
				    a = 0;
				    break;
			    case MotionEvent.ACTION_MOVE:
				    a = 1;
				    break;
			    case MotionEvent.ACTION_UP:
				    a = 2;
				    break;
		    }

		    HelloJni.touchEvent(a, ev.getX(i), ev.getY(i), i, ev.getPointerCount());
		}*/
		
/*        EngineProcess mp = (EngineProcess) Lozoware.getMP();
        LozowareAndroidUI ui = (LozowareAndroidUI) Lozoware.getUI();
        GL3DAndroid gl3d = (GL3DAndroid) ui.getGL3D();
        EngineState state = Lozoware.state;

        if (state == null || state.camera == null) {
            return true;
        }
		
        // ALTERNATE MODE

        switch (gl3d.getMode()) {
            case TILEPICKER:
                GL3DTilePickerAndroid tp = (GL3DTilePickerAndroid) gl3d.getTilePicker();
                tp.touchEvent(ev);
                return true;
            case OFFSETEDIT:
                GL3DOffsetEditAndroid oe = (GL3DOffsetEditAndroid) gl3d.getOffsetEdit();
                oe.touchEvent(ev);
                return true;
        }

        // NORMAL MODE

        final int pointerCount = ev.getPointerCount();
		
		// GESTURES
		
		boolean gestureDetected = false;

		if (pointerCount == 2)
		{
		    gestureDetected = gestureRecognizer.processEvent(ev);
			afterGestureTimer = 10;
			return true;
		}
		
		if (afterGestureTimer > 0)
		{
			afterGestureTimer--;
			return true;
		}

        // Process taps regardless of camera mode

        if (state != null) {
            if (ev.getAction() == MotionEvent.ACTION_DOWN) {
                if (pointerCount == 1) {
                    
                    mPxLastDownX = (int) ev.getX();
                    mPxLastDownY = (int) ev.getY();
                    
                    if (ui.getOnTouchDown() != null) {
                        ui.getOnTouchDown().m_input = new Node();
                        ui.getOnTouchDown().m_input.set("screenx", mPxLastDownX);
                        ui.getOnTouchDown().m_input.set("screeny", mPxLastDownY);
                        ui.getOnTouchDown().respondToNode();
                    }
                }
            }
            if (ev.getAction() == MotionEvent.ACTION_MOVE) {
                if (pointerCount == 1) {
                    
                    mPxLastMoveX = (int) ev.getX();
                    mPxLastMoveY = (int) ev.getY();
                    
                    if (ui.getOnTouchMove() != null) {
                        ui.getOnTouchMove().m_input = new Node();
                        ui.getOnTouchMove().m_input.set("screenx", mPxLastMoveX);
                        ui.getOnTouchMove().m_input.set("screeny", mPxLastMoveY);
                        ui.getOnTouchMove().respondToNode();
                    }
                }
            }
        }

        // Process taps specific to camera modes

        if (state != null && (state.getCameraMode() == CameraMode.FREEFLOATFPV || state.getCameraMode() == CameraMode.STATIC)) {

            if (ev.getAction() == MotionEvent.ACTION_DOWN) {
                if (pointerCount == 1) {
                    mPxLastDownX = (int) ev.getX();
                    mPxLastDownY = (int) ev.getY();
                }
            }
            if ((ev.getAction() == MotionEvent.ACTION_MOVE) && (state.getCameraMode() != CameraMode.STATIC)) {
                if (pointerCount == 1) {
                    if (m_afterZoomPanSkipTimer > 0) {
                        m_afterZoomPanSkipTimer--;
                    } else {
                        if (mLastPanX == 0) {
                            mLastPanX = (int) ev.getX();
                        }
                        if (mLastPanY == 0) {
                            mLastPanY = (int) ev.getY();
                        }

                        int panX = (int) ev.getX();
                        int panY = (int) ev.getY();

                        int deltaPanX = panX - mLastPanX;
                        int deltaPanY = panY - mLastPanY;

                        state.camera.mYaw += -deltaPanX / 8;
                        state.camera.mPitch += deltaPanY / 8;

                        if (state.camera.mYaw < 0) {
                            state.camera.mYaw += 360;
                        }
                        if (state.camera.mYaw > 360) {
                            state.camera.mYaw -= 360;
                        }
                        if (state.camera.mPitch < 0) {
                            state.camera.mPitch += 360;
                        }
                        if (state.camera.mPitch > 360) {
                            state.camera.mPitch -= 360;
                        }

                        mLastPanX = panX;
                        mLastPanY = panY;
                    }
                } else if (pointerCount > 1) {
                                        // Zooming

                     int pxNewZoomX1 = (int) ev.getX(0);
                     int pxNewZoomY1 = (int) ev.getY(0);
                     int pxNewZoomX2 = (int) ev.getX(1);
                     int pxNewZoomY2 = (int) ev.getY(1);

                     int deltaZoomX = pxNewZoomX1 - m_pxLastZoomX2;
                     int deltaZoomY = pxNewZoomY1 - m_pxLastZoomY2;

                     double zoomH = 0;

                     zoomH = Math.sqrt((deltaZoomX * deltaZoomX) + (deltaZoomY * deltaZoomY));

                     double deltaZoomH = zoomH - m_lastZoomH;

                     ui.getScreenDiagonalSize();

                     m_zoomDelay++;

                     double targetZoom = ma.getMapZoom() + deltaZoomH / 10;
                     if (targetZoom < 10) {
                     targetZoom = 10;
                     }
                     if (targetZoom > 130) {
                     targetZoom = 130;
                     }

                     double zoom = ma.getMapZoom();

                     double zoomDelta = 2.0f;
                     if (targetZoom > 100) {
                     zoomDelta = 1.0f;
                     }
                     if (targetZoom > 115) {
                     zoomDelta = 0.6f;
                     }

                     if (deltaZoomX > 0) {
                     Node3D camera = Lozoware.state.camera;
                     camera.setDeltaXY(camera.mYaw, deltaZoomX / 100);
                     camera.move();
                     } else {
                     Node3D camera = Lozoware.state.camera;
                     camera.setDeltaXY(camera.mYaw - 180, deltaZoomX / 100);
                     camera.move();
                     }

                     m_afterZoomPanSkipTimer = 10;

                     // Ready for next loop

                     m_lastZoomH = zoomH;

                     m_pxLastDeltaZoomX = deltaZoomX;
                     m_pxLastDeltaZoomY = deltaZoomY;

                     m_pxLastZoomX1 = pxNewZoomX1;
                     m_pxLastZoomY1 = pxNewZoomY1;
                     m_pxLastZoomX2 = pxNewZoomX2;
                     m_pxLastZoomY2 = pxNewZoomY2;
                }
            }
            if (ev.getAction() == MotionEvent.ACTION_UP) {
                if (pointerCount == 1) {
                    mLastPanX = 0;
                    mLastPanY = 0;

                    mPxLastUpX = (int) ev.getX();
                    mPxLastUpY = (int) ev.getY();

                    int tapThresh = 30;

                    if ((Math.abs(mPxLastUpX - mPxLastDownX) < tapThresh)
                            && (Math.abs(mPxLastUpY - mPxLastDownY) < tapThresh)) {

                        boolean entityTap = false;

                        if (!entityTap && ui.getOnTap() != null) {
                            ui.getOnTap().m_input = new Node();
                            ui.getOnTap().m_input.set("screenx", mPxLastUpX);
                            ui.getOnTap().m_input.set("screeny", mPxLastUpY);
                            ui.getOnTap().respondToNode();
                        }
                    }
                }
            }
        } else if (state != null && state.getCameraMode() == CameraMode.FREESCROLLFPV) {

            if (ev.getAction() == MotionEvent.ACTION_DOWN) {
                if (pointerCount == 1) {
                    mPxLastDownX = (int) ev.getX();
                    mPxLastDownY = (int) ev.getY();
                }
            }
            if (ev.getAction() == MotionEvent.ACTION_MOVE) {
                if (pointerCount == 1) {
                    if (m_afterZoomPanSkipTimer > 0) {
                        m_afterZoomPanSkipTimer--;
                    } else {
                        if (mLastPanX == 0) {
                            mLastPanX = (int) ev.getX();
                        }
                        if (mLastPanY == 0) {
                            mLastPanY = (int) ev.getY();
                        }

                        int panX = (int) ev.getX();
                        int panY = (int) ev.getY();

                        int deltaPanX = panX - mLastPanX;
                        int deltaPanY = panY - mLastPanY;

//                        if (deltaPanX < 20 && deltaPanY < 20)
//                        {
                        state.camera.mYaw += -deltaPanX / 8;
                        state.camera.mPitch += deltaPanY / 8;
//                        }

                        if (state.camera.mYaw < 0) {
                            state.camera.mYaw += 360;
                        }
                        if (state.camera.mYaw > 360) {
                            state.camera.mYaw -= 360;
                        }
                        if (state.camera.mPitch < 0) {
                            state.camera.mPitch += 360;
                        }
                        if (state.camera.mPitch > 360) {
                            state.camera.mPitch -= 360;
                        }

                        mLastPanX = panX;
                        mLastPanY = panY;
                    }
                } else if (pointerCount > 1) {
                    // Zooming

                    int pxNewZoomX1 = (int) ev.getX(0);
                    int pxNewZoomY1 = (int) ev.getY(0);
                    int pxNewZoomX2 = (int) ev.getX(1);
                    int pxNewZoomY2 = (int) ev.getY(1);

                    int deltaZoomX = pxNewZoomX1 - m_pxLastZoomX2;
                    int deltaZoomY = pxNewZoomY1 - m_pxLastZoomY2;

                    double zoomH = 0;

                    zoomH = Math.sqrt((deltaZoomX * deltaZoomX) + (deltaZoomY * deltaZoomY));

                    double deltaZoomH = zoomH - m_lastZoomH;

                    ui.getScreenDiagonalSize();

                    m_zoomDelay++;

                                        double targetZoom = ma.getMapZoom() + deltaZoomH / 10;
                     if (targetZoom < 10) {
                     targetZoom = 10;
                     }
                     if (targetZoom > 130) {
                     targetZoom = 130;
                     }

                     double zoom = ma.getMapZoom();

                     double zoomDelta = 2.0f;
                     if (targetZoom > 100) {
                     zoomDelta = 1.0f;
                     }
                     if (targetZoom > 115) {
                     zoomDelta = 0.6f;
                     }

                                        if (zoom > targetZoom) {
                     ma.setMapZoom(ma.getMapZoom() - zoomDelta);
                     }
                     if (zoom < targetZoom) {
                     ma.setMapZoom(ma.getMapZoom() + zoomDelta);
                     }

                    if (deltaZoomX > 0) {
                        Node3D camera = state.camera;
                        camera.setDeltaXY(camera.mYaw, deltaZoomX / 100);
                        camera.move();
                    } else {
                        Node3D camera = state.camera;
                        camera.setDeltaXY(camera.mYaw - 180, deltaZoomX / 100);
                        camera.move();
                    }

                    m_afterZoomPanSkipTimer = 10;

                    // Ready for next loop

                    m_lastZoomH = zoomH;

                    m_pxLastDeltaZoomX = deltaZoomX;
                    m_pxLastDeltaZoomY = deltaZoomY;

                    m_pxLastZoomX1 = pxNewZoomX1;
                    m_pxLastZoomY1 = pxNewZoomY1;
                    m_pxLastZoomX2 = pxNewZoomX2;
                    m_pxLastZoomY2 = pxNewZoomY2;
                }
            }
            if (ev.getAction() == MotionEvent.ACTION_UP) {
                if (pointerCount == 1) {
                    mLastPanX = 0;
                    mLastPanY = 0;

                    mPxLastUpX = (int) ev.getX();
                    mPxLastUpY = (int) ev.getY();

                    int tapThresh = 30;

                    if ((Math.abs(mPxLastUpX - mPxLastDownX) < tapThresh)
                            && (Math.abs(mPxLastUpY - mPxLastDownY) < tapThresh)) {

                        boolean entityTap = false;

                        if (!entityTap && ui.getOnTap() != null) {
                            ui.getOnTap().m_input = new Node();
                            ui.getOnTap().m_input.set("screenx", mPxLastUpX);
                            ui.getOnTap().m_input.set("screeny", mPxLastUpY);
                            ui.getOnTap().respondToNode();
                        }
                    }
                }
            }
        } else if (state != null && state.getCameraMode() == CameraMode.PLAYERFPV) {

            if (gl3d.getFlightControlsEnabled()) {
                float x = ev.getX();
                float y = ev.getY();

                float relX = x / ui.getScreenWidth();
                float relY = y / ui.getContentHeight();

                FlightControls fc = ui.getFlightControls();

                if (fc.mMoveZoneBeginRelX <= relX && relX <= fc.mMoveZoneEndRelX
                        && fc.mMoveZoneBeginRelY <= relY && relY <= fc.mMoveZoneEndRelY) {
                    if (ev.getAction() == MotionEvent.ACTION_DOWN) {
                        fc.mForwardBackAxis = ((1 - (relY - fc.mMoveZoneBeginRelY) / (fc.mMoveZoneEndRelY - fc.mMoveZoneBeginRelY)) - 0.5f) * 2;
                        fc.mStrafeAxis = ((1 - (relX - fc.mMoveZoneBeginRelX) / (fc.mMoveZoneEndRelX - fc.mMoveZoneBeginRelX)) - 0.5f) * 2;
                    }
                    if (ev.getAction() == MotionEvent.ACTION_UP) {
                        fc.mForwardBackAxis = 0;
                        fc.mStrafeAxis = 0;
                    }
//                        fc.mUpDownAxis = ((1 - (relY - fc.mUpDownZoneBeginRelY) / (fc.mUpDownZoneEndRelY - fc.mUpDownZoneBeginRelY)) - 0.5f) * 2;
                }
            }
        }*/
        return true;
    }
}
