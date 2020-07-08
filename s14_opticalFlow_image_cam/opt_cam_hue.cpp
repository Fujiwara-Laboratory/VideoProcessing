#include <opencv2/opencv.hpp>
#include <opencv2/optflow.hpp>

// 汎用的なHSV値をRGB値に変換する関数
void hsv2rgb(double h, double s, double v, double *r, double *g, double *b){
	// 一般的な色相の間隔にあわせるため!?+90度する
	if(h < 270) h += 90;
	else h -= 270;
	int i = (int)(h / 60.0) % 6;
	int p, q, t;
	if(s > 255) s = 255;
	double f = (double)(h / 60.0f) - (double)((int)(h / 60.0f));
	p = (int)(0.5 + v * (1.0f - (s / 255.0f)));
	q = (int)(0.5 + v * (1.0f - (s / 255.0f) * f));
	t = (int)(0.5 + v * (1.0f - (s / 255.0f) * (1.0f - f)));
	
	switch(i){ // 角度の範囲に応じたRGB値計算
		case 0 : *r = v; *g = t; *b = p; break;
		case 1 : *r = q; *g = v; *b = p; break;
		case 2 : *r = p; *g = v; *b = t; break;
		case 3 : *r = p; *g = q; *b = v; break;
		case 4 : *r = t; *g = p; *b = v; break;
		case 5 : *r = v; *g = p; *b = q; break;
	}
}

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
	cv::Mat srcM, prevM, nextM, bufM, hueM;

	// オプティカルフローの初期化
	cv::Mat flow;
	cv::Ptr<cv::DenseOpticalFlow> opt = cv::optflow::createOptFlow_SparseToDense();

	// フローのhue描画用の変数
	hueM = cv::Mat(cv::Size(c_w, c_h), CV_8UC3);
	double dx, dy, rad, mag, saturation_rate = 7, r, g, b;

	// 第1フレームを取得しておく
	cap >> bufM;
	cv::cvtColor(bufM, prevM, cv::COLOR_BGR2GRAY);
	
	while(1){
		tm.reset();
		tm.start(); // 処理時間の計測開始
		cap >> srcM; // キャプチャ

		cv::cvtColor(srcM, nextM, cv::COLOR_BGR2GRAY);
		
		opt->calc(prevM, nextM, flow); // オプティカルフローの計算

		// 密なオプティカルフローを色相で着色して描画
		for(j = 0; j < flow.rows; j++){
			for(i = 0; i < flow.cols; i++){
				dx = flow.at<cv::Vec2f>(j, i)[0];
				dy = flow.at<cv::Vec2f>(j, i)[1];
				rad = cv::fastAtan2(dy , dx);
				mag = cv::norm(cv::Point(dx , dy));
				
				if(cv::norm(cv::Point(dx, dy)) > 1){
					hsv2rgb(rad, mag * saturation_rate, 255, &r, &g, &b);
					hueM.at<cv::Vec3b>(j, i) = cv::Vec3b(b, g, r);
				}else{
					hueM.at<cv::Vec3b>(j, i) = cv::Vec3b(255, 255, 255);
				}
			}
		}

		tm.stop(); // 処理時間の計測終了
		sprintf(strCost, "%.0f", tm.getTimeMilli()); // 1フレームあたりの処理時間(ms)
		cv::putText(srcM, strCost, cv::Point(30, 30), cv::FONT_HERSHEY_PLAIN, 1.4, cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
		cv::imshow("cap", srcM);
		cv::imshow("flow hue", hueM);
		
		key = cv::waitKey(1);
		if(key == 'q' || key == 27) break;

		prevM = nextM.clone(); // 現フレームの画像を次のループでの前フレーム画像へ
	}
	
	cap.release();
	return 0;
}