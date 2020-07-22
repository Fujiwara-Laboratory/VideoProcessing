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

	// 検出結果の枠表示関連
	cv::Mat hmg;
	std::vector<cv::Point2f> obj_vertex(4), det_vertex(4);
	obj_vertex[0] = cv::Point2f(0, 0);
	obj_vertex[1] = cv::Point2f(objM.cols, 0);
	obj_vertex[2] = cv::Point2f(objM.cols, objM.rows);
	obj_vertex[3] = cv::Point2f(0, objM.rows);

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
		std::vector<cv::Point2f> mpt_o, mpt_s;
		
		for(i = 0; i < dmc.size(); i++){
			if(dmc[i][0].distance < dmc[i][1].distance * matching_rate){ // 一位と二位で離れているか確認
				// よさげな特徴点となるマッチング結果と座標を残す
				dmc_results.push_back(dmc[i][0]);
				mpt_o.push_back(kpts_o[dmc[i][0].queryIdx].pt);
				mpt_s.push_back(kpts_s[dmc[i][0].trainIdx].pt);
			}
		}
		printf("%d -> %d, %d -> %d, %d -> %d\n", dmc.size(), dmc_results.size(), kpts_o.size(), mpt_o.size(), kpts_s.size(), mpt_s.size());

		if(mpt_o.size() > 4){ // 一定以上のマッチング結果がある場合に処理する
			hmg = cv::findHomography(mpt_o, mpt_s); // マッチング結果から射影行列を求める
			if( !hmg.empty() ){
				// フレームに対して座標を射影推定する
				cv::perspectiveTransform(obj_vertex, det_vertex, hmg);

				// オブジェクトのボーダー描画 (射影されるので矩形ではない)
				cv::line(srcM, det_vertex[0], det_vertex[1], cv::Scalar(0, 255, 0), 3);
				cv::line(srcM, det_vertex[1], det_vertex[2], cv::Scalar(0, 255, 0), 3);
				cv::line(srcM, det_vertex[2], det_vertex[3], cv::Scalar(0, 255, 0), 3);
				cv::line(srcM, det_vertex[3], det_vertex[0], cv::Scalar(0, 255, 0), 3);
			}
		}

		tm.stop();
		sprintf(strCost, "%.0f", tm.getTimeMilli());
		cv::putText(srcM, strCost, cv::Point(30, 30), cv::FONT_HERSHEY_PLAIN, 1.4, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
		
		cv::imshow("match", srcM);
		key = cv::waitKey(1);
		
		if(key == 'q' || key == 27) break;
	}
	return 0;
}