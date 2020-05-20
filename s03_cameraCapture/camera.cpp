#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]){
	int key;
	cv::Mat frame;
	
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
		// 様々な処理
		// ...
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;
	}
	
	cap.release();
	return 0;
}