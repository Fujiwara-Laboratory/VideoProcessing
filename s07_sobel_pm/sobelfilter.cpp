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
	cv::Mat frame, srcM, grayM, sxM, sxabsM;
	cv::Mat zeroM(cv::Size(c_w, c_h), CV_32FC1, cv::Scalar(0));
	cv::Mat sxpM, sxmM;
	
	while(1){
		cap >> frame;  // キャプチャ
		
		srcM = frame.clone();
		cv::cvtColor(frame, grayM, cv::COLOR_BGR2GRAY);
		cv::imshow("gray", grayM);
		
		cv::Sobel(grayM, sxM, CV_32FC1, 1, 0, 3); // 横方向のエッジ強調
		
		sxpM = max(sxM, zeroM); // ゼロと比べて大きい値をコピー (プラスの情報)
		cv::convertScaleAbs(sxpM, sxabsM); // 絶対値を取って結果を8ビットデータへ
		cv::imshow("sobel plus", sxabsM);
		
		sxmM = min(sxM, zeroM); // ゼロと比べて小さい値をコピー (マイナスの情報)
		cv::convertScaleAbs(sxmM, sxabsM); // 絶対値を取って結果を8ビットデータへ
		cv::imshow("sobel minus", sxabsM);
		
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;
	}
	
	cap.release();
	return 0;
}