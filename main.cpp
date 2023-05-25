// title : yolo
// date : 2021.11.17 created
// author : sungryul lee

#include <iostream>
#include <ctime>
#include <unistd.h>
#include<sys/time.h>
#include <opencv2/opencv.hpp>
#include "dxl.hpp"
#include "yolo.hpp"
#include <signal.h>

using namespace std;
using namespace cv;
using namespace cv::dnn;
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main()
{
	string src = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)1280, height=(int)720, format=(string)NV12 ! \
			nvvidconv flip-method=0 ! video/x-raw, width=(int)640, height=(int)360, format=(string)BGRx ! videoconvert ! \
			video/x-raw, format=(string)BGR !appsink";	
	string dst = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! h264parse ! \
			rtph264pay pt=96 ! udpsink host=203.234.58.121 port=8001 sync=false";

	cv::VideoCapture source(src, CAP_GSTREAMER);
	if (!source.isOpened()) { cout << "Video error" << endl; return -1; }

	cv::VideoWriter writer(dst, 0, (double)30, cv::Size(640, 360), true);
	if(!writer.isOpened())  { cout << "Writer error" << endl; return -1;}
 
 	cv::dnn::Net net = yolo_init("cross-hands-yolov4-tiny.cfg", "cross-hands-yolov4-tiny.weights","obj.names");
	//cv::dnn::Net net = yolo_init("yolov4-tiny-robot-288.cfg", "yolov4-tiny-robot-288.weights","robot.names");
	//cv::dnn::Net net = yolo_init("yolov4-tiny-robot-416.cfg", "yolov4-tiny-robot-416.weights","robot.names");
	
	signal(SIGINT, ctrlc); 				//시그널 핸들러 지정
	cv::Mat frame;
	struct timeval start,end1,end2,end3,end4;
	double diff1,diff2,diff3,diff4;
	Dxl dxl;
	Mat result;
	//int error=0;
	int detect=0;	
	
	while (true)
	{
		gettimeofday(&start,NULL);
		source >> frame;
		if (frame.empty()){ cout << "frame empty" << endl; return -1; }
		gettimeofday(&end1,NULL);

		gettimeofday(&end2,NULL);
		
		//detect = yolo_inference(net,cv::Size(320,320),frame);
		detect = yolo_inference(net,cv::Size(288,288),frame);
		//detect = yolo_inference(net,cv::Size(416,416),frame);
		cout << "detect: " << detect << endl;
		gettimeofday(&end3,NULL);

		writer << frame;
		if(dxl.kbhit() && (dxl.getch() == 'q')) break;
		if (ctrl_c_pressed) break; 
		//usleep(50000);
		
		gettimeofday(&end4,NULL);
		diff1 = end1.tv_sec + end1.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
		diff2 = end2.tv_sec + end2.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
		diff3 = end3.tv_sec + end3.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
		diff4 = end4.tv_sec + end4.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
		cout  << "time1:" << diff1 << endl;
		cout  << "time2:" << diff2 << endl;
		cout  << "time3:" << diff3 << endl;
		cout  << "time4:" << diff4 << endl;		
				
	}
	
	return 0;
}

