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

	int key;
	cv::Mat srcM, prevM, nextM, bufM, dispM;

	// オプティカルフローの初期化
	cv::Mat flow;
	cv::Ptr<cv::DenseOpticalFlow> opt = cv::optflow::createOptFlow_SparseToDense();

	// フロー描画用の変数
	double dx, dy, flow_rate = 20; // この間隔でフローが描画される
	cv::Point p1, p2;
	cv::Scalar flow_ave;

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
		
		flow_ave = cv::mean(flow); // オプティカルフローの平均
		dx = flow_ave[0];
		dy = flow_ave[1];
		if(cv::norm(cv::Point(dx, dy)) > 0.7){ // この例では小さくなるため最小範囲を低くした
			p1 = cv::Point(flow.cols / 2, flow.rows / 2);
			p2 = cv::Point(flow.cols / 2 + dx * flow_rate, flow.rows / 2 + dy * flow_rate);
			cv::line(dispM, p1, p2, cv::Scalar(0, 0, 255), 2);
		}
		printf("%f, %f\n", flow_ave[0], flow_ave[1]);

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