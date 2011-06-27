#if !defined FACEDETECTOR
#define FACEDETECTOR

#include "cv.h"
#include "highgui.h"

class facedetector{

private:
	IplImage *output;

public:

	facedetector() : output(0){
	
	}

	static void detect_and_draw(IplImage* img ){
		//create the output file
		IplImage* procImage;
		procImage = cvCreateImage(cvSize(800,600),
			img->depth,3);
		//cvResize(img,procImage,CV_INTER_LINEAR);

		CvMemStorage* storage = cvCreateMemStorage(0);
		// Note that you must copy C:\Program Files\OpenCV\data\haarcascades\haarcascade_frontalface_alt2.xml
		// to your working directory
		CvHaarClassifierCascade* cascade = (CvHaarClassifierCascade*)cvLoad( "haarcascade_frontalface_alt2.xml" );
		double scale = 1.3;

		// Detect objects
		cvClearMemStorage( storage );
		CvSeq* objects = cvHaarDetectObjects( img, cascade, storage, 1.1, 4, CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0));

		CvRect* r;
		// Loop through objects and draw boxes
		for( int i = 0; i < (objects ? objects->total : 0 ); i++ )
		{
			r = ( CvRect* )cvGetSeqElem( objects, i );
			CvPoint pt1 = {r->x, r->y};
			CvPoint pt2 = {r->x + r->width, r->y + r->height};
			cvRectangle(img, pt1, pt2,CV_RGB(0,255,0),5,4,0);
			//cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ),
			//	colors[i%8]);
		}
		std::cout<<"faces found"<<std::endl;
		cvNamedWindow( "Output", CV_WINDOW_AUTOSIZE);
		std::cout<<"resizing"<<std::endl;
		//procImage=cvCloneImage(img);
		cvResize(img,procImage,CV_INTER_NN);
		//cvSaveImage("ProcImage",procImage);
		cvShowImage( "Output", procImage);
		cvWaitKey();

		cvReleaseImage( &img );
	}

};
#endif