package com.dai.t2paste;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import android.Manifest;
import android.R.string;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PictureCallback;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.RadioButton;
import android.widget.Toast;

public class MainActivity extends Activity
{
    public static String TAG = "PASTE";
    public static String SaveDir = "";
    public static String strSuffixAfterRetouch = "Result_Image.jpg";

    private int INTENT_RESULT_CODE = 1;
    private int INTENT_CAMPREVIEW_RESULT_CODE = 2;
    private int INTENT_SHOTCHECK_RESULT_CODE = 3;
    private int INTENT_IMAGEEDITING_RESULT_CODE = 4;


    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        ////setContentView(R.layout.activity_main);
        ////setContentView(R.layout.start_view);

        SaveDir = Environment.getExternalStorageDirectory() + "/Pictures/PASTE/" ;

        Intent intent = new Intent(this, StartActivity.class);
        startActivityForResult(intent, INTENT_RESULT_CODE);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) // 액티비티가 정상적으로 종료되었을 경우
        {
            if (requestCode == INTENT_RESULT_CODE) // requestCode==1 로 호출한 경우에만 처리합니다.
            {
                /*
                if(data.getStringExtra("data").equals("CamStart")) {
            		Log.d(MainActivity.TAG , ""+data.getStringExtra("data"));
	            	Intent intent = new Intent(this, CamPreviewActivity.class);
	            	startActivityForResult(intent, INTENT_CAMPREVIEW_RESULT_CODE);
            	}            	
            	else*/
                {
                    Log.d(MainActivity.TAG , "" + data.getStringExtra("data"));

                    String tmp = data.getStringExtra("data");
                    if (tmp.equals("btnEtc"))
                    {
                        // Test code.170704
                        Intent intent = new Intent(this, ImageGridActivity.class);
                        startActivity(intent);
                    }
                    else
                    {
                        Intent intent = new Intent(this, CamPreviewActivity.class);
                        startActivityForResult(intent, INTENT_CAMPREVIEW_RESULT_CODE);
                    }

                    //startActivity(intent);
                }
            }
            else if (requestCode == INTENT_CAMPREVIEW_RESULT_CODE) // requestCode==1 로 호출한 경우에만 처리합니다.
            {
                //if(data.getStringExtra("data").equals("ShotDone"))
                {//Toast.makeText(getApplicationContext(), "INTENT_SHOTCHECK_RESULT_CODE OK : "+data, Toast.LENGTH_SHORT).show();
                    Log.d(MainActivity.TAG , "" + data.getStringExtra("data"));
                    Intent intent = new Intent(this, ShotCheckActivity.class);
                    intent.putExtra("send", data.getStringExtra("data"));
                    startActivityForResult(intent, INTENT_SHOTCHECK_RESULT_CODE);
                }
            }
            else if (requestCode == INTENT_SHOTCHECK_RESULT_CODE) // requestCode==1 로 호출한 경우에만 처리합니다.
            {
                //if(data.getStringExtra("data").equals("ShotDone"))
                {

                    Log.d(MainActivity.TAG , "" + data.getStringExtra("data"));
                    Intent intent = new Intent(this, ImageEditingActivity.class);
                    intent.putExtra("send", data.getStringExtra("data"));
                    startActivityForResult(intent, INTENT_IMAGEEDITING_RESULT_CODE);
                }
            }
        }
        else if (resultCode == RESULT_CANCELED)
        {
            if ((requestCode == INTENT_SHOTCHECK_RESULT_CODE) || (requestCode == INTENT_IMAGEEDITING_RESULT_CODE))
            {
                if (data.getStringExtra("data").equals("CamStart"))
                {
                    Log.d(MainActivity.TAG , "" + data.getStringExtra("data"));
                    Intent intent = new Intent(this, CamPreviewActivity.class);
                    startActivityForResult(intent, INTENT_CAMPREVIEW_RESULT_CODE);
                }
            }
        }
    }


}
