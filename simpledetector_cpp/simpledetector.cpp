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

#include "SocketServer.h"
#include "arg_parser.h"


using namespace cv;
using namespace std;

typedef unsigned char byte;

byte color_lower[3] = {80, 100, 190}; //{0, 20, 190};
byte color_upper[3] = {180, 200, 255}; //{100, 140, 255};

static int cameraId = 2;
static int framesToCheck = 5;
static const struct Miksarus::ProgramOption options_list[]{
    {Miksarus::optVerbose, NOT_REQUIRED},
    { 
        {
            "camera",          // long name
            'c',                // short name 
            required_argument,  // is option value required
            "camera number (0 - any, default - 2)",
            [](char* argv) { cameraId = atoi(argv); }, 
    		nullptr
        },
		NOT_REQUIRED
	},
	{
		{
            "check-frames",          // long name
            'f',                // short name 
            required_argument,  // is option value required
            "how many frames the object must be detected (default - 5)",
            [](char* argv) { framesToCheck = atoi(argv); }, 
    		nullptr
        },
        NOT_REQUIRED                 // is option required (true or false)
    },
    {Miksarus::optNULL, NOT_REQUIRED}
};


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
    Miksarus::ParseProgramOptions(argc, argv, options_list);
	
	VideoCapture capture;
	capture.open(cameraId);

    if(!capture.isOpened())
        return fprintf( stderr, "Could not initialize video (%d) capture\n", cameraId), -2;

//	if (!capture.set(CAP_PROP_FRAME_WIDTH, 1024))
//	{
//		std::cerr << "ERROR: seekeing is not supported" << endl;
//	}
//CAP_PROP_FRAME_HEIGHT

	Mat frame, image;
	printf("[i] press Enter for capture image and Esc for quit!\n\n");
	namedWindow( "Image View", 1 );

	SocketServer server("iqr.socket");
	if(server.Start() != 0){
		std::cerr << "Can't start socket server!" << endl;
		return -3;
	}

	int framesSinceDetection = 0;
	Rect etalon;
	double etalon_border_coef = 1;
	while(true) {
		if(capture.isOpened())
		{
			Mat view0;
			capture >> view0;
			view0.copyTo(frame);

			Rect pos = detector(frame, image, color_lower, color_upper);
			int area = pos.width*pos.height;
			if(area > 0) {
				double border_coef = pos.width/pos.height;
				int max_size = max(pos.width, pos.height);
				int min_size = min(pos.width, pos.height);
				int ox = static_cast<int>(pos.x + pos.width/2);
				int oy = static_cast<int>(pos.y + pos.height/2);
				
				if (border_coef > 0.7 && border_coef < 1.3 && max_size < 400 && min_size > 50){
					printf("%i, %i (area: %i px^2)\n", ox, oy, area);
					rectangle(frame, pos, Scalar(0,255, 0));
					if( abs(pos.x - etalon.x) < 3 && abs(pos.y - etalon.y) < 3 ){
						framesSinceDetection++;
					} else {
						etalon = pos;
						framesSinceDetection = 1;
					}
				} else {
					framesSinceDetection = 0;
					server.SetDetectorState(false, 0, 0);
				}
				
				if (framesSinceDetection >= framesToCheck){
					framesSinceDetection = framesToCheck;
					server.SetDetectorState(true, ox, oy);
					printf("set_detected!\n");
				}
			} else {
				framesSinceDetection = 0;
				server.SetDetectorState(false, 0, 0);
			}

			imshow("Image View", frame);

			char key = (char)waitKey(capture.isOpened() ? 50 : 500);
			if( key == 27 )
				break;
		}
	}
	
	server.Stop();
	return 0;
}
