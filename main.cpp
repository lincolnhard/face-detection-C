/*
 * main.cpp
 *
 *  Created on: 2017年4月30日
 *      Author: lincolnhard
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include "fd.h"

int main
	(
	void
	)
{
	Mat im;
	Mat gray;
	VideoCapture cap;
	cap.open(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

	init_face_detection();
	while(1)
		{
		cap >> im;
		cvtColor(im, gray, CV_BGR2GRAY);
		vector_lincoln* fdresult = face_detect(gray.data);
		if(fdresult->num_elems > 0)
			{
			rect_u16_lincoln faceroi = ((rect_u16_lincoln*)fdresult->beginning)[0];
			rectangle(im, Rect(faceroi.x, faceroi.y, faceroi.w, faceroi.h), Scalar(0, 255, 0));
			}
		imshow("demo", im);
		if(waitKey(10) == 27)
			{
			break;
			}
		}
	free_face_detection();

	return EXIT_SUCCESS;
}


