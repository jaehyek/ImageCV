package com.dai.t2paste;

import java.io.File;
import java.util.ArrayList;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.DashPathEffect;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Paint.Cap;
import android.graphics.Path;
import android.graphics.Rect;
import android.graphics.RectF;
import android.media.ExifInterface;
import android.os.AsyncTask;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

public class ImageDisplayView extends View
{

    public String mfileName;
    public String mAftRetouching;
    public File imgFile;
    public Bitmap bm;
    public boolean displaySw = false;
    public int imgH, imgW;
    ArrayList<Vertex> arVertex;
    float mx = -1;
    float my = -1;
    public int TouchEventFlag = 2;
    DAIPA daipa;
    public int exifDegree = 0;
    ////public boolean ActionUpPress = false;

    private int CV_EVENT_FLAG_LBUTTON = 1;
    private int CV_EVENT_FLAG_RBUTTON = 2;

    // Blue:1 Green:2 Red:3
    private int CV_BG_BLUE = 1;
    private int CV_BG_GREEN = 2;
    private int CV_BG_RED = 3;
    int bg_color = 2;

    public final int MODE_NO = 0;
    public final int MODE_POLYGON = 1;
    public final int MODE_LINE = 2;
    public final int MODE_DELTACOLOR = 3;
    public final int MODE_HAIR = 4;
    public final int MODE_THRESHOLD = 5;

    boolean Backgrounding = true;

    int editingMode = 0;

    float bitmapDrawLeft = 0.0f;
    float bitmapDrawTop = 0.0f;
    float touchMovePointX = 0.0f;
    float touchMovePointY = 0.0f;
    float touchDownPointX = 0.0f;
    float touchDownPointY = 0.0f;
    float bitmapRatioX = 0.0f;
    float bitmapRatioY = 0.0f;
    float touchCursorX = 0.0f;
    float touchCursorY = 0.0f;

    float cursorOrgX = 0.0f;
    float cursorOrgY = 0.0f;

    float cursorOrgX_First = 0.0f;
    float cursorOrgY_First = 0.0f;

    float Cmx = 0.0f;
    float Cmy = 0.0f;

    float RectStartX = 0.0f;
    float RectStartY = 0.0f;
    float RectEndX = 0.0f;
    float RectEndY = 0.0f;

    float LineStartX = 0.0f;
    float LineStartY = 0.0f;
    float LineEndX = 0.0f;
    float LineEndY = 0.0f;

    float DeltaColorX = 0.0f;
    float DeltaColorY = 0.0f;

    float Mx = 0.0f;
    float My = 0.0f;
    float Mx2 = 0.0f;
    float My2 = 0.0f;
    float lineLen = 1.0f;
    float reSizeLen = 1.0f;
    boolean multiTouchStart = false;
    Context myContext;

    int manualMode = 0; // init
    int screenDisplayH = 0;
    int screenDisplayW = 0;

    float LineStrokeWidth = 1.0f;
    int DeltaColorTolerance = 0;

    boolean PolygonDone = false;
    boolean Line1Done = false;

    int line_color = Color.RED;

    boolean Hairesult = false;

    //public int mx, my;
    public ImageDisplayView(Context context)
    {
        super(context);
        myContext = context;
        Log.d(MainActivity.TAG, "DrawingView1");
        //invalidate();
    }

    public ImageDisplayView(Context context, AttributeSet attrs, int defStyle)
    {
        super(context, attrs, defStyle);
        myContext = context;
        Log.d(MainActivity.TAG, "DrawingView2");
    }

    public ImageDisplayView(Context context, AttributeSet attrs)
    {
        super(context, attrs);
        myContext = context;
        Log.d(MainActivity.TAG, "DrawingView3");
        arVertex = new ArrayList<Vertex>();
        daipa = new DAIPA();
    }

    public void ImageMoveAction(float x, float y)
    {

        bitmapDrawLeft = bitmapDrawLeft + (x - touchDownPointX);
        bitmapDrawTop = bitmapDrawTop + (y - touchDownPointY);

        touchDownPointX = x;
        touchDownPointY = y;
    }

    public void setBackgrounding(boolean b)
    {
        Backgrounding = b;
    }

    public boolean getBackgrounding()
    {
        return Backgrounding;
    }

    public void setEditingMode(int b)
    {
        editingMode = b;

        switch (editingMode)
        {
            case MODE_POLYGON:
                cursorOrgX = screenDisplayW / 2;
                cursorOrgY = screenDisplayH / 2;
                touchCursorX = cursorOrgX;
                touchCursorY = cursorOrgY;
                arVertex.clear();
                invalidate();
                break;

            case MODE_LINE:
                cursorOrgX = screenDisplayW / 2;
                cursorOrgY = screenDisplayH / 2;
                touchCursorX = cursorOrgX;
                touchCursorY = cursorOrgY;
                LineStartX = 0.0f;
                LineStartY = 0.0f;
                LineEndX = 0.0f;
                LineEndY = 0.0f;
                invalidate();
                break;

            //
            case MODE_DELTACOLOR:
                cursorOrgX = screenDisplayW / 2;
                cursorOrgY = screenDisplayH / 2;
                touchCursorX = cursorOrgX;
                touchCursorY = cursorOrgY;
                arVertex.clear();
                invalidate();
                break;

            case MODE_HAIR:
                cursorOrgX = screenDisplayW / 2;
                cursorOrgY = screenDisplayH / 2;
                touchCursorX = cursorOrgX;
                touchCursorY = cursorOrgY;
                arVertex.clear();
                invalidate();
                break;

            case MODE_THRESHOLD:
                cursorOrgX = screenDisplayW / 2;
                cursorOrgY = screenDisplayH / 2;
                touchCursorX = cursorOrgX;
                touchCursorY = cursorOrgY;
                arVertex.clear();
                invalidate();
                break;

            case MODE_NO:
                invalidate();
                break;
        }
    }

    public int getEditingMode()
    {
        return editingMode;
    }

    public void ImageResizeAction(float x1, float y1, float x2, float y2)
    {

        float lineLenTemp = (float) Math.sqrt(Math.pow(x2 - x1, 2) + Math.pow(y2 - y1, 2));

        if (lineLenTemp / lineLen > 5)
            lineLenTemp = lineLen * 5;
        ////Log.d(MainActivity.TAG, "reSizeLen : "+reSizeLen+" lineLen : "+lineLen+" lineLenTemp : "+lineLenTemp);

        //bitmapDrawLeft = bitmapDrawLeft - (((x2-x1)/2.0f) - touchDownPointX);
        //bitmapDrawTop = bitmapDrawTop - (((y2-y1)/2.0f) - touchDownPointY);

        //bitmapDrawLeft =  (((x2-x1)/2.0f) - touchDownPointX);
        //bitmapDrawTop =  (((y2-y1)/2.0f) - touchDownPointY);

        Mx = ((x2 - x1) / 2.0f) + x1;
        My = ((y2 - y1) / 2.0f) + y1;

        Cmx = (Mx - bitmapDrawLeft) / (bm.getWidth() * reSizeLen);
        Cmy = (My - bitmapDrawTop) / (bm.getHeight() * reSizeLen);

        ////bitmapDrawLeft = bitmapDrawLeft - (((bm.getWidth()*(lineLenTemp / lineLen)) * (Cmx / (bm.getWidth()*reSizeLen)) ))/2 ;
        ////bitmapDrawTop = bitmapDrawTop - (((bm.getHeight()*(lineLenTemp / lineLen)) * (Cmy / (bm.getHeight()*reSizeLen)) ))/2 ;

        bitmapRatioX = ((bm.getWidth() * (lineLenTemp / lineLen)) * Cmx);
        bitmapRatioY = ((bm.getHeight() * (lineLenTemp / lineLen)) * Cmy);

        bitmapDrawLeft = Mx - ((bm.getWidth() * (lineLenTemp / lineLen)) * Cmx);
        bitmapDrawTop = My - ((bm.getHeight() * (lineLenTemp / lineLen)) * Cmy);

        ////if(multiTouchStart == false)
        {
            bitmapDrawLeft = bitmapDrawLeft - (Mx2 - Mx);
            bitmapDrawTop = bitmapDrawTop - (My2 - My);
        }

        reSizeLen = lineLenTemp / lineLen;

        My2 = My;
        Mx2 = Mx;

        Log.d(MainActivity.TAG, "((x2-x1)/2.0f : " + ((x2 - x1) / 2.0f + ", (int)(bm.getWidth()*reSizeLen))  :" + (int) (bm.getWidth() * reSizeLen)));
    }

    public void setTouchDownPoint(MotionEvent event, int pointerCount)
    {

        switch (pointerCount)
        {
            case 1:
                touchDownPointX = event.getX();
                touchDownPointY = event.getY();
                break;

            case 2:
                touchDownPointX = event.getX(1);
                touchDownPointY = event.getY(1);
                break;
        }

        multiTouchStart = false;
    }

    public boolean onTouchEvent(MotionEvent event)
    {
        super.onTouchEvent(event);

        switch (getEditingMode())
        {
            case MODE_POLYGON:
                modePolygon(event.getAction(), event);
                break;

            case MODE_HAIR:
                modeHair(event.getAction(), event);
                break;

            case MODE_LINE:
                modeLine(event.getAction(), event);
                break;

            case MODE_DELTACOLOR:
                modeDeltacolor(event.getAction(), event);
                break;

            case MODE_THRESHOLD:
                modeThreshold(event.getAction(), event);
                break;
        }

/*		
		final int pointerCount = event.getPointerCount();
		float lineLenTemp = 0.0f;
		if(getEditingMode() == MODE_POLYGON)
		////if(manualMode == 1) 
		{
			if(event.getAction() == MotionEvent.ACTION_MOVE) {
				touchCursorX = event.getX();
				touchCursorY = event.getY();
			}
			else if(event.getAction() == MotionEvent.ACTION_DOWN) {				
				
			}
			else if(event.getAction() == MotionEvent.ACTION_UP) {
				arVertex.add(new Vertex(event.getX(), event.getY(), false));				

			}
			invalidate();
		}
		else if(event.getAction() == MotionEvent.ACTION_MOVE) {
			
			if(pointerCount == 1) {
				Log.d(MainActivity.TAG, "-------- ACTION_MOVE pointerCount : "+pointerCount);
				if(multiTouchStart == true)
					setTouchDownPoint(event, pointerCount);
				ImageMoveAction(event.getX(), event.getY());
				//multiTouchStart = false;
			}
			else if(pointerCount == 2) {
				
				lineLenTemp = (float)Math.sqrt(Math.pow(event.getX(1)-event.getX(0), 2)+Math.pow(event.getY(1)-event.getY(0), 2));
				if(multiTouchStart == false) {					
					lineLen = lineLenTemp / reSizeLen;// (float)Math.sqrt(Math.pow(event.getX(1)-event.getX(0), 2)+Math.pow(event.getY(1)-event.getY(0), 2));
					Mx2 = ((event.getX(1)-event.getX(0))/2.0f)+event.getX(0);
					My2 = ((event.getY(1)-event.getY(0))/2.0f)+event.getY(0);
					ImageResizeAction(event.getX(0), event.getY(0), event.getX(1), event.getY(1));
					multiTouchStart = true;
				
					Log.d(MainActivity.TAG, "-------- lineLen : "+lineLen);
				}
				else
					ImageResizeAction(event.getX(0), event.getY(0), event.getX(1), event.getY(1));				
			}
			
			invalidate();
			////Toast.makeText(myContext, "!!!!", Toast.LENGTH_LONG).show();
		}
		else if(event.getAction() == MotionEvent.ACTION_DOWN) {
			Log.d(MainActivity.TAG, "-------- ACTION_DOWN pointerCount : "+pointerCount);
			
			setTouchDownPoint(event, pointerCount);
		}
		else if(event.getAction() == MotionEvent.ACTION_POINTER_UP) {
			Log.d(MainActivity.TAG, "-------- ACTION_POINTER_UP pointerCount : "+pointerCount);
			
			setTouchDownPoint(event, pointerCount);
		}
		else if(event.getAction() == MotionEvent.ACTION_UP) {
			Log.d(MainActivity.TAG, "-------- ACTION_UP pointerCount : "+pointerCount);
			
			setTouchDownPoint(event, pointerCount);
		}
	*/

        ////return false; // 누를 때 이외의 이벤트는 무시
        return true;
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec)
    {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);

        DisplayMetrics metrics = getResources().getDisplayMetrics();
        Log.v("CustomView-density", "" + metrics.density);

        //모드를 출력해 보자.
        int widthMode = View.MeasureSpec.getMode(widthMeasureSpec);
        int heightMode = View.MeasureSpec.getMode(heightMeasureSpec);
        printMode("width mode : ", widthMode);
        printMode("height mode : ", heightMode);

        //측정된 폭과 높이를 출력해 보자 
        int width = View.MeasureSpec.getSize(widthMeasureSpec);
        int height = View.MeasureSpec.getSize(heightMeasureSpec);
        Log.v("CustomView-onMeasure", "width : " + width + " height : " + height);
        Log.d(MainActivity.TAG, "widthMeasureSpec:" + widthMeasureSpec + " heightMeasureSpec:" + heightMeasureSpec);
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom)
    {
        Log.v("CustomView-onLayout", "rect : (x, y, w, h) : " + left + " " + top + " " + (right - left) + " " + (bottom - top));
    }

    private void printMode(String tag, int mode)
    {
        switch (mode)
        {
            case MeasureSpec.AT_MOST:
                Log.v("CustomView-MeasureSpec", tag + " AT_MOST");
                break;
            case MeasureSpec.EXACTLY:
                Log.v("CustomView-MeasureSpec", tag + " EXACTLY");
                break;
            case MeasureSpec.UNSPECIFIED:
                Log.v("CustomView-MeasureSpec", tag + " UNSPECIFIED");
                break;
        }
    }

    @Override
    protected void onDraw(Canvas canvas)
    {
        /// view 에 그림을 그릴 때, call된다.
        int left = getLeft();
        int top = getTop();
        int width = getWidth();
        int height = getHeight();
        int mwidth = getMeasuredWidth();
        int mheight = getMeasuredHeight();

        Log.v("CustomView-onDraw", "rect : (x, y, w, h, mw, mh) : " + left + " " + top + " " + width + " " + height + " " + mwidth + " " + mheight);


        Log.d(MainActivity.TAG, "onDraw " + mfileName + " " + mAftRetouching);
        ////Log.d(MainActivity.TAG, "bitmapDrawLeft:"+bitmapDrawLeft+", bitmapDrawTop:" + bitmapDrawTop + " displaySw:"+displaySw);

        //super.onDraw(canvas);

        DashPathEffect dashPath = new DashPathEffect(new float[]{10, 4}, 1);
        Paint paint2 = new Paint();
        paint2.setColor(line_color);
        paint2.setStyle(Paint.Style.STROKE); // 선이 그려지도록
        paint2.setStrokeWidth(5f); // 선의 굵기 지정
        paint2.setPathEffect(dashPath);
        paint2.setTextSize(30);
        paint2.setAntiAlias(true);

        Paint paint = new Paint();
        paint.setColor(Color.RED);
        paint.setTextSize(30);
        paint.setAntiAlias(true);
        paint.setStrokeWidth(LineStrokeWidth); // 선의 굵기 지정
        paint.setStrokeCap(Cap.ROUND);
        paint.setStyle(Paint.Style.STROKE); // 선이 그려지도록

        Paint paintHair = new Paint();
        paintHair.setColor(Color.RED);
        paintHair.setTextSize(30);
        paintHair.setAntiAlias(true);
        paintHair.setStrokeWidth(100); // 선의 굵기 지정
        paintHair.setStrokeCap(Cap.ROUND);
        paintHair.setStyle(Paint.Style.STROKE); // 선이 그려지도록

        //원
        //canvas.drawCircle(0, 0, 1000, paint);
        //canvas.drawText("Circle", 200, 190, paint);
        //canvas.drawRect(0, 0, 10, 10, paint);
        if (displaySw)
        {
            //if(TouchEventFlag == CV_EVENT_FLAG_RBUTTON)
            ////canvas.drawBitmap(bm, 0, 0,paint);

            ////final Bitmap newBm = Bitmap.createScaledBitmap(bm, (int)(bm.getWidth()*reSizeLen), (int)(bm.getHeight()*reSizeLen), true);

            Log.d(MainActivity.TAG, "bitmapDrawLeft:" + bitmapDrawLeft + ", bitmapDrawTop:" + bitmapDrawTop);


            Rect BitmapDisplay = new Rect();
            BitmapDisplay.left = (bitmapDrawLeft >= 0) ? 0 : (int) (bitmapDrawLeft / reSizeLen) * (-1);
            BitmapDisplay.top = (bitmapDrawTop >= 0) ? 0 : (int) (bitmapDrawTop / reSizeLen) * (-1);
            BitmapDisplay.right = ((bm.getWidth() - bitmapDrawLeft) / reSizeLen <= screenDisplayW) ? bm.getWidth() : (int) (BitmapDisplay.left + (screenDisplayW / reSizeLen));
            BitmapDisplay.bottom = ((bm.getHeight() - bitmapDrawTop) / reSizeLen <= screenDisplayH) ? bm.getHeight() : (int) (BitmapDisplay.top + (screenDisplayH / reSizeLen));


            Rect ScreenDisplay = new Rect();
            ScreenDisplay.left = (bitmapDrawLeft <= 0) ? 0 : (int) bitmapDrawLeft;
            ScreenDisplay.top = (bitmapDrawTop <= 0) ? 0 : (int) bitmapDrawTop;
            ScreenDisplay.right = (((bm.getWidth() - bitmapDrawLeft) / reSizeLen) >= screenDisplayW) ? screenDisplayW : (int) (bm.getWidth() * reSizeLen) + (int) bitmapDrawLeft;
            ScreenDisplay.bottom = (((bm.getHeight() - bitmapDrawTop) / reSizeLen) >= screenDisplayH) ? screenDisplayH : (int) (bm.getHeight() * reSizeLen) + (int) bitmapDrawTop;


            ////canvas.drawBitmap(bm, new Rect(0, 0, (int)(bm.getWidth()*reSizeLen), (int)(bm.getHeight()*reSizeLen)), new Rect(ScreenDisplay.left, ScreenDisplay.top, screenDisplayW, screenDisplayH-320), paint);

            canvas.drawBitmap(bm, BitmapDisplay, ScreenDisplay, paint);


            ////Log.d(MainActivity.TAG , "bm.getWidth():"+bm.getWidth()+", bm.getHeight():" + bm.getHeight() +", bitmapDrawLeft:"+bitmapDrawLeft+", bitmapDrawTop:" + bitmapDrawTop +"reSizeLen:"+reSizeLen);
            Log.d(MainActivity.TAG, "S.left:" + ScreenDisplay.left + ", S.top:" + ScreenDisplay.top + ", S.right:" + ScreenDisplay.right + ", S.bottom:" + ScreenDisplay.bottom);
            Log.d(MainActivity.TAG, "B.left:" + BitmapDisplay.left + ", B.top:" + BitmapDisplay.top + ", B.right:" + BitmapDisplay.right + ", B.bottom:" + BitmapDisplay.bottom);
            ////	canvas.drawBitmap(newBm, bitmapDrawLeft, bitmapDrawTop, paint);

            ////canvas.drawCircle(bitmapDrawLeft, bitmapDrawTop, 50, paint);
            ////canvas.drawCircle(bitmapRatioX, bitmapRatioY, 50, paint);
            ////canvas.drawText("arVertex.size():"+arVertex.size()+" Mx:"+Mx+", My:"+My+", reSizeLen:"+reSizeLen, Mx, My, paint);
            //if((mx != -1) && (mx != -1))
            //canvas.drawCircle(mx, my, 50, paint);

            //for(int i2=0; i2<arVertex.size(); i2++)
            //	canvas.drawCircle(arVertex.get(i2).x, arVertex.get(i2).y, 50, paint);

            Log.d(MainActivity.TAG, "getEditingMode() : " + getEditingMode());
            switch (getEditingMode())
            {

                case MODE_POLYGON:
                    //if(manualMode == 1)
                {
                    Path path = new Path();
                    path.reset();
                    //Mouse cursor
                    canvas.drawCircle(touchCursorX, touchCursorY, 30, paint2);
                    canvas.drawLine(touchCursorX - 30, touchCursorY, touchCursorX + 30, touchCursorY, paint2);
                    canvas.drawLine(touchCursorX, touchCursorY - 30, touchCursorX, touchCursorY + 30, paint2);
                    for (int i = 0; i < arVertex.size(); i++)
                    {
                        //canvas.drawLine(arVertex.get(i).x, arVertex.get(i).y, arVertex.get(i+1).x, arVertex.get(i+1).y, paint2);

                        if (i == 0)
                            path.moveTo(arVertex.get(i).x, arVertex.get(i).y);
                        else
                            path.lineTo(arVertex.get(i).x, arVertex.get(i).y);
                    }
                    if (arVertex.size() > 0)
                    {
                        path.lineTo(touchCursorX, touchCursorY);
                        if (PolygonDone == true)
                            path.lineTo(arVertex.get(0).x, arVertex.get(0).y);
                    }


                    //if(arVertex.size() > 0)
                    //	canvas.drawLine(arVertex.get(arVertex.size()-1).x, arVertex.get(arVertex.size()-1).y, touchCursorX, touchCursorY, paint2);

                    canvas.drawPath(path, paint2);
                }
                break;

                case MODE_DELTACOLOR:
                    break;

                case MODE_LINE:

                    //canvas.drawLine(LineStartX, LineStartY, LineEndX, LineEndY, paint);

                    //Mouse cursor
                    canvas.drawCircle(touchCursorX, touchCursorY, 30, paint2);
                    canvas.drawLine(touchCursorX - 30, touchCursorY, touchCursorX + 30, touchCursorY, paint2);
                    canvas.drawLine(touchCursorX, touchCursorY - 30, touchCursorX, touchCursorY + 30, paint2);

                    //if(Line1Done == true)
                {
                    double d = LineStrokeWidth / 2;
                    double a = (LineEndY - LineStartY) / (double) (LineEndX - LineStartX);

                    float x1 = (float) (LineStartX - a * d / Math.sqrt(a * a + 1) + 0.5);
                    float y1 = (float) (LineStartY + d / Math.sqrt(a * a + 1) + 0.5);
                    float x2 = (float) (LineEndX - a * d / Math.sqrt(a * a + 1) + 0.5);
                    float y2 = (float) (LineEndY + d / Math.sqrt(a * a + 1) + 0.5);

                    Log.d(MainActivity.TAG, "MODE_LINE : x1:" + x1 + " y1:" + y1 + " x2:" + x2 + " y2:" + y2);

                    Path path = new Path();
                    path.reset();

                    path.moveTo(x1, y1);
                    path.lineTo(x2, y2);

                    float x1_ = (float) (LineStartX + a * d / Math.sqrt(a * a + 1) + 0.5);
                    float y1_ = (float) (LineStartY - d / Math.sqrt(a * a + 1) + 0.5);
                    float x2_ = (float) (LineEndX + a * d / Math.sqrt(a * a + 1) + 0.5);
                    float y2_ = (float) (LineEndY - d / Math.sqrt(a * a + 1) + 0.5);

                    float degree2 = (float) ((Math.atan2((double) (LineStartY - LineEndY), (double) (LineEndX - LineStartX)) * 180.0) / 3.1415926535);

                    Log.d(MainActivity.TAG, "MODE_LINE : x1:" + x1_ + " y1:" + y1_ + " x2:" + x2_ + " y2:" + y2_ + " degree2:" + degree2);

                    path.moveTo(x1_, y1_);
                    path.lineTo(x2_, y2_);

                    path.addArc(LineEndX - (LineStrokeWidth / 2), LineEndY - (LineStrokeWidth / 2), LineEndX + (LineStrokeWidth / 2), LineEndY + (LineStrokeWidth / 2), 270 - degree2, 180);
                    path.addArc(LineStartX - (LineStrokeWidth / 2), LineStartY - (LineStrokeWidth / 2), LineStartX + (LineStrokeWidth / 2), LineStartY + (LineStrokeWidth / 2), 90 - degree2, 180);

                    canvas.drawPath(path, paint2);
                }
                break;

                case MODE_HAIR:
                    for (int i = 0; i < arVertex.size(); i++)
                    {
                        canvas.drawCircle(arVertex.get(i).x, arVertex.get(i).y, 50, paintHair);


                    }
                    break;

                case MODE_THRESHOLD:
                    Paint paintThreshold = new Paint();
                    paintThreshold.setColor(line_color);
                    paintThreshold.setStyle(Paint.Style.STROKE); // 선이 그려지도록
                    paintThreshold.setStrokeWidth(5f); // 선의 굵기 지정
                    paintThreshold.setPathEffect(dashPath);
                    paintThreshold.setTextSize(30);
                    paintThreshold.setAntiAlias(true);

                    canvas.drawRect((RectStartX < RectEndX) ? RectStartX : RectEndX,
                            (RectStartY < RectEndY) ? RectStartY : RectEndY,
                            (RectStartX > RectEndX) ? RectStartX : RectEndX,
                            (RectStartY > RectEndY) ? RectStartY : RectEndY, paintThreshold);
                    break;
            }


        }
		/*
		File imgFile = new  File(MainActivity.SaveDir + mfileName + mAftRetouching);
		if(imgFile.exists()){	
			
		    //myBitmap = BitmapFactory.decodeFile(imgFile.getAbsolutePath());
			Bitmap bm = BitmapFactory.decodeFile(imgFile.getAbsolutePath());
		    //myBitmap = Bitmap.createBitmap(1500, 3000, Bitmap.Config.ARGB_8888);
			canvas.drawBitmap(bm, 0, 0,paint); 
			
		    myBitmap = bm.copy(Bitmap.Config. ARGB_8888,true); 
		    Canvas c = new Canvas(myBitmap);
		    //c.drawCircle((int)mx, (int)my, 205, paint);
		    
		    for(int i=0; i<arVertex.size(); i++) {
		    	//c.drawCircle((int)mx, (int)my, 205, paint);
		    	c.drawCircle(arVertex.get(i).x, arVertex.get(i).y, 50, paint);
				//canvas.drawLine(arVertex.get(i-1).x, arVertex.get(i-1).y, arVertex.get(i).x, arVertex.get(i).y,mPaint);
			}
		    
		    //canvas.drawBitmap(myBitmap, 0, myBitmap.getHeight(),paint);
		    ////((ImageEditingActivity) mContext).myImage.setImageBitmap(myBitmap);
		}*/
    }

    public void drawPolygon()
    {

    }

    public void setPaintingBackground()
    {

        // Blue:1 Green:2 Red:3
        if (bg_color == CV_BG_BLUE)
        {
            daipa.setPaintingBackground(CV_BG_GREEN);
            bg_color = CV_BG_GREEN;
            line_color = Color.RED;
        }
        else if (bg_color == CV_BG_GREEN)
        {
            daipa.setPaintingBackground(CV_BG_RED);
            bg_color = CV_BG_RED;
            line_color = Color.BLUE;
        }
        else if (bg_color == CV_BG_RED)
        {
            daipa.setPaintingBackground(CV_BG_BLUE);
            bg_color = CV_BG_BLUE;
            line_color = Color.GREEN;
        }
        mAftRetouching = MainActivity.strSuffixAfterRetouch;
        FileLoad();
        invalidate();
    }

    public void setPolygon(boolean bf)
    {

        int[] x = new int[arVertex.size()];
        int[] y = new int[arVertex.size()];

        for (int i = 0; i < arVertex.size(); i++)
        {
            x[i] = (int) ((arVertex.get(i).x - bitmapDrawLeft) / reSizeLen);
            y[i] = (int) ((arVertex.get(i).y - bitmapDrawTop) / reSizeLen);
            //canvas.drawLine(arVertex.get(i).x, arVertex.get(i).y, arVertex.get(i+1).x, arVertex.get(i+1).y, paint2);
        }
        daipa.setPolygon(x, y, bf);

        ////arVertex.clear();
        mAftRetouching = MainActivity.strSuffixAfterRetouch;
        FileLoad();

        PolygonDone = true;
        invalidate();
    }

    public void setDeltaColor(boolean bf)
    {

        daipa.setDeltaColor((int) ((DeltaColorX - bitmapDrawLeft) / reSizeLen), (int) ((DeltaColorY - bitmapDrawTop) / reSizeLen), bf, DeltaColorTolerance);

        mAftRetouching = MainActivity.strSuffixAfterRetouch;
        FileLoad();
        invalidate();
    }

    public void setLine(boolean bf)
    {

        if ((LineStartX == LineEndX) && (LineStartY == LineEndY))
            return;

        daipa.setLine((int) ((LineStartX - bitmapDrawLeft) / reSizeLen),
                (int) ((LineStartY - bitmapDrawTop) / reSizeLen),
                (int) ((LineEndX - bitmapDrawLeft) / reSizeLen),
                (int) ((LineEndY - bitmapDrawTop) / reSizeLen),
                bf, (int) (LineStrokeWidth / reSizeLen));

        //LineStartX = 0;
        //LineStartY = 0;
        //LineEndX = 0;
        //LineEndY = 0;
        mAftRetouching = MainActivity.strSuffixAfterRetouch;
        FileLoad();
        invalidate();
    }

    public void setThreshold(int pos)
    {
        daipa.thresControl((RectStartX < RectEndX) ? (int) ((RectStartX - bitmapDrawLeft) / reSizeLen) : (int) ((RectEndX - bitmapDrawLeft) / reSizeLen),
                (RectStartY < RectEndY) ? (int) ((RectStartY - bitmapDrawTop) / reSizeLen) : (int) ((RectEndY - bitmapDrawTop) / reSizeLen),
                (RectStartX > RectEndX) ? (int) ((RectStartX - bitmapDrawLeft) / reSizeLen) : (int) ((RectEndX - bitmapDrawLeft) / reSizeLen),
                (RectStartY > RectEndY) ? (int) ((RectStartY - bitmapDrawTop) / reSizeLen) : (int) ((RectEndY - bitmapDrawTop) / reSizeLen),
                (float) pos / 20.0f);

        ////mAftRetouching = "_ThresControl_ForegroundImage_diplay.jpg";
        mAftRetouching = MainActivity.strSuffixAfterRetouch;
        FileLoad();
        invalidate();
    }

    public void modeDeltacolor(int act, MotionEvent event)
    {
        final int pointerCount = event.getPointerCount();
        switch (act)
        {
            case MotionEvent.ACTION_DOWN:
                DeltaColorX = event.getX();
                DeltaColorY = event.getY();
                setTouchDownPoint(event, pointerCount);
                break;

            case MotionEvent.ACTION_MOVE:
                DeltaColorX = event.getX();
                DeltaColorY = event.getY();
                invalidate();
                break;

            case MotionEvent.ACTION_UP:
                setTouchDownPoint(event, pointerCount);
                break;
        }
    }

    public void modePolygon(int act, MotionEvent event)
    {

        switch (act)
        {
            case MotionEvent.ACTION_MOVE:
                cursorOrgX = event.getX() - cursorOrgX_First;
                cursorOrgY = event.getY() - cursorOrgY_First;

                //touchCursorX = event.getX();
                //touchCursorY = event.getY();

                touchCursorX = touchCursorX + cursorOrgX;
                touchCursorY = touchCursorY + cursorOrgY;

                cursorOrgX_First = event.getX();
                cursorOrgY_First = event.getY();
                break;

            case MotionEvent.ACTION_DOWN:
                cursorOrgX_First = event.getX();
                cursorOrgY_First = event.getY();
                if (PolygonDone == true)
                    arVertex.clear();
                PolygonDone = false;
                break;

            case MotionEvent.ACTION_UP:
                //arVertex.add(new Vertex(event.getX(), event.getY(), false));
                arVertex.add(new Vertex(touchCursorX, touchCursorY, false));
                break;
        }
        invalidate();
    }

    public void modeLine(int act, MotionEvent event)
    {
        final int pointerCount = event.getPointerCount();
        switch (act)
        {
            case MotionEvent.ACTION_MOVE:
                cursorOrgX = event.getX() - cursorOrgX_First;
                cursorOrgY = event.getY() - cursorOrgY_First;

                touchCursorX = touchCursorX + cursorOrgX;
                touchCursorY = touchCursorY + cursorOrgY;

                cursorOrgX_First = event.getX();
                cursorOrgY_First = event.getY();

                //LineEndX = event.getX();
                //LineEndY = event.getY();

                if (Line1Done == true)
                {
                    LineEndX = touchCursorX;
                    LineEndY = touchCursorY;
                }

                else
                {
                    LineStartX = LineEndX;
                    LineStartY = LineEndY;
                }
                break;

            case MotionEvent.ACTION_DOWN:
                cursorOrgX_First = event.getX();
                cursorOrgY_First = event.getY();

                //LineStartX = event.getX();
                //LineStartY = event.getY();

                if (Line1Done == true)
                {
                    //Line1Done = true;

                    LineStartX = touchCursorX;
                    LineStartY = touchCursorY;
                    LineEndX = LineStartX;// = touchCursorX;
                    LineEndY = LineStartY;// = touchCursorY;
                }

                //setTouchDownPoint(event, pointerCount);
                break;

            case MotionEvent.ACTION_UP:
                if (Line1Done == false)
                    Line1Done = true;
                else
                    Line1Done = false;


                //setTouchDownPoint(event, pointerCount);
                break;
        }
        invalidate();
    }

    public void modeThreshold(int act, MotionEvent event)
    {
        final int pointerCount = event.getPointerCount();
        switch (act)
        {
            case MotionEvent.ACTION_DOWN:
                RectStartX = event.getX();
                RectStartY = event.getY();
                RectEndX = RectStartX;
                RectEndY = RectStartY;
                setTouchDownPoint(event, pointerCount);
                break;

            case MotionEvent.ACTION_MOVE:
                RectEndX = event.getX();
                RectEndY = event.getY();
                invalidate();
                break;

            case MotionEvent.ACTION_UP:
                setTouchDownPoint(event, pointerCount);
			/*
			daipa.thresControl((RectStartX < RectEndX) ? (int)((RectStartX - bitmapDrawLeft)/reSizeLen) : (int)((RectEndX - bitmapDrawLeft)/reSizeLen),							
					(RectStartY < RectEndY) ? (int)((RectStartY - bitmapDrawTop)/reSizeLen) : (int)((RectEndY - bitmapDrawTop)/reSizeLen),							
					(RectStartX > RectEndX) ? (int)((RectStartX - bitmapDrawLeft)/reSizeLen) : (int)((RectEndX - bitmapDrawLeft)/reSizeLen),
					(RectStartY > RectEndY) ? (int)((RectStartY - bitmapDrawTop)/reSizeLen) : (int)((RectEndY - bitmapDrawTop)/reSizeLen),
					0.1f);
			
			mAftRetouching = "_ThresControl_ForegroundImage_diplay.jpg";
        	FileLoad();
        	*/
                break;
        }

    }

    public void modeHair(int act, MotionEvent event)
    {
        final int pointerCount = event.getPointerCount();
        switch (act)
        {

            case MotionEvent.ACTION_MOVE:
                Log.d(MainActivity.TAG, "ACTION_MOVE");


                float lineLenTemp = 0.0f;

                if (pointerCount == 1)
                {
                    Log.d(MainActivity.TAG, "-------- ACTION_MOVE pointerCount : " + pointerCount);
                    if (multiTouchStart == true)
                    {
                        setTouchDownPoint(event, pointerCount);
                        arVertex.clear();
                    }
                    ////ImageMoveAction(event.getX(), event.getY());
                    //multiTouchStart = false;

                    mx = event.getX();
                    my = event.getY();
                    float resizeMx = (mx - bitmapDrawLeft) / reSizeLen;
                    float resizeMy = (my - bitmapDrawTop) / reSizeLen;

                    ////TouchEventFlag = CV_EVENT_FLAG_LBUTTON;
                    daipa.retouchingDrag((int) resizeMx, (int) resizeMy, CV_EVENT_FLAG_LBUTTON);
                    arVertex.add(new Vertex(mx, my, false));
                    Log.d(MainActivity.TAG, "x : " + (int) mx + ", y : " + (int) my);

                }
                else if (pointerCount == 2)
                {
                    arVertex.clear();
                    lineLenTemp = (float) Math.sqrt(Math.pow(event.getX(1) - event.getX(0), 2) + Math.pow(event.getY(1) - event.getY(0), 2));
                    if (multiTouchStart == false)
                    {

                        lineLen = lineLenTemp / reSizeLen;// (float)Math.sqrt(Math.pow(event.getX(1)-event.getX(0), 2)+Math.pow(event.getY(1)-event.getY(0), 2));
                        Mx2 = ((event.getX(1) - event.getX(0)) / 2.0f) + event.getX(0);
                        My2 = ((event.getY(1) - event.getY(0)) / 2.0f) + event.getY(0);
                        ImageResizeAction(event.getX(0), event.getY(0), event.getX(1), event.getY(1));
                        multiTouchStart = true;


                        Log.d(MainActivity.TAG, "-------- lineLen : " + lineLen);
                    }
                    else
                        ImageResizeAction(event.getX(0), event.getY(0), event.getX(1), event.getY(1));
                }
                break;

            case MotionEvent.ACTION_UP:
                ((ImageEditingActivity) myContext).StartProgressDialog();

                Log.d(MainActivity.TAG, "ACTION_UP : pointerCount: " + pointerCount);
                setTouchDownPoint(event, pointerCount);
                ////TouchEventFlag = CV_EVENT_FLAG_RBUTTON;

                ExampleThread thread = new ExampleThread();
                thread.start();
        	/*
        	if(arVertex.size() > 0)
        		daipa.retouchingDrag(0, 0, CV_EVENT_FLAG_RBUTTON);
        	arVertex.clear();
        	
        	mAftRetouching = MainActivity.strSuffixAfterRetouch;
        	FileLoad();
        	((ImageEditingActivity) myContext).StopProgressDialog();
        	*/
                break;

            case MotionEvent.ACTION_DOWN:
                arVertex.clear();
                setTouchDownPoint(event, pointerCount);
                break;

            case MotionEvent.ACTION_POINTER_UP:
                setTouchDownPoint(event, pointerCount);
                break;

        }
        invalidate();
    }

    public void setImage2ScreenSize(int w, int h)
    {

        screenDisplayW = w;
        screenDisplayH = h;

        float wRatio = (float) w / (float) bm.getWidth();
        float hRatio = (float) h / (float) bm.getHeight();
        if (wRatio > hRatio)
            reSizeLen = hRatio;
        else
            reSizeLen = wRatio;

        bitmapDrawLeft = (w - (bm.getWidth() * reSizeLen)) / 2;
        bitmapDrawTop = (h - (bm.getHeight() * reSizeLen)) / 2;
        Log.d(MainActivity.TAG, "wwww : " + wRatio + ", hhhh : " + hRatio);
        Log.d(MainActivity.TAG, "wwww : " + bm.getWidth() + ", hhhh : " + bm.getHeight());
        invalidate();
    }

    public boolean FileLoad()
    {
        Log.d(MainActivity.TAG, "FileLoad " + mfileName + " " + mAftRetouching);
        imgFile = new File(MainActivity.SaveDir + mfileName + mAftRetouching);


        if (imgFile.exists())
        {
            ////bm = BitmapFactory.decodeFile(imgFile.getAbsolutePath());
            ///imgH = bm.getHeight();
            ///imgW = bm.getWidth();

            ////try
            ////{
            // 이미지를 상황에 맞게 회전시킨다
            ////ExifInterface exif = new ExifInterface("/sdcard/Pictures/PASTE/PASTE_" + mfileName + mAftRetouching);
            ////int exifOrientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL);
            ////int exifDegree = exifOrientationToDegrees(exifOrientation);
            /////image = rotate(image, exifDegree);

            ////myImage.setVisibility(View.VISIBLE);
            bm = BitmapFactory.decodeFile(imgFile.getAbsolutePath());

            bm = rotate(bm, 270); ////GN

            imgH = bm.getHeight();
            imgW = bm.getWidth();

            ////}
            ////catch(Exception e)
            ////{

            ////}

            return true;
        }
        else
            return false;

        //invalidate();
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
            m.postRotate(degrees);

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

    ////@Override
	/*
	public boolean onTouchEvent(MotionEvent event)
	{
		
		switch (event.getAction()){
		
        case MotionEvent.ACTION_MOVE :
        	Log.d(MainActivity.TAG, "ACTION_MOVE");

        	mx = event.getX();
        	my = event.getY();
            //ov.update((int)my, (int)my);
        	TouchEventFlag = CV_EVENT_FLAG_LBUTTON;
        	daipa.retouchingDrag((int)mx, (int)my, CV_EVENT_FLAG_LBUTTON);
        	arVertex.add(new Vertex(mx, my, false));
        	Log.d(MainActivity.TAG, "x : " + (int)mx +", y : " + (int)my);
        	
        	break;
        case MotionEvent.ACTION_UP :
        	Log.d(MainActivity.TAG, "ACTION_UP");
        	arVertex.clear();
        	TouchEventFlag = CV_EVENT_FLAG_RBUTTON;
        	daipa.retouchingDrag(0, 0, CV_EVENT_FLAG_RBUTTON);
        	mAftRetouching = "Result_Image_AftRetouching.jpg";
        	FileLoad();        	
        	break;        	
		}		
		invalidate();
		Log.d(MainActivity.TAG, "onTouchEvent : "+event.getAction());
		return true;
	}*/
/*	
	@Override
	public boolean performClick() {
	    return super.performClick();
	}
	*/

    public class Vertex
    {
        Vertex(float ax, float ay, boolean ad)
        {
            x = ax;
            y = ay;
            Draw = ad;
        }

        float x;
        float y;
        boolean Draw;
    }


    private class ExampleThread extends Thread
    {
        private static final String TAG = "ExampleThread";

        public ExampleThread()
        {
            // 초기화 작업
        }

        public void run()
        {
            //DAIPA daipa;
            //daipa = new DAIPA();
            if (arVertex.size() > 0)
                daipa.retouchingDrag(0, 0, CV_EVENT_FLAG_RBUTTON);

            //mAftRetouching = MainActivity.strSuffixAfterRetouch;
            //FileLoad();
            ((ImageEditingActivity) myContext).StopProgressDialog();
            ////Log.i("TEST", daipa.TEST(MainActivity.SaveDir + mfileName, exifDegree ));
            Hairesult = true;
        }
    }

}