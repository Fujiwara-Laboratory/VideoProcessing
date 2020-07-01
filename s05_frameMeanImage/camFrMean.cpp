#include <iostream>
#include <opencv2/opencv.hpp>

int main( int argc, char **argv){
	int c_w = 1280, c_h = 720;
	int key, meanSize = 3, i;
	cv::Mat frame, srcM, totalM, tmpM, dispM;
	std::vector<cv::Mat> IntgM;
	
	cv::VideoCapture cap(0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, c_w);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, c_h);
	
	// カメラオープン関連
	if(!cap.isOpened()) return -1;
	
	while(1){
		cap >> frame;  // キャプチャ
		cv::imshow("capture", frame);
		if (frame.empty()) printf("%d, %d\n", frame.rows, frame.cols);
		srcM = frame.clone();
		
		IntgM.push_back(srcM); // 蓄積用の配列に追加
		if(IntgM.size() == meanSize){ // 平均画像を作れる数になったら処理
			totalM =cv::Mat::zeros(srcM.size(), CV_32FC3); // 合計用の画像を初期化
			for(i = 0; i< meanSize; i++){
				IntgM[i].convertTo(tmpM, CV_32FC3, 1.0);
				totalM = totalM + tmpM; // 平均
			}
			totalM.convertTo(dispM, CV_8UC3, 1.0 / meanSize);
			cv::imshow("cam", dispM);

			IntgM.erase(IntgM.begin()); // 古いのから削除
		}
		
		key = cv::waitKey(30);
		if(key == 'q' || key == 27) break;
	}
	cv::destroyAllWindows();
	cap.release();
	return 0;
}
