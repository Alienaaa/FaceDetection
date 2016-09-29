// exp0515_2.cpp : �������̨Ӧ�ó������ڵ㡣
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

	CascadeClassifier  cascade ;		//������
	CascadeClassifier cascadeMouth;

	const char* cascade_name = "haarcascade_frontalface_alt2.xml";	
	const char* cascade_mouth_name = "haarcascade_mcs_mouth.xml";


	if( !cascade.load(cascade_name) )		//���ط���������
	{
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		fprintf( stderr,
			"Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
		return -1;
	}

	if( !cascadeMouth.load(cascade_mouth_name) )		//���ط���������
	{
		fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
		fprintf( stderr,
			"Usage: facedetect --cascade=\"<cascade_path>\" [filename|camera_index]\n" );
		return -1;
	}

	cvNamedWindow( "result", 1 );  //��������

	IplImage *pImg = cvLoadImage("test.jpg");
	IplImage* gray = cvCreateImage( cvSize(pImg->width,pImg->height), 8, 1 );

	//��Ӵ��룺ͼ��Ԥ����
	cvCvtColor( pImg, gray, CV_BGR2GRAY ); 
    cvEqualizeHist(gray, gray);

	std::vector<Rect> faces;
	std::vector<Rect> mouth;

	//��Ӵ��룺���ͼ���е�Ŀ��!
	cascade.detectMultiScale(gray,faces,1.1,2,0|CV_HAAR_SCALE_IMAGE,Size(80,80));


	for( int i = 0; i < faces.size(); i++ )
	{
		Rect r = faces[i];		//��faces���ݴ�CvSeqתΪCvRect ?
		CvPoint center;
		int radius;

		//��Ӵ��룺������
 		center.x =faces[i].x+ faces[i].width*0.5;
 		center.y = faces[i].y+ faces[i].height*0.5;
 		radius =0.3* (faces[i].width+faces[i].height);
		cvCircle(pImg,center,radius,CV_RGB(255,0,0),1,8,0);


		//��Ӵ��룺������
		Rect  mouthR;			
 		mouthR.x =faces[i].x+0.2*faces[i].width;
		mouthR.y = faces[i].y +0.6*faces[i].height;
 		mouthR.width = faces[i].width *0.8;
		mouthR.height = faces[i].height*0.3;
		cvSetImageROI(gray,mouthR);

		cvNamedWindow("test");
		cvShowImage("test",gray);
		
		//��Ӵ��룺���ͼ���е�Ŀ��
		cascadeMouth.detectMultiScale(gray,mouth,1.1,3,0,cvSize(10,10));
	
		for( i = 0; i < mouth.size(); i++ )
		{
			//��Ӵ��룺
			//cvRound ���غͲ�����ӽ�������ֵ
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

	cvReleaseImage( &pImg );			//�ͷ�֡�����ڴ�ռ�
	cvReleaseImage( &gray );
	cvDestroyWindow("result");		//���ٴ���

	return 0;
}
