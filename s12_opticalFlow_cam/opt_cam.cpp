#include <opencv2/opencv.hpp>
#include <opencv2/optflow.hpp>

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

	int key, i, j;
	cv::Mat srcM, prevM, nextM, bufM, dispM;

	// オプティカルフローの初期化
	cv::Mat flow;
	cv::Ptr<cv::DenseOpticalFlow> opt = cv::optflow::createOptFlow_SparseToDense();

	// フロー描画用の変数
	double dx, dy;
	int lstep = 20; // この間隔でフローが描画される
	cv::Point p1, p2;

	// 第1フレームを取得しておく
	cap >> bufM;
	cv::cvtColor(bufM, prevM, cv::COLOR_BGR2GRAY);
	
	while(1){
		tm.reset();
		tm.start(); // 処理時間の計測開始
		cap >> srcM; // キャプチャ
		dispM = srcM.clone();

		cv::cvtColor(srcM, nextM, cv::COLOR_BGR2GRAY);
		
		opt->calc(prevM, nextM, flow); // オプティカルフローの計算

		// 密なオプティカルフローの一部を線で描画
		for(j = 0; j < flow.rows; j += lstep){
			for(i = 0; i < flow.cols; i += lstep){
				dx = flow.at<cv::Vec2f>(j, i)[0];
				dy = flow.at<cv::Vec2f>(j, i)[1];
				
				if(cv::norm(cv::Point(dx, dy)) > 1){
					p1 = cv::Point(i, j);
					p2 = cv::Point(i + dx, j + dy);
					cv::line(dispM, p1, p2, cv::Scalar(0, 0, 255), 2);
				}
			}
		}

		tm.stop(); // 処理時間の計測終了
		sprintf(strCost, "%.0f", tm.getTimeMilli()); // 1フレームあたりの処理時間(ms)
		cv::putText(dispM, strCost, cv::Point(30, 30), cv::FONT_HERSHEY_PLAIN, 1.4, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
		cv::imshow("cap", dispM);
		
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;

		prevM = nextM.clone(); // 現フレームの画像を次のループでの前フレーム画像へ
	}
	
	cap.release();
	return 0;
}