#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

typedef unsigned char byte;

byte color_lower[3] = {80, 100, 190}; //{0, 20, 190};
byte color_upper[3] = {180, 200, 255}; //{100, 140, 255};

Rect detector(const Mat& frame, Mat& out, const byte* color_l, const byte* color_u){
	Rect res = Rect(0, 0, 0, 0);
	
	Mat mask;
	
	medianBlur(frame, out, 11);
	inRange(
		out, 
		Scalar(color_l[0], color_l[1], color_l[2]),
		Scalar(color_u[0], color_u[1], color_u[2]),
		mask
	);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
    findContours( mask, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
	if( contours.size() == 0 )
        return res;

	// iterate through all the top-level contours,
    // draw each connected component with its own random color
    int idx = 0, largestComp = 0;
    double maxArea = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
        const vector<Point>& c = contours[idx];
        double area = fabs(contourArea(Mat(c)));
        if( area > maxArea )
        {
            maxArea = area;
            largestComp = idx;
        }
    }
    res = boundingRect(contours[largestComp]);
	
	return res;
}

int main(int argc, char* argv[])
{
	int cameraId = 2;

	VideoCapture capture;
	capture.open(cameraId);

    if(!capture.isOpened())
        return fprintf( stderr, "Could not initialize video (%d) capture\n", cameraId), -2;

	if (!capture.set(CAP_PROP_FRAME_WIDTH, 1024))
	{
		std::cerr << "ERROR: seekeing is not supported" << endl;
	}
//CAP_PROP_FRAME_HEIGHT

	Mat frame, image;
	printf("[i] press Enter for capture image and Esc for quit!\n\n");
	namedWindow( "Image View", 1 );

	while(true){
		if(capture.isOpened() )
		{
			Mat view0;
			capture >> view0;
			view0.copyTo(frame);

			Rect pos = detector(frame, image, color_lower, color_upper);
			if(pos.width*pos.height > 0) {
				printf("%i,%i\n", 
					static_cast<int>(pos.x + pos.width/2),
					static_cast<int>(pos.y + pos.height/2)
				);
				rectangle(frame, pos, Scalar(0,255, 0));
			}

			imshow("Image View", frame);

			char key = (char)waitKey(capture.isOpened() ? 50 : 500);
			if( key == 27 )
				break;
		}
	}
	
	
	return 0;
}
