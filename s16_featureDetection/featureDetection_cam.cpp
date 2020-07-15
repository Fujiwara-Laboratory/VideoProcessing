#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]){
	int c_w = 640, c_h = 480;
	// カメラ関連
	cv::VideoCapture cap(0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, c_w);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, c_h);
	
	// カメラがオープンできたかの確認
	if(!cap.isOpened()) return -1;
	
	// 処理時間
	cv::TickMeter tm;
	char strCost[256];
	
	int key;
	cv::Mat srcM, grayM, dispM;
	
	// 特徴点関連の変数
	cv::Ptr<cv::Feature2D> fkeypt = cv::AKAZE::create();
	std::vector<cv::KeyPoint> kpts;
	cv::Mat desc;

	while(1){
		tm.reset();
		tm.start(); // 処理時間の計測開始
		cap >> srcM; // キャプチャ
		cv::cvtColor(srcM, grayM, cv::COLOR_BGR2GRAY);
		
		// 特徴点の検出と局所特徴量の計算
		fkeypt->detectAndCompute(grayM, cv::noArray(), kpts, desc);
		
		// 特徴点の描画
		cv::drawKeypoints(srcM, kpts, dispM, cv::Scalar(0, 255, 0));
		
		tm.stop(); // 処理時間の計測終了
		sprintf(strCost, "%.0f", tm.getTimeMilli()); // 1フレームあたりの処理時間(ms)
		cv::putText(dispM, strCost, cv::Point(30, 30), cv::FONT_HERSHEY_PLAIN, 1.4, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
		cv::imshow("cap", dispM);
		
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;
	}
	
	cap.release();
	return 0;
}