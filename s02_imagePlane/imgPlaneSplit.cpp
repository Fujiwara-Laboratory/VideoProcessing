#include <opencv2/opencv.hpp>

int main(int argc, char **argv){
	cv::Mat srcM;
	std::vector<cv::Mat> rgbPlanes;
	
	srcM = cv::imread(argv[1], cv::IMREAD_COLOR);
	if(srcM.empty()) return -1;
	
	cv::split(srcM, rgbPlanes);
	
	cv::imshow("b", rgbPlanes[0]);
	cv::imshow("g", rgbPlanes[1]);
	cv::imshow("r", rgbPlanes[2]);
	cv::imwrite("test.jpg", rgbPlanes[2]);
	
	cv::waitKey(0);
	
	return 0;
}
