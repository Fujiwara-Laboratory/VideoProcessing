#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]){
	int c_w = 640, c_h = 480;
	// カメラ関連
	cv::VideoCapture cap(0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, c_w);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, c_h);
	cap.set(cv::CAP_PROP_FPS, 30);
	
	// カメラがオープンできたかの確認
	if(!cap.isOpened()) return -1;
	
	int key;
	cv::Mat frame, srcM, grayM, sxM, syM, seM, edgeM;
	
	while(1){
		cap >> frame;  // キャプチャ
		cv::imshow("capture", frame);
		
		srcM = frame.clone();
		cv::cvtColor(frame, grayM, cv::COLOR_BGR2GRAY);
		cv::imshow("gray", grayM);
		
		cv::Sobel(grayM, sxM, CV_32FC1, 1, 0, 3); // 横方向のエッジ強調
		cv::Sobel(grayM, syM, CV_32FC1, 0, 1, 3); // 縦方向のエッジ強調
		
		cv::magnitude(sxM, syM, seM); // ユークリッド距離
		cv::convertScaleAbs(seM, edgeM); // 絶対値を取って結果を8ビットデータへ
		
		cv::imshow("sobel edge", edgeM);
		
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;
	}
	
	cap.release();
	return 0;
}