// exp0515_2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h" 
#include <vector>
using namespace std;
using namespace cv;

int main( int argc, char** argv )
{
	CvCapture* capture = 0;

	CascadeClassifier  cascade ;		//分类器
	CascadeClassifier cascadeMouth;

	const char* cascade_name = "haarcascade_frontalface_alt2.xml";	
	const char* cascade_mouth_name = "haarcascade_mcs_mouth.xml";


	if( !cascade.load(cascade_name) )		//加载分类器出错
	{
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		fprintf( stderr,
			"Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
		return -1;
	}

	if( !cascadeMouth.load(cascade_mouth_name) )		//加载分类器出错
	{
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		fprintf( stderr,
			"Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
		return -1;
	}

	cvNamedWindow( "result", 1 );  //创建窗口

	IplImage *pImg = cvLoadImage("test.jpg");
	IplImage* gray = cvCreateImage( cvSize(pImg->width,pImg->height), 8, 1 );

	//添加代码：图像预处理
	cvCvtColor( pImg, gray, CV_BGR2GRAY ); 
    cvEqualizeHist(gray, gray);

	std::vector<Rect> faces;
	std::vector<Rect> mouth;

	//添加代码：检测图像中的目标!
	cascade.detectMultiScale(gray,faces,1.1,2,0|CV_HAAR_SCALE_IMAGE,Size(80,80));


	for( int i = 0; i < faces.size(); i++ )
	{
		Rect r = faces[i];		//将faces数据从CvSeq转为CvRect ?
		CvPoint center;
		int radius;

		//添加代码：人脸框
 		center.x =faces[i].x+ faces[i].width*0.5;
 		center.y = faces[i].y+ faces[i].height*0.5;
 		radius =0.3* (faces[i].width+faces[i].height);
		cvCircle(pImg,center,radius,CV_RGB(255,0,0),1,8,0);


		//添加代码：检测嘴巴
		Rect  mouthR;			
 		mouthR.x =faces[i].x+0.2*faces[i].width;
		mouthR.y = faces[i].y +0.6*faces[i].height;
 		mouthR.width = faces[i].width *0.8;
		mouthR.height = faces[i].height*0.3;
		cvSetImageROI(gray,mouthR);

		cvNamedWindow("test");
		cvShowImage("test",gray);
		
		//添加代码：检测图像中的目标
		cascadeMouth.detectMultiScale(gray,mouth,1.1,3,0,cvSize(10,10));
	
		for( i = 0; i < mouth.size(); i++ )
		{
			//添加代码：
			//cvRound 返回和参数最接近的整数值
 			r = mouth[i];		
			center.x = cvRound(mouthR.x+ r.x+r.width*0.5);
			center.y = cvRound(mouthR.y+r.y+ r.height*0.5);
			radius = cvRound(0.3* (r.width+r.height));
 			cvCircle(pImg,center,radius,CV_RGB(0,0,255),1,8,0);

		}
		cvResetImageROI(gray);
	}

	cvShowImage( "result", pImg );
	cvWaitKey(0);

	cvReleaseImage( &pImg );			//释放帧副本内存空间
	cvReleaseImage( &gray );
	cvDestroyWindow("result");		//销毁窗口

	return 0;
}
