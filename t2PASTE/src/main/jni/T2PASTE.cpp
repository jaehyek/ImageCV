#include "T2PASTE.h"

#include <android/log.h>

#include <jni.h>
#include <time.h>

#include <math.h>


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/contrib/contrib.hpp>

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "guidedfilter.h"
#include "globalmatting.h"
#include "jpgexifinfo.h"

using namespace cv;
using namespace std;

int Pro_main(string fileName);

int Pro_main_(string fileName);

int Pro_mainTest(string fileName, string oFile, bool lfast_mode, bool lAdaptive_thres,
                 bool lAlign_Far2Near);

void RetouchFunc(int x, int y, int flags);

void RetouchFunc_170613(int x, int y, int flags);

void TimeCheckStart();

void TimeCheckEnd();

int Pro_main_D170217(string fileName);

void ThresControl(int x, int y, int radius, double Lthreshold);

void ThresControl_170614(Mat Dmap, int x1, int y1, int x2, int y2, double Lthreshold);

int mainD170428(string fileName);

void stopWatch();

void PaintingBackground(Mat InputImg, int Background_color);

void SaveResult(Mat Input_Image, int margin_crop_boundary);

void imageRotationSave(string name, Mat img);

void setPolygon(int *x, int *y, int n, bool bf);

////void setLine(int x1, int y1, int x2, int y2, bool bf, int thickness);
void setLine(float x1, float y1, float x2, float y2, bool bf, int thickness);

void setDeltaColor(int x1, int y1, bool bf, int tolerance);

int FF_GetPixel(int x, int y, int z);

void PutPixel(int x, int y, bool bf);

void FloodFill_8(int x, int y, int nFillColor, int Scalar1, int Scalar2, int Scalar3, bool bf);

void RetouchFunc_170623(int x, int y, int flags);

int mainD170814(string fileName, Mat imgN, Mat imgF);

const char *AndroidTag = "Native";
int rotationInfo = 0;

time_t starts;

string filePath;

int lu_x = 10000, lu_y = 10000, rd_x = 0, rd_y = 0;
Mat Result_Image;
Mat R_Trimap;
Mat RGB_Image;
Mat BackgroundImage;
////Mat AlphaMap;
Mat depthMap;
Mat thresholdDepthMap;

///int lu_x=10000,lu_y=10000,rd_x=0,rd_y=0;

// mode setting
bool Adaptive_thres = 1;

Mat alined_Original_NearFocusImage_Color;
Mat ForegroundImage;
Mat ForegroundImage_diplay;
Mat Trimap_Init;
Mat InputImage_for_retouching;
Mat AlphaMap;
Mat Binary_AlphaMap;
Mat raw_DepthMap;

Mat AlphaMap_display;

Mat aligned_NearFocusImage_Color;

Mat Old_aligned_NearFocusImage_Color;

int Orinetation = 0;

int bg_color = 2;


extern "C" {
JNIEXPORT
jstring
Java_com_dai_t2paste_DAIPA_TEST(JNIEnv *env, jobject obj, jstring fileName, jint orinetation,
                                jbyteArray imgN, jbyteArray imgF)
{
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Java_com_dai_t2paste_DAIPA_TEST START");
    time_t start, end;
    float gap;
    start = clock();

    char *nativeStr;
    const char *temp = env->GetStringUTFChars(fileName, 0);
    nativeStr = strdup(temp);
    env->ReleaseStringUTFChars(fileName, temp);


    TimeCheckStart();
    jbyte *_yuv420N = env->GetByteArrayElements(imgN, 0);
    Mat input1(1080 + 1080 / 2, 1920, CV_8UC1, (unsigned char *) _yuv420N);
    Mat out1(1080, 1920, CV_8UC3);
    cvtColor(input1, out1, CV_YUV420sp2RGB, 3); //YUV로 들어온 색상체계를 BGR로 바꾼다.
    TimeCheckEnd();

    TimeCheckStart();
    jbyte *_yuv420F = env->GetByteArrayElements(imgF, 0);
    Mat input2(1080 + 1080 / 2, 1920, CV_8UC1, (unsigned char *) _yuv420F);
    Mat out2(1080, 1920, CV_8UC3);
    cvtColor(input2, out2, CV_YUV420sp2RGB, 3); //YUV로 들어온 색상체계를 BGR로 바꾼다.
    TimeCheckEnd();

    TimeCheckStart();
    jbyte *_jpgN = env->GetByteArrayElements(imgN, 0);
    Mat jpgN(1920, 1080, CV_8UC3, (unsigned char *) _jpgN);
    Mat imgjpgN = imdecode(jpgN, IMREAD_COLOR);
    TimeCheckEnd();

    TimeCheckStart();
    jbyte *_jpgF = env->GetByteArrayElements(imgF, 0);
    Mat jpgF(1920, 1080, CV_8UC3, (unsigned char *) _jpgF);
    Mat imgjpgF = imdecode(jpgF, IMREAD_COLOR);
    TimeCheckEnd();
/*
	TimeCheckStart();
	filePath = string(nativeStr);
	imageRotationSave("_yuvN.jpg", imgjpgN); //GN_170428
	TimeCheckEnd();

	TimeCheckStart();
	filePath = string(nativeStr);
	imageRotationSave("_yuvF.jpg", imgjpgF); //GN_170428
	TimeCheckEnd();

*/

    Orinetation = orinetation;
#if 1
    mainD170814(string(nativeStr), imgjpgN, imgjpgF);
    ////Pro_main_D170217(string(nativeStr));
    //mainD170428("/sdcard/Pictures/PASTE/PASTE_20170714_171550");
    //mainD170428(string(nativeStr));
    ////mainD170428("/sdcard/Pictures/PASTE/TEST170613/PASTE_TEST_2");

#else
                                                                                                                            Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_1");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_2");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_3");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_4");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_5");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_6");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_7");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_8");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_9");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_10");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_11");
	Pro_main_D170217("/sdcard/Pictures/PASTE/TEST170207/PASTE_TEST_12");

#endif
    end = clock();

    char buff[256];            // 변환될 int형 변수를 저장할 공간
    gap = (float) (end - start) / (CLOCKS_PER_SEC);                    // 변환 시킬 int형 변수
    sprintf(buff, "time : %f sec", gap);

    //__android_log_print(ANDROID_LOG_DEBUG, "!!!", buff);
    //cout<<"수행시간 : "<<((end-begin)/CLOCKS_PER_SEC)<<endl;

    env->ReleaseByteArrayElements(imgN, _jpgN, JNI_ABORT); /* abort to not copy back contents */
    env->ReleaseByteArrayElements(imgF, _jpgF, JNI_ABORT); /* abort to not copy back contents */

    return env->NewStringUTF(buff);

}
}

void TimeCheckStart()
{
    starts = clock();
}

void TimeCheckEnd()
{
    float gap;
    time_t ends = clock();

    char buff[256];            // 변환될 int형 변수를 저장할 공간
    gap = (float) (ends - starts) / (CLOCKS_PER_SEC);                    // 변환 시킬 int형 변수
    sprintf(buff, "time : %f sec", gap);

    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "time : %f sec", gap);
}

void stopWatch()
{
    static double time = 0.0;
    static bool bCheckTime = false;
    static int timerCount = 0;
    if (!bCheckTime)
    {
        time = (double) getTickCount();
        timerCount++;
    }
    if (bCheckTime)
    {
        time = ((double) getTickCount() - time) / getTickFrequency();
        ////cout << "Time" << timerCount << ": " << time << " sec" << endl;
        __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "timerCount : %f sec", time);
    }

    bCheckTime = !bCheckTime;
}

void imageRotationSave(string name, Mat img)
{
    Mat rotatedImg;

    if (rotationInfo == 6)
    { // rot270(+inverse)

        //// Rotation
        Point2f pt(img.rows / 2.0, img.rows / 2.0);
        Mat rotate = getRotationMatrix2D(pt, 270, 1.0);
        warpAffine(img, rotatedImg, rotate, Size(img.rows, img.cols));
        //// Rotation
        imwrite(filePath + name, rotatedImg);//GN_170428
    }
    else
    {
        imwrite(filePath + name, img);
    }//GN_170428

}

int mainD170814(string fileName, Mat imgN, Mat imgF)
{
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Pro_main() START");

    filePath = fileName;

    // paramters
    int imresize_factor = 1;

    int size_window = 21;    // FMR 마스크 크기

    float thres_foreground = 0.6;   // foreground threshold 초기값

    int thres_crop_width = 0;    // 이미지 테두리 짤라라

    int r = 51;              // guided filter 크기

    double eps = 100;    // guided filter parameter

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Image read ////////////////////////////////////
    int i, j;

    String imageName_NearFocus(fileName + "_N.jpg");
    String imageName_FarFocus(fileName + "_F.jpg");
    //__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "fileName : %s", fileName);
    ////String imageName_Saveimg( "../img/TSET170403_HTC10/ID_50cm/ForegroundImage_diplay.jpg" ); //GN_170329
    ////String imageName_DepthMap( "../img/TSET170403_HTC10/ID_50cm/DepthMap.jpg" ); //GN_170329

    //GNS_170428
    rotationInfo = 6;//getRotationFromJpg(imageName_NearFocus.c_str());
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "rotationInfo : %d", rotationInfo);
    //GNE_170428

    Mat Original_NearFocusImage_Color;
    Mat Original_FarFocusImage_Color;
    Mat Original_NearFocusImage_Gray;
    Mat Original_FarFocusImage_Gray;;
    //	Mat Harf_NearFocusImage_Color;
    //	Mat Harf_FarFocusImage_Color;
    //	Mat Harf_NearFocusImage_Gray;
    //	Mat Harf_FarFocusImage_Gray;

# if 0
                                                                                                                            //GNS_170428
		Mat Temp_NearFocusImage_Color_input; // GN_170428
		Mat Temp_FarFocusImage_Color_input; // GN_170428

		if(rotationInfo == 0) { // rot270(+inverse)

			Temp_NearFocusImage_Color_input = imread( imageName_NearFocus,  IMREAD_COLOR );
			Temp_FarFocusImage_Color_input = imread( imageName_FarFocus, IMREAD_COLOR );

			if( Temp_NearFocusImage_Color_input.empty() || Temp_FarFocusImage_Color_input.empty() )
			{
				////cout <<  "Could not open or find the image" << std::endl ;
				__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Could not open or find the image");
				return -1;
			}
			//// Rotation
			Point2f pt(Temp_NearFocusImage_Color_input.rows / 2.0, Temp_NearFocusImage_Color_input.rows / 2.0);
			Mat rotate = getRotationMatrix2D(pt, 270, 1.0);
			Mat imgRotated;
			warpAffine(Temp_NearFocusImage_Color_input, Original_NearFocusImage_Color, rotate, Size(Temp_NearFocusImage_Color_input.rows,Temp_NearFocusImage_Color_input.cols) );
			warpAffine(Temp_FarFocusImage_Color_input, Original_FarFocusImage_Color, rotate,  Size(Temp_NearFocusImage_Color_input.rows,Temp_NearFocusImage_Color_input.cols) );
			//// Rotation
		}
		else {
			Original_NearFocusImage_Color = imread( imageName_NearFocus, IMREAD_COLOR );
			Original_FarFocusImage_Color = imread( imageName_FarFocus, IMREAD_COLOR );
			if( Original_NearFocusImage_Color.empty() || Original_FarFocusImage_Color.empty() )
			{
				////cout <<  "Could not open or find the image" << std::endl ;
				__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Could not open or find the image");
				return -1;
			}
		}
		//GNE_170428
#endif

    stopWatch();
    Original_NearFocusImage_Color = imgN.clone(); //imread( imageName_NearFocus, IMREAD_COLOR );
    Original_FarFocusImage_Color = imgF.clone();//imread( imageName_FarFocus, IMREAD_COLOR );
    stopWatch();

    stopWatch();
    if (Original_NearFocusImage_Color.empty() || Original_FarFocusImage_Color.empty())
    {
        ////cout <<  "Could not open or find the image" << std::endl ;
        __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Could not open or find the image");
        return -1;
    }

    cvtColor(Original_NearFocusImage_Color, Original_NearFocusImage_Gray, CV_RGB2GRAY);
    cvtColor(Original_FarFocusImage_Color, Original_FarFocusImage_Gray, CV_RGB2GRAY);
    stopWatch();
    //	resize(Original_NearFocusImage_Color, Harf_NearFocusImage_Color, Size(Original_NearFocusImage_Color.cols/imresize_factor, Original_NearFocusImage_Color.rows/imresize_factor));
    //	resize(Original_FarFocusImage_Color, Harf_FarFocusImage_Color, Size(Original_FarFocusImage_Color.cols/imresize_factor, Original_FarFocusImage_Color.rows/imresize_factor));
    //	cvtColor(Harf_NearFocusImage_Color, Harf_NearFocusImage_Gray, CV_RGB2GRAY);
    //	cvtColor(Harf_FarFocusImage_Color, Harf_FarFocusImage_Gray, CV_RGB2GRAY);

    //	namedWindow( "Near Focus Image", CV_WINDOW_AUTOSIZE );
    //	imshow( "Near Focus Image", Original_NearFocusImage_Color );

    //	namedWindow( "Far Focus Image", CV_WINDOW_AUTOSIZE );
    //	imshow( "Far Focus Image", Harf_FarFocusImage_Color );

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Image alignment ////////////////////////////////////
    stopWatch();

    vector<cv::KeyPoint> keypoints_N, keypoints_F;
    Mat descriptors_N, descriptors_F;

    ORB orb;

    OrbFeatureDetector detector(500, 1.2f, 4, 25, 0, 2, 0, 25);
    OrbDescriptorExtractor extractor;

    orb.detect(Original_NearFocusImage_Gray, keypoints_N);
    orb.compute(Original_NearFocusImage_Gray, keypoints_N, descriptors_N);

    orb.detect(Original_FarFocusImage_Gray, keypoints_F);
    orb.compute(Original_FarFocusImage_Gray, keypoints_F, descriptors_F);

    BFMatcher matcher;
    vector<DMatch> matches;
    Mat img_matches;

    matcher.match(descriptors_N, descriptors_F, matches);

    double max_dist = 0;
    double min_dist = 100;

    for (i = 0; i < descriptors_N.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    vector<cv::DMatch> good_matches;

    for (i = 0; i < descriptors_N.rows; i++)
    {
        if (matches[i].distance < (max_dist / 1.6))
        { good_matches.push_back(matches[i]); }
    }

    //	drawMatches(Harf_NearFocusImage_Gray, keypoints_N, Harf_FarFocusImage_Gray, keypoints_F, \
	//          good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
    //                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //	namedWindow( "img_matches", CV_WINDOW_FULLSCREEN );
    //	imshow("img_matches", img_matches );

    std::vector<Point2f> Gp_N;
    std::vector<Point2f> Gp_F;

    for (i = 0; i < good_matches.size(); i++)
    {
        Gp_N.push_back(keypoints_N[good_matches[i].queryIdx].pt);
        Gp_F.push_back(keypoints_F[good_matches[i].trainIdx].pt);
    }

    Mat transformMat = estimateRigidTransform(Gp_N, Gp_F, false);

    if (transformMat.dims != 0)
    {
        Size dsize = Original_NearFocusImage_Color.size();
        warpAffine(Original_NearFocusImage_Color, alined_Original_NearFocusImage_Color,
                   transformMat, dsize);
    }
    else
    {
        alined_Original_NearFocusImage_Color = Original_NearFocusImage_Color;         ////  얼라인을 못하는 경우 처리, 일단 원영상 그대로 가져옴...
    }

    //	namedWindow( "alined_Original_NearFocusImage_Color", CV_WINDOW_NORMAL );
    //	imshow( "alined_Original_NearFocusImage_Color", alined_Original_NearFocusImage_Color );

    //	namedWindow( "Harf_NearFocusImage_Color", CV_WINDOW_NORMAL );
    //	imshow( "Harf_NearFocusImage_Color", alined_Harf_NearFocusImage_Color );

    ////cout << "Image alignment" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Image alignment");
    stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// FMR-based depth extraction //////////////////////
    stopWatch();

    //	Mat alined_Harf_NearFocusImage_Color;
    Mat alined_Original_NearFocusImage_Gray;

    //	alined_Harf_NearFocusImage_Color = alined_Original_NearFocusImage_Color;
    //	Harf_FarFocusImage_Gray = Original_FarFocusImage_Gray;

    //	resize(alined_Original_NearFocusImage_Color, alined_Harf_NearFocusImage_Color, Size(alined_Original_NearFocusImage_Color.cols/imresize_factor, alined_Original_NearFocusImage_Color.rows/imresize_factor));
    cvtColor(alined_Original_NearFocusImage_Color, alined_Original_NearFocusImage_Gray,
             CV_RGB2GRAY);

    Mat grad_x, grad_y;
    Mat grad_x_square, grad_y_square;
    Mat FM_Nimg, FM_Fimg;
    Mat FM_Nfocus, FM_Ffocus;

    Sobel(alined_Original_NearFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(alined_Original_NearFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Nimg = grad_x_square + grad_y_square;
    //	boxFilter( FM_Nimg, FM_Nfocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
    GaussianBlur(FM_Nimg, FM_Nfocus, Size(size_window, size_window), 0, 0);

    Sobel(Original_FarFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(Original_FarFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Fimg = (grad_x_square + grad_y_square);
    //	boxFilter( FM_Fimg, FM_Ffocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
    GaussianBlur(FM_Fimg, FM_Ffocus, Size(size_window, size_window), 0, 0);

    float *FM_Nfocus_data = (float *) FM_Nfocus.data;
    float *FM_Ffocus_data = (float *) FM_Ffocus.data;
    float *FMRtemp;
    FMRtemp = new float[alined_Original_NearFocusImage_Gray.cols *
                        alined_Original_NearFocusImage_Gray.rows];

    int length = FM_Nfocus.rows * FM_Nfocus.cols;

    for (i = 0; i < length; i++)
    {
        if (FM_Ffocus_data[i] > FM_Nfocus_data[i])
        {
            FMRtemp[i] = 2 - FM_Nfocus_data[i] / (FM_Ffocus_data[i] + 0.000001);
        }
        else if (FM_Ffocus_data[i] < FM_Nfocus_data[i])
        {
            FMRtemp[i] = FM_Ffocus_data[i] / (FM_Nfocus_data[i] + 0.000001);
        }
        else
        {
            FMRtemp[i] = 2;
        }
    }

    ////cout << "FMR-based depth extraction" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "FMR-based depth extraction");
    stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Guided filtering ///////////////////////////////
    stopWatch();

    ////Mat aligned_NearFocusImage_Color;
    Mat FMR(alined_Original_NearFocusImage_Gray.rows, alined_Original_NearFocusImage_Gray.cols,
            CV_32F, FMRtemp);

    ////aligned_NearFocusImage_Color = alined_Original_NearFocusImage_Color; // testCode.170714
    resize(alined_Original_NearFocusImage_Color, aligned_NearFocusImage_Color,
           Size(alined_Original_NearFocusImage_Gray.cols / imresize_factor,
                alined_Original_NearFocusImage_Gray.rows / imresize_factor));
    resize(FMR, FMR, Size(FMR.cols / imresize_factor, FMR.rows / imresize_factor));

    Mat DepthMap(aligned_NearFocusImage_Color.rows, aligned_NearFocusImage_Color.cols, CV_32F);
    DepthMap = Scalar(0);

    Mat FMR_filtered = guidedFilter(aligned_NearFocusImage_Color, FMR, r, eps);
    DepthMap = (2 - FMR_filtered) / 2;
    DepthMap.convertTo(DepthMap, CV_32F);

    //	Rect rc(thres_crop_width,thres_crop_width,DepthMap_temp.cols-2*thres_crop_width,DepthMap_temp.rows-2*thres_crop_width);
    //	DepthMap_temp = DepthMap_temp(rc);
    //	DepthMap_temp.copyTo(DepthMap(rc));

    DepthMap = DepthMap * 255;
    DepthMap.convertTo(DepthMap, CV_8U);

    //	Mat cm_FMR;
    //	FMR = 255-FMR*127;
    //	FMR.convertTo(FMR,CV_8U);
    //	namedWindow( "FMR", CV_WINDOW_KEEPRATIO );
    //	applyColorMap(FMR, cm_FMR, COLORMAP_JET);
    //	imshow("FMR", cm_FMR);

    Mat cm_DepthMap;
    ////namedWindow( "DepthMap", CV_WINDOW_KEEPRATIO );
    applyColorMap(DepthMap, cm_DepthMap, COLORMAP_JET);
    ////imshow("DepthMap", cm_DepthMap);
    ////imwrite( fileName + "_DepthMap.jpg" /*imageName_DepthMap*/, cm_DepthMap);//GN_170428
    imageRotationSave("_DepthMap.jpg", cm_DepthMap); //GN_170428

    ////cout << "Guided filtering" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Guided filtering");
    stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// foreground extraction ////////////////////////////////////
    stopWatch();

    if (Adaptive_thres)
    {
        Mat Local_Binary_AlphaMap;
        Mat Local_DepthMap;

        float init_foreground_threshhold = threshold(DepthMap, Binary_AlphaMap, thres_foreground, 1,
                                                     CV_THRESH_BINARY | CV_THRESH_OTSU);

        int H = aligned_NearFocusImage_Color.rows;
        int W = aligned_NearFocusImage_Color.cols;

        int height_temp = cvRound(H / 4);
        int width_temp = cvRound(W / 8);

        int start_x, end_x, start_y, end_y;
        int local_width, local_height;

        float cnt_F, cnt_B;
        float balance_ratio;
        float Local_threshhold;

        for (int j = 0; j < H; j += cvRound(height_temp / 2))
        {
            for (int i = 0; i < W; i += cvRound(width_temp / 2))
            {
                start_x = i;
                end_x = i + width_temp;
                start_y = j;
                end_y = j + height_temp;

                if (end_x > W)
                {
                    end_x = W - 1;
                }
                if (end_y > H)
                {
                    end_y = H - 1;
                }

                local_width = end_x - start_x;
                local_height = end_y - start_y;

                Rect rc(start_x, start_y, local_width, local_height);

                Local_Binary_AlphaMap = Binary_AlphaMap(rc);

                cnt_F = 0, cnt_B = 0;
                for (int x = 0; x < Local_Binary_AlphaMap.cols; ++x)
                {
                    for (int y = 0; y < Local_Binary_AlphaMap.rows; ++y)
                    {
                        if (Local_Binary_AlphaMap.at<uchar>(y, x) == 1)
                        {
                            cnt_F = cnt_F + 1;
                        }
                        else if (Local_Binary_AlphaMap.at<uchar>(y, x) == 0)
                        {
                            cnt_B = cnt_B + 1;
                        }
                    }
                }
                balance_ratio = min(cnt_F, cnt_B) / max(cnt_F, cnt_B);

                if (balance_ratio > 0.3)
                {
                    Local_DepthMap = DepthMap(rc);

                    Local_threshhold = threshold(Local_DepthMap, Local_Binary_AlphaMap,
                                                 thres_foreground, 1,
                                                 CV_THRESH_BINARY | CV_THRESH_OTSU);

                    Local_Binary_AlphaMap.copyTo(Binary_AlphaMap(rc));
                }
            }
        }
    }
    else
    {
        threshold(DepthMap, Binary_AlphaMap, thres_foreground * 255, 1,
                  CV_THRESH_BINARY | CV_THRESH_OTSU);
    }

    AlphaMap = Binary_AlphaMap;

    //	Mat AlphaMap_display1 = AlphaMap*255;
    //	namedWindow( "AlphaMap1", CV_WINDOW_KEEPRATIO );
    //	imshow("AlphaMap1", AlphaMap_display1);

    ////cout << "foreground extraction" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "foreground extraction");
    stopWatch();

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Remove isolated blobs ////////////////////////////////////
    stopWatch();

    Mat dst1 = Mat::zeros(AlphaMap.rows, AlphaMap.cols, CV_8UC1);
    Mat dst2 = Mat::zeros(AlphaMap.rows, AlphaMap.cols, CV_8UC1);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(AlphaMap, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    int iNumSegments = contours.size();
    double area;
    double max_area = 0;
    int max_index = 0;;
    for (int i = 0; i < iNumSegments; i++)
    {
        vector<Point> approx;
        approxPolyDP(Mat(contours[i]), approx, 1.0e-10, true);
        area = fabs(cv::contourArea(approx));
        if (area > max_area)
        {
            max_area = area;
            max_index = i;
        }
    }
    for (int i = 0; i < iNumSegments; i++)
    {
        if (i == max_index)
        {
            drawContours(dst1, contours, i, 1, CV_FILLED, 8, hierarchy);
        }
        else
        {
            drawContours(dst1, contours, i, 0, CV_FILLED, 8, hierarchy);
        }
    }

    Mat invertAlphaMap;
    invertAlphaMap = -(dst1 - 1);

    findContours(invertAlphaMap, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    iNumSegments = contours.size();

    max_area = 0;
    max_index = 0;;
    for (int i = 0; i < iNumSegments; i++)
    {
        vector<Point> approx;
        approxPolyDP(Mat(contours[i]), approx, 1.0e-10, true);
        area = fabs(cv::contourArea(approx));
        if (area > max_area)
        {
            max_area = area;
            max_index = i;
        }
    }
    for (int i = 0; i < iNumSegments; i++)
    {
        if (i == max_index)
        {
            drawContours(dst2, contours, i, 0, CV_FILLED, 8, hierarchy);
        }
        else
        {
            drawContours(dst2, contours, i, 1, CV_FILLED, 8, hierarchy);
        }
    }

    //	AlphaMap = dst1+dst2;
    AlphaMap = max(dst1, dst2);

    ////Mat AlphaMap_display = AlphaMap*255;
    AlphaMap_display = AlphaMap * 255;
    ////namedWindow( "AlphaMap", CV_WINDOW_KEEPRATIO );
    ////imshow("AlphaMap", AlphaMap_display);
    ////imwrite(fileName+"_AlphaMap_display.jpg"/*imageName_Saveimg*/, AlphaMap_display);//GN_170428
    imageRotationSave("_AlphaMap_display.jpg", AlphaMap_display); //GN_170428

    ////cout << "Remove isolated blobs" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Remove isolated blobs");
    stopWatch();

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// DIsplay  ///////////////////////////////////

    int Background_color = bg_color;   // Blue:1 Green:2 Red:3

    PaintingBackground(aligned_NearFocusImage_Color, Background_color);

    ////imwrite(fileName+"_ForegroundImage_diplay.jpg"/*imageName_Saveimg*/, ForegroundImage_diplay);//GN_170428
    ////imwrite(fileName+"_Result_Image.jpg"/*imageName_Saveimg*/, ForegroundImage_diplay);//GN_170428
    imageRotationSave("_Result_Image.jpg", ForegroundImage_diplay); //GN_170428

    ////namedWindow( "ForegroundImage_diplay", CV_WINDOW_KEEPRATIO );
    ////imshow("ForegroundImage_diplay", ForegroundImage_diplay);

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Save result  ///////////////////////////////////
    int margin_crop_boundary = 20;

    SaveResult(aligned_NearFocusImage_Color, margin_crop_boundary);

    /*	Mat mat(480, 640, CV_8UC4);
	    createAlphaMat(mat);

	    vector<int> compression_params;
	    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	    compression_params.push_back(9);

	    imwrite("alpha.png", mat, compression_params);*/

    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////// application ////////////////////////////////////////
    String imageName_Background("/sdcard/Pictures/PASTE/N3.jpg"/*"../images/NN33.jpg"*/ );
    Mat BackgroundImage = imread(imageName_Background, IMREAD_COLOR);

    Mat Result_Image;
    //	Compositing(AlphaMap, BackgroundImage, 0, 0, 0, Result_Image);

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Retouching ////////////////////////////////////
    InputImage_for_retouching = aligned_NearFocusImage_Color;
    InputImage_for_retouching.convertTo(InputImage_for_retouching, CV_8UC3);

    Trimap_Init = AlphaMap * 255;
    Trimap_Init.convertTo(Trimap_Init, CV_8UC1);

    //	ThresControl(DepthMap, 500, 500, 100, 0.7);

    ////setMouseCallback("ForegroundImage_diplay", CallBackFunc, NULL);

    ////waitKey(0);

    ////return 0;

//		thresholdDepthMap.copyTo(DepthMap);
    thresholdDepthMap = DepthMap.clone();

    ////ThresControl_170614(thresholdDepthMap, 100, 100, 200, 200, 0.5f);

    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Pro_main() END");

}


int mainD170428(string fileName)
{
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Pro_main() START");

    filePath = fileName;

    // paramters
    int imresize_factor = 4;

    int size_window = 21;    // FMR 마스크 크기

    float thres_foreground = 0.6;   // foreground threshold 초기값

    int thres_crop_width = 0;    // 이미지 테두리 짤라라

    int r = 51;              // guided filter 크기

    double eps = 100;    // guided filter parameter

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Image read ////////////////////////////////////
    int i, j;

    String imageName_NearFocus(fileName + "_N.jpg");
    String imageName_FarFocus(fileName + "_F.jpg");
    ////String imageName_Saveimg( "../img/TSET170403_HTC10/ID_50cm/ForegroundImage_diplay.jpg" ); //GN_170329
    ////String imageName_DepthMap( "../img/TSET170403_HTC10/ID_50cm/DepthMap.jpg" ); //GN_170329

    //GNS_170428
    rotationInfo = getRotationFromJpg(imageName_NearFocus.c_str());
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "rotationInfo : %d", rotationInfo);
    //GNE_170428

    Mat Original_NearFocusImage_Color;
    Mat Original_FarFocusImage_Color;
    Mat Original_NearFocusImage_Gray;
    Mat Original_FarFocusImage_Gray;
    //	Mat Harf_NearFocusImage_Color;
    //	Mat Harf_FarFocusImage_Color;
    //	Mat Harf_NearFocusImage_Gray;
    //	Mat Harf_FarFocusImage_Gray;

# if 0
                                                                                                                            //GNS_170428
		Mat Temp_NearFocusImage_Color_input; // GN_170428
		Mat Temp_FarFocusImage_Color_input; // GN_170428

		if(rotationInfo == 0) { // rot270(+inverse)

			Temp_NearFocusImage_Color_input = imread( imageName_NearFocus,  IMREAD_COLOR );
			Temp_FarFocusImage_Color_input = imread( imageName_FarFocus, IMREAD_COLOR );

			if( Temp_NearFocusImage_Color_input.empty() || Temp_FarFocusImage_Color_input.empty() )
			{
				////cout <<  "Could not open or find the image" << std::endl ;
				__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Could not open or find the image");
				return -1;
			}
			//// Rotation
			Point2f pt(Temp_NearFocusImage_Color_input.rows / 2.0, Temp_NearFocusImage_Color_input.rows / 2.0);
			Mat rotate = getRotationMatrix2D(pt, 270, 1.0);
			Mat imgRotated;
			warpAffine(Temp_NearFocusImage_Color_input, Original_NearFocusImage_Color, rotate, Size(Temp_NearFocusImage_Color_input.rows,Temp_NearFocusImage_Color_input.cols) );
			warpAffine(Temp_FarFocusImage_Color_input, Original_FarFocusImage_Color, rotate,  Size(Temp_NearFocusImage_Color_input.rows,Temp_NearFocusImage_Color_input.cols) );
			//// Rotation
		}
		else {
			Original_NearFocusImage_Color = imread( imageName_NearFocus, IMREAD_COLOR );
			Original_FarFocusImage_Color = imread( imageName_FarFocus, IMREAD_COLOR );
			if( Original_NearFocusImage_Color.empty() || Original_FarFocusImage_Color.empty() )
			{
				////cout <<  "Could not open or find the image" << std::endl ;
				__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Could not open or find the image");
				return -1;
			}
		}
		//GNE_170428
#endif

    stopWatch();
    Original_NearFocusImage_Color = imread(imageName_NearFocus, IMREAD_COLOR);
    Original_FarFocusImage_Color = imread(imageName_FarFocus, IMREAD_COLOR);
    stopWatch();

    stopWatch();
    if (Original_NearFocusImage_Color.empty() || Original_FarFocusImage_Color.empty())
    {
        ////cout <<  "Could not open or find the image" << std::endl ;
        __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Could not open or find the image");
        return -1;
    }

    cvtColor(Original_NearFocusImage_Color, Original_NearFocusImage_Gray, CV_RGB2GRAY);
    cvtColor(Original_FarFocusImage_Color, Original_FarFocusImage_Gray, CV_RGB2GRAY);
    stopWatch();
    //	resize(Original_NearFocusImage_Color, Harf_NearFocusImage_Color, Size(Original_NearFocusImage_Color.cols/imresize_factor, Original_NearFocusImage_Color.rows/imresize_factor));
    //	resize(Original_FarFocusImage_Color, Harf_FarFocusImage_Color, Size(Original_FarFocusImage_Color.cols/imresize_factor, Original_FarFocusImage_Color.rows/imresize_factor));
    //	cvtColor(Harf_NearFocusImage_Color, Harf_NearFocusImage_Gray, CV_RGB2GRAY);
    //	cvtColor(Harf_FarFocusImage_Color, Harf_FarFocusImage_Gray, CV_RGB2GRAY);

    //	namedWindow( "Near Focus Image", CV_WINDOW_AUTOSIZE );
    //	imshow( "Near Focus Image", Original_NearFocusImage_Color );

    //	namedWindow( "Far Focus Image", CV_WINDOW_AUTOSIZE );
    //	imshow( "Far Focus Image", Harf_FarFocusImage_Color );

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Image alignment ////////////////////////////////////
    stopWatch();

    vector<cv::KeyPoint> keypoints_N, keypoints_F;
    Mat descriptors_N, descriptors_F;

    ORB orb;

    OrbFeatureDetector detector(500, 1.2f, 4, 25, 0, 2, 0, 25);
    OrbDescriptorExtractor extractor;

    orb.detect(Original_NearFocusImage_Gray, keypoints_N);
    orb.compute(Original_NearFocusImage_Gray, keypoints_N, descriptors_N);

    orb.detect(Original_FarFocusImage_Gray, keypoints_F);
    orb.compute(Original_FarFocusImage_Gray, keypoints_F, descriptors_F);

    BFMatcher matcher;
    vector<DMatch> matches;
    Mat img_matches;

    matcher.match(descriptors_N, descriptors_F, matches);

    double max_dist = 0;
    double min_dist = 100;

    for (i = 0; i < descriptors_N.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    vector<cv::DMatch> good_matches;

    for (i = 0; i < descriptors_N.rows; i++)
    {
        if (matches[i].distance < (max_dist / 1.6))
        { good_matches.push_back(matches[i]); }
    }

    //	drawMatches(Harf_NearFocusImage_Gray, keypoints_N, Harf_FarFocusImage_Gray, keypoints_F, \
	//          good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
    //                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //	namedWindow( "img_matches", CV_WINDOW_FULLSCREEN );
    //	imshow("img_matches", img_matches );

    std::vector<Point2f> Gp_N;
    std::vector<Point2f> Gp_F;

    for (i = 0; i < good_matches.size(); i++)
    {
        Gp_N.push_back(keypoints_N[good_matches[i].queryIdx].pt);
        Gp_F.push_back(keypoints_F[good_matches[i].trainIdx].pt);
    }

    Mat transformMat = estimateRigidTransform(Gp_N, Gp_F, false);

    if (transformMat.dims != 0)
    {
        Size dsize = Original_NearFocusImage_Color.size();
        warpAffine(Original_NearFocusImage_Color, alined_Original_NearFocusImage_Color,
                   transformMat, dsize);
    }
    else
    {
        alined_Original_NearFocusImage_Color = Original_NearFocusImage_Color;         ////  얼라인을 못하는 경우 처리, 일단 원영상 그대로 가져옴...
    }

    //	namedWindow( "alined_Original_NearFocusImage_Color", CV_WINDOW_NORMAL );
    //	imshow( "alined_Original_NearFocusImage_Color", alined_Original_NearFocusImage_Color );

    //	namedWindow( "Harf_NearFocusImage_Color", CV_WINDOW_NORMAL );
    //	imshow( "Harf_NearFocusImage_Color", alined_Harf_NearFocusImage_Color );

    ////cout << "Image alignment" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Image alignment");
    stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// FMR-based depth extraction //////////////////////
    stopWatch();

    //	Mat alined_Harf_NearFocusImage_Color;
    Mat alined_Original_NearFocusImage_Gray;

    //	alined_Harf_NearFocusImage_Color = alined_Original_NearFocusImage_Color;
    //	Harf_FarFocusImage_Gray = Original_FarFocusImage_Gray;

    //	resize(alined_Original_NearFocusImage_Color, alined_Harf_NearFocusImage_Color, Size(alined_Original_NearFocusImage_Color.cols/imresize_factor, alined_Original_NearFocusImage_Color.rows/imresize_factor));
    cvtColor(alined_Original_NearFocusImage_Color, alined_Original_NearFocusImage_Gray,
             CV_RGB2GRAY);

    Mat grad_x, grad_y;
    Mat grad_x_square, grad_y_square;
    Mat FM_Nimg, FM_Fimg;
    Mat FM_Nfocus, FM_Ffocus;

    Sobel(alined_Original_NearFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(alined_Original_NearFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Nimg = grad_x_square + grad_y_square;
    //	boxFilter( FM_Nimg, FM_Nfocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
    GaussianBlur(FM_Nimg, FM_Nfocus, Size(size_window, size_window), 0, 0);

    Sobel(Original_FarFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(Original_FarFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Fimg = (grad_x_square + grad_y_square);
    //	boxFilter( FM_Fimg, FM_Ffocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
    GaussianBlur(FM_Fimg, FM_Ffocus, Size(size_window, size_window), 0, 0);

    float *FM_Nfocus_data = (float *) FM_Nfocus.data;
    float *FM_Ffocus_data = (float *) FM_Ffocus.data;
    float *FMRtemp;
    FMRtemp = new float[alined_Original_NearFocusImage_Gray.cols *
                        alined_Original_NearFocusImage_Gray.rows];

    int length = FM_Nfocus.rows * FM_Nfocus.cols;

    for (i = 0; i < length; i++)
    {
        if (FM_Ffocus_data[i] > FM_Nfocus_data[i])
        {
            FMRtemp[i] = 2 - FM_Nfocus_data[i] / (FM_Ffocus_data[i] + 0.000001);
        }
        else if (FM_Ffocus_data[i] < FM_Nfocus_data[i])
        {
            FMRtemp[i] = FM_Ffocus_data[i] / (FM_Nfocus_data[i] + 0.000001);
        }
        else
        {
            FMRtemp[i] = 2;
        }
    }

    ////cout << "FMR-based depth extraction" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "FMR-based depth extraction");
    stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Guided filtering ///////////////////////////////
    stopWatch();

    ////Mat aligned_NearFocusImage_Color;
    Mat FMR(alined_Original_NearFocusImage_Gray.rows, alined_Original_NearFocusImage_Gray.cols,
            CV_32F, FMRtemp);

    ////aligned_NearFocusImage_Color = alined_Original_NearFocusImage_Color; // testCode.170714
    resize(alined_Original_NearFocusImage_Color, aligned_NearFocusImage_Color,
           Size(alined_Original_NearFocusImage_Gray.cols / imresize_factor,
                alined_Original_NearFocusImage_Gray.rows / imresize_factor));
    resize(FMR, FMR, Size(FMR.cols / imresize_factor, FMR.rows / imresize_factor));

    Mat DepthMap(aligned_NearFocusImage_Color.rows, aligned_NearFocusImage_Color.cols, CV_32F);
    DepthMap = Scalar(0);

    Mat FMR_filtered = guidedFilter(aligned_NearFocusImage_Color, FMR, r, eps);
    DepthMap = (2 - FMR_filtered) / 2;
    DepthMap.convertTo(DepthMap, CV_32F);

    //	Rect rc(thres_crop_width,thres_crop_width,DepthMap_temp.cols-2*thres_crop_width,DepthMap_temp.rows-2*thres_crop_width);
    //	DepthMap_temp = DepthMap_temp(rc);
    //	DepthMap_temp.copyTo(DepthMap(rc));

    DepthMap = DepthMap * 255;
    DepthMap.convertTo(DepthMap, CV_8U);

    //	Mat cm_FMR;
    //	FMR = 255-FMR*127;
    //	FMR.convertTo(FMR,CV_8U);
    //	namedWindow( "FMR", CV_WINDOW_KEEPRATIO );
    //	applyColorMap(FMR, cm_FMR, COLORMAP_JET);
    //	imshow("FMR", cm_FMR);

    Mat cm_DepthMap;
    ////namedWindow( "DepthMap", CV_WINDOW_KEEPRATIO );
    applyColorMap(DepthMap, cm_DepthMap, COLORMAP_JET);
    ////imshow("DepthMap", cm_DepthMap);
    ////imwrite( fileName + "_DepthMap.jpg" /*imageName_DepthMap*/, cm_DepthMap);//GN_170428
    imageRotationSave("_DepthMap.jpg", cm_DepthMap); //GN_170428

    ////cout << "Guided filtering" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Guided filtering");
    stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// foreground extraction ////////////////////////////////////
    stopWatch();

    if (Adaptive_thres)
    {
        Mat Local_Binary_AlphaMap;
        Mat Local_DepthMap;

        float init_foreground_threshhold = threshold(DepthMap, Binary_AlphaMap, thres_foreground, 1,
                                                     CV_THRESH_BINARY | CV_THRESH_OTSU);

        int H = aligned_NearFocusImage_Color.rows;
        int W = aligned_NearFocusImage_Color.cols;

        int height_temp = cvRound(H / 4);
        int width_temp = cvRound(W / 8);

        int start_x, end_x, start_y, end_y;
        int local_width, local_height;

        float cnt_F, cnt_B;
        float balance_ratio;
        float Local_threshhold;

        for (int j = 0; j < H; j += cvRound(height_temp / 2))
        {
            for (int i = 0; i < W; i += cvRound(width_temp / 2))
            {
                start_x = i;
                end_x = i + width_temp;
                start_y = j;
                end_y = j + height_temp;

                if (end_x > W)
                {
                    end_x = W - 1;
                }
                if (end_y > H)
                {
                    end_y = H - 1;
                }

                local_width = end_x - start_x;
                local_height = end_y - start_y;

                Rect rc(start_x, start_y, local_width, local_height);

                Local_Binary_AlphaMap = Binary_AlphaMap(rc);

                cnt_F = 0, cnt_B = 0;
                for (int x = 0; x < Local_Binary_AlphaMap.cols; ++x)
                {
                    for (int y = 0; y < Local_Binary_AlphaMap.rows; ++y)
                    {
                        if (Local_Binary_AlphaMap.at<uchar>(y, x) == 1)
                        {
                            cnt_F = cnt_F + 1;
                        }
                        else if (Local_Binary_AlphaMap.at<uchar>(y, x) == 0)
                        {
                            cnt_B = cnt_B + 1;
                        }
                    }
                }
                balance_ratio = min(cnt_F, cnt_B) / max(cnt_F, cnt_B);

                if (balance_ratio > 0.3)
                {
                    Local_DepthMap = DepthMap(rc);

                    Local_threshhold = threshold(Local_DepthMap, Local_Binary_AlphaMap,
                                                 thres_foreground, 1,
                                                 CV_THRESH_BINARY | CV_THRESH_OTSU);

                    Local_Binary_AlphaMap.copyTo(Binary_AlphaMap(rc));
                }
            }
        }
    }
    else
    {
        threshold(DepthMap, Binary_AlphaMap, thres_foreground * 255, 1,
                  CV_THRESH_BINARY | CV_THRESH_OTSU);
    }

    AlphaMap = Binary_AlphaMap;

    //	Mat AlphaMap_display1 = AlphaMap*255;
    //	namedWindow( "AlphaMap1", CV_WINDOW_KEEPRATIO );
    //	imshow("AlphaMap1", AlphaMap_display1);

    ////cout << "foreground extraction" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "foreground extraction");
    stopWatch();

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Remove isolated blobs ////////////////////////////////////
    stopWatch();

    Mat dst1 = Mat::zeros(AlphaMap.rows, AlphaMap.cols, CV_8UC1);
    Mat dst2 = Mat::zeros(AlphaMap.rows, AlphaMap.cols, CV_8UC1);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(AlphaMap, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    int iNumSegments = contours.size();
    double area;
    double max_area = 0;
    int max_index = 0;;
    for (int i = 0; i < iNumSegments; i++)
    {
        vector<Point> approx;
        approxPolyDP(Mat(contours[i]), approx, 1.0e-10, true);
        area = fabs(cv::contourArea(approx));
        if (area > max_area)
        {
            max_area = area;
            max_index = i;
        }
    }
    for (int i = 0; i < iNumSegments; i++)
    {
        if (i == max_index)
        {
            drawContours(dst1, contours, i, 1, CV_FILLED, 8, hierarchy);
        }
        else
        {
            drawContours(dst1, contours, i, 0, CV_FILLED, 8, hierarchy);
        }
    }

    Mat invertAlphaMap;
    invertAlphaMap = -(dst1 - 1);

    findContours(invertAlphaMap, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    iNumSegments = contours.size();

    max_area = 0;
    max_index = 0;;
    for (int i = 0; i < iNumSegments; i++)
    {
        vector<Point> approx;
        approxPolyDP(Mat(contours[i]), approx, 1.0e-10, true);
        area = fabs(cv::contourArea(approx));
        if (area > max_area)
        {
            max_area = area;
            max_index = i;
        }
    }
    for (int i = 0; i < iNumSegments; i++)
    {
        if (i == max_index)
        {
            drawContours(dst2, contours, i, 0, CV_FILLED, 8, hierarchy);
        }
        else
        {
            drawContours(dst2, contours, i, 1, CV_FILLED, 8, hierarchy);
        }
    }

    //	AlphaMap = dst1+dst2;
    AlphaMap = max(dst1, dst2);

    ////Mat AlphaMap_display = AlphaMap*255;
    AlphaMap_display = AlphaMap * 255;
    ////namedWindow( "AlphaMap", CV_WINDOW_KEEPRATIO );
    ////imshow("AlphaMap", AlphaMap_display);
    ////imwrite(fileName+"_AlphaMap_display.jpg"/*imageName_Saveimg*/, AlphaMap_display);//GN_170428
    imageRotationSave("_AlphaMap_display.jpg", AlphaMap_display); //GN_170428

    ////cout << "Remove isolated blobs" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Remove isolated blobs");
    stopWatch();

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// DIsplay  ///////////////////////////////////

    int Background_color = bg_color;   // Blue:1 Green:2 Red:3

    PaintingBackground(aligned_NearFocusImage_Color, Background_color);

    ////imwrite(fileName+"_ForegroundImage_diplay.jpg"/*imageName_Saveimg*/, ForegroundImage_diplay);//GN_170428
    ////imwrite(fileName+"_Result_Image.jpg"/*imageName_Saveimg*/, ForegroundImage_diplay);//GN_170428
    imageRotationSave("_Result_Image.jpg", ForegroundImage_diplay); //GN_170428

    ////namedWindow( "ForegroundImage_diplay", CV_WINDOW_KEEPRATIO );
    ////imshow("ForegroundImage_diplay", ForegroundImage_diplay);

    ///////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Save result  ///////////////////////////////////
    int margin_crop_boundary = 20;

    SaveResult(aligned_NearFocusImage_Color, margin_crop_boundary);

    /*	Mat mat(480, 640, CV_8UC4);
	    createAlphaMat(mat);

	    vector<int> compression_params;
	    compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION);
	    compression_params.push_back(9);

	    imwrite("alpha.png", mat, compression_params);*/

    /////////////////////////////////////////////////////////////////////////////////
    /////////////////////////// application ////////////////////////////////////////
    String imageName_Background("/sdcard/Pictures/PASTE/N3.jpg"/*"../images/NN33.jpg"*/ );
    Mat BackgroundImage = imread(imageName_Background, IMREAD_COLOR);

    Mat Result_Image;
    //	Compositing(AlphaMap, BackgroundImage, 0, 0, 0, Result_Image);

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Retouching ////////////////////////////////////
    InputImage_for_retouching = aligned_NearFocusImage_Color;
    InputImage_for_retouching.convertTo(InputImage_for_retouching, CV_8UC3);

    Trimap_Init = AlphaMap * 255;
    Trimap_Init.convertTo(Trimap_Init, CV_8UC1);

    //	ThresControl(DepthMap, 500, 500, 100, 0.7);

    ////setMouseCallback("ForegroundImage_diplay", CallBackFunc, NULL);

    ////waitKey(0);

    ////return 0;

    //thresholdDepthMap.copyTo(DepthMap);
    thresholdDepthMap = DepthMap.clone();

    ////ThresControl_170614(thresholdDepthMap, 100, 100, 200, 200, 0.5f);

    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "Pro_main() END");

}

#if 0
                                                                                                                        int Pro_main_D170217(string fileName)
{
	__android_log_print(ANDROID_LOG_DEBUG, "!!!", "Pro_main() START");

		filePath = fileName;

	// mode setting
		bool fast_mode = 1;

		bool Adaptive_thres = 1;

		bool Align_Far2Near = 0;

		bool Compositing_background = 0;//0

	// paramters
		int imresize_factor = 2;

		int size_window;
		if(fast_mode)
			size_window = 13;
		else
			size_window = 25;

		float thres_foreground = 0.6;
		float thres_background = 0.4;

		int thres_crop_width = 50;

		int thres_smoothing_factor = 2;
		int r;

		r=thres_smoothing_factor*size_window;

	/*	if(fast_mode)
			r = 25;
		else
			r = 50;
	*/
		double eps = 100;
	//	eps *= 100 * 100;   // intensity range [0, 255]
	/////////////////
		int i,j;

		String imageName_NearFocus( fileName + "_N.jpg" );
		String imageName_FarFocus( fileName + "_F.jpg" );
		String imageName_Background( "/sdcard/Pictures/PASTE/N3.jpg" );

		////

		int temp = getRotationFromJpg(imageName_NearFocus.c_str());
		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "orientation : %d",temp);

	//	String imageName_NearFocus( "../images/t1_N.jpg" );
	//	String imageName_FarFocus( "../images/t1_F.jpg" );
	//	String imageName_Background( "../images/N3.jpg" );
	//	String imageName_NearFocus( "../images/PASTE_N.jpg" );
	//  String imageName_FarFocus( "../images/PASTE_F.jpg" );
	//	String imageName_Background( "../images/test_background_HTC.jpg" );

		Mat NearFocusImage_Color_input;
		Mat FarFocusImage_Color_input;
		Mat NearFocusImage_Color;
		Mat FarFocusImage_Color;
		Mat NearFocusImage_Gray;
		Mat FarFocusImage_Gray;

		Mat Temp_NearFocusImage_Color_input;
		Mat Temp_FarFocusImage_Color_input;

		BackgroundImage = imread( imageName_Background, IMREAD_COLOR );

		if(temp == 6) {
		////NearFocusImage_Color_input = imread( imageName_NearFocus,  IMREAD_COLOR );
		////FarFocusImage_Color_input = imread( imageName_FarFocus, IMREAD_COLOR );
		Temp_NearFocusImage_Color_input = imread( imageName_NearFocus,  IMREAD_COLOR );
		Temp_FarFocusImage_Color_input = imread( imageName_FarFocus, IMREAD_COLOR );


/*
		if( NearFocusImage_Color_input.empty() || FarFocusImage_Color_input.empty() || BackgroundImage.empty() )
		{
			cout <<  "Could not open or find the image" << std::endl ;
			return -1;
		}
*/
		if( Temp_NearFocusImage_Color_input.empty() || Temp_FarFocusImage_Color_input.empty() || BackgroundImage.empty() )
		{
			cout <<  "Could not open or find the image" << std::endl ;
			return -1;
		}

		//// Rotation
		Point2f pt(Temp_NearFocusImage_Color_input.rows / 2.0, Temp_NearFocusImage_Color_input.rows / 2.0);

		Mat rotate = getRotationMatrix2D(pt, 270, 1.0);
		Mat imgRotated;

		warpAffine(Temp_NearFocusImage_Color_input, NearFocusImage_Color_input, rotate, Size(Temp_NearFocusImage_Color_input.rows,Temp_NearFocusImage_Color_input.cols) );
		warpAffine(Temp_FarFocusImage_Color_input, FarFocusImage_Color_input, rotate,  Size(Temp_NearFocusImage_Color_input.rows,Temp_NearFocusImage_Color_input.cols) );
		////imwrite( fileName + "_NNNN.jpg", NearFocusImage_Color_input );
		////imwrite( fileName + "_FFFF.jpg", FarFocusImage_Color_input );
		//// Rotation
		}
		else {
			NearFocusImage_Color_input = imread( imageName_NearFocus,  IMREAD_COLOR );
			FarFocusImage_Color_input = imread( imageName_FarFocus, IMREAD_COLOR );
		}
		resize(NearFocusImage_Color_input, NearFocusImage_Color, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
		resize(FarFocusImage_Color_input, FarFocusImage_Color, Size(FarFocusImage_Color_input.cols/imresize_factor, FarFocusImage_Color_input.rows/imresize_factor));

	/*	if(BackgroundImage.cols == NearFocusImage_Color_input.cols & BackgroundImage.rows == NearFocusImage_Color_input.rows){
			resize(BackgroundImage, BackgroundImage, Size(BackgroundImage.cols/imresize_factor, BackgroundImage.rows/imresize_factor));
		}
		else{
			resize(BackgroundImage, BackgroundImage, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
		}*/

		resize(BackgroundImage, BackgroundImage, Size(BackgroundImage.cols/imresize_factor, BackgroundImage.rows/imresize_factor));

	//	namedWindow( "Near Focus Image", CV_WINDOW_AUTOSIZE );
	//	imshow( "Near Focus Image", NearFocusImage_Color_input );

	//	namedWindow( "Far Focus Image", CV_WINDOW_AUTOSIZE );
	//	imshow( "Far Focus Image", FarFocusImage_Color );

		cvtColor(NearFocusImage_Color, NearFocusImage_Gray, CV_RGB2GRAY);
		cvtColor(FarFocusImage_Color, FarFocusImage_Gray, CV_RGB2GRAY);

	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Image alignment ////////////////////////////////////
		////stopWatch();

		vector<cv::KeyPoint> keypoints_N, keypoints_F;
		Mat descriptors_N, descriptors_F;

		ORB orb;

		OrbFeatureDetector detector(500, 1.2f, 4, 25, 0, 2, 0, 25);
		OrbDescriptorExtractor extractor;

		orb.detect(NearFocusImage_Gray, keypoints_N);
		orb.compute(NearFocusImage_Gray, keypoints_N, descriptors_N);

		orb.detect(FarFocusImage_Gray, keypoints_F);
		orb.compute(FarFocusImage_Gray, keypoints_F, descriptors_F);

	    BFMatcher matcher;
	    vector<DMatch> matches;
	    Mat img_matches;

		matcher.match (descriptors_N, descriptors_F, matches);

		double max_dist = 0; double min_dist = 100;

		for( i = 0; i < descriptors_N.rows; i++)
	        { double dist = matches[i].distance;
	            if( dist < min_dist ) min_dist = dist;
	            if( dist > max_dist ) max_dist = dist;
	        }

		vector< cv::DMatch >good_matches;

	        for( i = 0; i < descriptors_N.rows; i++ )

	        { if( matches[i].distance < (max_dist/1.6) )
	            { good_matches.push_back( matches[i]); }
	        }

	//	drawMatches(NearFocusImage_Gray, keypoints_N, FarFocusImage_Gray, keypoints_F, \
	//          good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
	//                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//	namedWindow( "img_matches", CV_WINDOW_FULLSCREEN );
	//	imshow("img_matches", img_matches );


		std::vector<Point2f> Gp_N;
		std::vector<Point2f> Gp_F;

		for( i = 0; i < good_matches.size(); i++ )
		{
			Gp_N.push_back( keypoints_N[ good_matches[i].queryIdx ].pt );
			Gp_F.push_back( keypoints_F[ good_matches[i].trainIdx ].pt );
		}

		Mat alined_FarFocusImage_Gray;
		Mat alined_NearFocusImage_Color;

		if(Align_Far2Near){
			Mat transformMat = estimateRigidTransform(Gp_F, Gp_N, false);

			if(transformMat.dims != 0){
				Size dsize = FarFocusImage_Gray.size();
				warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);
			}
			else{
				alined_FarFocusImage_Gray = FarFocusImage_Gray;
			}

			Size dsize = NearFocusImage_Gray.size();
			warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);

			////namedWindow( "FarFocusImage_Gray", CV_WINDOW_NORMAL );
			////imshow( "FarFocusImage_Gray", alined_FarFocusImage_Gray );

			////namedWindow( "NearFocusImage_Color", CV_WINDOW_NORMAL );
			////imshow( "NearFocusImage_Color", NearFocusImage_Color );
		}
		else{
	//		Mat transform_H = findHomography( Gp_N, Gp_F, CV_RANSAC );
	//		perspectiveTransform( NearFocusImage_Color, alined_NearFocusImage_Color, transform_H);

			Mat transformMat = estimateRigidTransform(Gp_N, Gp_F, false);

			if(transformMat.dims != 0){
				Size dsize = NearFocusImage_Color.size();
				warpAffine(NearFocusImage_Color, alined_NearFocusImage_Color, transformMat, dsize);
			}
			else{
				alined_NearFocusImage_Color = NearFocusImage_Color;
			}

			////namedWindow( "FarFocusImage_Gray", CV_WINDOW_NORMAL );
			////imshow( "FarFocusImage_Gray", FarFocusImage_Gray );

			////namedWindow( "NearFocusImage_Color", CV_WINDOW_NORMAL );
			////imshow( "NearFocusImage_Color", alined_NearFocusImage_Color );
		}

		////cout << "Image alignment" << endl;
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// FMR-based depth extraction ////////////////////////////////////
		/////stopWatch();

		Mat NearFocus_Image_Color;
		Mat NearFocus_Image_Gray;
		Mat FarFocus_Image_Gray;

		if(Align_Far2Near){
			NearFocus_Image_Color = NearFocusImage_Color;
			cvtColor(NearFocus_Image_Color, NearFocus_Image_Gray, CV_RGB2GRAY);
			FarFocus_Image_Gray = alined_FarFocusImage_Gray;
		}
		else{
			NearFocus_Image_Color = alined_NearFocusImage_Color;
			cvtColor(NearFocus_Image_Color, NearFocus_Image_Gray, CV_RGB2GRAY);
			FarFocus_Image_Gray = FarFocusImage_Gray;
		}

		Mat grad_x,grad_y;
		Mat grad_x_square, grad_y_square;
		Mat FM_Nimg,FM_Fimg;
		Mat FM_Nfocus,FM_Ffocus;

		Sobel( NearFocus_Image_Gray, grad_x, CV_32F, 1, 0, 3);
		multiply(grad_x, grad_x, grad_x_square);

		Sobel( NearFocus_Image_Gray, grad_y, CV_32F, 0, 1, 3);
		multiply(grad_y, grad_y, grad_y_square);

		FM_Nimg = grad_x_square + grad_y_square;
	//	boxFilter( FM_Nimg, FM_Nfocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
		GaussianBlur( FM_Nimg, FM_Nfocus, Size(size_window,size_window), 0, 0 );

		Sobel( FarFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
		multiply(grad_x, grad_x, grad_x_square);

		Sobel( FarFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
		multiply(grad_y, grad_y, grad_y_square);

		FM_Fimg = (grad_x_square + grad_y_square);
	//	boxFilter( FM_Fimg, FM_Ffocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
		GaussianBlur( FM_Fimg, FM_Ffocus, Size(size_window,size_window), 0, 0 );

		float* FM_Nfocus_data = (float*)FM_Nfocus.data;
		float* FM_Ffocus_data = (float*)FM_Ffocus.data;
		float* FMRtemp;
		FMRtemp = new float[NearFocus_Image_Color.cols*NearFocus_Image_Color.rows];

		int length = FM_Nfocus.rows  * FM_Nfocus.cols;

		for (i = 0; i < length; i++){
	//		FM_Ffocus_data[i]=FM_Ffocus_data[i]+20;
			if (FM_Ffocus_data[i] > FM_Nfocus_data[i]){
				FMRtemp[i] = 2 - FM_Nfocus_data[i] / (FM_Ffocus_data[i] + 0.000001);
			}
			else if (FM_Ffocus_data[i] < FM_Nfocus_data[i]){
				FMRtemp[i] = FM_Ffocus_data[i] / (FM_Nfocus_data[i] + 0.000001);
			}
			else{
				FMRtemp[i] = 2;
			}
	    }

		////cout << "FMR-based depth extraction" << endl;
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Guided filtering ////////////////////////////////////
		/////stopWatch();

		Mat FMR(NearFocus_Image_Color.rows, NearFocus_Image_Color.cols, CV_32F, FMRtemp);

		Rect rc(thres_crop_width,thres_crop_width,NearFocus_Image_Color.cols-2*thres_crop_width,NearFocus_Image_Color.rows-2*thres_crop_width);

		NearFocus_Image_Color = NearFocus_Image_Color(rc);
		FMR = FMR(rc);

		if(fast_mode){
			resize(NearFocus_Image_Color, NearFocus_Image_Color, Size(NearFocus_Image_Color.cols/imresize_factor, NearFocus_Image_Color.rows/imresize_factor));
			resize(FMR, FMR, Size(FMR.cols/imresize_factor, FMR.rows/imresize_factor));
		}

		Mat FMR_filtered = guidedFilter(NearFocus_Image_Color, FMR, r, eps);

	//	Mat FMR_filtered;
	//	bilateralFilter ( FMR, FMR_filtered, 50, 0.1, 25 );


		depthMap = (2-FMR_filtered)/2;

		Mat cm_FMR;
		FMR = 255-FMR*127;
		FMR.convertTo(FMR,CV_8U);
		////namedWindow( "FMR", CV_WINDOW_KEEPRATIO );
		////applyColorMap(FMR, cm_FMR, COLORMAP_JET);
		////imshow("FMR", cm_FMR);

		////cout << "Guided filtering" << endl;
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// foreground extraction ////////////////////////////////////
		////stopWatch();

	//////////////// alpha matting
	/*	Mat Trimap(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Mat Trimap_foreground(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Mat Trimap_background(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Trimap = Scalar(128);
		Trimap_foreground = Scalar(0);
		Trimap_background = Scalar(0);

		threshold(depthMap, Trimap_foreground, thres_foreground, 127, CV_THRESH_BINARY);
		threshold(depthMap, Trimap_background, thres_background, 128, CV_THRESH_BINARY_INV);

		Trimap_foreground.convertTo(Trimap_foreground,CV_8UC1);
		Trimap_background.convertTo(Trimap_background,CV_8UC1);
		add(Trimap,Trimap_foreground,Trimap);
		subtract(Trimap,Trimap_background,Trimap);

		namedWindow( "Trimap", CV_WINDOW_KEEPRATIO );
		imshow("Trimap", Trimap);
		Mat foreground, alpha;
		globalMatting(NearFocus_Image_Color, Trimap, foreground, alpha);

		double minVal;
		double maxVal;
		minMaxIdx(AlphaMap, &minVal, &maxVal);
		Mat AlphaMap_display;
		AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

		namedWindow( "foreground", CV_WINDOW_KEEPRATIO );
		imshow("foreground", foreground);
	*/
		if(Adaptive_thres)
		{
			Mat Binary_AlphaMap;//(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);

			depthMap = depthMap*255;
			depthMap.convertTo(depthMap,CV_8U);

			Mat cm_depthMap;
			////namedWindow( "depthMap", CV_WINDOW_KEEPRATIO );
			////applyColorMap(depthMap, cm_depthMap, COLORMAP_JET);
			////imshow("depthMap", cm_depthMap);

			float init_foreground_threshhold = threshold(depthMap, Binary_AlphaMap, thres_foreground, 1, CV_THRESH_BINARY | CV_THRESH_OTSU);

			AlphaMap = Binary_AlphaMap;

			double minVal;
			double maxVal;

			minMaxIdx(AlphaMap, &minVal, &maxVal);
			Mat AlphaMap_display;
			AlphaMap.convertTo(AlphaMap_display,CV_8UC3, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

			////namedWindow( "AlphaMap", CV_WINDOW_NORMAL );

			int H = NearFocus_Image_Color.rows;
			int W = NearFocus_Image_Color.cols;

			int height_temp = cvRound(H/4);
			int width_temp = cvRound(W/8);

			int start_x,end_x,start_y,end_y;
			int local_width,local_height;

			float cnt_F,cnt_B;
			float balance_ratio;
			float Local_threshhold;

		//	Mat foreground, alpha;

			for(int j=0;j<H;j+=cvRound(height_temp/2))
			{
				for(int i=0;i<W;i+=cvRound(width_temp/2))
				{
					start_x = i;
					end_x = i+width_temp;
					start_y = j;
					end_y = j+height_temp;

					if(end_x>W)
						end_x=W-1;
					if(end_y>H)
						end_y=H-1;

					local_width = end_x - start_x;
					local_height = end_y - start_y;

					Rect rc(start_x,start_y,local_width,local_height);

		//			rectangle(AlphaMap_display, rc,(0,0,255), 3);

					Mat Local_binary_alpha = Binary_AlphaMap(rc);

					cnt_F=0,cnt_B=0;

					for (int x = 0; x < Local_binary_alpha.cols; ++x){
						for (int y = 0; y < Local_binary_alpha.rows; ++y){
							if (Local_binary_alpha.at<uchar>(y, x) == 1)
								cnt_F=cnt_F+1;
							else if (Local_binary_alpha.at<uchar>(y, x) == 0)
								cnt_B=cnt_B+1;
						}
					}

					balance_ratio = min(cnt_F,cnt_B)/max(cnt_F,cnt_B);

		//			cout << "out (" << cnt_F << ", " << cnt_B << ", " << balance_ratio << ")" << endl;

					if (balance_ratio>0.3)
					{
		//				rectangle(AlphaMap_display, rc,(0,0,255), 3);
						Mat Local_depthMap = depthMap(rc);

						Mat Local_Binary_AlphaMap;
						Local_threshhold = threshold(Local_depthMap, Local_Binary_AlphaMap, thres_foreground, 1, CV_THRESH_BINARY | CV_THRESH_OTSU);

						Mat cropped_RGB_Image = NearFocus_Image_Color(rc);
						//// slow
		//				globalMatting(cropped_RGB_Image, cropped_R_Trimap, foreground, alpha);
						////
						Local_Binary_AlphaMap.copyTo(AlphaMap(rc));


					}

				}
			}

			////imshow("AlphaMap", AlphaMap_display);
		}
		else
		{
			AlphaMap = depthMap;

			threshold(depthMap, AlphaMap, thres_foreground, 1, CV_THRESH_BINARY);
		//	threshold(depthMap, AlphaMap, thres_background, 1, CV_THRESH_TOZERO);
		}

		////cout << "foreground extraction" << endl;
		////stopWatch();

	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Remove isolated blobs ////////////////////////////////////
		////stopWatch();

	    Mat dst1 = Mat::zeros(AlphaMap.rows, AlphaMap.cols, CV_8UC1);
		Mat dst2 = Mat::zeros(AlphaMap.rows, AlphaMap.cols, CV_8UC1);
	//	dst2=dst2*255;

		vector<vector<Point> > contours;
	    vector<Vec4i> hierarchy;

	    findContours( AlphaMap, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

		int iNumSegments = contours.size();
		double area;
		double max_area = 0;
		int max_index = 0;;
		for(int i=0;i<iNumSegments;i++)
		{
			vector<Point> approx;
			approxPolyDP(Mat(contours[i]), approx, 1.0e-10, true);
			area = fabs( cv::contourArea( approx ) );
			if (area>max_area){
				max_area = area;
				max_index = i;
			}
		}
		for(int i=0;i<iNumSegments;i++) {
			if (i==max_index){
				drawContours( dst1, contours, i, 1, CV_FILLED, 8, hierarchy );
			}
			else{
				drawContours( dst1, contours, i, 0, CV_FILLED, 8, hierarchy );
			}
	    }

		Mat invertAlphaMap;
		invertAlphaMap = -(dst1-1);

	    findContours( invertAlphaMap, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

		iNumSegments = contours.size();

		max_area = 0;
		max_index = 0;;
		for(int i=0;i<iNumSegments;i++)
		{
			vector<Point> approx;
			approxPolyDP(Mat(contours[i]), approx, 1.0e-10, true);
			area = fabs( cv::contourArea( approx ) );
			if (area>max_area){
				max_area = area;
				max_index = i;
			}
		}
		for(int i=0;i<iNumSegments;i++) {
			if (i==max_index){
				drawContours( dst2, contours, i, 0, CV_FILLED, 8, hierarchy );
			}
			else{
				drawContours( dst2, contours, i, 1, CV_FILLED, 8, hierarchy );
			}
	    }

	//    imshow( "Components1", dst1 );
	//	imshow( "Components2", dst2 );

		AlphaMap = dst1+dst2;

	//	imshow( "AlphaMap", AlphaMap );

		////cout << "Remove isolated blobs" << endl;
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Compositing ////////////////////////////////////
		////stopWatch();

		if (Compositing_background==0){
			resize(BackgroundImage, BackgroundImage, Size(NearFocus_Image_Color.cols, NearFocus_Image_Color.rows));
		}
		if(fast_mode){
			resize(NearFocusImage_Color_input, NearFocus_Image_Color, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
			Rect rc(thres_crop_width,thres_crop_width,NearFocus_Image_Color.cols-2*thres_crop_width,NearFocus_Image_Color.rows-2*thres_crop_width);
			NearFocus_Image_Color = NearFocus_Image_Color(rc);
			resize(AlphaMap, AlphaMap, Size(NearFocus_Image_Color.cols, NearFocus_Image_Color.rows));
			resize(depthMap, depthMap, Size(NearFocus_Image_Color.cols, NearFocus_Image_Color.rows));
		}

		NearFocus_Image_Color.convertTo(NearFocus_Image_Color,CV_32F);
		AlphaMap.convertTo(AlphaMap,CV_32F);
		BackgroundImage.convertTo(BackgroundImage,CV_32F);

		vector<Mat> ch_BackgroundImage;
		split(BackgroundImage, ch_BackgroundImage);
		vector<Mat> ch_NearFocus_Image_Color;
		split(NearFocus_Image_Color, ch_NearFocus_Image_Color);

		vector<Mat> ch_Result_Image(3);

		if (Compositing_background==1){
			ch_Result_Image[0] = ch_NearFocus_Image_Color[0].mul(AlphaMap) + ch_BackgroundImage[0].mul(1-AlphaMap);
			ch_Result_Image[1] = ch_NearFocus_Image_Color[1].mul(AlphaMap) + ch_BackgroundImage[1].mul(1-AlphaMap);
			ch_Result_Image[2] = ch_NearFocus_Image_Color[2].mul(AlphaMap) + ch_BackgroundImage[2].mul(1-AlphaMap);
		}
		else{
			ch_Result_Image[0] = ch_NearFocus_Image_Color[0].mul(AlphaMap);
			ch_Result_Image[1] = ch_NearFocus_Image_Color[1].mul(AlphaMap) + ch_NearFocus_Image_Color[1].mul(1-AlphaMap);
			ch_Result_Image[2] = ch_NearFocus_Image_Color[2].mul(AlphaMap);
		}

		merge(ch_Result_Image,Result_Image);

		////cout << "Compositing" << endl;
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// display ////////////////////////////////////
	/*	double minVal;
		double maxVal;

		minMaxIdx(AlphaMap, &minVal, &maxVal);
		Mat AlphaMap_display;
		AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

		namedWindow( "AlphaMap", CV_WINDOW_NORMAL );
		imshow("AlphaMap", AlphaMap_display);
	*/
		Result_Image.convertTo(Result_Image,CV_8U);
		////namedWindow( "Result_Image", CV_WINDOW_KEEPRATIO );
		////imshow("Result_Image", Result_Image);

		////imwrite( "Result_Image.jpg", Result_Image );
		imwrite( fileName + "_Result_Image.jpg", Result_Image );
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Retouching ////////////////////////////////////
		RGB_Image = NearFocus_Image_Color;
		RGB_Image.convertTo(RGB_Image,CV_8UC3);

		R_Trimap = AlphaMap*255;
		R_Trimap.convertTo(R_Trimap,CV_8UC1);

		ThresControl(500, 500, 100, 0.7);

		////setMouseCallback("Result_Image", CallBackFunc, NULL);

		////waitKey(0);

		////return 0;

		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "Pro_main() END");
}
#endif

void ThresControl_170614(Mat Dmap, int x1, int y1, int x2, int y2, double Lthreshold)
{
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "ThresControl_170614() START");
    int width, height;
    Scalar color2(0, 0, 255);
    int thickness = 3;

    ////lu_x = x-radius;
    ////lu_y = y-radius;
    ////rd_x = x+radius;
    ////rd_y = y+radius;

    lu_x = x1;
    lu_y = y1;
    rd_x = x2;
    rd_y = y2;

    if (lu_x < 0)
    {
        lu_x = 0;
    }
    if (lu_y < 0)
    {
        lu_y = 0;
    }
    if (rd_x > InputImage_for_retouching.cols - 1)
    {
        rd_x = InputImage_for_retouching.cols;
    }
    if (rd_y > InputImage_for_retouching.rows - 1)
    {
        rd_y = InputImage_for_retouching.rows;
    }
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag,
                        "InputImage_for_retouching.cols:%d, InputImage_for_retouching.rows:%d",
                        InputImage_for_retouching.cols, InputImage_for_retouching.rows);
    width = rd_x - lu_x;
    height = rd_y - lu_y;

    Rect rc(lu_x, lu_y, width, height);
//	rectangle(ForegroundImage_diplay, rc, color2, thickness);

    Mat cropped_InputImage_for_retouching = InputImage_for_retouching(rc);
    Mat croppped_DepthMap = Dmap(rc);
    Mat Local_AlphaMap;
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "ThresControl_170614() 3");
    croppped_DepthMap.convertTo(croppped_DepthMap, CV_32F);
    croppped_DepthMap = croppped_DepthMap / 255;

//	double init_Lthreshold = threshold(croppped_DepthMap, Local_AlphaMap, Lthreshold, 1, CV_THRESH_BINARY | CV_THRESH_OTSU);

    threshold(croppped_DepthMap, Local_AlphaMap, Lthreshold, 1, CV_THRESH_BINARY);

    cropped_InputImage_for_retouching.convertTo(cropped_InputImage_for_retouching, CV_32F);
    Local_AlphaMap.convertTo(Local_AlphaMap, CV_32F);

    vector<Mat> ch_cropped_InputImage_for_retouching;
    split(cropped_InputImage_for_retouching, ch_cropped_InputImage_for_retouching);

    Mat Local_ForegroundImage_diplay;
    vector<Mat> ch_Local_ForegroundImage_diplay(3);
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "ThresControl_170614() 4");

    if (bg_color == 1)
    {
        ch_Local_ForegroundImage_diplay[0] =
                ch_cropped_InputImage_for_retouching[0].mul(Local_AlphaMap) +
                ch_cropped_InputImage_for_retouching[1].mul(1 - Local_AlphaMap);
        ch_Local_ForegroundImage_diplay[1] = ch_cropped_InputImage_for_retouching[1].mul(
                Local_AlphaMap);
        ch_Local_ForegroundImage_diplay[2] = ch_cropped_InputImage_for_retouching[2].mul(
                Local_AlphaMap);
    }
    else if (bg_color == 2)
    {
        ch_Local_ForegroundImage_diplay[0] = ch_cropped_InputImage_for_retouching[0].mul(
                Local_AlphaMap);
        ch_Local_ForegroundImage_diplay[1] =
                ch_cropped_InputImage_for_retouching[1].mul(Local_AlphaMap) +
                ch_cropped_InputImage_for_retouching[1].mul(1 - Local_AlphaMap);
        ch_Local_ForegroundImage_diplay[2] = ch_cropped_InputImage_for_retouching[2].mul(
                Local_AlphaMap);
    }
    else if (bg_color == 3)
    {
        ch_Local_ForegroundImage_diplay[0] = ch_cropped_InputImage_for_retouching[0].mul(
                Local_AlphaMap);
        ch_Local_ForegroundImage_diplay[1] = ch_cropped_InputImage_for_retouching[1].mul(
                Local_AlphaMap);
        ch_Local_ForegroundImage_diplay[2] =
                ch_cropped_InputImage_for_retouching[2].mul(Local_AlphaMap) +
                ch_cropped_InputImage_for_retouching[1].mul(1 - Local_AlphaMap);
    }
    ////ch_Local_ForegroundImage_diplay[0] = ch_cropped_InputImage_for_retouching[0].mul(Local_AlphaMap);
    ////ch_Local_ForegroundImage_diplay[1] = ch_cropped_InputImage_for_retouching[1].mul(Local_AlphaMap) + ch_cropped_InputImage_for_retouching[1].mul(1-Local_AlphaMap);
    ////ch_Local_ForegroundImage_diplay[2] = ch_cropped_InputImage_for_retouching[2].mul(Local_AlphaMap);

    merge(ch_Local_ForegroundImage_diplay, Local_ForegroundImage_diplay);
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "ThresControl_170614() 5");
    Local_ForegroundImage_diplay.convertTo(Local_ForegroundImage_diplay, CV_8U);

    Local_ForegroundImage_diplay.copyTo(ForegroundImage_diplay(rc));

    Local_AlphaMap.copyTo(AlphaMap(rc));

    Trimap_Init = AlphaMap * 255;
    Trimap_Init.convertTo(Trimap_Init, CV_8UC1);

    ////imshow("Local_ForegroundImage_diplay", Local_ForegroundImage_diplay);
    ////imshow("ThresControl ForegroundImage_diplay", ForegroundImage_diplay);
    imwrite(filePath + "_Local_ForegroundImage_diplay.jpg", Local_ForegroundImage_diplay);

    imageRotationSave("_Result_Image.jpg", ForegroundImage_diplay); //GN_170711
    ////imwrite( filePath  +"_Result_Image.jpg", ForegroundImage_diplay );
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "ThresControl_170614() 6");
}

void ThresControl(int x, int y, int radius, double Lthreshold)
{
    int width, height;
    Scalar color2(0, 0, 255);
    int thickness = 3;

    lu_x = x - radius;
    lu_y = y - radius;
    rd_x = x + radius;
    rd_y = y + radius;

    if (lu_x < 0)
    {
        lu_x = 0;
    }
    if (lu_y < 0)
    {
        lu_y = 0;
    }
    if (rd_x > RGB_Image.cols - 1)
    {
        rd_x = RGB_Image.cols;
    }
    if (rd_y > RGB_Image.rows - 1)
    {
        rd_y = RGB_Image.rows;
    }

    width = rd_x - lu_x;
    height = rd_y - lu_y;

    Rect rc(lu_x, lu_y, width, height);
//	rectangle(Result_Image, rc, color2, thickness);
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "ThresControl() 1");
    Mat cropped_RGB_Image = RGB_Image(rc);
    Mat croppped_depthMap = depthMap(rc);
    Mat Local_AlphaMap;
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "ThresControl() 2");
    croppped_depthMap.convertTo(croppped_depthMap, CV_32F);
    croppped_depthMap = croppped_depthMap / 255;

    threshold(croppped_depthMap, Local_AlphaMap, Lthreshold, 1, CV_THRESH_BINARY);
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "ThresControl() 3");
    cropped_RGB_Image.convertTo(cropped_RGB_Image, CV_32F);
    Local_AlphaMap.convertTo(Local_AlphaMap, CV_32F);
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "ThresControl() 4");
    vector<Mat> ch_cropped_RGB_Image;
    split(cropped_RGB_Image, ch_cropped_RGB_Image);
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "ThresControl() 5");
    Mat Local_Result_Image;
    vector<Mat> ch_Local_Result_Image(3);

    ch_Local_Result_Image[0] = ch_cropped_RGB_Image[0].mul(Local_AlphaMap);
    ch_Local_Result_Image[1] = ch_cropped_RGB_Image[1].mul(Local_AlphaMap) +
                               ch_cropped_RGB_Image[1].mul(1 - Local_AlphaMap);
    ch_Local_Result_Image[2] = ch_cropped_RGB_Image[2].mul(Local_AlphaMap);

    merge(ch_Local_Result_Image, Local_Result_Image);

    Local_Result_Image.convertTo(Local_Result_Image, CV_8U);

    Local_Result_Image.copyTo(Result_Image(rc));

    imwrite(filePath + "Local_Result_Image.jpg", Local_Result_Image);
    imwrite(filePath + "ThresControl_Result_Image.jpg", Result_Image);
    ////imshow("Local_Result_Image", Local_Result_Image);
    ////imshow("ThresControl Result_Image", Result_Image);
}

int Pro_main(string fileName)
{
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Pro_main() START");

    filePath = fileName;

    ///////////////////////
    // mode setting
    bool fast_mode = 1;

    bool Adaptive_thres = 1;

    bool Align_Far2Near = 0;

    bool Compositing_background = 1;//// 0

    // paramters
    int imresize_factor = 2;

    int size_window;////
    if (fast_mode)
    {
        size_window = 13;
    }
    else
    {
        size_window = 25;
    }

    float thres_foreground = 0.6;
    float thres_background = 0.4;

    int thres_crop_width = 50;

    int thres_smoothing_factor = 2;////
    int r;

    r = thres_smoothing_factor * size_window;

    /*	if(fast_mode)
			r = 25;
		else
			r = 50;
	*/
    double eps = 100;
    //	eps *= 100 * 100;   // intensity range [0, 255]
    /////////////////
    int i, j;

    String imageName_NearFocus(fileName + "_N.jpg");
    String imageName_FarFocus(fileName + "_F.jpg");
    String imageName_Background("/sdcard/Pictures/PASTE/N3.jpg");


    ////
    //	String imageName_NearFocus( "../images/t1_N.jpg" );
    //	String imageName_FarFocus( "../images/t1_F.jpg" );
    //	String imageName_Background( "../images/N3.jpg" );
    //	String imageName_NearFocus( "../images/PASTE_N.jpg" );
    //  String imageName_FarFocus( "../images/PASTE_F.jpg" );
    //	String imageName_Background( "../images/test_background_HTC.jpg" );

    Mat NearFocusImage_Color_input;
    Mat FarFocusImage_Color_input;
    Mat NearFocusImage_Color;
    Mat FarFocusImage_Color;
    Mat NearFocusImage_Gray;
    Mat FarFocusImage_Gray;

    NearFocusImage_Color_input = imread(imageName_NearFocus, IMREAD_COLOR);
    FarFocusImage_Color_input = imread(imageName_FarFocus, IMREAD_COLOR);

    BackgroundImage = imread(imageName_Background, IMREAD_COLOR);

    if (NearFocusImage_Color_input.empty() || FarFocusImage_Color_input.empty() ||
        BackgroundImage.empty())
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    resize(NearFocusImage_Color_input, NearFocusImage_Color,
           Size(NearFocusImage_Color_input.cols / imresize_factor,
                NearFocusImage_Color_input.rows / imresize_factor));
    resize(FarFocusImage_Color_input, FarFocusImage_Color,
           Size(FarFocusImage_Color_input.cols / imresize_factor,
                FarFocusImage_Color_input.rows / imresize_factor));

    /*	if(BackgroundImage.cols == NearFocusImage_Color_input.cols & BackgroundImage.rows == NearFocusImage_Color_input.rows){
			resize(BackgroundImage, BackgroundImage, Size(BackgroundImage.cols/imresize_factor, BackgroundImage.rows/imresize_factor));
		}
		else{
			resize(BackgroundImage, BackgroundImage, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
		}*/

    resize(BackgroundImage, BackgroundImage,
           Size(BackgroundImage.cols / imresize_factor, BackgroundImage.rows / imresize_factor));

    //	namedWindow( "Near Focus Image", CV_WINDOW_AUTOSIZE );
    //	imshow( "Near Focus Image", NearFocusImage_Color_input );

    //	namedWindow( "Far Focus Image", CV_WINDOW_AUTOSIZE );
    //	imshow( "Far Focus Image", FarFocusImage_Color );

    cvtColor(NearFocusImage_Color, NearFocusImage_Gray, CV_RGB2GRAY);
    cvtColor(FarFocusImage_Color, FarFocusImage_Gray, CV_RGB2GRAY);

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Image alignment ////////////////////////////////////
    ////stopWatch();

    vector<cv::KeyPoint> keypoints_N, keypoints_F;
    Mat descriptors_N, descriptors_F;

    ORB orb;

    OrbFeatureDetector detector(500, 1.2f, 4, 25, 0, 2, 0, 25);
    OrbDescriptorExtractor extractor;

    orb.detect(NearFocusImage_Gray, keypoints_N);
    orb.compute(NearFocusImage_Gray, keypoints_N, descriptors_N);

    orb.detect(FarFocusImage_Gray, keypoints_F);
    orb.compute(FarFocusImage_Gray, keypoints_F, descriptors_F);

    BFMatcher matcher;
    vector<DMatch> matches;
    Mat img_matches;

    matcher.match(descriptors_N, descriptors_F, matches);

    double max_dist = 0;
    double min_dist = 100;

    for (i = 0; i < descriptors_N.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    vector<cv::DMatch> good_matches;

    for (i = 0; i < descriptors_N.rows; i++)
    {
        if (matches[i].distance < (max_dist / 1.6))
        { good_matches.push_back(matches[i]); }
    }

    //	drawMatches(NearFocusImage_Gray, keypoints_N, FarFocusImage_Gray, keypoints_F, \
	//          good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
    //                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //	namedWindow( "img_matches", CV_WINDOW_FULLSCREEN );
    //	imshow("img_matches", img_matches );


    std::vector<Point2f> Gp_N;
    std::vector<Point2f> Gp_F;

    for (i = 0; i < good_matches.size(); i++)
    {
        Gp_N.push_back(keypoints_N[good_matches[i].queryIdx].pt);
        Gp_F.push_back(keypoints_F[good_matches[i].trainIdx].pt);
    }

    Mat alined_FarFocusImage_Gray;
    Mat alined_NearFocusImage_Color;

    if (Align_Far2Near)
    {
        Mat transformMat = estimateRigidTransform(Gp_F, Gp_N, false);

        if (transformMat.dims != 0)
        {
            Size dsize = FarFocusImage_Gray.size();
            warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);
        }
        else
        {
            alined_FarFocusImage_Gray = FarFocusImage_Gray;
        }

        Size dsize = NearFocusImage_Gray.size();
        warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);

        ////namedWindow( "FarFocusImage_Gray", CV_WINDOW_NORMAL );
        ////imshow( "FarFocusImage_Gray", alined_FarFocusImage_Gray );

        ////namedWindow( "NearFocusImage_Color", CV_WINDOW_NORMAL );
        ////imshow( "NearFocusImage_Color", NearFocusImage_Color );
    }
    else
    {
        //		Mat transform_H = findHomography( Gp_N, Gp_F, CV_RANSAC );
        //		perspectiveTransform( NearFocusImage_Color, alined_NearFocusImage_Color, transform_H);

        Mat transformMat = estimateRigidTransform(Gp_N, Gp_F, false);

        if (transformMat.dims != 0)
        {
            Size dsize = NearFocusImage_Color.size();
            warpAffine(NearFocusImage_Color, alined_NearFocusImage_Color, transformMat, dsize);
        }
        else
        {
            alined_NearFocusImage_Color = NearFocusImage_Color;
        }

        ////namedWindow( "FarFocusImage_Gray", CV_WINDOW_NORMAL );
        ////imshow( "FarFocusImage_Gray", FarFocusImage_Gray );

        ////namedWindow( "NearFocusImage_Color", CV_WINDOW_NORMAL );
        ////imshow( "NearFocusImage_Color", alined_NearFocusImage_Color );
    }

    ////cout << "Image alignment" << endl;
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// FMR-based depth extraction ////////////////////////////////////
    ////stopWatch();

    Mat NearFocus_Image_Color;
    Mat NearFocus_Image_Gray;
    Mat FarFocus_Image_Gray;

    if (Align_Far2Near)
    {
        NearFocus_Image_Color = NearFocusImage_Color;
        cvtColor(NearFocus_Image_Color, NearFocus_Image_Gray, CV_RGB2GRAY);
        FarFocus_Image_Gray = alined_FarFocusImage_Gray;
    }
    else
    {
        NearFocus_Image_Color = alined_NearFocusImage_Color;
        cvtColor(NearFocus_Image_Color, NearFocus_Image_Gray, CV_RGB2GRAY);
        FarFocus_Image_Gray = FarFocusImage_Gray;
    }

    Mat grad_x, grad_y;
    Mat grad_x_square, grad_y_square;
    Mat FM_Nimg, FM_Fimg;
    Mat FM_Nfocus, FM_Ffocus;

    Sobel(NearFocus_Image_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(NearFocus_Image_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Nimg = grad_x_square + grad_y_square;
    //	boxFilter( FM_Nimg, FM_Nfocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
    GaussianBlur(FM_Nimg, FM_Nfocus, Size(size_window, size_window), 0, 0);

    Sobel(FarFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(FarFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Fimg = (grad_x_square + grad_y_square);
    //	boxFilter( FM_Fimg, FM_Ffocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
    GaussianBlur(FM_Fimg, FM_Ffocus, Size(size_window, size_window), 0, 0);

    float *FM_Nfocus_data = (float *) FM_Nfocus.data;
    float *FM_Ffocus_data = (float *) FM_Ffocus.data;
    float *FMRtemp;
    FMRtemp = new float[NearFocus_Image_Color.cols * NearFocus_Image_Color.rows];

    int length = FM_Nfocus.rows * FM_Nfocus.cols;

    for (i = 0; i < length; i++)
    {
        //		FM_Ffocus_data[i]=FM_Ffocus_data[i]+20;
        if (FM_Ffocus_data[i] > FM_Nfocus_data[i])
        {
            FMRtemp[i] = 2 - FM_Nfocus_data[i] / (FM_Ffocus_data[i] + 0.000001);
        }
        else if (FM_Ffocus_data[i] < FM_Nfocus_data[i])
        {
            FMRtemp[i] = FM_Ffocus_data[i] / (FM_Nfocus_data[i] + 0.000001);
        }
        else
        {
            FMRtemp[i] = 2;
        }
    }

    ////cout << "FMR-based depth extraction" << endl;
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Guided filtering ////////////////////////////////////
    ////stopWatch();

    Mat FMR(NearFocus_Image_Color.rows, NearFocus_Image_Color.cols, CV_32F, FMRtemp);

    Rect rc(thres_crop_width, thres_crop_width, NearFocus_Image_Color.cols - 2 * thres_crop_width,
            NearFocus_Image_Color.rows - 2 * thres_crop_width);

    NearFocus_Image_Color = NearFocus_Image_Color(rc);
    FMR = FMR(rc);

    if (fast_mode)
    {
        resize(NearFocus_Image_Color, NearFocus_Image_Color,
               Size(NearFocus_Image_Color.cols / imresize_factor,
                    NearFocus_Image_Color.rows / imresize_factor));
        resize(FMR, FMR, Size(FMR.cols / imresize_factor, FMR.rows / imresize_factor));
    }

    Mat FMR_filtered = guidedFilter(NearFocus_Image_Color, FMR, r, eps);

    //	Mat FMR_filtered;
    //	bilateralFilter ( FMR, FMR_filtered, 50, 0.1, 25 );


    Mat depthMap = (2 - FMR_filtered) / 2;

    Mat cm_FMR;
    FMR = 255 - FMR * 127;
    FMR.convertTo(FMR, CV_8U);
    ////namedWindow( "FMR", CV_WINDOW_KEEPRATIO );
    ////applyColorMap(FMR, cm_FMR, COLORMAP_JET);
    ////imshow("FMR", cm_FMR);

    ////cout << "Guided filtering" << endl;
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// foreground extraction ////////////////////////////////////
    ////stopWatch();

    //////////////// alpha matting
    /*	Mat Trimap(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Mat Trimap_foreground(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Mat Trimap_background(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Trimap = Scalar(128);
		Trimap_foreground = Scalar(0);
		Trimap_background = Scalar(0);

		threshold(depthMap, Trimap_foreground, thres_foreground, 127, CV_THRESH_BINARY);
		threshold(depthMap, Trimap_background, thres_background, 128, CV_THRESH_BINARY_INV);

		Trimap_foreground.convertTo(Trimap_foreground,CV_8UC1);
		Trimap_background.convertTo(Trimap_background,CV_8UC1);
		add(Trimap,Trimap_foreground,Trimap);
		subtract(Trimap,Trimap_background,Trimap);

		namedWindow( "Trimap", CV_WINDOW_KEEPRATIO );
		imshow("Trimap", Trimap);
		Mat foreground, alpha;
		globalMatting(NearFocus_Image_Color, Trimap, foreground, alpha);

		double minVal;
		double maxVal;
		minMaxIdx(AlphaMap, &minVal, &maxVal);
		Mat AlphaMap_display;
		AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

		namedWindow( "foreground", CV_WINDOW_KEEPRATIO );
		imshow("foreground", foreground);
	*/
    if (Adaptive_thres)
    {
        Mat Binary_AlphaMap;//(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);

        depthMap = depthMap * 255;
        depthMap.convertTo(depthMap, CV_8U);

        Mat cm_depthMap;
        ////namedWindow( "depthMap", CV_WINDOW_KEEPRATIO );
        ////applyColorMap(depthMap, cm_depthMap, COLORMAP_JET);
        ////imshow("depthMap", cm_depthMap);

        float init_foreground_threshhold = threshold(depthMap, Binary_AlphaMap, thres_foreground, 1,
                                                     CV_THRESH_BINARY | CV_THRESH_OTSU);

        AlphaMap = Binary_AlphaMap;

        double minVal;
        double maxVal;

        minMaxIdx(AlphaMap, &minVal, &maxVal);
        Mat AlphaMap_display;
        AlphaMap.convertTo(AlphaMap_display, CV_8UC3, 255.0 / (maxVal - minVal),
                           -minVal * 255.0 / (maxVal - minVal));

        ////namedWindow( "AlphaMap", CV_WINDOW_NORMAL );

        int H = NearFocus_Image_Color.rows;
        int W = NearFocus_Image_Color.cols;

        int height_temp = cvRound(H / 4);
        int width_temp = cvRound(W / 8);

        int start_x, end_x, start_y, end_y;
        int local_width, local_height;

        float cnt_F, cnt_B;
        float balance_ratio;
        float Local_threshhold;

        //	Mat foreground, alpha;

        for (int j = 0; j < H; j += cvRound(height_temp / 2))
        {
            for (int i = 0; i < W; i += cvRound(width_temp / 2))
            {
                start_x = i;
                end_x = i + width_temp;
                start_y = j;
                end_y = j + height_temp;

                if (end_x > W)
                {
                    end_x = W - 1;
                }
                if (end_y > H)
                {
                    end_y = H - 1;
                }

                local_width = end_x - start_x;
                local_height = end_y - start_y;

                Rect rc(start_x, start_y, local_width, local_height);

                //			rectangle(AlphaMap_display, rc,(0,0,255), 3);

                Mat Local_binary_alpha = Binary_AlphaMap(rc);

                cnt_F = 0, cnt_B = 0;

                for (int x = 0; x < Local_binary_alpha.cols; ++x)
                {
                    for (int y = 0; y < Local_binary_alpha.rows; ++y)
                    {
                        if (Local_binary_alpha.at<uchar>(y, x) == 1)
                        {
                            cnt_F = cnt_F + 1;
                        }
                        else if (Local_binary_alpha.at<uchar>(y, x) == 0)
                        {
                            cnt_B = cnt_B + 1;
                        }
                    }
                }

                balance_ratio = min(cnt_F, cnt_B) / max(cnt_F, cnt_B);

                //			cout << "out (" << cnt_F << ", " << cnt_B << ", " << balance_ratio << ")" << endl;

                if (balance_ratio > 0.3)
                {
                    //				rectangle(AlphaMap_display, rc,(0,0,255), 3);
                    Mat Local_depthMap = depthMap(rc);

                    Mat Local_Binary_AlphaMap;
                    Local_threshhold = threshold(Local_depthMap, Local_Binary_AlphaMap,
                                                 thres_foreground, 1,
                                                 CV_THRESH_BINARY | CV_THRESH_OTSU);

                    Mat cropped_RGB_Image = NearFocus_Image_Color(rc);
                    //// slow
                    //				globalMatting(cropped_RGB_Image, cropped_R_Trimap, foreground, alpha);
                    ////
                    Local_Binary_AlphaMap.copyTo(AlphaMap(rc));


                }

            }
        }

        ////imshow("AlphaMap", AlphaMap_display);
    }
    else
    {
        AlphaMap = depthMap;

        threshold(depthMap, AlphaMap, thres_foreground, 1, CV_THRESH_BINARY);
        //	threshold(depthMap, AlphaMap, thres_background, 1, CV_THRESH_TOZERO);
    }

    ////cout << "foreground extraction" << endl;
    ////stopWatch();

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Remove isolated blobs ////////////////////////////////////
    ////stopWatch();

    Mat dst1 = Mat::zeros(AlphaMap.rows, AlphaMap.cols, CV_8UC1);
    Mat dst2 = Mat::zeros(AlphaMap.rows, AlphaMap.cols, CV_8UC1);
    //	dst2=dst2*255;

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours(AlphaMap, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    int iNumSegments = contours.size();
    double area;
    double max_area = 0;
    int max_index = 0;;
    for (int i = 0; i < iNumSegments; i++)
    {
        vector<Point> approx;
        approxPolyDP(Mat(contours[i]), approx, 1.0e-10, true);
        area = fabs(cv::contourArea(approx));
        if (area > max_area)
        {
            max_area = area;
            max_index = i;
        }
    }
    for (int i = 0; i < iNumSegments; i++)
    {
        if (i == max_index)
        {
            drawContours(dst1, contours, i, 1, CV_FILLED, 8, hierarchy);
        }
        else
        {
            drawContours(dst1, contours, i, 0, CV_FILLED, 8, hierarchy);
        }
    }

    Mat invertAlphaMap;
    invertAlphaMap = -(dst1 - 1);

    findContours(invertAlphaMap, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

    iNumSegments = contours.size();

    max_area = 0;
    max_index = 0;;
    for (int i = 0; i < iNumSegments; i++)
    {
        vector<Point> approx;
        approxPolyDP(Mat(contours[i]), approx, 1.0e-10, true);
        area = fabs(cv::contourArea(approx));
        if (area > max_area)
        {
            max_area = area;
            max_index = i;
        }
    }
    for (int i = 0; i < iNumSegments; i++)
    {
        if (i == max_index)
        {
            drawContours(dst2, contours, i, 0, CV_FILLED, 8, hierarchy);
        }
        else
        {
            drawContours(dst2, contours, i, 1, CV_FILLED, 8, hierarchy);
        }
    }

    //    imshow( "Components1", dst1 );
    //	imshow( "Components2", dst2 );

    AlphaMap = dst1 + dst2;

    //	imshow( "AlphaMap", AlphaMap );

    ////cout << "Remove isolated blobs" << endl;
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Compositing ////////////////////////////////////
    ////stopWatch();

    //	if(fast_mode){
    resize(BackgroundImage, BackgroundImage,
           Size(NearFocus_Image_Color.cols, NearFocus_Image_Color.rows));
    //	}
    /*	if(fast_mode){
			resize(NearFocusImage_Color_input, NearFocus_Image_Color, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
			resize(AlphaMap, AlphaMap, Size(NearFocusImage_Color.cols, NearFocusImage_Color.rows));
		}*/

    NearFocus_Image_Color.convertTo(NearFocus_Image_Color, CV_32F);
    AlphaMap.convertTo(AlphaMap, CV_32F);
    BackgroundImage.convertTo(BackgroundImage, CV_32F);

    vector<Mat> ch_BackgroundImage;
    split(BackgroundImage, ch_BackgroundImage);
    vector<Mat> ch_NearFocus_Image_Color;
    split(NearFocus_Image_Color, ch_NearFocus_Image_Color);

    vector<Mat> ch_Result_Image(3);

    if (Compositing_background == 1)
    {
        ch_Result_Image[0] =
                ch_NearFocus_Image_Color[0].mul(AlphaMap) + ch_BackgroundImage[0].mul(1 - AlphaMap);
        ch_Result_Image[1] =
                ch_NearFocus_Image_Color[1].mul(AlphaMap) + ch_BackgroundImage[1].mul(1 - AlphaMap);
        ch_Result_Image[2] =
                ch_NearFocus_Image_Color[2].mul(AlphaMap) + ch_BackgroundImage[2].mul(1 - AlphaMap);
    }
    else
    {
        ch_Result_Image[0] = ch_NearFocus_Image_Color[0].mul(AlphaMap);
        ch_Result_Image[1] = ch_NearFocus_Image_Color[1].mul(AlphaMap) +
                             ch_NearFocus_Image_Color[1].mul(1 - AlphaMap);
        ch_Result_Image[2] = ch_NearFocus_Image_Color[2].mul(AlphaMap);
    }

    merge(ch_Result_Image, Result_Image);

    ////cout << "Compositing" << endl;
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// display ////////////////////////////////////
    /*	double minVal;
		double maxVal;

		minMaxIdx(AlphaMap, &minVal, &maxVal);
		Mat AlphaMap_display;
		AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

		namedWindow( "AlphaMap", CV_WINDOW_NORMAL );
		imshow("AlphaMap", AlphaMap_display);
	*/
    Result_Image.convertTo(Result_Image, CV_8U);
    ////namedWindow( "Result_Image", CV_WINDOW_KEEPRATIO );
    ////imshow("Result_Image", Result_Image);

    ////imwrite( "Result_Image.jpg", Result_Image );
    imwrite(fileName + "_Result_Image.jpg", Result_Image);
    ///TimeCheckEnd();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Retouching ////////////////////////////////////
    RGB_Image = NearFocus_Image_Color;
    RGB_Image.convertTo(RGB_Image, CV_8UC3);

    R_Trimap = AlphaMap * 255;
    R_Trimap.convertTo(R_Trimap, CV_8UC1);

    ////setMouseCallback("Result_Image", CallBackFunc, NULL);

    ////waitKey(0);

    ////return 0;

    ///////////////////////
#if 0
                                                                                                                            // mode setting
		bool fast_mode = 1;

		bool Adaptive_thres = 1;

		bool Align_Far2Near = 1; ////0

	// paramters
		int imresize_factor = 2;

		int size_window = 25;
		float thres_foreground = 0.6;
		float thres_background = 0.4;

		int r;

		if(fast_mode)
			r = 65;
		else
			r = 125;

		double eps = 0.01;
		eps *= 255 * 255;   // intensity range [0, 255]
	/////////////////
		int i,j;

		////String imageName_NearFocus( "../images/30f.jpg" );
		////String imageName_FarFocus( "../images/30ff.jpg" );
		////String imageName_Background( "../images/N3.jpg" );

		String imageName_NearFocus( fileName + "_N.jpg" );
		String imageName_FarFocus( fileName + "_F.jpg" );
		String imageName_Background( "/sdcard/Pictures/PASTE/N3.jpg" );

	//	String imageName_NearFocus( "../images/t1_N.jpg" );
	//	String imageName_FarFocus( "../images/t1_F.jpg" );
	//	String imageName_Background( "../images/N3.jpg" );
	//	String imageName_NearFocus( "../images/PASTE_N.jpg" );
	//  String imageName_FarFocus( "../images/PASTE_F.jpg" );
	//	String imageName_Background( "../images/test_background_HTC.jpg" );

		Mat NearFocusImage_Color_input;
		Mat FarFocusImage_Color_input;
		Mat NearFocusImage_Color;
		Mat FarFocusImage_Color;
		Mat NearFocusImage_Gray;
		Mat FarFocusImage_Gray;

		NearFocusImage_Color_input = imread( imageName_NearFocus, IMREAD_COLOR );
		FarFocusImage_Color_input = imread( imageName_FarFocus, IMREAD_COLOR );

		BackgroundImage = imread( imageName_Background, IMREAD_COLOR );

		if( NearFocusImage_Color_input.empty() || FarFocusImage_Color_input.empty() || BackgroundImage.empty() )
		{
			cout <<  "Could not open or find the image" << std::endl ;
			return -1;
		}

		resize(NearFocusImage_Color_input, NearFocusImage_Color, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
		resize(FarFocusImage_Color_input, FarFocusImage_Color, Size(FarFocusImage_Color_input.cols/imresize_factor, FarFocusImage_Color_input.rows/imresize_factor));

	/*	if(BackgroundImage.cols == NearFocusImage_Color_input.cols & BackgroundImage.rows == NearFocusImage_Color_input.rows){
			resize(BackgroundImage, BackgroundImage, Size(BackgroundImage.cols/imresize_factor, BackgroundImage.rows/imresize_factor));
		}
		else{
			resize(BackgroundImage, BackgroundImage, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
		}*/

		resize(BackgroundImage, BackgroundImage, Size(BackgroundImage.cols/imresize_factor, BackgroundImage.rows/imresize_factor));

	//	namedWindow( "Near Focus Image", CV_WINDOW_AUTOSIZE );
	//	imshow( "Near Focus Image", NearFocusImage_Color_input );

	//	namedWindow( "Far Focus Image", CV_WINDOW_AUTOSIZE );
	//	imshow( "Far Focus Image", FarFocusImage_Color );

		cvtColor(NearFocusImage_Color, NearFocusImage_Gray, CV_RGB2GRAY);
		cvtColor(FarFocusImage_Color, FarFocusImage_Gray, CV_RGB2GRAY);

	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Image alignment ////////////////////////////////////
		////stopWatch();

		vector<cv::KeyPoint> keypoints_N, keypoints_F;
		Mat descriptors_N, descriptors_F;

		ORB orb;

		OrbFeatureDetector detector(500, 1.2f, 4, 25, 0, 2, 0, 25);
		OrbDescriptorExtractor extractor;

		orb.detect(NearFocusImage_Gray, keypoints_N);
		orb.compute(NearFocusImage_Gray, keypoints_N, descriptors_N);

		orb.detect(FarFocusImage_Gray, keypoints_F);
		orb.compute(FarFocusImage_Gray, keypoints_F, descriptors_F);

	    BFMatcher matcher;
	    vector<DMatch> matches;
	    Mat img_matches;

		matcher.match (descriptors_N, descriptors_F, matches);

		double max_dist = 0; double min_dist = 100;

		for( i = 0; i < descriptors_N.rows; i++)
	        { double dist = matches[i].distance;
	            if( dist < min_dist ) min_dist = dist;
	            if( dist > max_dist ) max_dist = dist;
	        }

		vector< cv::DMatch >good_matches;

	        for( i = 0; i < descriptors_N.rows; i++ )

	        { if( matches[i].distance < (max_dist/1.6) )
	            { good_matches.push_back( matches[i]); }
	        }

	//	drawMatches(NearFocusImage_Gray, keypoints_N, FarFocusImage_Gray, keypoints_F, \
	//          good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
	//                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//	namedWindow( "img_matches", CV_WINDOW_FULLSCREEN );
	//	imshow("img_matches", img_matches );


		std::vector<Point2f> Gp_N;
		std::vector<Point2f> Gp_F;

		for( i = 0; i < good_matches.size(); i++ )
		{
			Gp_N.push_back( keypoints_N[ good_matches[i].queryIdx ].pt );
			Gp_F.push_back( keypoints_F[ good_matches[i].trainIdx ].pt );
		}

		Mat alined_FarFocusImage_Gray;
		Mat alined_NearFocusImage_Color;

		if(Align_Far2Near){
			Mat transformMat = estimateRigidTransform(Gp_F, Gp_N, false);

			if(transformMat.dims != 0){
				Size dsize = FarFocusImage_Gray.size();
				warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);
			}
			else{
				alined_FarFocusImage_Gray = FarFocusImage_Gray;
			}

			Size dsize = NearFocusImage_Gray.size();
			warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);

			////namedWindow( "FarFocusImage_Gray", CV_WINDOW_NORMAL );
			////imshow( "FarFocusImage_Gray", alined_FarFocusImage_Gray );

			////namedWindow( "NearFocusImage_Color", CV_WINDOW_NORMAL );
			////imshow( "NearFocusImage_Color", NearFocusImage_Color );
		}
		else{
	//		Mat transform_H = findHomography( Gp_N, Gp_F, CV_RANSAC );
	//		perspectiveTransform( NearFocusImage_Color, alined_NearFocusImage_Color, transform_H);

			Mat transformMat = estimateRigidTransform(Gp_N, Gp_F, false);

			if(transformMat.dims != 0){
				Size dsize = NearFocusImage_Color.size();
				warpAffine(NearFocusImage_Color, alined_NearFocusImage_Color, transformMat, dsize);
			}
			else{
				alined_NearFocusImage_Color = NearFocusImage_Color;
			}

			////namedWindow( "FarFocusImage_Gray", CV_WINDOW_NORMAL );
			////imshow( "FarFocusImage_Gray", FarFocusImage_Gray );

			////namedWindow( "NearFocusImage_Color", CV_WINDOW_NORMAL );
			////imshow( "NearFocusImage_Color", alined_NearFocusImage_Color );
		}

		////cout << "Image alignment" << endl;
		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.Image alignment");
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// FMR-based depth extraction ////////////////////////////////////
		////stopWatch();

		Mat NearFocus_Image_Color;
		Mat NearFocus_Image_Gray;
		Mat FarFocus_Image_Gray;

		if(Align_Far2Near){
			NearFocus_Image_Color = NearFocusImage_Color;
			cvtColor(NearFocus_Image_Color, NearFocus_Image_Gray, CV_RGB2GRAY);
			FarFocus_Image_Gray = alined_FarFocusImage_Gray;
		}
		else{
			NearFocus_Image_Color = alined_NearFocusImage_Color;
			cvtColor(NearFocus_Image_Color, NearFocus_Image_Gray, CV_RGB2GRAY);
			FarFocus_Image_Gray = FarFocusImage_Gray;
		}

		Mat grad_x,grad_y;
		Mat grad_x_square, grad_y_square;
		Mat FM_Nimg,FM_Fimg;
		Mat FM_Nfocus,FM_Ffocus;

		Sobel( NearFocus_Image_Gray, grad_x, CV_32F, 1, 0, 3);
		multiply(grad_x, grad_x, grad_x_square);

		Sobel( NearFocus_Image_Gray, grad_y, CV_32F, 0, 1, 3);
		multiply(grad_y, grad_y, grad_y_square);

		FM_Nimg = grad_x_square + grad_y_square;
	//	boxFilter( FM_Nimg, FM_Nfocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
		GaussianBlur( FM_Nimg, FM_Nfocus, Size(size_window,size_window), 0, 0 );

		Sobel( FarFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
		multiply(grad_x, grad_x, grad_x_square);

		Sobel( FarFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
		multiply(grad_y, grad_y, grad_y_square);

		FM_Fimg = (grad_x_square + grad_y_square);
	//	boxFilter( FM_Fimg, FM_Ffocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
		GaussianBlur( FM_Fimg, FM_Ffocus, Size(size_window,size_window), 0, 0 );

		float* FM_Nfocus_data = (float*)FM_Nfocus.data;
		float* FM_Ffocus_data = (float*)FM_Ffocus.data;
		float* FMRtemp;
		FMRtemp = new float[NearFocus_Image_Color.cols*NearFocus_Image_Color.rows];

		int length = FM_Nfocus.rows  * FM_Nfocus.cols;

		for (i = 0; i < length; i++){
	//		FM_Ffocus_data[i]=FM_Ffocus_data[i]+20;
			if (FM_Ffocus_data[i] >= FM_Nfocus_data[i]){
				FMRtemp[i] = 2 - FM_Nfocus_data[i] / (FM_Ffocus_data[i] + 0.000001);
			}
			else{
				FMRtemp[i] = FM_Ffocus_data[i] / (FM_Nfocus_data[i] + 0.000001);
			}
	    }

		////cout << "FMR-based depth extraction" << endl;
		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.FMR-based depth extraction");
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Guided filtering ////////////////////////////////////
		////stopWatch();

		Mat FMR(NearFocus_Image_Color.rows, NearFocus_Image_Color.cols, CV_32F, FMRtemp);

		if(fast_mode){
			resize(NearFocus_Image_Color, NearFocus_Image_Color, Size(NearFocus_Image_Color.cols/imresize_factor, NearFocus_Image_Color.rows/imresize_factor));
			resize(FMR, FMR, Size(FMR.cols/imresize_factor, FMR.rows/imresize_factor));
		}

		Mat FMR_filtered = guidedFilter(NearFocus_Image_Color, FMR, r, eps);

		Mat depthMap = (2-FMR_filtered)/2;

		////cout << "Guided filtering" << endl;
		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.Guided filtering");
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// foreground extraction ////////////////////////////////////
		////stopWatch();

	//////////////// alpha matting
	/*	Mat Trimap(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Mat Trimap_foreground(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Mat Trimap_background(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Trimap = Scalar(128);
		Trimap_foreground = Scalar(0);
		Trimap_background = Scalar(0);

		threshold(depthMap, Trimap_foreground, thres_foreground, 127, CV_THRESH_BINARY);
		threshold(depthMap, Trimap_background, thres_background, 128, CV_THRESH_BINARY_INV);

		Trimap_foreground.convertTo(Trimap_foreground,CV_8UC1);
		Trimap_background.convertTo(Trimap_background,CV_8UC1);
		add(Trimap,Trimap_foreground,Trimap);
		subtract(Trimap,Trimap_background,Trimap);

		namedWindow( "Trimap", CV_WINDOW_KEEPRATIO );
		imshow("Trimap", Trimap);
		Mat foreground, alpha;
		globalMatting(NearFocus_Image_Color, Trimap, foreground, alpha);

		double minVal;
		double maxVal;
		minMaxIdx(AlphaMap, &minVal, &maxVal);
		Mat AlphaMap_display;
		AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

		namedWindow( "foreground", CV_WINDOW_KEEPRATIO );
		imshow("foreground", foreground);
	*/
		if(Adaptive_thres)
		{
			Mat Binary_AlphaMap;//(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);

			depthMap = depthMap*255;
			depthMap.convertTo(depthMap,CV_8U);

			float init_foreground_threshhold = threshold(depthMap, Binary_AlphaMap, thres_foreground, 1, CV_THRESH_BINARY | CV_THRESH_OTSU);

			AlphaMap = Binary_AlphaMap;

			double minVal;
			double maxVal;

			minMaxIdx(AlphaMap, &minVal, &maxVal);
			Mat AlphaMap_display;
			AlphaMap.convertTo(AlphaMap_display,CV_8UC3, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

			////namedWindow( "AlphaMap", CV_WINDOW_NORMAL );

			int H = NearFocus_Image_Color.rows;
			int W = NearFocus_Image_Color.cols;

			int height_temp = cvRound(H/4);
			int width_temp = cvRound(W/8);

			int start_x,end_x,start_y,end_y;
			int local_width,local_height;

			float cnt_F,cnt_B;
			float balance_ratio;
			float Local_threshhold;

		//	Mat foreground, alpha;

			for(int j=0;j<H;j+=cvRound(height_temp/2))
			{
				for(int i=0;i<W;i+=cvRound(width_temp/2))
				{
					start_x = i;
					end_x = i+width_temp;
					start_y = j;
					end_y = j+height_temp;

					if(end_x>W)
						end_x=W-1;
					if(end_y>H)
						end_y=H-1;

					local_width = end_x - start_x;
					local_height = end_y - start_y;

					Rect rc(start_x,start_y,local_width,local_height);

		//			rectangle(AlphaMap_display, rc,(0,0,255), 3);

					Mat Local_binary_alpha = Binary_AlphaMap(rc);

					cnt_F=0,cnt_B=0;

					for (int x = 0; x < Local_binary_alpha.cols; ++x){
						for (int y = 0; y < Local_binary_alpha.rows; ++y){
							if (Local_binary_alpha.at<uchar>(y, x) == 1)
								cnt_F=cnt_F+1;
							else if (Local_binary_alpha.at<uchar>(y, x) == 0)
								cnt_B=cnt_B+1;
						}
					}

					balance_ratio = min(cnt_F,cnt_B)/max(cnt_F,cnt_B);

		//			cout << "out (" << cnt_F << ", " << cnt_B << ", " << balance_ratio << ")" << endl;

					if (balance_ratio>0.3)
					{
		//				rectangle(AlphaMap_display, rc,(0,0,255), 3);
						Mat Local_depthMap = depthMap(rc);

						Mat Local_Binary_AlphaMap;
						Local_threshhold = threshold(Local_depthMap, Local_Binary_AlphaMap, thres_foreground, 1, CV_THRESH_BINARY | CV_THRESH_OTSU);

						Mat cropped_RGB_Image = NearFocus_Image_Color(rc);
						//// slow
		//				globalMatting(cropped_RGB_Image, cropped_R_Trimap, foreground, alpha);
						////
						Local_Binary_AlphaMap.copyTo(AlphaMap(rc));


					}

				}
			}

			////imshow("AlphaMap", AlphaMap_display);
		}
		else
		{
			AlphaMap = depthMap;

			threshold(depthMap, AlphaMap, thres_foreground, 1, CV_THRESH_BINARY);
		//	threshold(depthMap, AlphaMap, thres_background, 1, CV_THRESH_TOZERO);
		}

		////cout << "foreground extraction" << endl;
		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.foreground extraction");
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Compositing ////////////////////////////////////
		////stopWatch();

	//	if(fast_mode){
	//		resize(BackgroundImage, BackgroundImage, Size(BackgroundImage.cols/imresize_factor, BackgroundImage.rows/imresize_factor));
	//	}
		if(fast_mode){
			resize(NearFocusImage_Color_input, NearFocus_Image_Color, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
			resize(AlphaMap, AlphaMap, Size(NearFocusImage_Color.cols, NearFocusImage_Color.rows));
		}

		NearFocus_Image_Color.convertTo(NearFocus_Image_Color,CV_32F);
		AlphaMap.convertTo(AlphaMap,CV_32F);
		BackgroundImage.convertTo(BackgroundImage,CV_32F);

		vector<Mat> ch_BackgroundImage;
		split(BackgroundImage, ch_BackgroundImage);
		vector<Mat> ch_NearFocus_Image_Color;
		split(NearFocus_Image_Color, ch_NearFocus_Image_Color);

		vector<Mat> ch_Result_Image(3);

		ch_Result_Image[0] = ch_NearFocus_Image_Color[0].mul(AlphaMap) + ch_BackgroundImage[0].mul(1-AlphaMap);
		ch_Result_Image[1] = ch_NearFocus_Image_Color[1].mul(AlphaMap) + ch_BackgroundImage[1].mul(1-AlphaMap);
		ch_Result_Image[2] = ch_NearFocus_Image_Color[2].mul(AlphaMap) + ch_BackgroundImage[2].mul(1-AlphaMap);

		merge(ch_Result_Image,Result_Image);

		////cout << "Compositing" << endl;
		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.Compositing");
		////stopWatch();
	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// display ////////////////////////////////////
	/*	double minVal;
		double maxVal;

		minMaxIdx(AlphaMap, &minVal, &maxVal);
		Mat AlphaMap_display;
		AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

		namedWindow( "AlphaMap", CV_WINDOW_NORMAL );
		imshow("AlphaMap", AlphaMap_display);
	*/
		Result_Image.convertTo(Result_Image,CV_8U);
		////namedWindow( "Result_Image", CV_WINDOW_KEEPRATIO );
		////imshow("Result_Image", Result_Image);

		////imwrite( "Result_Image.jpg", Result_Image );
		imwrite( fileName + "_Result_Image.jpg", Result_Image );

	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////// Retouching ////////////////////////////////////
		RGB_Image = NearFocus_Image_Color;
		RGB_Image.convertTo(RGB_Image,CV_8UC3);

		R_Trimap = AlphaMap*255;
		R_Trimap.convertTo(R_Trimap,CV_8UC1);

		////setMouseCallback("Result_Image", CallBackFunc, NULL);

		////waitKey(0);

		////return 0;
#endif
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Pro_main() END");
}


int Pro_main_(string fileName)
{
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Pro_main() START");
// paramters
    TimeCheckStart();
    filePath = fileName;
    int size_window = 25;
    float thres_foreground = 0.6;
    float thres_background = 0.3;

    int r = 125;
    double eps = 0.01;
    eps *= 255 * 255;   // intensity range [0, 255]
/////////////////
    int i, j;
    int imresize_factor = 2;

    ////String imageName_NearFocus( "/sdcard/Pictures/PASTE/PASTE_N.jpg" );
    ////String imageName_FarFocus( "/sdcard/Pictures/PASTE/PASTE_F.jpg" );

    String imageName_NearFocus(fileName + "_N.jpg");
    String imageName_FarFocus(fileName + "_F.jpg");

    String imageName_Background("/sdcard/Pictures/PASTE/N3.jpg");

    Mat NearFocusImage_Color_input;
    Mat FarFocusImage_Color_input;
    Mat NearFocusImage_Color;
    Mat FarFocusImage_Color;
    Mat NearFocusImage_Gray;
    Mat FarFocusImage_Gray;
    ///Mat BackgroundImage;

    NearFocusImage_Color_input = imread(imageName_NearFocus, IMREAD_COLOR);
    FarFocusImage_Color_input = imread(imageName_FarFocus, IMREAD_COLOR);

    BackgroundImage = imread(imageName_Background, IMREAD_COLOR);

    if (NearFocusImage_Color_input.empty() || FarFocusImage_Color_input.empty() ||
        BackgroundImage.empty())
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    resize(NearFocusImage_Color_input, NearFocusImage_Color,
           Size(NearFocusImage_Color_input.cols / imresize_factor,
                NearFocusImage_Color_input.rows / imresize_factor));
    resize(FarFocusImage_Color_input, FarFocusImage_Color,
           Size(FarFocusImage_Color_input.cols / imresize_factor,
                FarFocusImage_Color_input.rows / imresize_factor));
    resize(BackgroundImage, BackgroundImage,
           Size(BackgroundImage.cols / imresize_factor, BackgroundImage.rows / imresize_factor));

//	namedWindow( "Near Focus Image", CV_WINDOW_NORMAL );
//	imshow( "Near Focus Image", NearFocusImage_Color_input );

//	namedWindow( "Far Focus Image", CV_WINDOW_NORMAL );
//	imshow( "Far Focus Image", FarFocusImage_Color );

    cvtColor(NearFocusImage_Color, NearFocusImage_Gray, CV_RGB2GRAY);
    cvtColor(FarFocusImage_Color, FarFocusImage_Gray, CV_RGB2GRAY);
    TimeCheckEnd();

    TimeCheckStart();
    vector<cv::KeyPoint> keypoints_N, keypoints_F;
    Mat descriptors_N, descriptors_F;

    ORB orb;

    int minHessian = 500;

    OrbFeatureDetector detector(25, 1.0f, 2, 10, 0, 2, 0, 10);
    OrbDescriptorExtractor extractor;

    orb.detect(NearFocusImage_Gray, keypoints_N);
    orb.compute(NearFocusImage_Gray, keypoints_N, descriptors_N);

    orb.detect(FarFocusImage_Gray, keypoints_F);
    orb.compute(FarFocusImage_Gray, keypoints_F, descriptors_F);

    BFMatcher matcher;
    vector<DMatch> matches;
    Mat img_matches;

    matcher.match(descriptors_N, descriptors_F, matches);

    double max_dist = 0;
    double min_dist = 100;

////TimeCheckStart();
    for (i = 0; i < descriptors_N.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }
////TimeCheckEnd();

    vector<cv::DMatch> good_matches;

////TimeCheckStart();
    for (i = 0; i < descriptors_N.rows; i++)
    {
        if (matches[i].distance < (max_dist / 1.6))
        { good_matches.push_back(matches[i]); }
    }
////TimeCheckEnd();
//	drawMatches(NearFocusImage_Gray, keypoints_N, FarFocusImage_Gray, keypoints_F, \
//               good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
//                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

//	imshow("match result", img_matches );


    std::vector<Point2f> Gp_N;
    std::vector<Point2f> Gp_F;

////TimeCheckStart();
    for (i = 0; i < good_matches.size(); i++)
    {
        Gp_N.push_back(keypoints_N[good_matches[i].queryIdx].pt);
        Gp_F.push_back(keypoints_F[good_matches[i].trainIdx].pt);
    }
////TimeCheckEnd();
/*	Mat transformMat = estimateRigidTransform(Gp_F, Gp_N, true);
	Mat alined_FarFocusImage_Gray;
	Size dsize = NearFocusImage_Gray.size();
	warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);
	*/

    Mat transformMat = estimateRigidTransform(Gp_N, Gp_F, true);
    Mat alined_NearFocusImage_Color;
    Size dsize = NearFocusImage_Color.size();
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "t1");
    warpAffine(NearFocusImage_Color, alined_NearFocusImage_Color, transformMat, dsize);
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "t2");
    TimeCheckEnd();

    TimeCheckStart();

//	namedWindow( "alined_FarFocusImage_Gray", CV_WINDOW_NORMAL );
//	imshow( "alined_FarFocusImage_Gray", FarFocusImage_Gray );

//	namedWindow( "NearFocusImage_Gray", CV_WINDOW_NORMAL );
//	imshow( "NearFocusImage_Gray", alined_NearFocusImage_Color );

    Mat alined_NearFocusImage_Gray;
    cvtColor(alined_NearFocusImage_Color, alined_NearFocusImage_Gray, CV_RGB2GRAY);

    Mat grad_x, grad_y;
    Mat grad_x_square, grad_y_square;
    Mat FM_Nimg, FM_Fimg;
    Mat FM_Nfocus, FM_Ffocus;
//	Mat FMR(NearFocusImage_Gray.cols,NearFocusImage_Gray.rows,CV_32F);

    Sobel(alined_NearFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(alined_NearFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Nimg = grad_x_square + grad_y_square;
    boxFilter(FM_Nimg, FM_Nfocus, CV_32F, Size(size_window, size_window), Point(-1, -1), true);
//	GaussianBlur( FM_Nimg, FM_Nfocus, Size(size_window,size_window), 0, 0 );

    Sobel(FarFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(FarFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Fimg = grad_x_square + grad_y_square;
    boxFilter(FM_Fimg, FM_Ffocus, CV_32F, Size(size_window, size_window), Point(-1, -1), true);
//	GaussianBlur( FM_Fimg, FM_Ffocus, Size(size_window,size_window), 0, 0 );

    float *FM_Nfocus_data = (float *) FM_Nfocus.data;
    float *FM_Ffocus_data = (float *) FM_Ffocus.data;
    float *FMRtemp;
    FMRtemp = new float[alined_NearFocusImage_Color.cols * alined_NearFocusImage_Color.rows];

    int length = FM_Nfocus.rows * FM_Nfocus.cols;

////TimeCheckStart();
    for (i = 0; i < length; i++)
    {

        if (FM_Ffocus_data[i] >= FM_Nfocus_data[i])
        {
            FMRtemp[i] = 2 - FM_Nfocus_data[i] / (FM_Ffocus_data[i] + 0.000001);
        }
        else
        {
            FMRtemp[i] = FM_Ffocus_data[i] / (FM_Nfocus_data[i] + 0.000001);
        }

    }
    TimeCheckEnd();
////TimeCheckEnd();
    TimeCheckStart();
    Mat FMR(alined_NearFocusImage_Color.rows, alined_NearFocusImage_Color.cols, CV_32F, FMRtemp);

    Mat FMR_filtered = guidedFilter(alined_NearFocusImage_Color, FMR, r, eps);
    TimeCheckEnd();

    TimeCheckStart();
    ////Mat AlphaMap = (2-FMR_filtered)/2;
    AlphaMap = (2 - FMR_filtered) / 2;

    threshold(AlphaMap, AlphaMap, thres_foreground, 1, CV_THRESH_BINARY);
    threshold(AlphaMap, AlphaMap, thres_background, 1, CV_THRESH_TOZERO);

    alined_NearFocusImage_Color.convertTo(alined_NearFocusImage_Color, CV_32F);
    AlphaMap.convertTo(AlphaMap, CV_32F);
    BackgroundImage.convertTo(BackgroundImage, CV_32F);

    vector<Mat> ch_BackgroundImage;
    split(BackgroundImage, ch_BackgroundImage);
    vector<Mat> ch_alined_NearFocusImage_Color;
    split(alined_NearFocusImage_Color, ch_alined_NearFocusImage_Color);

    vector<Mat> ch_Result_Image(3);

    ch_Result_Image[0] = ch_alined_NearFocusImage_Color[0].mul(AlphaMap) +
                         ch_BackgroundImage[0].mul(1 - AlphaMap);
    ch_Result_Image[1] = ch_alined_NearFocusImage_Color[1].mul(AlphaMap) +
                         ch_BackgroundImage[1].mul(1 - AlphaMap);
    ch_Result_Image[2] = ch_alined_NearFocusImage_Color[2].mul(AlphaMap) +
                         ch_BackgroundImage[2].mul(1 - AlphaMap);

    ///Mat Result_Image;
    merge(ch_Result_Image, Result_Image);
    TimeCheckEnd();

    TimeCheckStart();
///////////// display 용
/*	double minVal;
	double maxVal;

	minMaxIdx(AlphaMap, &minVal, &maxVal);
	Mat AlphaMap_display;
	AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

	namedWindow( "AlphaMap", CV_WINDOW_NORMAL );
	imshow("AlphaMap", AlphaMap_display);
*/
    Result_Image.convertTo(Result_Image, CV_8U);
    ////namedWindow( "Result_Image", CV_WINDOW_NORMAL );
    ////imshow("Result_Image", Result_Image);

    imwrite(fileName + "_Result_Image.jpg", Result_Image);
////////////////////////////////////////////////////////////////
    TimeCheckEnd();
    ////waitKey(0);

    ////return 0;

    //  retouching
    RGB_Image = alined_NearFocusImage_Color;
    RGB_Image.convertTo(RGB_Image, CV_8UC3);

    R_Trimap = AlphaMap * 255;
    R_Trimap.convertTo(R_Trimap, CV_8UC1);

    ////setMouseCallback("Result_Image", CallBackFunc, NULL);

    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Pro_main() END");
}

extern "C" {
JNIEXPORT
void Java_com_dai_t2paste_DAIPA_retouchingDrag(JNIEnv *env, jobject obj, jint x, jint y, jint flags)
{
    ////Trimap_Init = AlphaMap*255;
    ////Trimap_Init.convertTo(Trimap_Init,CV_8UC1);
    if (rotationInfo == 6)
    { // rot270(+inverse)
        RetouchFunc_170623(y, ForegroundImage_diplay.rows - x, flags);
    }
    else
    {
        RetouchFunc_170623(x, y, flags);
    }

    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag,
                        "retouchingDrag() START : x=%d, y=%d, flags=%d", x, y, flags);
    /*
	int Background_color = 2;   // Blue:1 Green:2 Red:3
	PaintingBackground(aligned_NearFocusImage_Color, Background_color);
	imwrite( filePath  +"_DC.jpg", ForegroundImage_diplay );
	*/
}
}

extern "C" {
JNIEXPORT
void Java_com_dai_t2paste_DAIPA_setPolygon(JNIEnv *env, jobject obj, jintArray x, jintArray y,
                                           jboolean bf)
{
    int n = env->GetArrayLength(x);
    int *arrListX = env->GetIntArrayElements(x, NULL);
    int *arrListY = env->GetIntArrayElements(y, NULL);
    for (int i = 0; i < n; i++)
    {

        __android_log_print(ANDROID_LOG_DEBUG, AndroidTag,
                            "setPolygon() START : x[%d]=%d, y[%d]=%d", i, arrListX[i], i,
                            arrListY[i]);
    }
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "setPolygon() START : bf=%d", bf);

    setPolygon(arrListX, arrListY, n, bf);
    //env->ReleaseIntArrayElements(x, arrListX, 0);
    //env->ReleaseIntArrayElements(y, arrListY, 0);
}
}

void setPolygon(int *x, int *y, int n, bool bf)
{
    Point PointArray[1][n];

    int i = 0;
    if (rotationInfo == 6)
    { // rot270(+inverse)
        for (i = 0; i < n; i++)
        {
            PointArray[0][i] = Point(y[i], ForegroundImage_diplay.rows - x[i]);
            __android_log_print(ANDROID_LOG_DEBUG, AndroidTag,
                                "!setPolygon() START : x[%d]=%d, y[%d]=%d", i, x[i], i, y[i]);
        }
    }
    else
    {
        for (i = 0; i < n; i++)
        {
            PointArray[0][i] = Point(x[i], y[i]);
            __android_log_print(ANDROID_LOG_DEBUG, AndroidTag,
                                "!setPolygon() START : x[%d]=%d, y[%d]=%d", i, x[i], i, y[i]);
        }
    }
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "!setPolygon() START : bf=%d", bf);

    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "ThresControl_170614() START");

    const Point *ppt[1] = {PointArray[0]};
    int npt[] = {n};

    if (bf == false)
    {
        fillPoly(AlphaMap, ppt, npt, 1, Scalar(1, 1, 1), 8);
    }
    else
    {
        fillPoly(AlphaMap, ppt, npt, 1, Scalar(0, 0, 0), 8);
    }

    int Background_color = bg_color;   // Blue:1 Green:2 Red:3

    PaintingBackground(aligned_NearFocusImage_Color, Background_color);

    imageRotationSave("_Result_Image.jpg", ForegroundImage_diplay); //GN_170711
    ////imwrite( filePath  +"_Result_Image.jpg", ForegroundImage_diplay );


}

int FF_GetPixel(int x, int y, int z)
{
    ////__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "FF_GetPixel() START 0:%d, 1:%d 2:%d", aligned_NearFocusImage_Color.at<uchar>(y, x, 0), aligned_NearFocusImage_Color.at<uchar>(y, x, 1), aligned_NearFocusImage_Color.at<uchar>(y, x, 2));

    int rst = Old_aligned_NearFocusImage_Color.at<Vec3b>(y, x)[z];
    return rst;

    ////__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "FF_GetPixel() END ");
}

void PutPixel(int x, int y, bool bf)
{
    ///__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "PutPixel() START ");
    if (bf == false)
    {
        AlphaMap.at<float>(y, x) = 1;
    }
    else
    {
        AlphaMap.at<float>(y, x) = 0;
    }

    Old_aligned_NearFocusImage_Color.at<Vec3b>(y, x)[0] = 128;
    Old_aligned_NearFocusImage_Color.at<Vec3b>(y, x)[1] = 128;
    Old_aligned_NearFocusImage_Color.at<Vec3b>(y, x)[2] = 128;

    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "-------PutPixel() END ");
}

void FloodFill_8(int x, int y, int tolerance, int Scalar1, int Scalar2, int Scalar3, bool bf)
{
    ////__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "FloodFill_8() START1 x:%d, y:%d", x, y);
    if ((x < 0) || (x > aligned_NearFocusImage_Color.cols) || (y < 0) ||
        (y > aligned_NearFocusImage_Color.rows))
    {
        return;
    }
/*
	if( (Scalar1 == -1) && (Scalar2 == -1) && (Scalar3 == -1) )
	{
		////__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "FloodFill_8() START2 x:%d, y:%d", x, y);
	//if(nSelColor ==  Scalar(0, 0, 0))
		//nSelColor = FF_GetPixel(x, y);
		Scalar1 = FF_GetPixel(x, y, 0);
		Scalar2 = FF_GetPixel(x, y, 1);
		Scalar3 = FF_GetPixel(x, y, 2);
	}
*/
    ////__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "FloodFill_8() START3 x:%d, y:%d", x, y);
    if ((FF_GetPixel(x, y, 0) < (((Scalar1 + tolerance) > 255) ? 255 : (Scalar1 + tolerance))) &&
        (FF_GetPixel(x, y, 0) > (((Scalar1 - tolerance) < 0) ? 0 : (Scalar1 - tolerance))))
    {
        if ((FF_GetPixel(x, y, 1) <
             (((Scalar2 + tolerance) > 255) ? 255 : (Scalar2 + tolerance))) &&
            (FF_GetPixel(x, y, 1) > (((Scalar2 - tolerance) < 0) ? 0 : (Scalar2 - tolerance))))
        {
            if ((FF_GetPixel(x, y, 2) <
                 (((Scalar3 + tolerance) > 255) ? 255 : (Scalar3 + tolerance))) &&
                (FF_GetPixel(x, y, 2) > (((Scalar3 - tolerance) < 0) ? 0 : (Scalar3 - tolerance))))
            {
                PutPixel(x, y, bf);
                FloodFill_8(x - 1, y, tolerance, Scalar1, Scalar2, Scalar3, bf);
                FloodFill_8(x + 1, y, tolerance, Scalar1, Scalar2, Scalar3, bf);
                FloodFill_8(x, y - 1, tolerance, Scalar1, Scalar2, Scalar3, bf);
                FloodFill_8(x, y + 1, tolerance, Scalar1, Scalar2, Scalar3, bf);

                FloodFill_8(x + 1, y + 1, tolerance, Scalar1, Scalar2, Scalar3, bf);
                FloodFill_8(x + 1, y - 1, tolerance, Scalar1, Scalar2, Scalar3, bf);
                FloodFill_8(x - 1, y + 1, tolerance, Scalar1, Scalar2, Scalar3, bf);
                FloodFill_8(x - 1, y - 1, tolerance, Scalar1, Scalar2, Scalar3, bf);
            }
        }
    }

}

void setDeltaColor(int x1, int y1, bool bf, int tolerance)
{
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag,
                        "setDeltaColor() START1 row:%d, x:%d, col:%d, y:%d",
                        aligned_NearFocusImage_Color.rows, x1, aligned_NearFocusImage_Color.cols,
                        y1);

    Old_aligned_NearFocusImage_Color = aligned_NearFocusImage_Color.clone();

    //FloodFill_8(x1, y1, tolerance, FF_GetPixel(x1, y1, 0), FF_GetPixel(x1, y1, 1), FF_GetPixel(x1, y1, 2), bf);
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "setDeltaColor() START2 c:%d, r:%d, ch:%d",
                        AlphaMap.cols, AlphaMap.rows, AlphaMap.channels());
/*
	string r;
	int type = aligned_NearFocusImage_Color.type();

		uchar depth = type & CV_MAT_DEPTH_MASK;
		uchar chans = 1 + (type >> CV_CN_SHIFT);

		switch (depth)
		{
			case CV_8U:  r = "8U"; break;
			case CV_8S:  r = "8S"; break;
			case CV_16U: r = "16U"; break;
			case CV_16S: r = "16S"; break;
			case CV_32S: r = "32S"; break;
			case CV_32F: r = "32F"; break;
			case CV_64F: r = "64F"; break;
			default:     r = "User"; break;
		}
		__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "setDeltaColor() START3 Matrix: %s %d(w)x%d(h) ch:%d\n", r.c_str(), aligned_NearFocusImage_Color.cols, aligned_NearFocusImage_Color.rows, aligned_NearFocusImage_Color.channels());


	//
	for (int x = 0; x < AlphaMap.cols; x++)//750
		for (int y = 0; y < AlphaMap.rows; y++)//1000
		{

			////AlphaMap.at<uchar>(y, x) = 255;

			AlphaMap.at<float>(y, x) = 1;

		}
*/

    Mat mask;
    mask.create(aligned_NearFocusImage_Color.rows + 2, aligned_NearFocusImage_Color.cols + 2,
                CV_8UC1);
    mask = Scalar::all(0);

    Point PointXY = Point(x1, y1);
    Rect ccomp;
    ////Mat Old_aligned_NearFocusImage_Color = aligned_NearFocusImage_Color;

    ////
    floodFill(Old_aligned_NearFocusImage_Color, mask, PointXY, cvScalarAll(128.0), &ccomp,
              cvScalarAll((double) (tolerance)), cvScalarAll((double) (tolerance)),
              8 | CV_FLOODFILL_MASK_ONLY | (255 << 8));

    //floodFill(aligned_NearFocusImage_Color, PointXY, Scalar(255,255,255), &ccomp, cvScalar(tolerance/10.0f, tolerance/10.0f, tolerance/10.0f), cvScalar(tolerance/10.0f, tolerance/10.0f, tolerance/10.0f));


    for (int x = 0; x < aligned_NearFocusImage_Color.cols; x++)
    {
        for (int y = 0; y < aligned_NearFocusImage_Color.rows; y++)
        {
            //__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "setDeltaColor() y+1:%d, x+1:%d, mask:%d",x, y,  mask.at<uchar>(y+1, x+1));

            if (mask.at<uchar>(y + 1, x + 1) == 255)
            {
                if (bf == false)
                {
                    AlphaMap.at<float>(y, x) = 1;
                }
                else
                {
                    AlphaMap.at<float>(y, x) = 0;
                }

            }

        }
    }
//threshold(mask, mask, 1, 128, THRESH_BINARY);
    int Background_color = bg_color;   // Blue:1 Green:2 Red:3

    PaintingBackground(aligned_NearFocusImage_Color, Background_color);

    imwrite(filePath + "_Result_Image.jpg", ForegroundImage_diplay);
    ////imwrite( filePath  +"_DC.jpg", mask );
}

extern "C" {
JNIEXPORT
void
Java_com_dai_t2paste_DAIPA_setDeltaColor(JNIEnv *env, jobject obj, jint x1, jint y1, jboolean bf,
                                         jint tolerance)
{
    setDeltaColor(x1, y1, bf, tolerance);
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "setDeltaColor() END : %d, %d, %d", x1, y1,
                        tolerance);
}
}

void setLine(int x1, int y1, int x2, int y2, bool bf, int thickness)
{
    Point PointStart;
    Point PointEnd;

    if (rotationInfo == 6)
    { // rot270(+inverse)
        PointStart = Point(y1, ForegroundImage_diplay.rows - x1);
        PointEnd = Point(y2, ForegroundImage_diplay.rows - x2);
    }
    else
    {
        PointStart = Point(x1, y1);
        PointEnd = Point(x2, y2);
    }

    if (bf == false)
    {
        line(AlphaMap, PointStart, PointEnd, Scalar(1, 1, 1), thickness, 4);
    }
    else
    {
        line(AlphaMap, PointStart, PointEnd, Scalar(0, 0, 0), thickness, 4);
    }

    int Background_color = bg_color;   // Blue:1 Green:2 Red:3

    PaintingBackground(aligned_NearFocusImage_Color, Background_color);

    imageRotationSave("_Result_Image.jpg", ForegroundImage_diplay); //GN_170711
    ////imwrite( filePath  +"_Result_Image.jpg", ForegroundImage_diplay );

    ////__android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "setLine() END : x1:%f, y1:%f, x2:%f, y2:%f", x1, y1, x2, y2);
}

extern "C" {
JNIEXPORT
void
Java_com_dai_t2paste_DAIPA_setLine(JNIEnv *env, jobject obj, jint x1, jint y1, jint x2, jint y2,
                                   jboolean bf, jint thickness)
{
    setLine(x1, y1, x2, y2, bf, thickness);
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "setLine() START : %d, %d, %d, %d, %i", x1,
                        y1, x2, y2, thickness);
}
}

extern "C" {
JNIEXPORT
void Java_com_dai_t2paste_DAIPA_setPaintingBackground(JNIEnv *env, jobject obj, jint color)
{
    bg_color = color;
    PaintingBackground(aligned_NearFocusImage_Color, color);
    imageRotationSave("_Result_Image.jpg", ForegroundImage_diplay); //GN_170711
    ////imwrite( filePath  +"_Result_Image.jpg", ForegroundImage_diplay );
}
}

extern "C" {
JNIEXPORT
void Java_com_dai_t2paste_DAIPA_thresControl(JNIEnv *env, jobject obj, jint x1, jint y1, jint x2,
                                             jint y2, jdouble Lthreshold)
{
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag,
                        "thresControl() START : %d, %d, %d, %d, %d, %f", thresholdDepthMap.rows, x1,
                        y1, x2, y2, Lthreshold);

    if (rotationInfo == 6)
    { // rot270(+inverse)
        ThresControl_170614(thresholdDepthMap, y1, thresholdDepthMap.rows - x2, y2,
                            thresholdDepthMap.rows - x1, Lthreshold);
        //ThresControl_170614(thresholdDepthMap,  y2, x2, y1, x1, Lthreshold);
    }
    else
    {
        ThresControl_170614(thresholdDepthMap, x1, y1, x2, y2, Lthreshold);
    }

    ///ThresControl_170614(thresholdDepthMap, x1, y1, x2, y2, Lthreshold);
    ////ThresControl_170614(thresholdDepthMap, 100, 100, 200, 200, 0.5f);
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "thresControl() End");
}
}

void RetouchFunc_170623(int x, int y, int flags)
{
    int circle_radius;

    circle_radius = 21;

    Scalar color(128);
    Scalar color2(0, 0, 255);
    int thickness = 3;
    float thres_foreground_alpha = 0.8;
    int thres_expansion_known = 10;
    float thres_cnt = 0.05;

    float alpha_threshold;

    double minVal;
    double maxVal;

    Mat foreground, Local_AlphaMap, Local_Alphamap_Init, alpha_binary;

    if (flags & CV_EVENT_FLAG_LBUTTON)
    {
        Point center(x, y);

        if (x < lu_x)
        {
            lu_x = x;
        }
        if (y < lu_y)
        {
            lu_y = y;
        }
        if (x > rd_x)
        {
            rd_x = x;
        }
        if (y > rd_y)
        {
            rd_y = y;
        }

        circle(ForegroundImage_diplay, center, circle_radius, color, CV_FILLED);
        circle(Trimap_Init, center, circle_radius, color, CV_FILLED);

        /////imshow("ForegroundImage_diplay", ForegroundImage_diplay);

        /////cout << "Mouse position (" << x << ", " << y << ")" << endl;
    }
    else if ((flags & CV_EVENT_FLAG_RBUTTON) && lu_x != 10000)
    {
        lu_x = lu_x - circle_radius;
        lu_y = lu_y - circle_radius;
        rd_x = rd_x + circle_radius;
        rd_y = rd_y + circle_radius;

        if (lu_x < 0)
        {
            lu_x = 0;
        }
        if (lu_y < 0)
        {
            lu_y = 0;
        }
        if (rd_x > InputImage_for_retouching.cols - 1)
        {
            rd_x = InputImage_for_retouching.cols;
        }
        if (rd_y > InputImage_for_retouching.rows - 1)
        {
            rd_y = InputImage_for_retouching.rows;
        }

        int width_temp, height_temp;
        int width, height;

        width_temp = rd_x - lu_x;
        height_temp = rd_y - lu_y;

        if (width_temp > height_temp)
        {
            lu_y = lu_y - thres_expansion_known;
            rd_y = rd_y + thres_expansion_known;
        }
        else
        {
            lu_x = lu_x - thres_expansion_known;
            rd_x = rd_x + thres_expansion_known;
        }

        if (lu_x < 0)
        {
            lu_x = 0;
        }
        if (lu_y < 0)
        {
            lu_y = 0;
        }
        if (rd_x > InputImage_for_retouching.cols - 1)
        {
            rd_x = InputImage_for_retouching.cols;
        }
        if (rd_y > InputImage_for_retouching.rows - 1)
        {
            rd_y = InputImage_for_retouching.rows;
        }

        width = rd_x - lu_x;
        height = rd_y - lu_y;

        Rect rc(lu_x, lu_y, width, height);

        /////rectangle(ForegroundImage_diplay, rc, color2, thickness);
        /////imshow("ForegroundImage_diplay", ForegroundImage_diplay);
        cout << "window (" << lu_x << ", " << lu_y << ", " << rd_x << ", " << rd_y << ", " << width
             << ", " << height << ")" << endl;
//		expansionOfKnownRegions(InputImage_for_retouching, Trimap_Init, 9);

        Mat cropped_InputImage_for_retouching = InputImage_for_retouching(rc);
        Mat cropped_Trimap = Trimap_Init(rc);
//		Mat Local_raw_DepthMap = raw_DepthMap(rc);

        int cnt_F = 0, cnt_B = 0;

        for (int x = 0; x < cropped_Trimap.cols; ++x)
        {
            for (int y = 0; y < cropped_Trimap.rows; ++y)
            {
                if (cropped_Trimap.at<uchar>(y, x) == 255)
                {
                    cnt_F = cnt_F + 1;
                }
                else if (cropped_Trimap.at<uchar>(y, x) == 0)
                {
                    cnt_B = cnt_B + 1;
                }
            }
        }

        /////cout << "cnt pixel (" << cnt_F << ", " << cnt_B << ")" << endl;

        int tmp = 0;
        if (cnt_F * thres_cnt > cnt_B)
        {
            cropped_Trimap = 255;
        }
        else if (cnt_B * thres_cnt > cnt_F)
        {
            cropped_Trimap = tmp;
        }

        globalMatting(cropped_InputImage_for_retouching, cropped_Trimap, foreground,
                      Local_Alphamap_Init);

        // filter the result with guided filter
        Local_AlphaMap = guidedFilter(cropped_InputImage_for_retouching, Local_Alphamap_Init, 10,
                                      1e-5);

        for (int x = 0; x < cropped_Trimap.cols; ++x)
        {
            for (int y = 0; y < cropped_Trimap.rows; ++y)
            {
                if (cropped_Trimap.at<uchar>(y, x) == 0)
                {
                    Local_AlphaMap.at<uchar>(y, x) = 0;
                }
                else if (cropped_Trimap.at<uchar>(y, x) == 255)
                {
                    Local_AlphaMap.at<uchar>(y, x) = 255;
                }
            }
        }

//		Local_AlphaMap = (Local_AlphaMap + Local_raw_DepthMap)/2;

        Local_AlphaMap.convertTo(Local_AlphaMap, CV_8U);
        alpha_threshold = threshold(Local_AlphaMap, alpha_binary, 0.5, 1,
                                    CV_THRESH_BINARY | CV_THRESH_OTSU);

        ///////////////////////////////////////////////////////////// 이 부분 추가 170626
        if (cnt_F * thres_cnt > cnt_B)
        {
            alpha_threshold = 0;
        }
        else if (cnt_B * thres_cnt > cnt_F)
        {
            alpha_threshold = 255;
        }

        ////////////////////////////////////////////////////////////////
        for (int x = 0; x < alpha_binary.cols; ++x)
        {
            for (int y = 0; y < alpha_binary.rows; ++y)
            {
                if (Local_AlphaMap.at<uchar>(y, x) < alpha_threshold)
                {
                    Local_AlphaMap.at<uchar>(y, x) = 0;
                }
                else if (Local_AlphaMap.at<uchar>(y, x) >= alpha_threshold)
                {
                    Local_AlphaMap.at<uchar>(y, x) = 1;
                }
            }
        }

        cropped_InputImage_for_retouching.convertTo(cropped_InputImage_for_retouching, CV_32F);
        Local_AlphaMap.convertTo(Local_AlphaMap, CV_32F);

        vector<Mat> ch_cropped_InputImage_for_retouching;
        split(cropped_InputImage_for_retouching, ch_cropped_InputImage_for_retouching);

        vector<Mat> ch_cropped_ForegroundImage_diplay(3);
        __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "RetouchFunc_170623() bg_color:%d",
                            bg_color);
        if (bg_color == 1)
        {
            ch_cropped_ForegroundImage_diplay[0] =
                    ch_cropped_InputImage_for_retouching[0].mul(Local_AlphaMap) +
                    ch_cropped_InputImage_for_retouching[1].mul(1 - Local_AlphaMap);
            ch_cropped_ForegroundImage_diplay[1] = ch_cropped_InputImage_for_retouching[1].mul(
                    Local_AlphaMap);
            ch_cropped_ForegroundImage_diplay[2] = ch_cropped_InputImage_for_retouching[2].mul(
                    Local_AlphaMap);
        }
        else if (bg_color == 2)
        {
            ch_cropped_ForegroundImage_diplay[0] = ch_cropped_InputImage_for_retouching[0].mul(
                    Local_AlphaMap);
            ch_cropped_ForegroundImage_diplay[1] =
                    ch_cropped_InputImage_for_retouching[1].mul(Local_AlphaMap) +
                    ch_cropped_InputImage_for_retouching[1].mul(1 - Local_AlphaMap);
            ch_cropped_ForegroundImage_diplay[2] = ch_cropped_InputImage_for_retouching[2].mul(
                    Local_AlphaMap);
        }
        else if (bg_color == 3)
        {
            ch_cropped_ForegroundImage_diplay[0] = ch_cropped_InputImage_for_retouching[0].mul(
                    Local_AlphaMap);
            ch_cropped_ForegroundImage_diplay[1] = ch_cropped_InputImage_for_retouching[1].mul(
                    Local_AlphaMap);
            ch_cropped_ForegroundImage_diplay[2] =
                    ch_cropped_InputImage_for_retouching[2].mul(Local_AlphaMap) +
                    ch_cropped_InputImage_for_retouching[1].mul(1 - Local_AlphaMap);
        }

        Mat cropped_ForegroundImage_diplay;
        merge(ch_cropped_ForegroundImage_diplay, cropped_ForegroundImage_diplay);

        cropped_ForegroundImage_diplay.convertTo(cropped_ForegroundImage_diplay, CV_8UC3);

        cropped_ForegroundImage_diplay.copyTo(ForegroundImage_diplay(rc));
        Local_AlphaMap.copyTo(AlphaMap(rc));

        Local_AlphaMap = Local_AlphaMap * 255;
        Local_AlphaMap.convertTo(Local_AlphaMap, CV_8UC1);
        Local_AlphaMap.copyTo(Trimap_Init(rc));

        /////imshow("ForegroundImage_diplay", ForegroundImage_diplay);
        imageRotationSave("_Result_Image.jpg", ForegroundImage_diplay); //GN_170711
        ////imwrite( filePath  +"_Result_Image.jpg", ForegroundImage_diplay );

        lu_x = 10000, lu_y = 10000, rd_x = 0, rd_y = 0;
    }
}


void RetouchFunc_170613(int x, int y, int flags)
{
    int circle_radius;

    circle_radius = 21;

    Scalar color(128);
    Scalar color2(0, 0, 255);
    int thickness = 3;
    float thres_foreground_alpha = 0.8;
    int thres_expansion_known = 10;
    float thres_cnt = 0.05;

    float alpha_threshold;

    double minVal;
    double maxVal;

    Mat foreground, Local_AlphaMap, Local_Alphamap_Init, alpha_binary;

    if (flags & CV_EVENT_FLAG_LBUTTON)
    {
        Point center(x, y);

        if (x < lu_x)
        {
            lu_x = x;
        }
        if (y < lu_y)
        {
            lu_y = y;
        }
        if (x > rd_x)
        {
            rd_x = x;
        }
        if (y > rd_y)
        {
            rd_y = y;
        }

        circle(ForegroundImage_diplay, center, circle_radius, color, CV_FILLED);
        circle(Trimap_Init, center, circle_radius, color, CV_FILLED);

        ////imshow("ForegroundImage_diplay", ForegroundImage_diplay);

        ////cout << "Mouse position (" << x << ", " << y << ")" << endl;
    }
    else if ((flags & CV_EVENT_FLAG_RBUTTON) && lu_x != 10000)
    {
        lu_x = lu_x - circle_radius;
        lu_y = lu_y - circle_radius;
        rd_x = rd_x + circle_radius;
        rd_y = rd_y + circle_radius;

        if (lu_x < 0)
        {
            lu_x = 0;
        }
        if (lu_y < 0)
        {
            lu_y = 0;
        }
        if (rd_x > InputImage_for_retouching.cols - 1)
        {
            rd_x = InputImage_for_retouching.cols;
        }
        if (rd_y > InputImage_for_retouching.rows - 1)
        {
            rd_y = InputImage_for_retouching.rows;
        }

        int width_temp, height_temp;
        int width, height;

        width_temp = rd_x - lu_x;
        height_temp = rd_y - lu_y;

        if (width_temp > height_temp)
        {
            lu_y = lu_y - thres_expansion_known;
            rd_y = rd_y + thres_expansion_known;
        }
        else
        {
            lu_x = lu_x - thres_expansion_known;
            rd_x = rd_x + thres_expansion_known;
        }

        if (lu_x < 0)
        {
            lu_x = 0;
        }
        if (lu_y < 0)
        {
            lu_y = 0;
        }
        if (rd_x > InputImage_for_retouching.cols - 1)
        {
            rd_x = InputImage_for_retouching.cols;
        }
        if (rd_y > InputImage_for_retouching.rows - 1)
        {
            rd_y = InputImage_for_retouching.rows;
        }

        width = rd_x - lu_x;
        height = rd_y - lu_y;

        Rect rc(lu_x, lu_y, width, height);

        ////rectangle(ForegroundImage_diplay, rc, color2, thickness);
        ////imshow("ForegroundImage_diplay", ForegroundImage_diplay);
        ////cout << "window (" << lu_x << ", " << lu_y << ", " << rd_x << ", " << rd_y << ", " << width << ", " << height << ")" << endl;
//		expansionOfKnownRegions(InputImage_for_retouching, Trimap_Init, 9);

        Mat cropped_InputImage_for_retouching = InputImage_for_retouching(rc);
        Mat cropped_Trimap = Trimap_Init(rc);
//		Mat Local_raw_DepthMap = raw_DepthMap(rc);

        int cnt_F = 0, cnt_B = 0;

        for (int x = 0; x < cropped_Trimap.cols; ++x)
        {
            for (int y = 0; y < cropped_Trimap.rows; ++y)
            {
                if (cropped_Trimap.at<uchar>(y, x) == 255)
                {
                    cnt_F = cnt_F + 1;
                }
                else if (cropped_Trimap.at<uchar>(y, x) == 0)
                {
                    cnt_B = cnt_B + 1;
                }
            }
        }

        ////cout << "cnt pixel (" << cnt_F << ", " << cnt_B << ")" << endl;

        int tmp = 0;////
        if (cnt_F * thres_cnt > cnt_B)
        {
            cropped_Trimap = 255;
        }
        else if (cnt_B * thres_cnt > cnt_F)
        {
            cropped_Trimap = tmp;
        }

        globalMatting(cropped_InputImage_for_retouching, cropped_Trimap, foreground,
                      Local_Alphamap_Init);

        // filter the result with guided filter
        Local_AlphaMap = guidedFilter(cropped_InputImage_for_retouching, Local_Alphamap_Init, 10,
                                      1e-5);

        for (int x = 0; x < cropped_Trimap.cols; ++x)
        {
            for (int y = 0; y < cropped_Trimap.rows; ++y)
            {
                if (cropped_Trimap.at<uchar>(y, x) == 0)
                {
                    Local_AlphaMap.at<uchar>(y, x) = 0;
                }
                else if (cropped_Trimap.at<uchar>(y, x) == 255)
                {
                    Local_AlphaMap.at<uchar>(y, x) = 255;
                }
            }
        }

//		Local_AlphaMap = (Local_AlphaMap + Local_raw_DepthMap)/2;

        Local_AlphaMap.convertTo(Local_AlphaMap, CV_8U);
        alpha_threshold = threshold(Local_AlphaMap, alpha_binary, 0.5, 1,
                                    CV_THRESH_BINARY | CV_THRESH_OTSU);

        for (int x = 0; x < alpha_binary.cols; ++x)
        {
            for (int y = 0; y < alpha_binary.rows; ++y)
            {
                if (Local_AlphaMap.at<uchar>(y, x) < alpha_threshold)
                {
                    Local_AlphaMap.at<uchar>(y, x) = 0;
                }
                else if (Local_AlphaMap.at<uchar>(y, x) >= alpha_threshold)
                {
                    Local_AlphaMap.at<uchar>(y, x) = 1;
                }
            }
        }

        cropped_InputImage_for_retouching.convertTo(cropped_InputImage_for_retouching, CV_32F);
        Local_AlphaMap.convertTo(Local_AlphaMap, CV_32F);

        vector<Mat> ch_cropped_InputImage_for_retouching;
        split(cropped_InputImage_for_retouching, ch_cropped_InputImage_for_retouching);

        vector<Mat> ch_cropped_ForegroundImage_diplay(3);

        ch_cropped_ForegroundImage_diplay[0] = ch_cropped_InputImage_for_retouching[0].mul(
                Local_AlphaMap);
        ch_cropped_ForegroundImage_diplay[1] =
                ch_cropped_InputImage_for_retouching[1].mul(Local_AlphaMap) +
                ch_cropped_InputImage_for_retouching[1].mul(1 - Local_AlphaMap);
        ch_cropped_ForegroundImage_diplay[2] = ch_cropped_InputImage_for_retouching[2].mul(
                Local_AlphaMap);

        Mat cropped_ForegroundImage_diplay;
        merge(ch_cropped_ForegroundImage_diplay, cropped_ForegroundImage_diplay);

        cropped_ForegroundImage_diplay.convertTo(cropped_ForegroundImage_diplay, CV_8UC3);

        cropped_ForegroundImage_diplay.copyTo(ForegroundImage_diplay(rc));
        Local_AlphaMap.copyTo(AlphaMap(rc));

        Local_AlphaMap = Local_AlphaMap * 255;
        Local_AlphaMap.convertTo(Local_AlphaMap, CV_8UC1);
        Local_AlphaMap.copyTo(Trimap_Init(rc));

        ////imshow("ForegroundImage_diplay", ForegroundImage_diplay);

        imwrite(filePath + "_Result_Image_AftRetouching.jpg", Result_Image);
        imwrite(filePath + "_Result_Image.jpg", ForegroundImage_diplay);

        lu_x = 10000, lu_y = 10000, rd_x = 0, rd_y = 0;
    }
}


//void CallBackFunc(int event, int x, int y, int flags, void* userdata)
void RetouchFunc(int x, int y, int flags)
{
    __android_log_print(ANDROID_LOG_DEBUG, AndroidTag, "RetouchFunc() START");
    bool fast_mode = 1;

    int circle_radius = 25;
    Scalar color(128);
    Scalar color2(0, 0, 255);
    int thickness = 3;
    float thres_foreground_alpha = 0.8;
    int thres_expansion_known = 10;
    float thres_cnt = 0.05;

    float alpha_threshold;

    double minVal;
    double maxVal;

    Mat foreground, alpha, alpha_temp, alpha_binary;

    if (flags & CV_EVENT_FLAG_LBUTTON)
    {
        Point center(x, y);

        if (x < lu_x)
        {
            lu_x = x;
        }
        if (y < lu_y)
        {
            lu_y = y;
        }
        if (x > rd_x)
        {
            rd_x = x;
        }
        if (y > rd_y)
        {
            rd_y = y;
        }

        circle(Result_Image, center, circle_radius, color, CV_FILLED);
        circle(R_Trimap, center, circle_radius, color, CV_FILLED);

        ////imshow("Result_Image", Result_Image);

        ////cout << "Mouse position (" << x << ", " << y << ")" << endl;
    }
    else if ((flags & CV_EVENT_FLAG_RBUTTON) && lu_x != 10000)
    {
        lu_x = lu_x - circle_radius;
        lu_y = lu_y - circle_radius;
        rd_x = rd_x + circle_radius;
        rd_y = rd_y + circle_radius;

        if (lu_x < 0)
        {
            lu_x = 0;
        }
        if (lu_y < 0)
        {
            lu_y = 0;
        }
        if (rd_x > RGB_Image.cols - 1)
        {
            rd_x = RGB_Image.cols;
        }
        if (rd_y > RGB_Image.rows - 1)
        {
            rd_y = RGB_Image.rows;
        }

        int width_temp, height_temp;
        int width, height;

        width_temp = rd_x - lu_x;
        height_temp = rd_y - lu_y;

        if (width_temp > height_temp)
        {
            lu_y = lu_y - thres_expansion_known;
            rd_y = rd_y + thres_expansion_known;
        }
        else
        {
            lu_x = lu_x - thres_expansion_known;
            rd_x = rd_x + thres_expansion_known;
        }

        if (lu_x < 0)
        {
            lu_x = 0;
        }
        if (lu_y < 0)
        {
            lu_y = 0;
        }
        if (rd_x > RGB_Image.cols - 1)
        {
            rd_x = RGB_Image.cols;
        }
        if (rd_y > RGB_Image.rows - 1)
        {
            rd_y = RGB_Image.rows;
        }

        width = rd_x - lu_x;
        height = rd_y - lu_y;

        Rect rc(lu_x, lu_y, width, height);

        rectangle(Result_Image, rc, color2, thickness);

        ////imshow("Result_Image", Result_Image);

        ////cout << "window (" << lu_x << ", " << lu_y << ", " << rd_x << ", " << rd_y << ", " << width << ", " << height << ")" << endl;
//		expansionOfKnownRegions(RGB_Image, R_Trimap, 9);

        Mat cropped_RGB_Image = RGB_Image(rc);
        Mat cropped_R_Trimap = R_Trimap(rc);
        Mat cropped_BackgroundImage = BackgroundImage(rc);

        int cnt_F = 0, cnt_B = 0;

        for (int x = 0; x < cropped_R_Trimap.cols; ++x)
        {
            for (int y = 0; y < cropped_R_Trimap.rows; ++y)
            {
                if (cropped_R_Trimap.at<uchar>(y, x) == 255)
                {
                    cnt_F = cnt_F + 1;
                }
                else if (cropped_R_Trimap.at<uchar>(y, x) == 0)
                {
                    cnt_B = cnt_B + 1;
                }
            }
        }

        cout << "cnt pixel (" << cnt_F << ", " << cnt_B << ")" << endl;

        int tmp = 0;
        if (cnt_F * thres_cnt > cnt_B)
        {
            cropped_R_Trimap = 255;
        }
        else if (cnt_B * thres_cnt > cnt_F)
        {
            cropped_R_Trimap = tmp;
        }

        if (fast_mode)
        {
            resize(cropped_RGB_Image, cropped_RGB_Image,
                   Size(cropped_RGB_Image.cols / 2, cropped_RGB_Image.rows / 2));
            resize(cropped_R_Trimap, cropped_R_Trimap,
                   Size(cropped_R_Trimap.cols / 2, cropped_R_Trimap.rows / 2), INTER_NEAREST);
        }

        globalMatting(cropped_RGB_Image, cropped_R_Trimap, foreground, alpha_temp);

        // filter the result with guided filter
        alpha = guidedFilter(cropped_RGB_Image, alpha_temp, 10, 1e-5);

        if (fast_mode)
        {
            resize(alpha, alpha, Size(width, height));
            resize(cropped_R_Trimap, cropped_R_Trimap, Size(width, height), INTER_NEAREST);
        }

        for (int x = 0; x < cropped_R_Trimap.cols; ++x)
        {
            for (int y = 0; y < cropped_R_Trimap.rows; ++y)
            {
                if (cropped_R_Trimap.at<uchar>(y, x) == 0)
                {
                    alpha.at<uchar>(y, x) = 0;
                }
                else if (cropped_R_Trimap.at<uchar>(y, x) == 255)
                {
                    alpha.at<uchar>(y, x) = 255;
                }
            }
        }
        ////imshow("alpha_filtered", alpha);

        alpha.convertTo(alpha, CV_8U);
        alpha_threshold = threshold(alpha, alpha_binary, 0.5, 1, CV_THRESH_BINARY | CV_THRESH_OTSU);

        for (int x = 0; x < alpha_binary.cols; ++x)
        {
            for (int y = 0; y < alpha_binary.rows; ++y)
            {
                if (alpha.at<uchar>(y, x) < alpha_threshold - 255 * 0.2)
                {
                    alpha.at<uchar>(y, x) = 0;
                }
                else if (alpha.at<uchar>(y, x) > alpha_threshold + 255 * 0.2)
                {
                    alpha.at<uchar>(y, x) = 255;
                }
            }
        }

//		alpha.convertTo(alpha,CV_32F);
        alpha_binary = alpha_binary * 255;
        ////imshow("alpha_binary", alpha_binary);
        ////imshow("alpha_ggg", alpha);

        alpha.convertTo(alpha, CV_32F);
        alpha = alpha / 255;

//		Mat cropped_AlphaMap = AlphaMap(rc);
//		addWeighted(cropped_AlphaMap,0.5,alpha,0.5,0,alpha);

        cropped_RGB_Image = RGB_Image(rc);

        cropped_RGB_Image.convertTo(cropped_RGB_Image, CV_32F);
        alpha.convertTo(alpha, CV_32F);
        cropped_BackgroundImage.convertTo(cropped_BackgroundImage, CV_32F);

        vector<Mat> ch_cropped_BackgroundImage;
        split(cropped_BackgroundImage, ch_cropped_BackgroundImage);
        vector<Mat> ch_cropped_RGB_Image;
        split(cropped_RGB_Image, ch_cropped_RGB_Image);

        vector<Mat> ch_cropped_Result_Image(3);

        ch_cropped_Result_Image[0] =
                ch_cropped_RGB_Image[0].mul(alpha) + ch_cropped_BackgroundImage[0].mul(1 - alpha);
        ch_cropped_Result_Image[1] =
                ch_cropped_RGB_Image[1].mul(alpha) + ch_cropped_BackgroundImage[1].mul(1 - alpha);
        ch_cropped_Result_Image[2] =
                ch_cropped_RGB_Image[2].mul(alpha) + ch_cropped_BackgroundImage[2].mul(1 - alpha);

        Mat cropped_Result_Image;
        merge(ch_cropped_Result_Image, cropped_Result_Image);

        cropped_Result_Image.convertTo(cropped_Result_Image, CV_8UC3);

        cropped_Result_Image.copyTo(Result_Image(rc));

//		threshold(alpha, alpha, thres_foreground_alpha, 1, CV_THRESH_BINARY);
        alpha_binary = alpha_binary * 255;
        alpha_binary.convertTo(alpha_binary, CV_8UC1);
        alpha_binary.copyTo(R_Trimap(rc));

//		imwrite( "local_alpha.jpg", alpha );

        ////imshow("Result_Image", Result_Image);

        ////imwrite( "Result_Image_AftRetouching.jpg", Result_Image );

        ////imwrite( filePath  +"_Result_Image_AftRetouching.jpg", Result_Image );
        imwrite(filePath + "_Result_Image.jpg", Result_Image);

//		namedWindow( "R_Trimap", CV_WINDOW_AUTOSIZE );
//		imshow("R_Trimap", R_Trimap);

        lu_x = 10000, lu_y = 10000, rd_x = 0, rd_y = 0;

//		imwrite("cropped_Result_Image.png", cropped_Result_Image);
    }
}


/*
void RetouchFunc(int x, int y, int flags)
{
	__android_log_print(ANDROID_LOG_DEBUG, "!!!", "RetouchFunc() START");
	int circle_radius = 25;
	Scalar color(128);
	Scalar color2(0,0,255);
	int thickness=3;
	float thres_foreground_alpha = 0.5;
	int thres_expansion_known=10;
	float thres_cnt=0.05;

	Mat foreground, alpha;

	if(flags&CV_EVENT_FLAG_LBUTTON)
	{
		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "CV_EVENT_FLAG_LBUTTON");
		Point center(x, y);

		if (x<lu_x){
			lu_x=x;
		}
		if (y<lu_y){
			lu_y=y;
		}
		if (x>rd_x){
			rd_x=x;
		}
		if (y>rd_y){
			rd_y=y;
		}

		circle(Result_Image, center, circle_radius, color, CV_FILLED);
		circle(R_Trimap, center, circle_radius, color, CV_FILLED);

		////imshow("Result_Image", Result_Image);

//		cout << "Mouse position (" << x << ", " << y << ")" << endl;
	}
	else if(flags&CV_EVENT_FLAG_RBUTTON)
	{
		__android_log_print(ANDROID_LOG_DEBUG, "!!!", "CV_EVENT_FLAG_RBUTTON");
		lu_x = lu_x-circle_radius;
		lu_y = lu_y-circle_radius;
		rd_x = rd_x+circle_radius;
		rd_y = rd_y+circle_radius;

		if(lu_x<0){
			lu_x=0;
		}
		if(lu_y<0){
			lu_y=0;
		}
		if(rd_x>RGB_Image.cols-1){
			rd_x = RGB_Image.cols;
		}
		if(rd_y>RGB_Image.rows-1){
			rd_y = RGB_Image.rows;
		}

		int width_temp,height_temp;
		int width,height;

		width_temp = rd_x - lu_x;
		height_temp = rd_y - lu_y;

		if (width_temp>height_temp){
			lu_y = lu_y-thres_expansion_known;
			rd_y = rd_y+thres_expansion_known;
		}
		else{
			lu_x = lu_x-thres_expansion_known;
			rd_x = rd_x+thres_expansion_known;
		}

		if(lu_x<0){
			lu_x=0;
		}
		if(lu_y<0){
			lu_y=0;
		}
		if(rd_x>RGB_Image.cols-1){
			rd_x = RGB_Image.cols;
		}
		if(rd_y>RGB_Image.rows-1){
			rd_y = RGB_Image.rows;
		}

		width = rd_x - lu_x;
		height = rd_y - lu_y;

		Rect rc(lu_x,lu_y,width,height);

		rectangle(Result_Image, rc, color2, thickness);

		////imshow("Result_Image", Result_Image);

		////cout << "window (" << lu_x << ", " << lu_y << ", " << rd_x << ", " << rd_y << ", " << width_temp << ", " << height_temp << ")" << endl;
//		expansionOfKnownRegions(RGB_Image, R_Trimap, 9);

		Mat cropped_RGB_Image = RGB_Image(rc);
		Mat cropped_R_Trimap = R_Trimap(rc);
		Mat cropped_BackgroundImage = BackgroundImage(rc);

		int cnt_F=0,cnt_B=0;

		for (int x = 0; x < cropped_R_Trimap.cols; ++x){
			for (int y = 0; y < cropped_R_Trimap.rows; ++y){
				if (cropped_R_Trimap.at<uchar>(y, x) == 255)
					cnt_F=cnt_F+1;
				else if (cropped_R_Trimap.at<uchar>(y, x) == 0)
					cnt_B=cnt_B+1;
			}
		}

		cout << "cnt pixel (" << cnt_F << ", " << cnt_B << ")" << endl;

		int tmp = 0;
		if (cnt_F*thres_cnt > cnt_B){
			cropped_R_Trimap = 255;
		}
		else if (cnt_B*thres_cnt > cnt_F){
			cropped_R_Trimap = tmp;
		}

		globalMatting(cropped_RGB_Image, cropped_R_Trimap, foreground, alpha);

		// filter the result with guided filter
		alpha = guidedFilter(cropped_RGB_Image, alpha, 10, 1e-5);
		for (int x = 0; x < cropped_R_Trimap.cols; ++x)
		{
			for (int y = 0; y < cropped_R_Trimap.rows; ++y)
			{
				if (cropped_R_Trimap.at<uchar>(y, x) == 0)
					alpha.at<uchar>(y, x) = 0;
				else if (cropped_R_Trimap.at<uchar>(y, x) == 255)
					alpha.at<uchar>(y, x) = 255;
			}
		}

		cropped_RGB_Image.convertTo(cropped_RGB_Image,CV_32F);
		alpha.convertTo(alpha,CV_32F);
		cropped_BackgroundImage.convertTo(cropped_BackgroundImage,CV_32F);

		vector<Mat> ch_cropped_BackgroundImage;
		split(cropped_BackgroundImage, ch_cropped_BackgroundImage);
		vector<Mat> ch_cropped_RGB_Image;
		split(cropped_RGB_Image, ch_cropped_RGB_Image);

		vector<Mat> ch_cropped_Result_Image(3);

//		Mat cropped_AlphaMap = AlphaMap(rc);

		alpha=alpha/255;
//		addWeighted(cropped_AlphaMap,0.5,alpha,0.5,0,alpha);

		ch_cropped_Result_Image[0] = ch_cropped_RGB_Image[0].mul(alpha) + ch_cropped_BackgroundImage[0].mul(1-alpha);
		ch_cropped_Result_Image[1] = ch_cropped_RGB_Image[1].mul(alpha) + ch_cropped_BackgroundImage[1].mul(1-alpha);
		ch_cropped_Result_Image[2] = ch_cropped_RGB_Image[2].mul(alpha) + ch_cropped_BackgroundImage[2].mul(1-alpha);

		Mat cropped_Result_Image;
		merge(ch_cropped_Result_Image,cropped_Result_Image);

		cropped_Result_Image.convertTo(cropped_Result_Image,CV_8UC3);

		cropped_Result_Image.copyTo(Result_Image(rc));

		threshold(alpha, alpha, thres_foreground_alpha, 1, CV_THRESH_BINARY);
		alpha=alpha*255;
		alpha.convertTo(alpha,CV_8UC1);
		alpha.copyTo(R_Trimap(rc));

		////imshow("Result_Image", Result_Image);

		imwrite( filePath  +"_Result_Image_AftRetouching.jpg", Result_Image );

//		namedWindow( "R_Trimap", CV_WINDOW_AUTOSIZE );
//		imshow("R_Trimap", R_Trimap);

		lu_x=10000,lu_y=10000,rd_x=0,rd_y=0;

//		imwrite("cropped_Result_Image.png", cropped_Result_Image);
	}
}
*/


int Pro_mainTest(string fileName, string oFile, bool lfast_mode, bool lAdaptive_thres,
                 bool lAlign_Far2Near)
{
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Pro_mainTest() START");

    time_t start, end;
    float gap;
    start = clock();

    filePath = fileName;

    // mode setting
    bool fast_mode = lfast_mode;//1;

    bool Adaptive_thres = lAdaptive_thres;//1;

    bool Align_Far2Near = lAlign_Far2Near;//1; ////0

    // paramters
    int imresize_factor = 2;

    int size_window = 25;
    float thres_foreground = 0.6;
    float thres_background = 0.4;

    int r;

    if (fast_mode)
    {
        r = 65;
    }
    else
    {
        r = 125;
    }

    double eps = 0.01;
    eps *= 255 * 255;   // intensity range [0, 255]
    /////////////////
    int i, j;

    ////String imageName_NearFocus( "../images/30f.jpg" );
    ////String imageName_FarFocus( "../images/30ff.jpg" );
    ////String imageName_Background( "../images/N3.jpg" );

    String imageName_NearFocus(fileName + "_N.jpg");
    String imageName_FarFocus(fileName + "_F.jpg");
    String imageName_Background("/sdcard/Pictures/PASTE/N3.jpg");

    //	String imageName_NearFocus( "../images/t1_N.jpg" );
    //	String imageName_FarFocus( "../images/t1_F.jpg" );
    //	String imageName_Background( "../images/N3.jpg" );
    //	String imageName_NearFocus( "../images/PASTE_N.jpg" );
    //  String imageName_FarFocus( "../images/PASTE_F.jpg" );
    //	String imageName_Background( "../images/test_background_HTC.jpg" );

    Mat NearFocusImage_Color_input;
    Mat FarFocusImage_Color_input;
    Mat NearFocusImage_Color;
    Mat FarFocusImage_Color;
    Mat NearFocusImage_Gray;
    Mat FarFocusImage_Gray;

    NearFocusImage_Color_input = imread(imageName_NearFocus, IMREAD_COLOR);
    FarFocusImage_Color_input = imread(imageName_FarFocus, IMREAD_COLOR);

    BackgroundImage = imread(imageName_Background, IMREAD_COLOR);

    if (NearFocusImage_Color_input.empty() || FarFocusImage_Color_input.empty() ||
        BackgroundImage.empty())
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    resize(NearFocusImage_Color_input, NearFocusImage_Color,
           Size(NearFocusImage_Color_input.cols / imresize_factor,
                NearFocusImage_Color_input.rows / imresize_factor));
    resize(FarFocusImage_Color_input, FarFocusImage_Color,
           Size(FarFocusImage_Color_input.cols / imresize_factor,
                FarFocusImage_Color_input.rows / imresize_factor));

    /*	if(BackgroundImage.cols == NearFocusImage_Color_input.cols & BackgroundImage.rows == NearFocusImage_Color_input.rows){
			resize(BackgroundImage, BackgroundImage, Size(BackgroundImage.cols/imresize_factor, BackgroundImage.rows/imresize_factor));
		}
		else{
			resize(BackgroundImage, BackgroundImage, Size(NearFocusImage_Color_input.cols/imresize_factor, NearFocusImage_Color_input.rows/imresize_factor));
		}*/

    resize(BackgroundImage, BackgroundImage,
           Size(BackgroundImage.cols / imresize_factor, BackgroundImage.rows / imresize_factor));

    //	namedWindow( "Near Focus Image", CV_WINDOW_AUTOSIZE );
    //	imshow( "Near Focus Image", NearFocusImage_Color_input );

    //	namedWindow( "Far Focus Image", CV_WINDOW_AUTOSIZE );
    //	imshow( "Far Focus Image", FarFocusImage_Color );

    cvtColor(NearFocusImage_Color, NearFocusImage_Gray, CV_RGB2GRAY);
    cvtColor(FarFocusImage_Color, FarFocusImage_Gray, CV_RGB2GRAY);

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Image alignment ////////////////////////////////////
    ////stopWatch();

    vector<cv::KeyPoint> keypoints_N, keypoints_F;
    Mat descriptors_N, descriptors_F;

    ORB orb;

    OrbFeatureDetector detector(500, 1.2f, 4, 25, 0, 2, 0, 25);
    OrbDescriptorExtractor extractor;

    orb.detect(NearFocusImage_Gray, keypoints_N);
    orb.compute(NearFocusImage_Gray, keypoints_N, descriptors_N);

    orb.detect(FarFocusImage_Gray, keypoints_F);
    orb.compute(FarFocusImage_Gray, keypoints_F, descriptors_F);

    BFMatcher matcher;
    vector<DMatch> matches;
    Mat img_matches;

    matcher.match(descriptors_N, descriptors_F, matches);

    double max_dist = 0;
    double min_dist = 100;

    for (i = 0; i < descriptors_N.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist) min_dist = dist;
        if (dist > max_dist) max_dist = dist;
    }

    vector<cv::DMatch> good_matches;

    for (i = 0; i < descriptors_N.rows; i++)
    {
        if (matches[i].distance < (max_dist / 1.6))
        { good_matches.push_back(matches[i]); }
    }

    //	drawMatches(NearFocusImage_Gray, keypoints_N, FarFocusImage_Gray, keypoints_F, \
	//          good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
    //                vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    //	namedWindow( "img_matches", CV_WINDOW_FULLSCREEN );
    //	imshow("img_matches", img_matches );


    std::vector<Point2f> Gp_N;
    std::vector<Point2f> Gp_F;

    for (i = 0; i < good_matches.size(); i++)
    {
        Gp_N.push_back(keypoints_N[good_matches[i].queryIdx].pt);
        Gp_F.push_back(keypoints_F[good_matches[i].trainIdx].pt);
    }

    Mat alined_FarFocusImage_Gray;
    Mat alined_NearFocusImage_Color;

    if (Align_Far2Near)
    {
        Mat transformMat = estimateRigidTransform(Gp_F, Gp_N, false);

        if (transformMat.dims != 0)
        {
            Size dsize = FarFocusImage_Gray.size();
            warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);
        }
        else
        {
            alined_FarFocusImage_Gray = FarFocusImage_Gray;
        }

        Size dsize = NearFocusImage_Gray.size();
        warpAffine(FarFocusImage_Gray, alined_FarFocusImage_Gray, transformMat, dsize);

        ////namedWindow( "FarFocusImage_Gray", CV_WINDOW_NORMAL );
        ////imshow( "FarFocusImage_Gray", alined_FarFocusImage_Gray );

        ////namedWindow( "NearFocusImage_Color", CV_WINDOW_NORMAL );
        ////imshow( "NearFocusImage_Color", NearFocusImage_Color );
    }
    else
    {
        //		Mat transform_H = findHomography( Gp_N, Gp_F, CV_RANSAC );
        //		perspectiveTransform( NearFocusImage_Color, alined_NearFocusImage_Color, transform_H);

        Mat transformMat = estimateRigidTransform(Gp_N, Gp_F, false);

        if (transformMat.dims != 0)
        {
            Size dsize = NearFocusImage_Color.size();
            warpAffine(NearFocusImage_Color, alined_NearFocusImage_Color, transformMat, dsize);
        }
        else
        {
            alined_NearFocusImage_Color = NearFocusImage_Color;
        }

        ////namedWindow( "FarFocusImage_Gray", CV_WINDOW_NORMAL );
        ////imshow( "FarFocusImage_Gray", FarFocusImage_Gray );

        ////namedWindow( "NearFocusImage_Color", CV_WINDOW_NORMAL );
        ////imshow( "NearFocusImage_Color", alined_NearFocusImage_Color );
    }

    ////cout << "Image alignment" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.Image alignment");
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// FMR-based depth extraction ////////////////////////////////////
    ////stopWatch();

    Mat NearFocus_Image_Color;
    Mat NearFocus_Image_Gray;
    Mat FarFocus_Image_Gray;

    if (Align_Far2Near)
    {
        NearFocus_Image_Color = NearFocusImage_Color;
        cvtColor(NearFocus_Image_Color, NearFocus_Image_Gray, CV_RGB2GRAY);
        FarFocus_Image_Gray = alined_FarFocusImage_Gray;
    }
    else
    {
        NearFocus_Image_Color = alined_NearFocusImage_Color;
        cvtColor(NearFocus_Image_Color, NearFocus_Image_Gray, CV_RGB2GRAY);
        FarFocus_Image_Gray = FarFocusImage_Gray;
    }

    Mat grad_x, grad_y;
    Mat grad_x_square, grad_y_square;
    Mat FM_Nimg, FM_Fimg;
    Mat FM_Nfocus, FM_Ffocus;

    Sobel(NearFocus_Image_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(NearFocus_Image_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Nimg = grad_x_square + grad_y_square;
    //	boxFilter( FM_Nimg, FM_Nfocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
    GaussianBlur(FM_Nimg, FM_Nfocus, Size(size_window, size_window), 0, 0);

    Sobel(FarFocusImage_Gray, grad_x, CV_32F, 1, 0, 3);
    multiply(grad_x, grad_x, grad_x_square);

    Sobel(FarFocusImage_Gray, grad_y, CV_32F, 0, 1, 3);
    multiply(grad_y, grad_y, grad_y_square);

    FM_Fimg = (grad_x_square + grad_y_square);
    //	boxFilter( FM_Fimg, FM_Ffocus, CV_32F, Size(size_window,size_window),Point(-1,-1),true);
    GaussianBlur(FM_Fimg, FM_Ffocus, Size(size_window, size_window), 0, 0);

    float *FM_Nfocus_data = (float *) FM_Nfocus.data;
    float *FM_Ffocus_data = (float *) FM_Ffocus.data;
    float *FMRtemp;
    FMRtemp = new float[NearFocus_Image_Color.cols * NearFocus_Image_Color.rows];

    int length = FM_Nfocus.rows * FM_Nfocus.cols;

    for (i = 0; i < length; i++)
    {
        //		FM_Ffocus_data[i]=FM_Ffocus_data[i]+20;
        if (FM_Ffocus_data[i] >= FM_Nfocus_data[i])
        {
            FMRtemp[i] = 2 - FM_Nfocus_data[i] / (FM_Ffocus_data[i] + 0.000001);
        }
        else
        {
            FMRtemp[i] = FM_Ffocus_data[i] / (FM_Nfocus_data[i] + 0.000001);
        }
    }

    ////cout << "FMR-based depth extraction" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.FMR-based depth extraction");
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Guided filtering ////////////////////////////////////
    ////stopWatch();

    Mat FMR(NearFocus_Image_Color.rows, NearFocus_Image_Color.cols, CV_32F, FMRtemp);

    if (fast_mode)
    {
        resize(NearFocus_Image_Color, NearFocus_Image_Color,
               Size(NearFocus_Image_Color.cols / imresize_factor,
                    NearFocus_Image_Color.rows / imresize_factor));
        resize(FMR, FMR, Size(FMR.cols / imresize_factor, FMR.rows / imresize_factor));
    }

    Mat FMR_filtered = guidedFilter(NearFocus_Image_Color, FMR, r, eps);

    Mat depthMap = (2 - FMR_filtered) / 2;

    ////cout << "Guided filtering" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.Guided filtering");
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// foreground extraction ////////////////////////////////////
    ////stopWatch();

    //////////////// alpha matting
    /*	Mat Trimap(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Mat Trimap_foreground(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Mat Trimap_background(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);
		Trimap = Scalar(128);
		Trimap_foreground = Scalar(0);
		Trimap_background = Scalar(0);

		threshold(depthMap, Trimap_foreground, thres_foreground, 127, CV_THRESH_BINARY);
		threshold(depthMap, Trimap_background, thres_background, 128, CV_THRESH_BINARY_INV);

		Trimap_foreground.convertTo(Trimap_foreground,CV_8UC1);
		Trimap_background.convertTo(Trimap_background,CV_8UC1);
		add(Trimap,Trimap_foreground,Trimap);
		subtract(Trimap,Trimap_background,Trimap);

		namedWindow( "Trimap", CV_WINDOW_KEEPRATIO );
		imshow("Trimap", Trimap);
		Mat foreground, alpha;
		globalMatting(NearFocus_Image_Color, Trimap, foreground, alpha);

		double minVal;
		double maxVal;
		minMaxIdx(AlphaMap, &minVal, &maxVal);
		Mat AlphaMap_display;
		AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

		namedWindow( "foreground", CV_WINDOW_KEEPRATIO );
		imshow("foreground", foreground);
	*/
    if (Adaptive_thres)
    {
        Mat Binary_AlphaMap;//(NearFocus_Image_Color.rows,NearFocus_Image_Color.cols,CV_8UC1);

        depthMap = depthMap * 255;
        depthMap.convertTo(depthMap, CV_8U);

        float init_foreground_threshhold = threshold(depthMap, Binary_AlphaMap, thres_foreground, 1,
                                                     CV_THRESH_BINARY | CV_THRESH_OTSU);

        AlphaMap = Binary_AlphaMap;

        double minVal;
        double maxVal;

        minMaxIdx(AlphaMap, &minVal, &maxVal);
        Mat AlphaMap_display;
        AlphaMap.convertTo(AlphaMap_display, CV_8UC3, 255.0 / (maxVal - minVal),
                           -minVal * 255.0 / (maxVal - minVal));

        ////namedWindow( "AlphaMap", CV_WINDOW_NORMAL );

        int H = NearFocus_Image_Color.rows;
        int W = NearFocus_Image_Color.cols;

        int height_temp = cvRound(H / 4);
        int width_temp = cvRound(W / 8);

        int start_x, end_x, start_y, end_y;
        int local_width, local_height;

        float cnt_F, cnt_B;
        float balance_ratio;
        float Local_threshhold;

        //	Mat foreground, alpha;

        for (int j = 0; j < H; j += cvRound(height_temp / 2))
        {
            for (int i = 0; i < W; i += cvRound(width_temp / 2))
            {
                start_x = i;
                end_x = i + width_temp;
                start_y = j;
                end_y = j + height_temp;

                if (end_x > W)
                {
                    end_x = W - 1;
                }
                if (end_y > H)
                {
                    end_y = H - 1;
                }

                local_width = end_x - start_x;
                local_height = end_y - start_y;

                Rect rc(start_x, start_y, local_width, local_height);

                //			rectangle(AlphaMap_display, rc,(0,0,255), 3);

                Mat Local_binary_alpha = Binary_AlphaMap(rc);

                cnt_F = 0, cnt_B = 0;

                for (int x = 0; x < Local_binary_alpha.cols; ++x)
                {
                    for (int y = 0; y < Local_binary_alpha.rows; ++y)
                    {
                        if (Local_binary_alpha.at<uchar>(y, x) == 1)
                        {
                            cnt_F = cnt_F + 1;
                        }
                        else if (Local_binary_alpha.at<uchar>(y, x) == 0)
                        {
                            cnt_B = cnt_B + 1;
                        }
                    }
                }

                balance_ratio = min(cnt_F, cnt_B) / max(cnt_F, cnt_B);

                //			cout << "out (" << cnt_F << ", " << cnt_B << ", " << balance_ratio << ")" << endl;

                if (balance_ratio > 0.3)
                {
                    //				rectangle(AlphaMap_display, rc,(0,0,255), 3);
                    Mat Local_depthMap = depthMap(rc);

                    Mat Local_Binary_AlphaMap;
                    Local_threshhold = threshold(Local_depthMap, Local_Binary_AlphaMap,
                                                 thres_foreground, 1,
                                                 CV_THRESH_BINARY | CV_THRESH_OTSU);

                    Mat cropped_RGB_Image = NearFocus_Image_Color(rc);
                    //// slow
                    //				globalMatting(cropped_RGB_Image, cropped_R_Trimap, foreground, alpha);
                    ////
                    Local_Binary_AlphaMap.copyTo(AlphaMap(rc));


                }

            }
        }

        ////imshow("AlphaMap", AlphaMap_display);
    }
    else
    {
        AlphaMap = depthMap;

        threshold(depthMap, AlphaMap, thres_foreground, 1, CV_THRESH_BINARY);
        //	threshold(depthMap, AlphaMap, thres_background, 1, CV_THRESH_TOZERO);
    }

    ////cout << "foreground extraction" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.foreground extraction");
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Compositing ////////////////////////////////////
    ////stopWatch();

    //	if(fast_mode){
    //		resize(BackgroundImage, BackgroundImage, Size(BackgroundImage.cols/imresize_factor, BackgroundImage.rows/imresize_factor));
    //	}
    if (fast_mode)
    {
        resize(NearFocusImage_Color_input, NearFocus_Image_Color,
               Size(NearFocusImage_Color_input.cols / imresize_factor,
                    NearFocusImage_Color_input.rows / imresize_factor));
        resize(AlphaMap, AlphaMap, Size(NearFocusImage_Color.cols, NearFocusImage_Color.rows));
    }

    NearFocus_Image_Color.convertTo(NearFocus_Image_Color, CV_32F);
    AlphaMap.convertTo(AlphaMap, CV_32F);
    BackgroundImage.convertTo(BackgroundImage, CV_32F);

    vector<Mat> ch_BackgroundImage;
    split(BackgroundImage, ch_BackgroundImage);
    vector<Mat> ch_NearFocus_Image_Color;
    split(NearFocus_Image_Color, ch_NearFocus_Image_Color);

    vector<Mat> ch_Result_Image(3);

    ch_Result_Image[0] =
            ch_NearFocus_Image_Color[0].mul(AlphaMap) + ch_BackgroundImage[0].mul(1 - AlphaMap);
    ch_Result_Image[1] =
            ch_NearFocus_Image_Color[1].mul(AlphaMap) + ch_BackgroundImage[1].mul(1 - AlphaMap);
    ch_Result_Image[2] =
            ch_NearFocus_Image_Color[2].mul(AlphaMap) + ch_BackgroundImage[2].mul(1 - AlphaMap);

    merge(ch_Result_Image, Result_Image);

    ////cout << "Compositing" << endl;
    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Native.Compositing");
    ////stopWatch();
    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// display ////////////////////////////////////
    /*	double minVal;
		double maxVal;

		minMaxIdx(AlphaMap, &minVal, &maxVal);
		Mat AlphaMap_display;
		AlphaMap.convertTo(AlphaMap_display,CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));

		namedWindow( "AlphaMap", CV_WINDOW_NORMAL );
		imshow("AlphaMap", AlphaMap_display);
	*/
    Result_Image.convertTo(Result_Image, CV_8U);
    ////namedWindow( "Result_Image", CV_WINDOW_KEEPRATIO );
    ////imshow("Result_Image", Result_Image);

    ////imwrite( "Result_Image.jpg", Result_Image );
    ////imwrite( fileName + "_Result_Image.jpg", Result_Image );
    end = clock();
    char buff[256];            // 변환될 int형 변수를 저장할 공간
    gap = (float) (end - start) / (CLOCKS_PER_SEC);                    // 변환 시킬 int형 변수
    sprintf(buff, "%5.2fS", gap);

    imwrite(oFile + buff + ".jpg", Result_Image);

    //////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Retouching ////////////////////////////////////
    /*
		RGB_Image = NearFocus_Image_Color;
		RGB_Image.convertTo(RGB_Image,CV_8UC3);

		R_Trimap = AlphaMap*255;
		R_Trimap.convertTo(R_Trimap,CV_8UC1);
*/
    ////setMouseCallback("Result_Image", CallBackFunc, NULL);

    ////waitKey(0);

    ////return 0;

    __android_log_print(ANDROID_LOG_DEBUG, "!!!", "Pro_mainTest() END : ");
}

void PaintingBackground(Mat InputImg, int Background_color)
{
    InputImg.convertTo(InputImg, CV_32F);
    AlphaMap.convertTo(AlphaMap, CV_32F);

    vector<Mat> ch_InputImg;
    split(InputImg, ch_InputImg);

    vector<Mat> ch_ForegroundImage_diplay(3);

    if (Background_color == 1)
    {          //// Blue
        ch_ForegroundImage_diplay[0] =
                ch_InputImg[0].mul(AlphaMap) + ch_InputImg[1].mul(1 - AlphaMap);
        ch_ForegroundImage_diplay[1] = ch_InputImg[1].mul(AlphaMap);
        ch_ForegroundImage_diplay[2] = ch_InputImg[2].mul(AlphaMap);
    }
    else if (Background_color == 2)
    {   //// green
        ch_ForegroundImage_diplay[0] = ch_InputImg[0].mul(AlphaMap);
        ch_ForegroundImage_diplay[1] =
                ch_InputImg[1].mul(AlphaMap) + ch_InputImg[1].mul(1 - AlphaMap);
        ch_ForegroundImage_diplay[2] = ch_InputImg[2].mul(AlphaMap);
    }
    else
    {                           /// red
        ch_ForegroundImage_diplay[0] = ch_InputImg[0].mul(AlphaMap);
        ch_ForegroundImage_diplay[1] = ch_InputImg[1].mul(AlphaMap);
        ch_ForegroundImage_diplay[2] =
                ch_InputImg[2].mul(AlphaMap) + ch_InputImg[1].mul(1 - AlphaMap);
    }

    merge(ch_ForegroundImage_diplay, ForegroundImage_diplay);
    ForegroundImage_diplay.convertTo(ForegroundImage_diplay, CV_8UC3);
//	imwrite(imageName_Saveimg, ForegroundImage_diplay);//GN_170329

//	namedWindow( "ForegroundImage_diplay", CV_WINDOW_KEEPRATIO );
//	imshow("ForegroundImage_diplay", ForegroundImage_diplay);
}

void SaveResult(Mat Input_Image, int margin_crop_boundary)
{
    int H = AlphaMap.rows;
    int W = AlphaMap.cols;

    int init_x = margin_crop_boundary;
    int init_y = margin_crop_boundary;
    int crop_width = W - 2 * margin_crop_boundary;
    int crop_height = H - 2 * margin_crop_boundary;

    Rect rc(init_x, init_y, crop_width, crop_height);

    Mat Cropped_Image = Input_Image(rc);
    Mat Cropped_AlphaMap = AlphaMap(rc);

    Cropped_Image.convertTo(Cropped_Image, CV_8UC3);
    Cropped_AlphaMap.convertTo(Cropped_AlphaMap, CV_8U);

    Cropped_AlphaMap = Cropped_AlphaMap * 255;

    ////namedWindow( "Cropped_Image", CV_WINDOW_KEEPRATIO );
    ////imshow("Cropped_Image", Cropped_Image);
    ////imwrite( filePath + "_Cropped_Image.jpg", Cropped_Image);//GN_170428
    imageRotationSave("_Cropped_Image.jpg", Cropped_Image); //GN_170428


    ////namedWindow( "Cropped_AlphaMap", CV_WINDOW_KEEPRATIO );
    ////imshow("Cropped_AlphaMap", Cropped_AlphaMap);
    ////imwrite( filePath + "_Cropped_AlphaMap.jpg", Cropped_AlphaMap);//GN_170428
    imageRotationSave("_Cropped_AlphaMap.jpg", Cropped_AlphaMap); //GN_170428
}


