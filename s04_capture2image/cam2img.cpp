#include <opencv2/opencv.hpp>

int main(int argc, char *argv[]){
	int key, imageNum = 0;
	cv::Mat frame;
	char saveFileName[256];
	
	// カメラ関連
	cv::VideoCapture cap(0);
	cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
	cap.set(cv::CAP_PROP_FPS, 30);
	
	// カメラがオープンできたかの確認
	if(!cap.isOpened()) return -1;
	
	while(1){
		cap >> frame; // キャプチャ
		cv::imshow("capture", frame);
		// 様々な処理
		// ...
		key = cv::waitKey(1);
		if(key == 'q' || key == 27){
			break;
		}else if(key == 's'){
			sprintf(saveFileName, "cap_%03d.jpg", imageNum);
			cv::imwrite(saveFileName, frame);
			imageNum++;
		}
	}
	
	cap.release();
	return 0;
}