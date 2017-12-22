package com.dai.t2paste;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;

import android.content.ContentResolver;
import android.content.Context;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.media.ExifInterface;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.provider.MediaStore.Images;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.Toast;

// 아래의 code들은  다음의 site을 참조한다.
// https://m.blog.naver.com/PostView.nhn?blogId=netrance&logNo=110140109549&proxyReferer=https%3A%2F%2Fwww.google.co.kr%2F

public class ImageGridAdapter extends BaseAdapter
{

    Context context = null;

    //-----------------------------------------------------------
    // imageIDs는 이미지 파일들의 리소스 ID들을 담는 배열입니다.
    // 이 배열의 원소들은 자식 뷰들인 ImageView 뷰들이 무엇을 보여주는지를
    // 결정하는데 활용될 것입니다.
    int[] imageIDs = null;

    ArrayList lst = new ArrayList();

    int THUMBNAIL_SIZE = 8;

    public int exifDegree;



    public ImageGridAdapter(Context context)
    {
        //public ImageGridAdapter(Context context, int[] imageIDs) {
        this.context = context;
        //this.imageIDs = imageIDs;

        //String tmp[] = getTitleList();
        Log.d(MainActivity.TAG, "tmp " + getTitleList());
    }

    public int getCount()
    {
        //return (null != imageIDs) ? imageIDs.length : 0;
        Log.d(MainActivity.TAG, "getCount() lst.size(): " + lst.size());
        return lst.size();
    }

    public Object getItem(int position)
    {
        //return (null != imageIDs) ? imageIDs[position] : 0;
        Log.d(MainActivity.TAG, "getItem(int position): " + position);
        return null;
    }

    public long getItemId(int position)
    {
        //return position;
        Log.d(MainActivity.TAG, "getItemId(int position) " + position);
        return 0;
    }

    public void getFileImg()
    {

    }

    public String[] getTitleList() //알아 보기 쉽게 메소드 부터 시작합니다.
    {
        try
        {
            FilenameFilter fileFilter = new FilenameFilter()  //이부분은 특정 확장자만 가지고 오고 싶을 경우 사용하시면 됩니다.
            {
                @Override
                public boolean accept(File dir, String name)
                {
                    Boolean ret =  name.endsWith(MainActivity.strSuffixAfterRetouch); //이 부분에 사용하고 싶은 확장자를 넣으시면 됩니다.
                    return ret ;


                    // 아래의 파일 확장자로 파일이 졶재해야 하나, 현재는 생성되지 않으므로 표시할 이미지가 없다. --> 수정 필요.
//					return name.endsWith("_yuvN.jpg");
                } //end accept
            };
            //File file = new File("/sdcard/A"); //경로를 SD카드로 잡은거고 그 안에 있는 A폴더 입니다. 입맛에 따라 바꾸세요.
            File file = new File(MainActivity.SaveDir);

            File[] files = file.listFiles(fileFilter);//위에 만들어 두신 필터를 넣으세요. 만약 필요치 않으시면 fileFilter를 지우세요.
            String[] titleList = new String[files.length]; //파일이 있는 만큼 어레이 생성했구요
            for (int i = 0; i < files.length; i++)
            {
                titleList[i] = files[i].getName();    //루프로 돌면서 어레이에 하나씩 집어 넣습니다.
                lst.add(files[i].getName());
                Log.d(MainActivity.TAG, "tmp " + titleList[i] + ", i:" + i);
            }

            return titleList;
        }
        catch (Exception e)
        {
            Log.d(MainActivity.TAG, e.getMessage());
            return null;
        }
    }

    public View getView(int position, View convertView, ViewGroup parent)
    {

        ImageView imageView = null;

        if (null != convertView)
            imageView = (ImageView) convertView;
        else
        {
            //---------------------------------------------------------------
            // GridView 뷰를 구성할 ImageView 뷰의 비트맵을 정의합니다.
            // 그리고 그것의 크기를 320*240으로 줄입니다.
            // 크기를 줄이는 이유는 메모리 부족 문제를 막을 수 있기 때문입니다.

            //Bitmap bmp;
            //= BitmapFactory.decodeResource(context.getResources(), imageIDs[position]);
            //bmp = Bitmap.createScaledBitmap(bmp, 320, 240, false);            
            /*
            File imgFile;
            imgFile = new  File( MainActivity.SaveDir+(String) lst.get(position) );
            Log.d("test123", "getView : "+(String) lst.get(position));
           //if(imgFile.exists()) 
            {
            	bmp = BitmapFactory.decodeFile(imgFile.getAbsolutePath());
            	bmp = Bitmap.createScaledBitmap(bmp, bmp.getWidth()/4, bmp.getHeight()/4, false);
            }
            */
            //Uri uri = Uri.fromFile(new File( "/sdcard/Pictures/PASTE/"+(String) lst.get(position) ));
            /*
            Bitmap thumbnail = getThumbnail2(context, MainActivity.SaveDir+(String) lst.get(position) );
            		    
		    thumbnail = rotate(thumbnail, ImageRotation("/sdcard/Pictures/PASTE/"+(String) lst.get(position)) ); ////GN
		    Log.d("test123", "exifDegree : "+exifDegree+"path : "+MainActivity.SaveDir+(String) lst.get(position) );
		    */

            File file = new File(MainActivity.SaveDir, (String) lst.get(position));
            BitmapFactory.Options bmOptions = new BitmapFactory.Options();
            Bitmap bitmap = BitmapFactory.decodeFile(file.getAbsolutePath(), bmOptions);
            Bitmap thumbnail = Bitmap.createScaledBitmap(bitmap, 320, 240, true);
            Bitmap thumbnail2 = rotate(thumbnail, 270) ;

            //---------------------------------------------------------------
            // GridView 뷰를 구성할 ImageView 뷰들을 정의합니다.
            // 뷰에 지정할 이미지는 앞에서 정의한 비트맵 객체입니다.
            imageView = new ImageView(context);
            imageView.setAdjustViewBounds(true);
//            imageView.setImageBitmap(thumbnail);
            imageView.setImageBitmap(thumbnail2);

            //---------------------------------------------------------------
            // 지금은 사용하지 않는 코드입니다.
            //imageView.setMaxWidth(320);
            //imageView.setMaxHeight(240);
            //imageView.setImageResource(imageIDs[position]);

            //---------------------------------------------------------------
            // 사진 항목들의 클릭을 처리하는 ImageClickListener 객체를 정의합니다.
            // 그리고 그것을 ImageView의 클릭 리스너로 설정합니다.

            ImageClickListener imageViewClickListener
                    ////= new ImageClickListener(context, imageIDs[position]);
                    = new ImageClickListener(context, (String) lst.get(position));
            imageView.setOnClickListener(imageViewClickListener);
        }

        return imageView;
    }

    public int ImageRotation(String path)
    {
        try
        {
            ExifInterface exif = new ExifInterface(path);
            int exifOrientation = exif.getAttributeInt(ExifInterface.TAG_ORIENTATION, ExifInterface.ORIENTATION_NORMAL);
            exifDegree = exifOrientationToDegrees(exifOrientation);
            return exifDegree;
        }
        catch (Exception e)
        {

        }
        return -1;
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

    private Bitmap getThumbnail2(Context context, String path)
    {
        Cursor cursor = context.getContentResolver().query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, new String[]{MediaStore.MediaColumns._ID}, MediaStore.MediaColumns.DATA + "=?",
                new String[]{path}, null);
        if (cursor != null && cursor.moveToFirst())
        {
            int id = cursor.getInt(cursor.getColumnIndex(MediaStore.MediaColumns._ID));
            cursor.close();
            return MediaStore.Images.Thumbnails.getThumbnail(context.getContentResolver(), id, MediaStore.Images.Thumbnails.MINI_KIND, null);
        }

        cursor.close();
        return null;
    }


}
