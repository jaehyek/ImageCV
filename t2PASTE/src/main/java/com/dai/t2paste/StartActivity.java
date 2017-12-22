package com.dai.t2paste;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class StartActivity extends Activity
{
    private static final int REQUEST_EXTERNAL_STORAGE = 2;
    private static final int REQUEST_CAMERA = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.start_view);

        findViewById(R.id.btnFacebook).setOnClickListener(mClickListener);
        findViewById(R.id.btnKakao).setOnClickListener(mClickListener);
        findViewById(R.id.btnEtc).setOnClickListener(mClickListener);

        checkPermission(REQUEST_CAMERA);
    }

    Button.OnClickListener mClickListener = new View.OnClickListener()
    {

        public void onClick(View v)
        {
            ////Toast toast;
            Intent intent = getIntent();
            switch (v.getId())
            {
                case R.id.btnFacebook:
                    //toast = Toast.makeText(StartActivity.this, "btnFacebook", Toast.LENGTH_SHORT);
                    //toast.show();
                    intent.putExtra("data", "btnFacebook");
                    setResult(RESULT_OK, intent);
                    finish();
                    break;
                case R.id.btnKakao:
                    //toast = Toast.makeText(StartActivity.this, "btnKakao", Toast.LENGTH_SHORT);
                    //toast.show();
                    intent.putExtra("data", "btnKakao");
                    setResult(RESULT_OK, intent);
                    finish();
                    break;
                case R.id.btnEtc:
                    //toast = Toast.makeText(StartActivity.this, "btnEtc", Toast.LENGTH_SHORT);
                    //toast.show();
                    intent.putExtra("data", "btnEtc");
                    setResult(RESULT_OK, intent);
                    finish();
                    break;
            }
        }
    };

    private void checkPermission(int id)
    {
        ////Log.i(MainActivity.TAG , "CheckPermission : " + checkSelfPermission(permission));
        switch (id)
        {
            case REQUEST_EXTERNAL_STORAGE:
                if (checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                        != PackageManager.PERMISSION_GRANTED
                        || checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE)
                        != PackageManager.PERMISSION_GRANTED)
                {

                    // Should we show an explanation?
                    if (shouldShowRequestPermissionRationale(Manifest.permission.WRITE_EXTERNAL_STORAGE))
                    {
                        // Explain to the user why we need to write the permission.
                        Toast.makeText(this, "Read/Write external storage", Toast.LENGTH_SHORT).show();
                    }

                    requestPermissions(new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE},
                            REQUEST_EXTERNAL_STORAGE);

                    // MY_PERMISSION_REQUEST_STORAGE is an
                    // app-defined int constant

                } else
                {
                    Log.e(MainActivity.TAG, "permission authorized");
                /*
                mTextureView = (TextureView) findViewById(R.id.texture);        		
        		mTextureView.setSurfaceTextureListener(new CanvasListener());
        		mTextureView.setOpaque(false);        		
        		mEtCnt = (EditText) findViewById(R.id.editText1);
        		mEtCnt.setText(String.valueOf(PCnt));        		
        		findViewById(R.id.btn_takepicture).setOnClickListener(mClickListener);
        		*/

                }
                break;

            case REQUEST_CAMERA:
                if (checkSelfPermission(Manifest.permission.CAMERA)
                        != PackageManager.PERMISSION_GRANTED)
                {

                    // Should we show an explanation?
                    if (shouldShowRequestPermissionRationale(Manifest.permission.CAMERA))
                    {
                        // Explain to the user why we need to write the permission.
                        Toast.makeText(this, "camera", Toast.LENGTH_SHORT).show();
                    }

                    requestPermissions(new String[]{Manifest.permission.CAMERA},
                            REQUEST_CAMERA);

                } else
                {
                    Log.e(MainActivity.TAG , "permission authorized");
                /*
                mTextureView = (TextureView) findViewById(R.id.texture);        		
        		mTextureView.setSurfaceTextureListener(new CanvasListener());
        		mTextureView.setOpaque(false);        		
        		mEtCnt = (EditText) findViewById(R.id.editText1);
        		mEtCnt.setText(String.valueOf(PCnt));        		
        		findViewById(R.id.btn_takepicture).setOnClickListener(mClickListener);
           */

                    ////writeFile();
                }
                break;
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults)
    {
        switch (requestCode)
        {
            case REQUEST_EXTERNAL_STORAGE:
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED
                        && grantResults[1] == PackageManager.PERMISSION_GRANTED)
                {

                    ////writeFile();

                    // permission was granted, yay! do the
                    // calendar task you need to do.
                    /*
                	mTextureView = (TextureView) findViewById(R.id.texture);            		
            		mTextureView.setSurfaceTextureListener(new CanvasListener());
            		mTextureView.setOpaque(false);            		
            		mEtCnt = (EditText) findViewById(R.id.editText1);
            		mEtCnt.setText(String.valueOf(PCnt));            		
            		findViewById(R.id.btn_takepicture).setOnClickListener(mClickListener);
                	*/


                } else
                {

                    Log.d(MainActivity.TAG , "Permission always deny");

                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                }
                break;

            case REQUEST_CAMERA:
                if (grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {

                    ////writeFile();

                    // permission was granted, yay! do the
                    // calendar task you need to do.


                    checkPermission(REQUEST_EXTERNAL_STORAGE);

                } else
                {

                    Log.d(MainActivity.TAG , "Permission always deny");

                    // permission denied, boo! Disable the
                    // functionality that depends on this permission.
                }
                break;
        }
    }

}
