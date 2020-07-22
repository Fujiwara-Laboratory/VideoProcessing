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
	cv::Mat srcM, grayM, dispM;
	
	// 特徴点検出
	cv::Ptr<cv::Feature2D> fpm = cv::AKAZE::create();
	std::vector<cv::KeyPoint> kpts_o, kpts_s;
	cv::Mat desc_o, desc_s;

	// テンプレート画像を事前に読み込んでおき局所特徴量の計算までしておく
	cv::Mat objM, obj_grayM;
	objM = cv::imread(argv[1], cv::IMREAD_COLOR);
	if(objM.empty()) return -1;
	cv::cvtColor(objM, obj_grayM, cv::COLOR_BGR2GRAY);
	fpm->detectAndCompute(obj_grayM, cv::noArray(), kpts_o, desc_o);
	if(kpts_o.size() == 0) return -1; // 特徴点が得られなければ終了

	// 局所特徴量のマッチング処理関連
	cv::Ptr<cv::DescriptorMatcher> matcherBF = cv::DescriptorMatcher::create("BruteForce");
	double matching_rate = 0.6;

	while(1){
		tm.reset();
		tm.start();
		cap >> srcM; // キャプチャ

		// フレームから特徴点検出
		cv::cvtColor(srcM, grayM, cv::COLOR_BGR2GRAY);
		fpm->detectAndCompute(grayM, cv::noArray(), kpts_s, desc_s);
		if(kpts_s.size() == 0) continue; // 特徴点が得られなければスキップ
		
		std::vector<std::vector<cv::DMatch> > dmc; // ループ前で宣言する場合は毎フレームクリアが必要
		matcherBF->knnMatch(desc_o, desc_s, dmc, 2); // 局所特徴量をマッチング

		// マッチングで得られた点から選別をする (よく知られたアルゴリズム)
		std::vector<cv::DMatch> dmc_results;
		
		for(i = 0; i < dmc.size(); i++){
			if(dmc[i][0].distance < dmc[i][1].distance * matching_rate){ // 一位と二位で離れているか確認
				// よさげな特徴点となるマッチング結果を残す
				dmc_results.push_back(dmc[i][0]);
			}
		}
		printf("%d -> %d, %d, %d\n", dmc.size(), dmc_results.size(), kpts_o.size(), kpts_s.size());

		cv::drawMatches(objM, kpts_o, srcM, kpts_s, dmc_results, dispM);

		tm.stop();
		sprintf(strCost, "%.0f", tm.getTimeMilli());
		cv::putText(dispM, strCost, cv::Point(30, 30), cv::FONT_HERSHEY_PLAIN, 1.4, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
		
		cv::imshow("match", dispM);
		key = cv::waitKey(1);
		
		if(key == 'q' || key == 27) break;
	}
	return 0;
}