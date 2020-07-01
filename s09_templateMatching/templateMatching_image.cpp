#include <opencv2/opencv.hpp>

int main(int argc, char **argv){
	cv::Mat srcM, tmpM, mapM;
	
	srcM = cv::imread(argv[1], -1); // 探索対象の画像
	tmpM = cv::imread(argv[2], -1); // テンプレート画像
	if(srcM.empty() || tmpM.empty()) return -1; 
	
	// テンプレートマッチング
	cv::matchTemplate(srcM, tmpM, mapM, cv::TM_CCOEFF_NORMED);
	
	// 最大のスコアの場所を探す
	cv::Point max_pt;
	double maxVal;
	cv::minMaxLoc(mapM, NULL, &maxVal, NULL, &max_pt);

	cv::Rect roi_rect(max_pt.x, max_pt.y, tmpM.cols, tmpM.rows);
	printf("(%d, %d) %f\n", max_pt.x, max_pt.y, maxVal);

	// 探索結果の場所に矩形を描画
	cv::rectangle(srcM, roi_rect, cv::Scalar(0, 0, 255), 3, 4);
	
	cv::imshow("result image", srcM);
	cv::imshow("map image", mapM);
	cv::waitKey(0);
	
	return(0);
}
