package com.dai.t2paste;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PictureCallback;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.Toast;

public class ShotCheckActivity extends Activity
{
    private int INTENT_RESULT_CODE = 1;

    ImageView imageviewShotView;
    private Context mContext = this;
    private boolean BGResult = false;
    //private Handler mHandler;
    private ProgressDialog mProgressDialog;
    private String mFilenameBase;
    public int exifDegree;
    ////ImageDisplayView myV;////
    ////ViewGroup.LayoutParams params;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_shotcheck);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        Intent intent = getIntent();
        mFilenameBase = intent.getExtras().getString("send");

        findViewById(R.id.btnOK).setOnClickListener(mClickListener);
        findViewById(R.id.btnBack).setOnClickListener(mClickListener);

        //mHandler = new Handler();

        ////myV = (ImageDisplayView) findViewById(R.id.ImageDisplayView);

        imageviewShotView = (ImageView) findViewById(R.id.iv_shotview);

        File imgFile = new File(MainActivity.SaveDir + mFilenameBase + MainActivity.strSuffixAfterRetouch);
        Log.d(MainActivity.TAG, "mFilenameBase : " + mFilenameBase);
        sendBroadcast(new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE, Uri.parse("file://" + MainActivity.SaveDir + mFilenameBase + MainActivity.strSuffixAfterRetouch)));

        if (imgFile.exists())
        {
            try
            {
                // 이미지를 상황에 맞게 회전시킨다
                //ExifInterface exif = new ExifInterface(MainActivity.SaveDir + mFilenameBase + MainActivity.strSuffixAfterRetouch);
                //int exifOrientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL);
                //exifDegree = exifOrientationToDegrees(exifOrientation);
                exifDegree = 270;
                
                Log.d(MainActivity.TAG, "exifDegree2 : " + exifDegree);

                imageviewShotView.setVisibility(View.VISIBLE);
                Bitmap myBitmap = BitmapFactory.decodeFile(imgFile.getAbsolutePath());


                myBitmap = rotate(myBitmap, exifDegree); ////GN

                ////
                DisplayMetrics dm;
                dm = getApplicationContext().getResources().getDisplayMetrics();
                //myV.setImage2ScreenSize(dm.widthPixels, dm.heightPixels);
                //screenDisplayW = w;
                //screenDisplayH = h;

                float wRatio = (float) dm.widthPixels / (float) myBitmap.getWidth();
                float hRatio = (float) dm.heightPixels / (float) myBitmap.getHeight();
                float reSizeLen = 1.0f;
                if (wRatio > hRatio)
                    reSizeLen = hRatio;
                else
                    reSizeLen = wRatio;

                Bitmap resized = Bitmap.createScaledBitmap(myBitmap, (int) (myBitmap.getWidth() * reSizeLen), (int) (myBitmap.getHeight() * reSizeLen), true);
                ////
                imageviewShotView.setImageBitmap(resized);
            }
            catch (Exception e)
            {

            }
        }

        ///ExampleThread thread = new ExampleThread();
        ///thread.start();
    }

    public int exifOrientationToDegrees(int exifOrientation)
    {
        if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_90)
        {
            return 90;
        }
        else if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_180)
        {
            return 180;
        }
        else if (exifOrientation == ExifInterface.ORIENTATION_ROTATE_270)
        {
            return 270;
        }
        return 0;
    }

    public Bitmap rotate(Bitmap bitmap, int degrees)
    {
        if (degrees != 0 && bitmap != null)
        {
            Matrix m = new Matrix();
            m.setRotate(degrees, (float) bitmap.getWidth() / 2,
                    (float) bitmap.getHeight() / 2);

            try
            {
                Bitmap converted = Bitmap.createBitmap(bitmap, 0, 0,
                        bitmap.getWidth(), bitmap.getHeight(), m, true);
                if (bitmap != converted)
                {
                    bitmap.recycle();
                    bitmap = converted;
                }
            }
            catch (OutOfMemoryError ex)
            {
                // 메모리가 부족하여 회전을 시키지 못할 경우 그냥 원본을 반환합니다.
            }
        }
        return bitmap;
    }

    @Override
    public void onBackPressed()
    {
        Intent intent = getIntent();
        //Toast.makeText(mContext, "btnBack", Toast.LENGTH_SHORT).show();
        intent.putExtra("data", "CamStart");
        setResult(RESULT_CANCELED, intent);
        finish();
        super.onBackPressed();
    }

    Button.OnClickListener mClickListener = new View.OnClickListener()
    {
        public void onClick(View v)
        {
            Intent intent = getIntent();
            switch (v.getId())
            {
                case R.id.btnOK:
                    /*
                	Toast.makeText(mContext, "btnOK", Toast.LENGTH_SHORT).show();                	 
            		intent.putExtra("data", "ImgEdit");
                    setResult(RESULT_OK,intent);
            		finish();
            		*/

                    CheckTypesTask task = new CheckTypesTask();
                    task.execute();

                    break;

                case R.id.btnBack:
                    //Toast.makeText(mContext, "btnBack", Toast.LENGTH_SHORT).show();
                    intent.putExtra("data", "CamStart");
                    setResult(RESULT_CANCELED, intent);
                    finish();
                    break;
            }
        }
    };

    private class CheckTypesTask extends AsyncTask<Void, Void, Void>
    {

        ProgressDialog asyncDialog = new ProgressDialog(
                ShotCheckActivity.this);

        @Override
        protected void onPreExecute()
        {
            asyncDialog.setProgressStyle(ProgressDialog.THEME_HOLO_LIGHT);
            asyncDialog.setMessage("Processing\nPlease wait...");
            asyncDialog.setCancelable(false);

            // show dialog
            asyncDialog.show();
            super.onPreExecute();
        }

        @Override
        protected Void doInBackground(Void... arg0)
        {
            try
            {
            	/*
            	DAIPA daipa;
				daipa = new DAIPA();
				Log.i("TEST", daipa.TEST());
				Thread.sleep(10);
				*/

                while (true)
                {
                    //asyncDialog.setProgress(i * 30);

                    File files = new File(MainActivity.SaveDir,  mFilenameBase + MainActivity.strSuffixAfterRetouch);
                    //new File(MainActivity.SaveDir+mFilenameBase+"Result_Image");
                    //파일 유무를 확인합니다.
                    if (files.exists() == true)
                        break;

                    Thread.sleep(50);
                }
            }
            catch (InterruptedException e)
            {
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result)
        {
            asyncDialog.dismiss();

            Intent intent = getIntent();
            //Toast.makeText(mContext, "btnOK : "+mFilenameBase, Toast.LENGTH_SHORT).show();                	 
            intent.putExtra("data", mFilenameBase);
            setResult(RESULT_OK, intent);
            finish();

            super.onPostExecute(result);
        }
    }
}
