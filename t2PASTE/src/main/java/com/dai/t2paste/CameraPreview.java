package com.dai.t2paste;

import java.io.IOException;

import android.content.Context;
import android.content.res.Configuration;
import android.hardware.Camera;
import android.os.Handler;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback
{
    String TAG = "CAMERA";
    private SurfaceHolder mHolder;
    private Camera mCamera;

    public CameraPreview(Context context, Camera camera)
    {
        super(context);
        mCamera = camera;

        // SurfaceHolder 
        mHolder = getHolder();
        mHolder.addCallback(this);
        // deprecated �릺�뿀吏�留� 3.0 �씠�븯 踰꾩졏�뿉�꽌 �븘�닔 硫붿냼�뱶�씪�꽌 �샇異쒗빐�몺.
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder)
    {
        // Surface媛� �깮�꽦�릺�뿀�쑝�땲 �봽由щ럭瑜� �뼱�뵒�뿉 �쓣�슱吏� 吏��젙�빐以��떎. (holder 濡� 諛쏆� SurfaceHolder�뿉 肉뚮젮以��떎. 
        try
        {
            Camera.Parameters parameters = mCamera.getParameters();
            if (getResources().getConfiguration().orientation != Configuration.ORIENTATION_LANDSCAPE)
            {
                parameters.set("orientation", "portrait");
                mCamera.setDisplayOrientation(90);
                parameters.setRotation(90);
            }
            else
            {
                parameters.set("orientation", "landscape");
                mCamera.setDisplayOrientation(0);
                parameters.setRotation(0);
            }
            Log.e(MainActivity.TAG, "focus mode : " + parameters.getFocusMode());

		

			/*
			int m_resWidth;
			int m_resHeight;
			m_resWidth = mCamera.getParameters().getPictureSize().width;
			m_resHeight = mCamera.getParameters().getPictureSize().height;			
			//Camera.Parameters parameters = mCamera.getParameters();
			//아래 숫자를 변경하여 자신이 원하는 해상도로 변경한다
			m_resWidth = 2560;
			m_resHeight = 1440;
			parameters.setPictureSize(m_resWidth, m_resHeight);
			////mCamera.setParameters(parameters);
			*/
            mCamera.setParameters(parameters);
            mCamera.setPreviewDisplay(holder);
            mCamera.startPreview();
        }
        catch (IOException e)
        {
            Log.d(MainActivity.TAG, "Error setting camera preview: " + e.getMessage());
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder)
    {
        if (mCamera != null)
        {
            mCamera.stopPreview();
            mCamera.release();
            mCamera = null;
        }
    }

    private Camera.Size getBestPreviewSize(int width, int height)
    {

        int tmp = 0;
        if (getResources().getConfiguration().orientation != Configuration.ORIENTATION_LANDSCAPE)
        {

            tmp = width;
            width = height;
            height = width;
        }

        Camera.Size result = null;
        Camera.Parameters p = mCamera.getParameters();
        for (Camera.Size size : p.getSupportedPreviewSizes())
        {
            if (size.width <= width && size.height <= height)
            {
                if (result == null)
                {
                    result = size;
                }
                else
                {
                    int resultArea = result.width * result.height;
                    int newArea = size.width * size.height;

                    if (newArea > resultArea)
                    {
                        result = size;
                    }
                }
            }
        }
        return result;
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h)
    {

        if (mHolder.getSurface() == null)
        {
            return;
        }

        try
        {
            mCamera.stopPreview();
        }
        catch (Exception e)
        {

        }

        Camera.Parameters parameters = mCamera.getParameters();
        Camera.Size size = getBestPreviewSize(w, h);
        ////parameters.setPreviewSize(1920, 1080);
        parameters.setPreviewSize(size.width, size.height);
        mCamera.setParameters(parameters);

        try
        {
            mCamera.setPreviewDisplay(mHolder);
            mCamera.startPreview();

        }
        catch (Exception e)
        {
            Log.d(MainActivity.TAG, "Error starting camera preview: " + e.getMessage());
        }
    }

}
