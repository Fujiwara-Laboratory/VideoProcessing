#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]){
	int key;
	cv::Mat frame, srcM, grayM, binM, blurM, cannyM;
	
	// カメラ関連
	cv::VideoCapture cap(0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	cap.set(cv::CAP_PROP_FPS, 30);
	
	// カメラがオープンできたかの確認
	if(!cap.isOpened()) return -1;
	
	while(1){
		cap >> frame;  // キャプチャ
		cv::imshow("capture", frame);
		
		srcM = frame.clone();
		cv::cvtColor(frame, grayM, cv::COLOR_BGR2GRAY);
		
		cv::threshold(grayM, binM, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
		cv::GaussianBlur(srcM, blurM, cv::Size(51, 51), 10, 10);
		cv::Canny(grayM, cannyM, 5, 30);
		
		cv::imshow("result binary", binM);
		cv::imshow("result blur", blurM);
		cv::imshow("result canny", cannyM);
		
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;
	}
	
	cap.release();
	return 0;
}