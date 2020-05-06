#include <iostream>
#include <opencv2/opencv.hpp>

#include <sys/stat.h>

int main(int argc, char **argv){
	cv::Mat img(cv::Size(600, 400), CV_8UC3, cv::Scalar(cv::Vec3b(255, 255, 255)));
	
	cv::rectangle(img, cv::Point(100, 50), cv::Point(300, 350), cv::Scalar(0, 0, 255), 3, cv::LINE_4);
	cv::line(img, cv::Point(350, 100), cv::Point(550, 300), cv::Scalar(255, 0, 255), 2, cv::LINE_AA);
	cv::imshow("disp", img);
	cv::imwrite("test.png", img);

	cv::waitKey(0);
	return 0;
}
