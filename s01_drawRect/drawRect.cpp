#include <opencv2/opencv.hpp>

int main(int argc, char **argv){
	cv::Mat img(cv::Size(600, 400), CV_8UC3, cv::Scalar(cv::Vec3b(255, 0, 0)));
	
	cv::rectangle(img, cv::Point(100, 50), cv::Point(300, 350), cv::Scalar(0, 0, 255), 3, cv::LINE_4);
	cv::line(img, cv::Point(350, 100), cv::Point(550, 300), cv::Scalar(255, 0, 255), 2, cv::LINE_4);
	cv::putText(img, "test", cv::Point(50, 30), cv::FONT_HERSHEY_PLAIN, 2, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
	cv::imshow("disp", img);
	cv::imwrite("test.jpg", img);
	
	cv::Mat grayM;
	cv::cvtColor(img, grayM, cv::COLOR_BGR2GRAY);
	cv::imshow("gray", grayM);
	
	cv::waitKey(0);
	
	return 0;
}
