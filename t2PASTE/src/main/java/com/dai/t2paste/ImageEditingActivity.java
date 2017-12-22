package com.dai.t2paste;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import com.dai.t2paste.ImageDisplayView.Vertex;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PictureCallback;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnTouchListener;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

public class ImageEditingActivity extends Activity
{

    private int INTENT_RESULT_CODE = 1;

    private int CV_EVENT_FLAG_LBUTTON = 1;
    private int CV_EVENT_FLAG_RBUTTON = 2;

    private boolean CV_BACKGROUND = true;
    private boolean CV_FOREGROUND = false;

    ImageView myImage;
    private Context mContext = this;

    //private Handler mHandler;
    private ProgressDialog mProgressDialog;

    private String mfileName;
    private String mAftRetouching;


    Bitmap myBitmap;
    float mx;
    float my;

    ImageDisplayView myV;
    ViewGroup.LayoutParams params;

    SeekBar sb_threshold;
    TextView tv_threshold;

    SeekBar sb_thickness;
    TextView tv_thickness;

    SeekBar sb_deltacolor;
    TextView tv_deltacolor;

    Button btn_backgrounding;
    Button btn_foregrounding;

    Button btn_Backgrounding_deltacolor;
    Button btn_Foregrounding_deltacolor;

    DisplayMetrics dm;

    boolean ProgressDialogCnt = false;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_imgedit);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

        Intent intent = getIntent();
        ////mfileName = intent.getExtras().getString("send");
        ////mAftRetouching = MainActivity.strSuffixAfterRetouch;

        myV = (ImageDisplayView) findViewById(R.id.ImageDisplayView);
        params = myV.getLayoutParams();

        myV.mfileName = intent.getExtras().getString("send");
        myV.mAftRetouching = MainActivity.strSuffixAfterRetouch;
        myV.displaySw = myV.FileLoad();

        dm = getApplicationContext().getResources().getDisplayMetrics();
        myV.setImage2ScreenSize(dm.widthPixels, dm.heightPixels);

        ////params.width = myV.imgW;
        ////params.height = myV.imgH;

        myV.setLayoutParams(params);
        Log.d(MainActivity.TAG, "myV.imgW:" + myV.imgW + " myV.imgH:" + myV.imgH);

        ////daipa = new DAIPA();

        // BG / FB
        findViewById(R.id.btnPolygon).setOnClickListener(mClickListener);
        findViewById(R.id.btnLine).setOnClickListener(mClickListener);
        findViewById(R.id.btnDeltacolor).setOnClickListener(mClickListener);
        findViewById(R.id.btnHair).setOnClickListener(mClickListener);
        findViewById(R.id.btnThreshold).setOnClickListener(mClickListener);

        // Edit tool
        findViewById(R.id.btnBack).setOnClickListener(mClickListener);
        findViewById(R.id.btnReset).setOnClickListener(mClickListener);
        findViewById(R.id.btnUndo).setOnClickListener(mClickListener);
        findViewById(R.id.btnRedo).setOnClickListener(mClickListener);
        findViewById(R.id.btnColorchange).setOnClickListener(mClickListener);
        findViewById(R.id.btnOriginalSize).setOnClickListener(mClickListener);
        findViewById(R.id.btnOK).setOnClickListener(mClickListener);

        // Sub menu
        ////findViewById(R.id.btnBackgrounding).setOnClickListener(mClickListener);
        ////findViewById(R.id.btnForegrounding).setOnClickListener(mClickListener);

        btn_backgrounding = (Button) findViewById(R.id.btnBackgrounding);
        btn_foregrounding = (Button) findViewById(R.id.btnForegrounding);

        btn_Backgrounding_deltacolor = (Button) findViewById(R.id.btnBackgrounding_deltacolor);
        btn_Foregrounding_deltacolor = (Button) findViewById(R.id.btnForegrounding_deltacolor);

        btn_backgrounding.setOnClickListener(mClickListener);
        btn_foregrounding.setOnClickListener(mClickListener);

        btn_Backgrounding_deltacolor.setOnClickListener(mClickListener);
        btn_Foregrounding_deltacolor.setOnClickListener(mClickListener);

        sb_threshold = (SeekBar) findViewById(R.id.sb_threshold);
        tv_threshold = (TextView) findViewById(R.id.tv_threshold);

        sb_thickness = (SeekBar) findViewById(R.id.sb_thickness);
        tv_thickness = (TextView) findViewById(R.id.tv_thickness);

        sb_deltacolor = (SeekBar) findViewById(R.id.sb_deltacolor);
        tv_deltacolor = (TextView) findViewById(R.id.tv_deltacolor);

        sb_threshold.setVisibility(View.GONE);
        tv_threshold.setVisibility(View.GONE);

        sb_thickness.setVisibility(View.GONE);
        tv_thickness.setVisibility(View.GONE);

        sb_deltacolor.setVisibility(View.GONE);
        tv_deltacolor.setVisibility(View.GONE);

        btn_Backgrounding_deltacolor.setVisibility(View.GONE);
        btn_Foregrounding_deltacolor.setVisibility(View.GONE);

        btn_backgrounding.setVisibility(View.GONE);
        btn_foregrounding.setVisibility(View.GONE);

        sb_threshold.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
        {
            public void onStopTrackingTouch(SeekBar seekBar)
            {
                // tv.setText("onStop TrackingTouch");
            }

            public void onStartTrackingTouch(SeekBar seekBar)
            {
                //tv.setText("onStart TrackingTouch");

            }

            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser)
            {
                int tmp = progress + 1;
                //tv.setText("onProgressChanged : " + progress);
                if (myV.getEditingMode() == myV.MODE_THRESHOLD)
                {

                    Log.d(MainActivity.TAG, "myV.RectEndX : " + (int) myV.RectEndX + ", myV.RectStartX : " + (int) myV.RectStartX +
                            "myV.RectEndY : " + (int) myV.RectEndY + ", myV.RectStartY : " + (int) myV.RectStartY);

                    if (!((myV.RectEndX == myV.RectStartX) || (myV.RectEndY == myV.RectStartY)))
                    {
                        tv_threshold.setText(" Threshold : " + tmp);
                        myV.setThreshold(tmp);
                    }
                    else
                        sb_threshold.setProgress(0);
                }
                /*
            	else if(myV.getEditingMode() == myV.MODE_LINE) {
            		
            		Log.d(MainActivity.TAG, "myV.RectEndX : " + (int)myV.RectEndX +", myV.RectStartX : " + (int)myV.RectStartX + 
            				"myV.RectEndY : " + (int)myV.RectEndY +", myV.RectStartY : " + (int)myV.RectStartY);
            		
	            	if(!((myV.RectEndX == myV.RectStartX) || (myV.RectEndY == myV.RectStartY))) {
	            		tv_threshold.setText(" Threshold : "+ progress);
	            		myV.setThreshold(progress+1);
	            	}
	            	else
	            		sb_threshold.setProgress(0);
	            }
            	else if(myV.getEditingMode() == myV.MODE_DELTACOLOR) {
            		
            		Log.d(MainActivity.TAG, "myV.RectEndX : " + (int)myV.RectEndX +", myV.RectStartX : " + (int)myV.RectStartX + 
            				"myV.RectEndY : " + (int)myV.RectEndY +", myV.RectStartY : " + (int)myV.RectStartY);
            		
	            	if(!((myV.RectEndX == myV.RectStartX) || (myV.RectEndY == myV.RectStartY))) {
	            		tv_threshold.setText(" Threshold : "+ progress);
	            		myV.setThreshold(progress+1);
	            	}
	            	else
	            		sb_threshold.setProgress(0);
	            }
            	*/

            }
        });

        sb_thickness.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
        {
            public void onStopTrackingTouch(SeekBar seekBar)
            {
                // tv.setText("onStop TrackingTouch");
            }

            public void onStartTrackingTouch(SeekBar seekBar)
            {
                //tv.setText("onStart TrackingTouch");

            }

            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser)
            {
                int tmp = progress + 1;
                //tv.setText("onProgressChanged : " + progress);
                if (myV.getEditingMode() == myV.MODE_DELTACOLOR)
                {
                    myV.DeltaColorTolerance = tmp;
                    tv_thickness.setText(" DeltaColor : " + tmp);
                }
                else if (myV.getEditingMode() == myV.MODE_LINE)
                {
                    myV.LineStrokeWidth = tmp;
                    myV.invalidate();
                    tv_thickness.setText(" Thickness : " + tmp);
                }

                //myV.setThreshold(progress);
            }
        });

        sb_deltacolor.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()
        {
            public void onStopTrackingTouch(SeekBar seekBar)
            {
                // tv.setText("onStop TrackingTouch");
            }

            public void onStartTrackingTouch(SeekBar seekBar)
            {
                //tv.setText("onStart TrackingTouch");

            }

            public void onProgressChanged(SeekBar seekBar, int progress,
                                          boolean fromUser)
            {

                int tmp = progress + 1;
                //tv.setText("onProgressChanged : " + progress);
                if (myV.getEditingMode() == myV.MODE_DELTACOLOR)
                {
                    myV.DeltaColorTolerance = tmp;
                    tv_deltacolor.setText(" DeltaColor : " + tmp);
                }
                else if (myV.getEditingMode() == myV.MODE_LINE)
                {
                    myV.LineStrokeWidth = tmp;
                    tv_deltacolor.setText(" Thickness : " + tmp);
                }

                //myV.setThreshold(progress);
            }
        });

        ////LinearLayout ll = (LinearLayout)findViewById(R.id.fl_1);
        ////final DrawingView ov = new DrawingView(this);
        ////ll.addView(ov);

        //mHandler = new Handler();

        ////myImage = (ImageView) findViewById(R.id.iv_shotview);
		/*
		File imgFile = new  File(MainActivity.SaveDir + mfileName +MainActivity.strSuffixAfterRetouch);

		if(!imgFile.exists()){
			myImage.setVisibility(View.VISIBLE);
		    myBitmap = BitmapFactory.decodeFile(imgFile.getAbsolutePath());		    
		    myImage.setImageBitmap(myBitmap);
		}
		*/
		
/*		
		myImage.setOnTouchListener(new View.OnTouchListener() {
	        @Override
	        public boolean onTouch(View v, MotionEvent event) {
	            switch (event.getAction()){
	                case MotionEvent.ACTION_UP :
	                	daipa.retouchingDrag(0, 0, CV_EVENT_FLAG_RBUTTON);
	                	mAftRetouching = "Result_Image_AftRetouching.jpg";
	                	File imgFile = new  File("/sdcard/Pictures/PASTE/PASTE_" + mfileName + mAftRetouching);

	            		if(imgFile.exists()){
	            			myImage.setVisibility(View.VISIBLE);
	            		    Bitmap myBitmap = BitmapFactory.decodeFile(imgFile.getAbsolutePath());		    
	            		    myImage.setImageBitmap(myBitmap.);
	            		    
	            		}
	                	arVertex.clear();
	                   break;
	                case MotionEvent.ACTION_DOWN :
	                	//mx = event.getX();
	                	//my = event.getY();
	                	
	                	break;
	                case MotionEvent.ACTION_MOVE:
	                    ////imageView.setX(event.getRawX());
	                    ////imageView.setY(event.getRawY());
	                	mx = event.getX();
	                	my = event.getY();
		                //ov.update((int)my, (int)my);
	                	arVertex.add(new Vertex(event.getX(), event.getY(), false));
	                	
	                	ov.postInvalidate();
		                myImage.postInvalidate();
		                //myImage.invalidate();
		                ////daipa.retouchingDrag((int)mx, (int)my, CV_EVENT_FLAG_LBUTTON);		    			
		                String str;
		                str = "Coordinate : ( " + (int)mx + ", " + (int)my + " )";
		                ////Toast.makeText(mContext, str, Toast.LENGTH_SHORT).show();
		                Log.d(MainActivity.TAG, "ACTION_MOVE");
	                    break;
	            }
	            return true;
	        }
	    });
	    */

    }

    @Override
    public void onBackPressed()
    {
        Intent intent = getIntent();
        Toast.makeText(mContext, "btnBack", Toast.LENGTH_SHORT).show();
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

            AlertDialog.Builder alert = new AlertDialog.Builder(ImageEditingActivity.this);
            alert.setPositiveButton("OK", new DialogInterface.OnClickListener()
            {
                @Override
                public void onClick(DialogInterface dialog, int which)
                {
                    dialog.dismiss();     //닫기
                }
            });

            switch (v.getId())
            {
                // Edit tool
                case R.id.btnBack:
                    Toast.makeText(mContext, "btnBack", Toast.LENGTH_SHORT).show();
                    intent.putExtra("data", "CamStart");
                    setResult(RESULT_CANCELED, intent);
                    finish();
                    break;

                case R.id.btnReset:

                    alert.setMessage("리셋 기능은 개발 중입니다.");
                    alert.show();
                    break;

                case R.id.btnUndo:
                    alert.setMessage("undo 기능은 개발 중입니다.");
                    alert.show();
                    break;
                case R.id.btnRedo:
                    alert.setMessage("redo 기능은 개발 중입니다.");
                    alert.show();
                    break;
                case R.id.btnColorchange:
                    //alert.setMessage("background color 변경 기능은 개발 중입니다.");
                    //alert.show();
                    myV.setPaintingBackground();
                    break;
                case R.id.btnOriginalSize:
                    myV.setImage2ScreenSize(dm.widthPixels, dm.heightPixels);
                    //alert.setMessage("원래 크기 정위치 기능은 개발 중입니다.");
                    //alert.show();
                    break;
                case R.id.btnOK:
                    alert.setMessage("Saving images.\nReturn to Picture mode.");
                    alert.show();
                    break;

                // BG/FB
                case R.id.btnPolygon:
                    setEdit(myV.MODE_POLYGON, R.id.btnPolygon, R.drawable.btn_polygon, R.drawable.btn_polygon_press);
                    //alert.setMessage("다각형 기능은 개발 중입니다. "+myV.getEditingMode());
                    //alert.show();

                    break;

                case R.id.btnLine:
                    sb_threshold.setProgress(0);
                    setEdit(myV.MODE_LINE, R.id.btnLine, R.drawable.btn_line, R.drawable.btn_line_press);
                    //alert.setMessage("라인 기능은 개발 중입니다. "+myV.getEditingMode());
                    //alert.show();

                    break;

                case R.id.btnDeltacolor:
                    sb_threshold.setProgress(0);
                    setEdit(myV.MODE_DELTACOLOR, R.id.btnDeltacolor, R.drawable.btn_deltacolor, R.drawable.btn_deltacolor_press);
                    //alert.setMessage("델타 칼라 기능은 개발 중입니다. "+myV.getEditingMode());
                    //alert.show();

                    break;

                case R.id.btnHair:
                    setEdit(myV.MODE_HAIR, R.id.btnHair, R.drawable.btn_hair, R.drawable.btn_hair_press);
                    //alert.setMessage("헤어  기능은 개발 중입니다. "+myV.getEditingMode());
                    //alert.show();

                    break;

                case R.id.btnThreshold:
                    myV.RectStartX = 0.0f;
                    myV.RectStartY = 0.0f;
                    myV.RectEndX = 0.0f;
                    myV.RectEndY = 0.0f;
                    sb_threshold.setProgress(0);
                    setEdit(myV.MODE_THRESHOLD, R.id.btnThreshold, R.drawable.btn_threshold, R.drawable.btn_threshold_press);
                    //alert.setMessage("스레스홀드 기능은 개발 중입니다. "+myV.getEditingMode());
                    //alert.show();

                    break;

                // Sub menu
                case R.id.btnBackgrounding:
                    ////alert.setMessage("Backgrounding 기능은 개발 중입니다. ");
                    ////alert.show();
                    if (myV.getEditingMode() == myV.MODE_POLYGON)
                        myV.setPolygon(CV_BACKGROUND);
                    else if (myV.getEditingMode() == myV.MODE_LINE)
                    {
                        myV.setLine(CV_BACKGROUND);
                    }
                    break;

                case R.id.btnForegrounding:
                    ////alert.setMessage("Foregrounding 기능은 개발 중입니다. ");
                    ////alert.show();
                    if (myV.getEditingMode() == myV.MODE_POLYGON)
                        myV.setPolygon(CV_FOREGROUND);
                    else if (myV.getEditingMode() == myV.MODE_LINE)
                    {
                        myV.setLine(CV_FOREGROUND);
                    }
                    break;

                case R.id.btnBackgrounding_deltacolor:
                    if (myV.getEditingMode() == myV.MODE_DELTACOLOR)
                        myV.setDeltaColor(CV_BACKGROUND);
                    break;

                case R.id.btnForegrounding_deltacolor:
                    if (myV.getEditingMode() == myV.MODE_DELTACOLOR)
                        myV.setDeltaColor(CV_FOREGROUND);
                    break;
            }
        }
    };

    public void setEdit(int mode, int id, int drawableId, int drawablePressId)
    {

        if (myV.getEditingMode() == mode)
        {
            findViewById(id).setBackground(getDrawable(drawableId));
            myV.setEditingMode(myV.MODE_NO);
            sb_threshold.setVisibility(View.GONE);
            tv_threshold.setVisibility(View.GONE);
            btn_backgrounding.setVisibility(View.GONE);
            btn_foregrounding.setVisibility(View.GONE);
            sb_thickness.setVisibility(View.GONE);
            tv_thickness.setVisibility(View.GONE);
            sb_deltacolor.setVisibility(View.GONE);
            tv_deltacolor.setVisibility(View.GONE);
            btn_Backgrounding_deltacolor.setVisibility(View.GONE);
            btn_Foregrounding_deltacolor.setVisibility(View.GONE);
        }
        else
        {

            if (myV.getEditingMode() == myV.MODE_POLYGON)
            {
                findViewById(R.id.btnPolygon).setBackground(getDrawable(R.drawable.btn_polygon));
                btn_backgrounding.setVisibility(View.GONE);
                btn_foregrounding.setVisibility(View.GONE);
            }
            else if (myV.getEditingMode() == myV.MODE_LINE)
            {
                findViewById(R.id.btnLine).setBackground(getDrawable(R.drawable.btn_line));
                btn_backgrounding.setVisibility(View.GONE);
                btn_foregrounding.setVisibility(View.GONE);
                sb_thickness.setVisibility(View.GONE);
                tv_thickness.setVisibility(View.GONE);
            }
            else if (myV.getEditingMode() == myV.MODE_DELTACOLOR)
            {
                findViewById(R.id.btnDeltacolor).setBackground(getDrawable(R.drawable.btn_deltacolor));
                btn_Backgrounding_deltacolor.setVisibility(View.GONE);
                btn_Foregrounding_deltacolor.setVisibility(View.GONE);
                sb_deltacolor.setVisibility(View.GONE);
                tv_deltacolor.setVisibility(View.GONE);
            }
            else if (myV.getEditingMode() == myV.MODE_HAIR)
                findViewById(R.id.btnHair).setBackground(getDrawable(R.drawable.btn_hair));
            else if (myV.getEditingMode() == myV.MODE_THRESHOLD)
            {
                findViewById(R.id.btnThreshold).setBackground(getDrawable(R.drawable.btn_threshold));
                sb_threshold.setVisibility(View.GONE);
                tv_threshold.setVisibility(View.GONE);
            }

            findViewById(id).setBackground(getDrawable(drawablePressId));
            myV.setEditingMode(mode);

            if (myV.getEditingMode() == myV.MODE_POLYGON)
            {
                btn_backgrounding.setVisibility(View.VISIBLE);
                btn_foregrounding.setVisibility(View.VISIBLE);
            }
            else if (mode == myV.MODE_LINE)
            {
                btn_backgrounding.setVisibility(View.VISIBLE);
                btn_foregrounding.setVisibility(View.VISIBLE);
                sb_thickness.setVisibility(View.VISIBLE);
                tv_thickness.setVisibility(View.VISIBLE);
                int tmp = sb_threshold.getProgress() + 1;
                tv_thickness.setText(" Thickness : " + tmp);
            }
            else if (mode == myV.MODE_THRESHOLD)
            {
                sb_threshold.setVisibility(View.VISIBLE);
                tv_threshold.setVisibility(View.VISIBLE);
                int tmp = sb_threshold.getProgress() + 1;
                tv_threshold.setText(" Threshold : " + tmp);
            }
            else if (mode == myV.MODE_DELTACOLOR)
            {
                btn_Backgrounding_deltacolor.setVisibility(View.VISIBLE);
                btn_Foregrounding_deltacolor.setVisibility(View.VISIBLE);
                sb_deltacolor.setVisibility(View.VISIBLE);
                tv_deltacolor.setVisibility(View.VISIBLE);
                int tmp = sb_threshold.getProgress() + 1;
                tv_thickness.setText(" Tolerance : " + tmp);
            }
        }

    }

    public void StartProgressDialog()
    {
        ProgressDialogCnt = false;
        CheckTypesTask task = new CheckTypesTask();
        task.execute();
        Log.d(MainActivity.TAG, "StartProgressDialog : " + ProgressDialogCnt);
    }

    public void StopProgressDialog()
    {
        ProgressDialogCnt = true;

        myV.arVertex.clear();
        myV.mAftRetouching = MainActivity.strSuffixAfterRetouch;
        myV.FileLoad();

        Log.d(MainActivity.TAG , "StopProgressDialog : " + ProgressDialogCnt);
    }


    private class CheckTypesTask extends AsyncTask<Void, Void, Void>
    {

        ProgressDialog asyncDialog = new ProgressDialog(
                ImageEditingActivity.this);

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
                Log.d(MainActivity.TAG , "doInBackground1 : " + ProgressDialogCnt);
                while (!ProgressDialogCnt)
                {
                    //asyncDialog.setProgress(i * 30);
                    Log.d(MainActivity.TAG , "doInBackground2 : " + ProgressDialogCnt);
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
            myV.FileLoad();
            myV.invalidate();
            super.onPostExecute(result);
        }
    }

}
