#include "tm_main.h"

int main(int argc, char **argv){
	cv::Mat srcM, tmpM, mapM;
	int x, y;
	
	srcM = cv::imread(argv[1], -1); // 探索対象の画像
	tmpM = cv::imread(argv[2], -1); // テンプレート画像
	if(srcM.empty() || tmpM.empty()) return -1; 
	
	// 探索画像全体に対して，テンプレートのマッチング値を計算
	mapM = cv::Mat::zeros(srcM.size(), CV_8UC1);
	GenMatchMap(srcM, tmpM, mapM, x, y);
	cv::Rect roi_rect(x, y, tmpM.cols, tmpM.rows);

	// 探索結果の場所に矩形を描画
	cv::rectangle(srcM, roi_rect, cv::Scalar(0, 0, 255), 3, 4);

	cv::imshow("result image", srcM);
	cv::imshow("map image", mapM);
	cv::waitKey(0);

	return 0;
}
