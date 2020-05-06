#include <opencv2/opencv.hpp>

int main(int argc, char **argv){
	cv::Mat srcM;
	
	srcM = cv::imread(argv[1], -1);
	if(srcM.empty()) return -1;
	
	cv::imshow("Input Image", srcM);
	cv::waitKey(0);
	
	return 0;
}
