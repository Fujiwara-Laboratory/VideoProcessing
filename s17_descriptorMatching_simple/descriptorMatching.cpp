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
	
	// 特徴点関連
	cv::Ptr<cv::Feature2D> fkeypt = cv::AKAZE::create();
	std::vector<cv::KeyPoint> kpts_o, kpts_s;
	cv::Mat desc_o, desc_s;

	// 特徴量マッチング関連
	cv::Mat objM, obj_grayM;

	// テンプレート画像を事前に読み込んでおき局所特徴量の計算までしておく
	objM = cv::imread(argv[1], cv::IMREAD_COLOR);
	if(objM.empty()) return -1;
	cv::cvtColor(objM, obj_grayM, cv::COLOR_BGR2GRAY);
	fkeypt->detectAndCompute(obj_grayM, cv::noArray(), kpts_o, desc_o);
	if(kpts_o.size() == 0) return -1; // 特徴点が得られなければ終了

	// 局所特徴量のマッチング処理関連
	cv::Ptr<cv::DescriptorMatcher> matcherBF = cv::DescriptorMatcher::create("BruteForce");
	std::vector<cv::DMatch> dmc;

	while(1){
		tm.reset();
		tm.start(); // 処理時間の計測開始
		cap >> srcM; // キャプチャ
		cv::cvtColor(srcM, grayM, cv::COLOR_BGR2GRAY);
		
		// 特徴点の検出と局所特徴量の計算
		fkeypt->detectAndCompute(grayM, cv::noArray(), kpts_s, desc_s);
		if(kpts_s.size() == 0) continue; // 特徴点が得られなければスキップ
		
		// 局所特徴量間のマッチング
		matcherBF->match(desc_o, desc_s, dmc);

		// マッチング結果の出力・表示
		cv::drawMatches(objM, kpts_o, srcM, kpts_s, dmc, dispM);
		cv::imshow("result match", dispM);
		
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