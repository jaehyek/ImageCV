package com.dai.t2paste;

import java.io.File;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

public class ImageActivity extends Activity
{

    File imgFile = null;
    String imageID = null;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.simple_image);

        //----------------------------------------------------------------
        // 확대되는 이미지를 보여주기 위해 ImageView 뷰를 설정합니다.
        ImageView imageView = (ImageView) findViewById(R.id.imageView);
        setImage(imageView);

        findViewById(R.id.btn_test1).setOnClickListener(mClickListener);
        findViewById(R.id.btn_test2).setOnClickListener(mClickListener);
        findViewById(R.id.btn_test3).setOnClickListener(mClickListener);
        findViewById(R.id.btn_test4).setOnClickListener(mClickListener);
    }

    private void setImage(ImageView imageView)
    {
        //----------------------------------------------------------------
        // 초기 액티비티의 GridView 뷰의 이미지 항목을 클릭할 때 생성된 인텐트는
        // 이 액티비티는 getIntent 메소드를 호출하여 접근할 수 있습니다.
        Intent receivedIntent = getIntent();

        //----------------------------------------------------------------
        // 확대되는 이미지의 리소스 ID를 인텐트로부터 읽어들이고,
        // 그것을 ImageView 뷰의 이미지 리소스로 설정합니다.

        ////int imageID = (Integer)receivedIntent.getExtras().get("image ID");        
        imageID = receivedIntent.getStringExtra("image ID");
        ////imageView.setImageResource(imageID);

        imgFile = new File("/sdcard/Pictures/PASTE/" + imageID);

        if (imgFile.exists())
        {

            Bitmap myBitmap = BitmapFactory.decodeFile(imgFile.getAbsolutePath());
            imageView.setImageBitmap(myBitmap);

        }
    }

    Button.OnClickListener mClickListener = new View.OnClickListener()
    {
        public void onClick(View v)
        {
            Intent intent = getIntent();

            switch (v.getId())
            {
                // Edit tool
                case R.id.btn_test1:
                    Log.d("test123", "btn_test1 ");
                    break;

                case R.id.btn_test2:
                    intent.putExtra("data", imageID);
                    setResult(RESULT_OK, intent);
                    finish();
                    Log.d("test123", "btn_test2 ");
                    break;

                case R.id.btn_test3:
                    Log.d("test123", "btn_test3 ");
                    break;

                case R.id.btn_test4:
                    if (imgFile.delete())
                    {
                        Log.i("test123", "file remove = " + imgFile.getName() + ", 삭제 성공");
                    }
                    else
                    {
                        Log.i("test123", "file remove = " + imgFile.getName() + ", 삭제 실패");
                    }
                    intent.putExtra("data", "Delete");
                    setResult(RESULT_OK, intent);
                    finish();
                    Log.d("test123", "btn_test4 ");
                    break;


            }
        }
    };
}
