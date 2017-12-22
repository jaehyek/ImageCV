package com.dai.t2paste;

import android.app.Activity;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.widget.GridView;

public class ImageGridActivity extends Activity
{

    private int INTENT_RESULT_CODE = 1;
    private int INTENT_CAMPREVIEW_RESULT_CODE = 2;
    private int INTENT_SHOTCHECK_RESULT_CODE = 3;
    private int INTENT_IMAGEEDITING_RESULT_CODE = 4;

    GridView gridViewImages = null;
    ImageGridAdapter imageGridAdapter = null;
    //------------------------------------------------------------
    // imageIDs 배열은 GridView 뷰를 구성하는 이미지 파일들의 리소스 ID들을 담습니다.

    private int[] imageIDs = new int[]{
            R.drawable.btn_colorchange,
            R.drawable.btn_deltacolor,
            R.drawable.btn_hair,
            R.drawable.btn_line,
            R.drawable.btn_line,

    };

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.grid_image);

//        addImageToGallery(MainActivity.SaveDir,  getBaseContext());

        //-----------------------------------------------------------------------
        // 사진들을 보여줄 GridView 뷰의 어댑터 객체를 정의하고 그것을 이 뷰의 어댑터로 설정합니다.        
        gridViewImages = (GridView) findViewById(R.id.gridViewImages);
        ////ImageGridAdapter imageGridAdapter = new ImageGridAdapter(this, imageIDs);
        imageGridAdapter = new ImageGridAdapter(this);
        gridViewImages.setAdapter(imageGridAdapter);
    }

    public static void addImageToGallery(final String filePath, final Context context)
    {
        ContentValues values = new ContentValues();
        values.put(MediaStore.Images.Media.DATE_TAKEN, System.currentTimeMillis());
        values.put(MediaStore.Images.Media.MIME_TYPE, "image/jpeg");
        values.put(MediaStore.MediaColumns.DATA, filePath);
        context.getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);
        if (resultCode == RESULT_OK) // 액티비티가 정상적으로 종료되었을 경우
        {
            if (requestCode == INTENT_RESULT_CODE) // requestCode==1 로 호출한 경우에만 처리합니다.
            {
                if (data.getStringExtra("data") == "Delete")
                {
                    gridViewImages.setAdapter(imageGridAdapter);
                    imageGridAdapter.notifyDataSetChanged();

                }
                /*
            	else  {
            		int n = data.getStringExtra("data").indexOf(".");
            		String path = data.getStringExtra("data").substring(6, n-2);
            		
            		Log.d("test123", "Edit : "+path); 
            		
	            	Intent intent = new Intent(this, ShotCheckActivity.class);
	            	intent.putExtra("send", path);
	            	startActivityForResult(intent, INTENT_SHOTCHECK_RESULT_CODE);
	            	
            	}*/

                Log.d(MainActivity.TAG, "ImageGridActivity " + data.getStringExtra("data"));
            }
            else if (requestCode == INTENT_SHOTCHECK_RESULT_CODE) // requestCode==1 로 호출한 경우에만 처리합니다.
            {
                //if(data.getStringExtra("data").equals("ShotDone"))
                {
                    //DAIPA daipa;
                    //daipa = new DAIPA();
                    //Log.i("TEST", daipa.TEST(MainActivity.SaveDir + mfileName, 0, yuv420Data1, yuv420Data2 ));

                    Log.d(MainActivity.TAG, "" + data.getStringExtra("data"));
                    Intent intent = new Intent(this, ImageEditingActivity.class);
                    intent.putExtra("send", data.getStringExtra("data"));
                    startActivityForResult(intent, INTENT_IMAGEEDITING_RESULT_CODE);
                }
            }
        }
    }
} 