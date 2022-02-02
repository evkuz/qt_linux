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

// For getting IP task
#include <iostream>     ///< cout
#include <cstring>      ///< memset
#include <errno.h>      ///< errno
#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close

// For Socket Server
#include "SocketServer.h"
#include "arg_parser.h"

// For Http Server
#include "httpserver/http_server.h"
#include "httpserver/http_headers.h"
#include "httpserver/http_content_type.h"
#include <mutex>
#include <condition_variable>
#include <iterator>


using namespace cv;
using namespace std;

typedef unsigned char byte;

byte color_lower[3] = {135, 135, 245}; //{0, 20, 190};
byte color_upper[3] = {250, 254, 255}; //{100, 140, 255};

const char* windowName = "Cube Detector";  

static int cameraId = 0;
static int framesToCheck = 5;
static bool changeFrameSize = true;
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
	{
		{
            "do-not-change-size", // long name
            'd',                  // short name 
            no_argument,    // is option value required
            "if set, frame size will not be changed",
            [](char* argv) { changeFrameSize = false; }, 
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


bool calibrationInProgress = true;
bool finishCalibration = false;
int calibZoneX1 = -1;
int calibZoneY1 = -1;
int calibZoneX2 = -1;
int calibZoneY2 = -1;

void calibrate_colors(const Mat& frame, int x1, int y1, int x2, int y2){
	color_lower[0] = 255;
	color_lower[1] = 255;
	color_lower[2] = 255;

	color_upper[0] = 0;
	color_upper[1] = 0;
	color_upper[2] = 0;
	
	cv::Rect roi(x1, y1, x2 - x1, y2 - y1);
	cv::Mat frame_roi;
	frame(roi).copyTo(frame_roi);

	int cn = frame_roi.channels();
	uint8_t* pixelPtr = (uint8_t*)frame_roi.data;
	uint8_t r, g, b;

	for(int i = 0; i < frame_roi.rows; i++)
	{
		for(int j = 0; j < frame_roi.cols; j++)
		{
			b = pixelPtr[i*frame_roi.cols*cn + j*cn + 0]; // B
			g = pixelPtr[i*frame_roi.cols*cn + j*cn + 1]; // G
			r = pixelPtr[i*frame_roi.cols*cn + j*cn + 2]; // R
			
			if (b < color_lower[0])
				color_lower[0] = b;
			if (b > color_upper[0])
				color_upper[0] = b;

			if (g < color_lower[1])
				color_lower[1] = g;
			if (g > color_upper[1])
				color_upper[1] = g;

			if (r < color_lower[2])
				color_lower[2] = r;
			if (r > color_upper[2])
				color_upper[2] = r;
		}
	}

	uint8_t minValue = 3;
	uint8_t maxValue = 255 - minValue;

	if(color_lower[0] <= minValue)
		color_lower[0] = 0;
	else
		color_lower[0] -= minValue;
	
	if(color_lower[1] <= minValue)
		color_lower[1] = 0;
	else
		color_lower[1] -= minValue;

	if(color_lower[2] <= minValue)
		color_lower[2] = 0;
	else
		color_lower[2] -= minValue;


	if(color_upper[0] >= maxValue)
		color_upper[0] = 255;
	else
		color_upper[0] += minValue;
	
	if(color_upper[1] >= maxValue)
		color_upper[1] = 255;
	else
		color_upper[1] += minValue;

	if(color_upper[2] >= maxValue)
		color_upper[2] = 255;
	else
		color_upper[2] += minValue;

	printf(
		"B: %d - %d\nG: %d - %d\nR: %d - %d\n",
		color_lower[0], color_upper[0], 
		color_lower[1], color_upper[1],
		color_lower[2], color_upper[2]
	);
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
        calibZoneX1 = x;
		calibZoneY1 = y;
		finishCalibration = false;
     }
     else if  ( event == EVENT_LBUTTONUP )
     {
		 if( (calibZoneX2 - calibZoneX1) > 0 && (calibZoneY2 - calibZoneY1) > 0 )
		 	finishCalibration = true;
     }
     else if ( event == EVENT_MOUSEMOVE && !finishCalibration )
     {
		if(calibZoneX1 >=0 && calibZoneY1 >=0){
			calibZoneX2 = x;
			calibZoneY2 = y;
		}
     }
}


int calibEffectFrameCount=0;
bool calibShowDone = false;

void DrawCalibration(Mat& frame){
	if(calibrationInProgress){
		if(finishCalibration){
			calibrate_colors(
				frame,
				calibZoneX1, calibZoneY1,
				calibZoneX2, calibZoneY2
			);
			calibrationInProgress = false;
			finishCalibration = false;
			calibShowDone = true;
			calibEffectFrameCount = 0;
			calibZoneX1 = -1;
		 	calibZoneY1 = -1;
		}
		cv::putText(frame, //target image
            "COLOR CALIBRATION", //text
            cv::Point(10, 18), //top-left position
            cv::FONT_HERSHEY_DUPLEX,
            0.5,
            CV_RGB(0, 255, 0), //font color
            2
		);
		if (calibZoneX1 >= 0 && calibZoneX2 >= 0 && calibZoneY1 >= 0 && calibZoneY2 >= 0)
		{
			Rect rect(calibZoneX1, calibZoneY1, (calibZoneX2 - calibZoneX1), (calibZoneY2 - calibZoneY1));
			rectangle(frame, rect, Scalar(255,0, 127), 2);
		}
	}
	if(calibShowDone){
		if(calibEffectFrameCount < 9){
			cv::putText(frame, //target image
				"CALIBRATION DONE!", //text
				cv::Point(frame.cols/2-80, frame.rows/2-20), //top-left position
				cv::FONT_HERSHEY_DUPLEX,
				0.8,
				CV_RGB(255, 0, 127), //font color
				2
			);
			calibEffectFrameCount++;
		} else {
			calibEffectFrameCount = 0;
			calibShowDone = false;
		}
	}
}


void DrawCameraMarks(Mat& frame){
	int step = 50;

	cv::Point p1(95, 31);
	cv::Point p2(frame.cols - 66, 56);
	cv::Point p3(91, frame.rows - 549);
	cv::Point p4(frame.cols - 74, frame.rows - 528);


	cv::drawMarker(frame, p1,  cv::Scalar(0, 0, 0), MARKER_CROSS, 20, 2);
	cv::drawMarker(frame, p2,  cv::Scalar(0, 0, 0), MARKER_CROSS, 20, 2);
	cv::drawMarker(frame, p3,  cv::Scalar(0, 0, 0), MARKER_CROSS, 20, 2);
	cv::drawMarker(frame, p4,  cv::Scalar(0, 0, 0), MARKER_CROSS, 20, 2);
}


// Getting my IP section
// -----------------------------
int get_my_ip(char* buffer){
	int res = -1;
	const char* google_dns_server = "8.8.8.8";
    int dns_port = 53;

    struct sockaddr_in serv;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    //Socket could not be created
    if(sock < 0)
    {
        std::cout << "Socket error" << std::endl;
		return res;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(google_dns_server);
    serv.sin_port = htons(dns_port);

    int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
    if (err < 0)
    {
        std::cout << "Error number: " << errno
            << ". Error message: " << strerror(errno) << std::endl;
		return res;
    }

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*)&name, &namelen);

    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
    if(p != NULL)
    {
        std::cout << "Local IP address is: " << buffer << std::endl;
		res = 0;
    }
    else
    {
        std::cout << "Error number: " << errno
            << ". Error message: " << strerror(errno) << std::endl;
    }

    close(sock);

	return res;
}
// -------------------/ end Getting my IP section
//

int main(int argc, char* argv[])
{
	Miksarus::ParseProgramOptions(argc, argv, options_list);
	
	// getting my IP
    char ip_addr[80];
    if (get_my_ip(ip_addr) != 0) {
    	return -1;
    }
    //const char* ip_addr = "192.168.1.175";

	VideoCapture capture;
	capture.open(cameraId);

    if(!capture.isOpened())
        return fprintf( stderr, "Could not initialize video (%d) capture\n", cameraId), -2;
	
	if (changeFrameSize) {
		capture.set(CAP_PROP_FRAME_WIDTH, 2304);
		capture.set(CAP_PROP_FRAME_HEIGHT, 1536);
	}
	
	double frame_width = capture.get( CAP_PROP_FRAME_WIDTH );
	double frame_height = capture.get( CAP_PROP_FRAME_HEIGHT );
	
	Mat frame, image, frame_orig;
	printf("[i] press 's' for capture image and 'Esc' or 'q' for quit!\n\n");
	namedWindow( windowName, 1 );

    //set the callback function for any mouse event
    setMouseCallback(windowName, CallBackFunc, NULL);

	SocketServer server("iqr.socket");
	if(server.Start() != 0){
		std::cerr << "Can't start socket server!" << endl;
		return -3;
	}

	// HTTP Server
	// ------------------------
	std::uint16_t SrvPort = 5555;
	std::uint16_t SrvThreadCount = 4;
	Mat serverFrame;
	std::mutex Mtx;
	std::mutex frameLock;
	std::condition_variable frameCondition;
	bool frameReady = false;

	using namespace IQRNetwork;
	HttpServer Srv(ip_addr, SrvPort, SrvThreadCount,
	[&] (IHttpRequestPtr req)
	{
		std::string Path = req->GetPath();
		// {
		// std::stringstream Io;
		// Io << "Path: " << Path << std::endl
		// 	<< Http::Request::Header::Host::Name << ": "
		// 		<< req->GetHeaderAttr(Http::Request::Header::Host::Value) << std::endl
		// 	<< Http::Request::Header::Referer::Name << ": "
		// 		<< req->GetHeaderAttr(Http::Request::Header::Referer::Value) << std::endl;
		// std::lock_guard<std::mutex> Lock(Mtx);
		// std::cout << Io.str() << std::endl;
		// }
		if (Path == "/video_feed")
		{
			cv::Mat resizedFrame;
			{
				std::unique_lock<std::mutex> locker(frameLock);
				while(!frameReady) // от ложных пробуждений
					frameCondition.wait(locker);
				//std::cout << "Have frame!" << std::endl;
				resize(serverFrame, resizedFrame, cv::Size(640, 480), 0, 0, cv::INTER_LINEAR);
			}
			
			std::vector<int> p;
			p.push_back(cv::IMWRITE_JPEG_QUALITY);
			p.push_back(50);
			
			int bufferSize = 1000000;
			vector<unsigned char> buf(bufferSize);
			imencode(".jpeg", resizedFrame, buf, p);

			req->SetResponseCode(200);
			req->SetResponseAttr(Http::Response::Header::Server::Value, "Vehicle image server");
			req->SetResponseAttr(Http::Response::Header::ContentType::Value, "multipart/x-mixed-replace; boundary=frame");
			req->SetResponseAttr("Connection", "keep-alive");
			//req->SetResponseAttr(Http::Response::Header::ContentType::Value, "image/jpeg");
			
			std::string frameBlockBegin = "--frame\r\nContent-Type: image/jpeg\r\n\r\n";
			std::vector<unsigned char> dataS(frameBlockBegin.size());
			for(char& c : frameBlockBegin) {
				dataS.push_back(static_cast<unsigned char>(c));
			}
			buf.insert(buf.begin(), dataS.begin(), dataS.end());
			req->SetResponseBuf(buf.data(), buf.size());
		} 
		else
		{
			req->SetResponseCode(400);
			req->SetResponseAttr(Http::Response::Header::Server::Value, "Vehicle image server");
			
			//req->SetResponseBuf(buf.data(), buf.size());
		}
	});
	

	//
	int framesSinceDetection = 0;
	Rect etalon;
	double etalon_border_coef = 1;
	while(true) {
		if(capture.isOpened())
		{
			Mat view0;
			capture >> view0;
			view0.copyTo(frame);
			// view0 = imread("frame4.png", 1);
			// double coef = 800. / view0.cols;
			// resize(view0, frame, cv::Size(view0.cols * coef,view0.rows * coef), 0, 0, cv::INTER_LINEAR);

			Rect pos = detector(frame, image, color_lower, color_upper);
			int area = pos.width*pos.height;
			if(area > 0) {
				rectangle(frame, pos, Scalar(10,10, 255),2);
				double border_coef = static_cast<double>(pos.width)/pos.height;
				
				double w = static_cast<double>(pos.width)/frame_width;
				double h = static_cast<double>(pos.height)/frame_height;
				
				int ox = static_cast<int>(pos.x + pos.width/2);
				int oy = static_cast<int>(pos.y + pos.height/2);
				//printf("w: %0.3f, h: %0.3f, border_coef:%0.3f\n", w, h, border_coef);
				if (border_coef > 0.6 && border_coef < 1.6 && w < 0.16 && w > 0.020 && h < 0.19 && h > 0.04){
					printf("%i, %i (w: %i, h: %i, area: %i px^2)\n", ox, oy, pos.width, pos.height, area);
					rectangle(frame, pos, Scalar(0,255, 0),2);
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

			DrawCalibration(frame);
			DrawCameraMarks(frame);
			imshow(windowName, frame);
			
			{
				std::unique_lock<std::mutex> locker(frameLock);
				frame.copyTo(serverFrame);
				frameReady = true;
				frameCondition.notify_one();
			}

			char key = (char)waitKey(capture.isOpened() ? 50 : 500);
			if( key == 115 ) {
				imwrite("frame.png", frame_orig);
				printf("Image was saved to file 'frame.png'");
			}
			
			if( key == 99 ) {
				calibrationInProgress = true;
			}
			//printf("key: %d\n", key);
			if( key == 27 || key == 113) {
				if(calibrationInProgress)
					calibrationInProgress = false;
				else
					break;
			}
		}
	}
	
	server.Stop();
	return 0;
}
