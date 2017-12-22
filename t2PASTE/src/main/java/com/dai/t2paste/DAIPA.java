package com.dai.t2paste;

////import android.content.res.AssetManager;
////import android.graphics.Bitmap;

public class DAIPA
{
    static
    {
        System.loadLibrary("T2PASTE");
    }


    //Native method

    //public native String getNativeMsg(AssetManager asset, Bitmap denseDMapBmp, Bitmap ForegroundBmp);
    //public native void nativeCreated(AssetManager asset);

    public native String TEST(String fileName, int orinetation, byte[] imgN, byte[] imgF);

    public native void retouchingDrag(int x, int y, int flags);

    public native void thresControl(int x1, int y1, int x2, int y2, double Lthreshold);

    public native void setPolygon(int[] arrayX, int[] arrayY, boolean bf);

    public native void setLine(int x1, int y1, int x2, int y2, boolean bf, int thickness);

    //public native void setLine(float x1, float y1, float x2, float y2, boolean bf, int thickness);
    public native void setDeltaColor(int x1, int y1, boolean bf, int tolerance);

    public native void setPaintingBackground(int color); // Blue:1 Green:2 Red:3


}
