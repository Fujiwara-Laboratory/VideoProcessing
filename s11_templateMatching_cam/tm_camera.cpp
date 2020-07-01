#include <opencv2/opencv.hpp>

int main(int argc, char **argv){
	int key;
	cv::Mat frame;
	
	// カメラ関連
	cv::VideoCapture cap(1);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	
	// カメラがオープンできたかの確認
	if(!cap.isOpened()) return -1;
	
	cv::Mat srcM, tmpM, mapM; // 探索画像、探索結果
	// テンプレート画像
	tmpM = cv::imread(argv[1], 1);
	if(tmpM.empty()) return -1; 
	cv::Rect roi_rect(0, 0, tmpM.cols, tmpM.rows);
	cv::Point max_pt;
	double maxVal;
	
	while(1){
		cap >> frame;  // キャプチャ
		srcM = frame.clone();
		
		// テンプレートマッチング
		cv::matchTemplate(srcM, tmpM, mapM, cv::TM_CCORR_NORMED);
		
		// 最大のスコアの場所を探す
		cv::minMaxLoc(mapM, NULL, &maxVal, NULL, &max_pt);
		roi_rect.x = max_pt.x;
		roi_rect.y = max_pt.y;
		printf("(%d, %d) %f\n", max_pt.x, max_pt.y, maxVal);
		// 探索結果の場所に矩形を描画
		if(maxVal > 0.5){
			cv::rectangle(srcM, roi_rect, cv::Scalar(0, 0, 255), 3, 4);
		}
		
		cv::imshow("result image", srcM);
		cv::imshow("map image", mapM);
		
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;
	}
	
	return(0);
}
