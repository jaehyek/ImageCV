package com.dai.t2paste;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Toast;


public class ImageClickListener implements OnClickListener
{

    private int INTENT_RESULT_CODE = 1;
    private int INTENT_CAMPREVIEW_RESULT_CODE = 2;
    private int INTENT_SHOTCHECK_RESULT_CODE = 3;
    private int INTENT_IMAGEEDITING_RESULT_CODE = 4;

    Context context;

    //-----------------------------------------------------------
    // imageID는 확대해서 보여줄 이미지의 리소스 ID입니다.

    ////int imageID;
    String strFilename;


    public ImageClickListener(Context context, String strFilename)
    {
        ////public ImageClickListener(Context context, int strFilename) {
        this.context = context;
        this.strFilename = strFilename;
    }

    public void onClick(View v)
    {
        //---------------------------------------------------------
        // 확대된 이미지를 보여주는 액티비티를 실행하기 위해 인텐트 객체를 정의합니다.
        // 그리고 이 액티비티에 전달할 strFilename의 값을 이 객체에 저장합니다.
        // 인텐트 객체를 정의 후 이 액티비티를 실행합니다.

        //Intent intent = new Intent(context, ImageActivity.class);
        //intent.putExtra("image ID", strFilename);
        //context.startActivity(intent);        
        //((Activity) context).startActivityForResult(intent, INTENT_RESULT_CODE);

        // strFilename format = PASTE_20171221_091806_AlphaMap_display.jpg
        // base = PASTE_20171221_091806_


        String path = strFilename.substring(0, 22);

        Log.d(MainActivity.TAG, "Edit : " + path);

        Intent intent = new Intent(context, ShotCheckActivity.class);
        intent.putExtra("send", path);
        ((Activity) context).startActivityForResult(intent, INTENT_SHOTCHECK_RESULT_CODE);

    }


}

