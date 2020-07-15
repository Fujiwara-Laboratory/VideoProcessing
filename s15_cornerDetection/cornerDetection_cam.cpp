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
	
	int key, i;
	cv::Mat srcM, grayM;
	
	// 特徴点を格納する配列
	std::vector<cv::Point2f> pts;

	while(1){
		tm.reset();
		tm.start(); // 処理時間の計測開始
		cap >> srcM; // キャプチャ
		cv::cvtColor(srcM, grayM, cv::COLOR_BGR2GRAY);
		
		// 特徴点の検出
		cv::goodFeaturesToTrack(grayM, pts, 50, 0.01, 20);
		
		for(i = 0; i < pts.size(); i++){ // 特徴点の描画
			cv::circle(srcM, pts[i], 5, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
		}
		
		tm.stop(); // 処理時間の計測終了
		sprintf(strCost, "%.0f", tm.getTimeMilli()); // 1フレームあたりの処理時間(ms)
		cv::putText(srcM, strCost, cv::Point(30, 30), cv::FONT_HERSHEY_PLAIN, 1.4, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
		cv::imshow("cap", srcM);
		
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;
	}
	
	cap.release();
	return 0;
}