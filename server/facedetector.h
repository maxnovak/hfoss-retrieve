/*#if !defined FACEDETECTOR
#define FACEDETECTOR
*/
#include "cv.h"
#include "highgui.h"

class facedetector{


public:
/*
	bool itIsInitialized(IplImage *image){
		return output &&(output->width == image->width) && (output->height == image->height);
	}

	void initializeMe(IplImage *image){
		CvHaarClassifierCascade *pCascade = 0;
		CvMemStorage *pStorage = 0;
		CvSeq *pFaceRectSeq;
		int i;
		IplImage *output;
	
		pStorage = cvCreateMemStorage(0);
		pCascade = (CvHaarClassifierCascade *)cvLoad
		((OPENCV_ROOT"/data/haarcascades/haarcascade_frontalface_default.xml"),
			0, 0, 0 );



		cvReleaseImage(&output);
		output= cvCreateImage(cvSize(image->width,image->height),
									image->depth, image->nChannels);
	  }*/

	IplImage process(IplImage *image){
		pFaceRectSeq = cvHaarDetectObjects(image,pCascade, pStorage, 1.1, 3, CV_HAAR_DO_CANNY_PRUNING, cvSize(0,0));
		for (i=0; i<(pFaceRectSeq? pFaceRectSeq->total:0);i++)
		{
			CvRect *r=(CvRect*)cvGetSeqElem(pFaceRectSeq,i);
			CvPoint pt1 = {r->x, r->y};
			CvPoint pt2 = {r->x + r-width, r->y + r->height};
			cvRectangle(image, pt1, pt2, CV_RGB(0,225,0),3,4,0);
			return image;
	}

	/*inline IplImage processImage(IplImage *image){
		if (!itIsInitialized(image)){
			initializeMe(image);
		}
		image = process(image);
		return image;
	}*/
/*
	~facedetector(){
		release();
	}*/
};