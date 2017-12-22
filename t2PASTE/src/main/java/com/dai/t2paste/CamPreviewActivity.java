package com.dai.t2paste;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.concurrent.Semaphore;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Configuration;
import android.graphics.Color;
import android.graphics.ImageFormat;
import android.graphics.Matrix;
import android.graphics.Point;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraCaptureSession;
import android.hardware.camera2.CameraCharacteristics;
import android.hardware.camera2.CameraDevice;
import android.hardware.camera2.CameraManager;
import android.hardware.camera2.CameraMetadata;
import android.hardware.camera2.CaptureRequest;
import android.hardware.camera2.CaptureResult;
import android.hardware.camera2.DngCreator;
import android.hardware.camera2.TotalCaptureResult;
import android.hardware.camera2.params.Face;
import android.hardware.camera2.params.MeteringRectangle;
import android.hardware.camera2.params.StreamConfigurationMap;
import android.media.Image;
import android.media.ImageReader;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.HandlerThread;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.util.Range;
import android.util.Size;
import android.util.SparseIntArray;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.Toast;


public class CamPreviewActivity extends Activity
{
    private TextureView mCameraTextureView;
    private Preview mPreview;

    public boolean CamMacro = true;
    public boolean CamManual = false;
    Activity mainActivity = this;

    int mCenterX = 0;
    int mCenterY = 0;

    private static final String TAG = "Preview7";

    static final int REQUEST_CAMERA = 1;

    private String mfileName;

    private HandlerThread mBackgroundThread = null;
    private Handler mBackgroundHandler;

    Integer mSensorOrientation;

    private Integer mAfState;
    private Float mFocusDistance;
    int mSaveNum = 0;
    boolean mPicSave = false;
    boolean mPicSave10 = true;
    float mFDistance10;
    float mFDistance;

    int shotCnt = 0;
    byte[] yuv420Data1;// = new byte[Yb + Ub + Vb];
    byte[] yuv420Data2;

    boolean BGResult = false;

    private static final SparseIntArray ORIENTATIONS = new SparseIntArray();

    static
    {
        ORIENTATIONS.append(Surface.ROTATION_0, 90);
        ORIENTATIONS.append(Surface.ROTATION_90, 0);
        ORIENTATIONS.append(Surface.ROTATION_180, 270);
        ORIENTATIONS.append(Surface.ROTATION_270, 180);
    }

    private int getOrientation(int rotation)
    {
        // Sensor orientation is 90 for most devices, or 270 for some devices (eg. Nexus 5X)
        // We have to take that into account and rotate JPEG properly.
        // For devices with orientation of 90, we simply return our mapping from ORIENTATIONS.
        // For devices with orientation of 270, we need to rotate the JPEG 180 degrees.
        return (ORIENTATIONS.get(rotation) + mSensorOrientation + 270) % 360;
    }

    public SimpleDateFormat TimeCheck()
    {
        long now = System.currentTimeMillis();
        // 현재시간을 date 변수에 저장한다.
        Date date = new Date(now);
        // 시간을 나타냇 포맷을 정한다 ( yyyy/MM/dd 같은 형태로 변형 가능 )
        SimpleDateFormat sdfNow = new SimpleDateFormat("yyyyMMdd_HHmmss");
        return sdfNow;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cam);

        mCameraTextureView = (TextureView) findViewById(R.id.texture);
        mPreview = new Preview(this, mCameraTextureView);
        findViewById(R.id.btn_picture).setOnClickListener(mClickListener);
        findViewById(R.id.btn_s).setOnClickListener(mClickListener);

        ////mCameraTextureView.setOnTouchListener(mTouchEvent);
        FileDirCheck();

        mCameraTextureView.setOnTouchListener(new View.OnTouchListener()
        {

            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, MotionEvent event)
            {
                switch (event.getAction() & MotionEvent.ACTION_MASK)
                {
                    case MotionEvent.ACTION_DOWN:
                        Log.e(MainActivity.TAG, "MotionEvent.ACTION_DOWN");
                        //stopBackgroundThread();
                        CameraManager manager = (CameraManager) getSystemService(Context.CAMERA_SERVICE);
                        try
                        {

                            //mPreviewBuilder.set(CaptureRequest.CONTROL_AF_MODE, CameraMetadata.CONTROL_AF_MODE_AUTO);
                            CameraCharacteristics characteristics = manager.getCameraCharacteristics(mPreview.mCameraDevice.getId());

                            Rect rect = characteristics.get(CameraCharacteristics.SENSOR_INFO_ACTIVE_ARRAY_SIZE);
                            Log.i("onAreaTouchEvent", "SENSOR_INFO_ACTIVE_ARRAY_SIZE,,,,,,,,rect.left--->" + rect.left + ",,,rect.top--->" + rect.top + ",,,,rect.right--->" + rect.right + ",,,,rect.bottom---->" + rect.bottom);
                            Size size = characteristics.get(CameraCharacteristics.SENSOR_INFO_PIXEL_ARRAY_SIZE);
                            Log.i("onAreaTouchEvent", "mCameraCharacteristics,,,,size.getWidth()--->" + size.getWidth() + ",,,size.getHeight()--->" + size.getHeight());
                            int areaSize = 200;
                            int right = rect.bottom;//rect.right;
                            int bottom = rect.right;//rect.bottom;
                            int viewWidth = mCameraTextureView.getWidth();
                            int viewHeight = mCameraTextureView.getHeight();
                            int ll, rr;
                            Rect newRect;
                            int centerX = (int) event.getY();//event.getX();
                            int centerY = (int) event.getX();//event.getY();
                            Log.e("Preview7 ", "event.getX():" + event.getX() + " event.getY():" + event.getY());
                            ll = ((centerX * right) - areaSize) / viewWidth;
                            rr = ((centerY * bottom) - areaSize) / viewHeight;
                            int focusLeft = mPreview.clamp(ll, 0, right);
                            int focusBottom = mPreview.clamp(rr, 0, bottom);
                            Log.i("focus_position", "focusLeft--->" + focusLeft + ",,,focusTop--->" + focusBottom + ",,,focusRight--->" + (focusLeft + areaSize) + ",,,focusBottom--->" + (focusBottom + areaSize));
                            newRect = new Rect(focusLeft, focusBottom, focusLeft + areaSize, focusBottom + areaSize);
                            MeteringRectangle meteringRectangle = new MeteringRectangle(newRect, 500);
                            MeteringRectangle[] meteringRectangleArr = {meteringRectangle};

                            CameraCaptureSession.CaptureCallback captureCallbackHandler = new CameraCaptureSession.CaptureCallback()
                            {
                                @Override
                                public void onCaptureCompleted(CameraCaptureSession session, CaptureRequest request, TotalCaptureResult result)
                                {
                                    super.onCaptureCompleted(session, request, result);
                                    //// mManualFocusEngaged = false;

                                    if (request.getTag() == "FOCUS_TAG")
                                    {
                                        Log.e(MainActivity.TAG, "FOCUS_TAG");
                                        //the focus trigger is complete -
                                        //resume repeating (preview surface will get frames), clear AF trigger
                                        //mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CameraMetadata.CONTROL_AF_TRIGGER_IDLE);
                                        //Toast.makeText(getApplicationContext(), "onCaptureCompleted OK", Toast.LENGTH_SHORT).show();

                                        try
                                        {
                                            mPreview.mPreviewSession.setRepeatingRequest(mPreview.mPreviewBuilder.build(), mCaptureCallback, mBackgroundHandler);
                                            //(mPreviewBuilder.build(), mCaptureCallback, mBackgroundHandler);
                                        }
                                        catch (CameraAccessException e)
                                        {

                                            e.printStackTrace();
                                        }
                                    }
                                }
                            };


                            //mPreview.mPreviewSession.stopRepeating();
                            mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
                            //mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AF_MODE, CameraMetadata.CONTROL_AF_MODE_AUTO);
                            mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CameraMetadata.CONTROL_AF_TRIGGER_CANCEL);
                            //mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AF_MODE, CameraMetadata.CONTROL_AF_MODE_OFF);
                            //mPreview.mPreviewBuilder.setTag("NO_TAG");
                            mPreview.mPreviewSession.capture(mPreview.mPreviewBuilder.build(), mCaptureCallback, mBackgroundHandler);

                            //mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CameraMetadata.CONTROL_AF_TRIGGER_CANCEL);

                            mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AE_REGIONS, meteringRectangleArr);
                            mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AF_REGIONS, meteringRectangleArr);

                            //mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
                            mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AF_MODE, CameraMetadata.CONTROL_AF_MODE_AUTO);
                            mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AF_TRIGGER, CameraMetadata.CONTROL_AF_TRIGGER_START);
                            mPreview.mPreviewBuilder.set(CaptureRequest.CONTROL_AE_PRECAPTURE_TRIGGER, CameraMetadata.CONTROL_AE_PRECAPTURE_TRIGGER_START);
                            mPreview.mPreviewBuilder.setTag("FOCUS_TAG");
                            ////startBackgroundThread();

                            try
                            {
                                mPreview.mPreviewSession.capture(mPreview.mPreviewBuilder.build(), captureCallbackHandler, mBackgroundHandler);
                            }
                            catch (CameraAccessException e)
                            {

                                e.printStackTrace();
                            }

                        }
                        catch (CameraAccessException e)
                        {
                            e.printStackTrace();
                        }

                        break;
                }
                return true;
            }
        });
    }

    Button.OnClickListener mClickListener = new View.OnClickListener()
    {
        public void onClick(View v)
        {
            BGResult = false;
            ;
            switch (v.getId())
            {
                case R.id.btn_picture: // Test code
                    if (mAfState == 4)
                    {

                        mPicSave = true;
                        long now = System.currentTimeMillis();
                        Date date = new Date(now);
                        SimpleDateFormat sdfNow = new SimpleDateFormat("yyyyMMdd_HHmmss");
                        mfileName = sdfNow.format(date);
                        mSaveNum = 0;
                        mFDistance10 = mFocusDistance / 9.0f;
                        mFDistance = mFocusDistance;
                        CamManual = true;
                        mPreview.takePicture(mFDistance, mfileName, mSaveNum);
                        Log.e(MainActivity.TAG, "save1 : " + mFDistance);
                        mPicSave10 = false;


                    }
                    else
                    {
                        new AlertDialog.Builder(CamPreviewActivity.this)
                                .setTitle("시작 불가")
                                .setMessage("초점을 맞추거나 렌즈가 이동중입니다..\n잠시 후 다시 진행하세요.")
                                .setNeutralButton("닫기", new DialogInterface.OnClickListener()
                                {
                                    public void onClick(DialogInterface dlg, int sumthin)
                                    {
                                    }
                                })
                                .show();
                    }

                    break;
                case R.id.btn_s: // 2 picture
                    if (mAfState == 4)
                    {
                        try
                        {
                            mPreview.mPreviewSession.stopRepeating();
                        }
                        catch (CameraAccessException e)
                        {
                            // TODO Auto-generated catch block
                            e.printStackTrace();
                        }
                        Log.e("Time Check", "take Picture button : " + TimeCheck());
                        mPicSave = true;
                        long now = System.currentTimeMillis();
                        Date date = new Date(now);
                        SimpleDateFormat sdfNow = new SimpleDateFormat("yyyyMMdd_HHmmss");
                        mfileName = sdfNow.format(date);
                        mSaveNum = 0;
                        mFDistance10 = mFocusDistance / 9.0f;
                        mFDistance = mFocusDistance;
                        CamManual = true;
                        mPreview.takePicture(mFDistance, mfileName, mSaveNum);
                        ////mSaveNum = 8;
                        Log.e(MainActivity.TAG, "save1 : " + mFDistance);
                        mPicSave10 = false;
                    }
                    else
                    {
                        new AlertDialog.Builder(CamPreviewActivity.this)
                                .setTitle("시작 불가")
                                .setMessage("초점을 맞추거나 렌즈가 이동중입니다..\n잠시 후 다시 진행하세요.")
                                .setNeutralButton("닫기", new DialogInterface.OnClickListener()
                                {
                                    public void onClick(DialogInterface dlg, int sumthin)
                                    {
                                    }
                                })
                                .show();
                    }
                    break;
            }
        }
    };

    public void FileDirCheck()
    {

        File file = new File(MainActivity.SaveDir);
        if (!file.exists())
        { // 원하는 경로에 폴더가 있는지 확인
            file.mkdirs();
            /*
        	new AlertDialog.Builder(CamPreviewActivity.this)
            .setTitle("없음")
            .setMessage("없어요.")
            .setNeutralButton("닫기", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dlg, int sumthin) {
                }
            })
            .show();
            */
        }/*
        else {
        	new AlertDialog.Builder(CamPreviewActivity.this)
            .setTitle("있음")
            .setMessage("있어요.")
            .setNeutralButton("닫기", new DialogInterface.OnClickListener() {
                public void onClick(DialogInterface dlg, int sumthin) {
                }
            })
            .show();
        }*/
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
    {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        switch (requestCode)
        {
            case REQUEST_CAMERA:
                for (int i = 0; i < permissions.length; i++)
                {
                    String permission = permissions[i];
                    int grantResult = grantResults[i];
                    if (permission.equals(Manifest.permission.CAMERA))
                    {
                        if (grantResult == PackageManager.PERMISSION_GRANTED)
                        {
                            mCameraTextureView = (TextureView) findViewById(R.id.texture);
                            mPreview = new Preview(mainActivity, mCameraTextureView);
                            Log.d(TAG, "mPreview set");
                        }
                        else
                        {
                            Toast.makeText(this, "Should have camera permission to run", Toast.LENGTH_LONG).show();
                            finish();
                        }
                    }
                }
                break;
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        mPreview.onResume();
        startBackgroundThread();
    }

    @Override
    protected void onPause()
    {
        super.onPause();
        mPreview.onPause();
        stopBackgroundThread();
    }


    public class Preview extends Thread
    {
        private final static String TAG = "Preview7 ";

        private Size mPreviewSize;
        private Context mContext;
        private CameraDevice mCameraDevice;
        private CaptureRequest.Builder mPreviewBuilder;
        private CameraCaptureSession mPreviewSession;
        private TextureView mTextureView;

        private static final int MAX_PREVIEW_WIDTH = 1920;
        private static final int MAX_PREVIEW_HEIGHT = 1080;

        public Preview(Context context, TextureView textureView)
        {
            mContext = context;
            mTextureView = textureView;
        }

        private String getBackFacingCameraId(CameraManager cManager)
        {
            try
            {
                for (final String cameraId : cManager.getCameraIdList())
                {
                    CameraCharacteristics characteristics = cManager.getCameraCharacteristics(cameraId);
                    int cOrientation = characteristics.get(CameraCharacteristics.LENS_FACING);
                    if (cOrientation == CameraCharacteristics.LENS_FACING_BACK) return cameraId;
                }
            }
            catch (CameraAccessException e)
            {
                e.printStackTrace();
            }
            return null;
        }

        public void openCamera(int width, int height)
        {
            CameraManager manager = (CameraManager) mContext.getSystemService(Context.CAMERA_SERVICE);
            Log.e(MainActivity.TAG, "openCamera E");
            try
            {
                String cameraId = getBackFacingCameraId(manager);
                CameraCharacteristics characteristics = manager.getCameraCharacteristics(cameraId);
                StreamConfigurationMap map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
                mPreviewSize = map.getOutputSizes(SurfaceTexture.class)[0];

                ImageReader mImageReader;
                mImageReader = getMaxSizeImageReader(map, ImageFormat.JPEG);
                float heightRatio = (float) mImageReader.getHeight() / height;
                float widthRatio = (float) mImageReader.getWidth() / width;
                float previewSizeW;
                float previewSizeH;
                if (heightRatio < widthRatio)
                {
                    previewSizeW = (float) mImageReader.getWidth() / widthRatio;
                    previewSizeH = (float) mImageReader.getHeight() / widthRatio;
                }
                else
                {
                    previewSizeW = (float) mImageReader.getWidth() / heightRatio;
                    previewSizeH = (float) mImageReader.getHeight() / heightRatio;
                }
                ////ImageView img = (ImageView) findViewById(R.id.imgView);
                LayoutParams params = (LayoutParams) mCameraTextureView.getLayoutParams();
                params.width = (int) previewSizeW;
                params.height = (int) previewSizeH;

                mCameraTextureView.setLayoutParams(params);

                Log.e(MainActivity.TAG, "previewSizeW" + previewSizeW + " previewSizeH" + previewSizeH);


                Log.e(MainActivity.TAG, "mPreviewSize.getHeight()" + mPreviewSize.getHeight() + " mPreviewSize.getWidth()" + mPreviewSize.getWidth());
                ////Log.e(MainActivity.TAG, "previewSize.getHeight()"+previewSize.getHeight()+" previewSize.getWidth()"+previewSize.getWidth());
                manager.openCamera(cameraId, mStateCallback, null);

            }
            catch (CameraAccessException e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            Log.e(MainActivity.TAG, "openCamera X");
        }

        public String getCameraId(CameraManager cameraManager, int facing) throws CameraAccessException
        {
            for (String cameraId : cameraManager.getCameraIdList())
            {
                CameraCharacteristics characteristics = cameraManager.getCameraCharacteristics(cameraId);
                if (characteristics.get(CameraCharacteristics.LENS_FACING) == facing)
                {
                    return cameraId;
                }
            }
            return null;
        }

        public ImageReader getMaxSizeImageReader(StreamConfigurationMap map, int imageFormat) throws CameraAccessException
        {
            Size[] sizes = map.getOutputSizes(imageFormat);
            Size maxSize = sizes[0];
            for (Size size : sizes)
            {
                if (size.getWidth() > maxSize.getWidth())
                {
                    maxSize = size;
                }
            }
            ImageReader imageReader = ImageReader.newInstance(
                    //maxSize.getWidth(), maxSize.getHeight(), // for landscape.
                    maxSize.getHeight(), maxSize.getWidth(), // for portrait.
                    imageFormat, /*maxImages*/1);
            return imageReader;
        }

        public Size getBestPreviewSize(StreamConfigurationMap map, ImageReader imageSize) throws CameraAccessException
        {
            //float imageAspect = (float) imageSize.getWidth() / imageSize.getHeight(); // for landscape.
            float imageAspect = (float) imageSize.getHeight() / imageSize.getWidth(); // for portrait
            float minDiff = 1000000000000F;
            Size[] previewSizes = map.getOutputSizes(SurfaceTexture.class);
            Size previewSize = previewSizes[0];
            for (Size size : previewSizes)
            {
                float previewAspect = (float) size.getWidth() / size.getHeight();
                float diff = Math.abs(imageAspect - previewAspect);
                if (diff < minDiff)
                {
                    previewSize = size;
                    minDiff = diff;
                }
                if (diff == 0.0F) break;
            }
            return previewSize;
        }


        void configureTransformKeepAspect(TextureView textureView, int previewWidth, int previewHeight)
        {
            int rotation = getWindowManager().getDefaultDisplay().getRotation();
            Matrix matrix = new Matrix();
            RectF viewRect = new RectF(0, 0, textureView.getWidth(), textureView.getHeight());
            RectF bufferRect = new RectF(0, 0, previewHeight, previewWidth);
            PointF center = new PointF(viewRect.centerX(), viewRect.centerY());

            if (Surface.ROTATION_90 == rotation || Surface.ROTATION_270 == rotation)
            {
                bufferRect.offset(center.x - bufferRect.centerX(), center.y - bufferRect.centerY());
                matrix.setRectToRect(viewRect, bufferRect, Matrix.ScaleToFit.FILL);

                float scale = Math.min(
                        (float) textureView.getWidth() / previewWidth,
                        (float) textureView.getHeight() / previewHeight);
                matrix.postScale(scale, scale, center.x, center.y);
                Log.e(MainActivity.TAG, "1scale:" + scale + " scale:" + scale + " center.x:" + center.x + " center.y:" + center.y);

                matrix.postRotate(90 * (rotation - 2), center.x, center.y);
            }
            else
            {
                bufferRect.offset(center.x - bufferRect.centerX(), center.y - bufferRect.centerY());
                matrix.setRectToRect(viewRect, bufferRect, Matrix.ScaleToFit.FILL);

                float scale = Math.min(
                        (float) textureView.getWidth() / previewHeight,
                        (float) textureView.getHeight() / previewWidth);
                matrix.postScale(scale, scale, center.x, center.y);

                matrix.postRotate(90 * rotation, center.x, center.y);

                Log.e(MainActivity.TAG, "2scale:" + scale + " scale:" + scale + " center.x:" + center.x + " center.y:" + center.y);
            }

            textureView.setTransform(matrix);
        }

        private TextureView.SurfaceTextureListener mSurfaceTextureListener = new TextureView.SurfaceTextureListener()
        {

            @Override
            public void onSurfaceTextureAvailable(SurfaceTexture surface,
                                                  int width, int height)
            {
                // TODO Auto-generated method stub
                Log.e(MainActivity.TAG, "onSurfaceTextureAvailable, width=" + width + ",height=" + height);
                openCamera(width, height);
            }

            @Override
            public void onSurfaceTextureSizeChanged(SurfaceTexture surface,
                                                    int width, int height)
            {
                // TODO Auto-generated method stub

                Log.e(MainActivity.TAG, "onSurfaceTextureSizeChanged");
            }

            @Override
            public boolean onSurfaceTextureDestroyed(SurfaceTexture surface)
            {
                // TODO Auto-generated method stub
                return false;
            }

            @Override
            public void onSurfaceTextureUpdated(SurfaceTexture surface)
            {
                // TODO Auto-generated method stub
            }
        };

        private CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback()
        {

            @Override
            public void onOpened(CameraDevice camera)
            {
                // TODO Auto-generated method stub
                Log.e(MainActivity.TAG, "onOpened");
                mCameraDevice = camera;
                configureTransformKeepAspect(mTextureView, mPreviewSize.getWidth(), mPreviewSize.getHeight());
                configureTransformKeepAspect(mCameraTextureView, mPreviewSize.getWidth(), mPreviewSize.getHeight());


                startPreview();
            }

            @Override
            public void onDisconnected(CameraDevice camera)
            {
                // TODO Auto-generated method stub
                Log.e(MainActivity.TAG, "onDisconnected");
            }

            @Override
            public void onError(CameraDevice camera, int error)
            {
                // TODO Auto-generated method stub
                Log.e(MainActivity.TAG, "onError");
            }

        };

        protected void startPreview()
        {
            // TODO Auto-generated method stub
            if (null == mCameraDevice || !mTextureView.isAvailable() || null == mPreviewSize)
            {
                Log.e(MainActivity.TAG, "startPreview fail, return");
            }

            SurfaceTexture texture = mTextureView.getSurfaceTexture();
            if (null == texture)
            {
                Log.e(MainActivity.TAG, "texture is null, return");
                return;
            }

            texture.setDefaultBufferSize(mPreviewSize.getWidth(), mPreviewSize.getHeight());

            Log.e(MainActivity.TAG, "2mPreviewSize.getHeight()" + mPreviewSize.getHeight() + " mPreviewSize.getWidth()" + mPreviewSize.getWidth());
            Surface surface = new Surface(texture);

            try
            {
                mPreviewBuilder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
            }
            catch (CameraAccessException e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
            mPreviewBuilder.addTarget(surface);

            try
            {
                mCameraDevice.createCaptureSession(Arrays.asList(surface), new CameraCaptureSession.StateCallback()
                {

                    @Override
                    public void onConfigured(CameraCaptureSession session)
                    {
                        // TODO Auto-generated method stub
                        mPreviewSession = session;
                        /*
                        try {
    						mPreviewSession.capture(mPreviewBuilder.build(), mCaptureCallback, mBackgroundHandler);
    					} catch (CameraAccessException e) {

    						e.printStackTrace();
    					}
                        */
                        updatePreview();
                    }

                    @Override
                    public void onConfigureFailed(CameraCaptureSession session)
                    {
                        // TODO Auto-generated method stub
                        Toast.makeText(mContext, "onConfigureFailed", Toast.LENGTH_LONG).show();
                    }
                }, null);
            }
            catch (CameraAccessException e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        protected void updatePreview()
        {
            // TODO Auto-generated method stub
            if (null == mCameraDevice)
            {
                Log.e(MainActivity.TAG, "updatePreview error, return");
            }

            mPreviewBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
            mPreviewBuilder.set(CaptureRequest.CONTROL_AE_MODE, CameraMetadata.CONTROL_AE_MODE_ON);
            if (CamManual)
            {
                mPreviewBuilder.set(CaptureRequest.CONTROL_AF_MODE, CameraMetadata.CONTROL_AF_MODE_OFF);
                ////mPreviewBuilder.set(CaptureRequest.LENS_FOCUS_DISTANCE, 10.0f);
                Log.e(MainActivity.TAG, "CamManual : true");
            }
            else
            {
                mPreviewBuilder.set(CaptureRequest.CONTROL_AF_MODE, CameraMetadata.CONTROL_AF_MODE_AUTO);
                mPreviewBuilder.set(CaptureRequest.CONTROL_AE_MODE, CameraMetadata.CONTROL_AE_MODE_ON);
                mPreviewBuilder.set(CaptureRequest.CONTROL_AWB_MODE, CameraMetadata.CONTROL_AWB_MODE_AUTO);
                Log.e(MainActivity.TAG, "CamManual : false");
            }
            Log.e(MainActivity.TAG, "CamMacro1");

            try
            {
                mPreviewSession.setRepeatingRequest(mPreviewBuilder.build(), mCaptureCallback, mBackgroundHandler);
                ////mPreviewSession.setRepeatingRequest(mPreviewBuilder.build(), null, backgroundHandler);
            }
            catch (CameraAccessException e)
            {
                // TODO Auto-generated catch block
                e.printStackTrace();
            }
        }

        public void setSurfaceTextureListener()
        {
            mTextureView.setSurfaceTextureListener(mSurfaceTextureListener);
        }

        public void onResume()
        {
            Log.d(TAG, "onResume");
            setSurfaceTextureListener();
        }

        private Semaphore mCameraOpenCloseLock = new Semaphore(1);

        public void onPause()
        {
            // TODO Auto-generated method stub
            Log.d(TAG, "onPause");
            try
            {
                mCameraOpenCloseLock.acquire();
                if (null != mCameraDevice)
                {
                    mCameraDevice.close();
                    mCameraDevice = null;
                    Log.d(TAG, "CameraDevice Close");
                }
            }
            catch (InterruptedException e)
            {
                throw new RuntimeException("Interrupted while trying to lock camera closing.");
            }
            finally
            {
                mCameraOpenCloseLock.release();
            }
        }

        //Clamp the inputs.
        private int clamp(int x, int min, int max)
        {
            if (x < min)
            {
                return min;
            }
            else if (x > max)
            {
                return max;
            }
            else
            {
                return x;
            }
        }

        CaptureResult mCaptureResult = null;

        ////protected void takePicture(int l, int r, int t, int b, Float FocusDistance, String NP) {
        protected void takePicture(Float FocusDistance, final String mfileName, int N)
        {
            Log.e(MainActivity.TAG, "takePicture");
            Log.e("Time Check", "takePicture() Start : " + TimeCheck());
            shotCnt = 0;

            if (null == mCameraDevice)
            {
                Log.e(MainActivity.TAG, "mCameraDevice is null, return");
                return;
            }

            CameraManager manager = (CameraManager) getSystemService(Context.CAMERA_SERVICE);
            try
            {
                ////final float mFocusDistance = FocusDistance;
                String cameraId = getBackFacingCameraId(manager);
                final CameraCharacteristics characteristics = manager.getCameraCharacteristics(cameraId);
                //characteristics.get(CameraCharacteristics.SENSOR_ORIENTATION);

                Range range1 = characteristics.get(CameraCharacteristics.SENSOR_INFO_SENSITIVITY_RANGE);
                int minmin = (Integer) range1.getLower();
                int maxmax = (Integer) range1.getUpper();
                Log.e(MainActivity.TAG, "minmin:" + minmin + " maxmax:" + maxmax + "cameraId:" + cameraId);
                /*
    			Size[] jpegSizes = null;
    			if (characteristics != null) {
    	            jpegSizes = characteristics
    	                    .get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP)
    	                    .getOutputSizes(ImageFormat.JPEG); ////
    	        }*/
                int width = 640;
                int height = 480;

                Size[] jpegSizes = null;
                StreamConfigurationMap map = characteristics.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
                if (map != null)
                {
                    jpegSizes = map.getOutputSizes(ImageFormat.JPEG);
                }

                if (jpegSizes != null && 0 < jpegSizes.length)
                {
                    width = jpegSizes[0].getWidth();
                    height = jpegSizes[0].getHeight();
                }
                width = 1920;
                height = 1080;

                //test code : 170714
                for (int test = 0; test < jpegSizes.length; test++)
                {
                    Log.e("testCode", "jpegSizes[0].getWidth():" + jpegSizes[test].getWidth() + " jpegSizes[0].getHeight():" + jpegSizes[test].getHeight());
                }

                Log.e(MainActivity.TAG, "width1:" + width + " height1:" + height);

                final ImageReader reader = ImageReader.newInstance(width, height, ImageFormat.JPEG, /*maxImages*/3);//ImageFormat.YUV_420_888, 3);

                List<Surface> outputSurfaces = new ArrayList<Surface>(2);
                outputSurfaces.add(reader.getSurface());
                ////outputSurfaces.add(new Surface(mTextureView.getSurfaceTexture())); //GN170404

                final CaptureRequest.Builder captureBuilder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE);//TEMPLATE_PREVIEW);

                captureBuilder.addTarget(reader.getSurface());

                captureBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
                captureBuilder.set(CaptureRequest.CONTROL_AE_MODE, CameraMetadata.CONTROL_AE_MODE_ON);
                captureBuilder.set(CaptureRequest.CONTROL_AF_MODE, CameraMetadata.CONTROL_AF_MODE_OFF);
                captureBuilder.set(CaptureRequest.LENS_FOCUS_DISTANCE, FocusDistance);


                ////mPreviewSession.stopRepeating();
                Log.e(MainActivity.TAG, "PT.(captureBuilder.SENSOR_SENSITIVITY): " + captureBuilder.get(CaptureRequest.SENSOR_SENSITIVITY));
                // Orientation
                mSensorOrientation = characteristics.get(CameraCharacteristics.SENSOR_ORIENTATION);
                Log.e(MainActivity.TAG, "TEST-1");
                int rotation = getWindowManager().getDefaultDisplay().getRotation();
                Log.e(MainActivity.TAG, "TEST-2");
                captureBuilder.set(CaptureRequest.JPEG_ORIENTATION, getOrientation(rotation));
                Log.e(MainActivity.TAG, "TEST-3:" + getOrientation(rotation));
                captureBuilder.set(CaptureRequest.JPEG_QUALITY, (byte) 100);

                captureBuilder.set(CaptureRequest.JPEG_THUMBNAIL_QUALITY, (byte) 100);//////Test 170711

                Log.e(MainActivity.TAG, "TEST-4");
                Log.e("Time Check", "Camera setting : " + TimeCheck());

                final File file, file2, fileN, fileF;

                if (N == 0)
                    file = new File(MainActivity.SaveDir, "PASTE_" + mfileName + "_N.jpg");
                else if (N == 9)
                    file = new File(MainActivity.SaveDir, "PASTE_" + mfileName + "_F.jpg");
                else
                    file = new File(MainActivity.SaveDir, "PASTE_" + mfileName + "_N" + N + "_F" + FocusDistance + ".jpg");

                file2 = new File(MainActivity.SaveDir, "PASTE_" + mfileName + "_F.jpg");

                fileN = new File(MainActivity.SaveDir, "PASTE_" + mfileName + "_bN.jpg");
                fileF = new File(MainActivity.SaveDir, "PASTE_" + mfileName + "_bF.jpg");

                Log.e(MainActivity.TAG, "TEST-5");

                final ImageReader.OnImageAvailableListener readerListener = new ImageReader.OnImageAvailableListener()
                {

                    @Override
                    public void onImageAvailable(ImageReader reader)
                    {
                        Image image = null;
                        Log.e(MainActivity.TAG, "TEST-66");
                        switch (ImageFormat.JPEG)
                        {
                            case ImageFormat.JPEG:
                                //case ImageFormat.FLEX_RGB_888:
                                try
                                {

                                    image = reader.acquireLatestImage();
                                    //image = reader.acquireLatestImage();
                                    //ByteBuffer buffer = image.getPlanes()[0].getBuffer();
                                    //byte[] bytes = new byte[buffer.capacity()];
                                    //buffer.get(bytes);
                                    Log.e(MainActivity.TAG, "TEST-6");

                                    Log.e(MainActivity.TAG, "shotCnt : " + shotCnt);
                                    if (shotCnt == 0)
                                    {

                                        ByteBuffer buffer = image.getPlanes()[0].getBuffer();

                                        // jaehyek 다음을 comment-out
//        	                        byte[] bytes = new byte[buffer.capacity()];
//        	                        buffer.get(bytes);
//    	                        	yuv420Data1 = bytes.clone();

                                        //  대신 아래 2줄을 사용한다.
                                        yuv420Data1 = new byte[buffer.capacity()];
                                        buffer.get(yuv420Data1);
                                    }
                                    else if (shotCnt == 1)
                                    { // Test20171020 : else if(shotCnt == 1){

                                        ByteBuffer buffer = image.getPlanes()[0].getBuffer();

                                        // jaehyek 다음을 comment-out
//        	                        byte[] bytes = new byte[buffer.capacity()];
//        	                        buffer.get(bytes);
//    	                        	yuv420Data2 = bytes.clone();

                                        //  대신 아래 2줄을 사용한다.
                                        yuv420Data2 = new byte[buffer.capacity()];
                                        buffer.get(yuv420Data2);

                                        Log.e(MainActivity.TAG, "shotCnt2 : " + shotCnt);
                                        // jaehyek
                                        DAIPA daipa;
                                        daipa = new DAIPA();
                                        Log.i("TEST", daipa.TEST(MainActivity.SaveDir + mfileName, 0, yuv420Data1, yuv420Data2));
    		            			
    		            			/*
    		            			OutputStream output1 = null;
    		                        try {
    		                        	output1 = new FileOutputStream( fileN );
    		                        	output1.write(yuv420Data1);
    		                        }
    		                        finally {
    		                        	if (null != output1) {
    		                        		output1.close();    		                        		
    		                        	}
    		                        }
    		                        
    		                        OutputStream output2 = null;
    		                        try {    		                        	
    		                        	output2 = new FileOutputStream( fileF );    		                            
    		                        	output2.write(yuv420Data2);
    		                        }
    		                        finally {
    		                        	if (null != output2) {
    		                        		output2.close();    		                        		
    		                        	}
    		                        }
    		                        */
                                        BGResult = true;
                                    }
                                    shotCnt++;
                                    byte[] bytes = null;
                                    save(bytes);
                                    Log.e(MainActivity.TAG, "TEST-61");
                                }
                                catch (FileNotFoundException e)
                                {
                                    e.printStackTrace();
                                }
                                catch (IOException e)
                                {
                                    e.printStackTrace();
                                }
                                finally
                                {
                                    if (image != null)
                                    {
                                        image.close();
                                    }
                                }
                                break;

                            case ImageFormat.YUV_420_888:
                                //Image image = null;
                                try
                                {
                                    image = reader.acquireNextImage();
                                    if (image == null)
                                        return;

                                    image.close();
                                }
                                catch (Exception e)
                                {
                                    if (image != null)
                                        image.close();
                                }
                                break;

                            case ImageFormat.RAW_SENSOR:
                                DngCreator dngCreator = new DngCreator(characteristics, mCaptureResult);
                                FileOutputStream output = null;
                                try
                                {
                                    image = reader.acquireLatestImage();
                                    Log.e("RAW_SENSOR", "RAW_SENSOR 1");
                                    output = new FileOutputStream(file);
                                    //dngCreator.setThumbnail(pixels)
                                    dngCreator.writeImage(output, image);
                                    //success = true;
                                }
                                catch (IOException e)
                                {
                                    e.printStackTrace();
                                }
                                finally
                                {
                                    image.close();
                                    try
                                    {
                                        output.close();
                                    }
                                    catch (IOException e)
                                    {
                                        // TODO Auto-generated catch block
                                        e.printStackTrace();
                                    }
                                }
                                break;
                        }


                    }

                    private void save(byte[] bytes) throws IOException
                    {
    					/*
                        OutputStream output = null;
                        try {
                        	Log.e(MainActivity.TAG, "TEST-7 mSaveNum:"+mSaveNum);
                        	if(mSaveNum == 0) {                        	
                        		output = new FileOutputStream( file );
                                output.write(bytes);
                                mSaveNum = 9;
                        	}
                        	else {
                        		output = new FileOutputStream(file2) ;
                                output.write(bytes);
                        	}
                            ////startPreview();
                            mPicSave = false;
                            
                        } finally {
                            if (null != output) {
                                output.close();
                            }
                        }*/
                    }

                };

                HandlerThread thread1 = new HandlerThread("CameraPicture");
                thread1.start();
                final Handler backgroudHandler1 = new Handler(thread1.getLooper());
                // reader
                reader.setOnImageAvailableListener(readerListener, backgroudHandler1);

                final CameraCaptureSession.CaptureCallback captureListener = new CameraCaptureSession.CaptureCallback()
                {

                    public void process(CaptureResult result)
                    {

                        Log.e(MainActivity.TAG, "process2 : " + result.get(CaptureResult.LENS_FOCUS_DISTANCE) + "CONTROL_AF_STATE : " + result.get(CaptureResult.CONTROL_AF_STATE));
                    }

                    @Override
                    public void onCaptureStarted(CameraCaptureSession session, CaptureRequest request, long timestamp, long frameNumber)
                    {
                        super.onCaptureStarted(session, request, timestamp, frameNumber);

                        Log.e(MainActivity.TAG, "onCaptureStarted");
                    }

                    @Override
                    public void onCaptureCompleted(CameraCaptureSession session,
                                                   CaptureRequest request, TotalCaptureResult result)
                    {
                        Log.e(MainActivity.TAG, "TTT1");
                        Log.e(MainActivity.TAG, "CaptureResult.LENS_FOCUS_DISTANCE af5 : " + result.get(CaptureResult.LENS_FOCUS_DISTANCE));

                        super.onCaptureCompleted(session, request, result);

                        process(result);


                        if (shotCnt < 2)
                        {
                            mFDistance = 0;
                            captureBuilder.set(CaptureRequest.CONTROL_MODE, CameraMetadata.CONTROL_MODE_AUTO);
                            captureBuilder.set(CaptureRequest.CONTROL_AE_MODE, CameraMetadata.CONTROL_AE_MODE_ON);
                            captureBuilder.set(CaptureRequest.CONTROL_AF_MODE, CameraMetadata.CONTROL_AF_MODE_OFF);
                            captureBuilder.set(CaptureRequest.LENS_FOCUS_DISTANCE, mFDistance);
                            captureBuilder.build();
                            Log.e(MainActivity.TAG, "captureBuilder.build(): " + captureBuilder.build());

                            //mPreviewSession.stopRepeating();
                            try
                            {

                                //captureBuilder.build();
                                //session.setRepeatingRequest(captureBuilder.build(), this, backgroudHandler1);
                                session.capture(captureBuilder.build(), this, backgroudHandler1);
                            }
                            catch (CameraAccessException e)
                            {

                                e.printStackTrace();
                            }
                        }
                        else
                        {


                            Log.e(MainActivity.TAG, "finish()");
                            Intent intent2 = getIntent();
                            intent2.putExtra("data", mfileName);
                            setResult(RESULT_OK, intent2);
                            finish();
                        }
                        ////startPreview();

                    }

                };

                mCameraDevice.createCaptureSession(outputSurfaces, new CameraCaptureSession.StateCallback()
                {

                    @Override
                    public void onConfigured(CameraCaptureSession session)
                    {
                        Log.e(MainActivity.TAG, "TTT2");

                        try
                        {
                            Log.e(MainActivity.TAG, "TTT3");
                            //session.setRepeatingRequest(captureBuilder.build(), captureListener, backgroudHandler1);
                            session.capture(captureBuilder.build(), captureListener, backgroudHandler1);
                            Log.e(MainActivity.TAG, "TTT4");
                        }
                        catch (CameraAccessException e)
                        {
                            Log.e(MainActivity.TAG, "TTT5");
                            e.printStackTrace();
                        }

                    }

                    @Override
                    public void onConfigureFailed(CameraCaptureSession session)
                    {
                        Log.e(MainActivity.TAG, "TTT6");
                    }


                }, backgroudHandler1);

            }
            catch (CameraAccessException e)
            {
                e.printStackTrace();
            }

        }

    }

    private CameraCaptureSession.CaptureCallback mCaptureCallback = new CameraCaptureSession.CaptureCallback()
    {

        private void process(CaptureResult result)
        {
            mAfState = result.get(CaptureResult.CONTROL_AF_STATE);
            mFocusDistance = result.get(CaptureResult.LENS_FOCUS_DISTANCE);
            Log.e(MainActivity.TAG, "process : " + result.get(CaptureResult.LENS_FOCUS_DISTANCE) + "CONTROL_AF_STATE : " + mAfState);

            if ((mSaveNum > (-1)) && (mSaveNum < 9) && (mPicSave == false) && (mPicSave10 == false))
            {
                mPicSave = true;
                mSaveNum++;
                if (mSaveNum == 9)
                    mFDistance = 0;
                else
                    mFDistance = mFDistance - mFDistance10;

                mPreview.takePicture(mFDistance, mfileName, mSaveNum);
                Log.e(MainActivity.TAG, "save2 : " + mFDistance);
                if (mSaveNum == 9)
                {
                    mPicSave10 = true;
                    CamManual = false;
                    ////Toast.makeText(getApplicationContext(), "Save OK : "+mfileName, Toast.LENGTH_SHORT).show();

                    Intent intent2 = getIntent();
                    intent2.putExtra("data", mfileName);
                    setResult(RESULT_OK, intent2);
                    ////finish();

                }
            }
        }

        @Override
        public void onCaptureProgressed(final CameraCaptureSession session, final CaptureRequest request, final CaptureResult partialResult)
        {
            Log.e(MainActivity.TAG, "CaptureResult.LENS_FOCUS_DISTANCE af6 : " + partialResult.get(CaptureResult.LENS_FOCUS_DISTANCE));
            super.onCaptureProgressed(session, request, partialResult);
        }

        @Override
        public void onCaptureCompleted(CameraCaptureSession session, CaptureRequest request, TotalCaptureResult result)
        {

            super.onCaptureCompleted(session, request, result);
            process(result);
        }
    };

    private void startBackgroundThread()
    {
        if (mBackgroundThread == null)
        {
            mBackgroundThread = new HandlerThread("CameraPreview");
            mBackgroundThread.start();
            mBackgroundHandler = new Handler(mBackgroundThread.getLooper());
        }
    }

    /**
     * Stops the background thread and its {@link Handler}.
     */

    private void stopBackgroundThread()
    {
        if (mBackgroundThread != null)
        {
            Log.e(MainActivity.TAG, "if (mBackgroundThread != null)");
            mBackgroundThread.quitSafely();
            try
            {
                mBackgroundThread.join();
                mBackgroundThread = null;
                mBackgroundHandler = null;
                Log.e(MainActivity.TAG, "try");
            }
            catch (InterruptedException e)
            {
                // ignore
                Log.e(MainActivity.TAG, "catch");
            }
        }
    }


}